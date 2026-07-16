""" A set of functions to: 
- crop a point cloud to within a box
- best approximate a point cloud, either as a sphere or cuboid. 
- return the points of the boundary curve of a point cloud as seen from above"""  

from typing import Tuple
import open3d as o3d
import numpy as np
import scipy
from scipy.spatial.transform import Rotation as R
from shape_msgs.msg import SolidPrimitive

def sum_of_pcd_distances(source: o3d.geometry.PointCloud, target: o3d.geometry.PointCloud) -> float:
    return np.sum(source.compute_point_cloud_distance(target))

def complete_partial_cloud(
    pcd: o3d.geometry.PointCloud,
    view_axis: np.ndarray = np.array([0.0, 0.0, 1.0]),
) -> o3d.geometry.PointCloud:
    """Approximate the occluded far side of a single-view point cloud.

    A depth camera only sees the near surface of an object; the far side (along
    the camera view/depth axis) is missing, so a bounding box fit to the raw
    cloud is too shallow and the object is under-sized for grasping/collision.

    We make ONE reasonable assumption — that the object is a roughly convex,
    front/back-symmetric solid — and reflect the visible points across the
    object's FAR silhouette plane (the deepest visible cross-section along the
    view axis). For a convex object the camera can only see up to that silhouette
    rim, so its mirror image is a good estimate of the occluded back:

      * A ball/fruit: the silhouette is the equator, so the reflected front
        hemisphere reconstructs the full sphere.
      * A cup/bottle/can: reconstructs the full cylinder depth.
      * A flat face viewed straight on has ~no visible depth, so the reflection
        lands on itself and adds ~nothing (we honestly can't infer depth we
        never saw, and we do not fabricate it).

    A high percentile (not the max) defines the silhouette plane so a single
    noisy far point can't over-inflate the result. The cloud is in the camera
    optical frame, where +Z is the view/depth axis — hence the default axis.

    Returns a new cloud with the original points plus the reflected front; the
    original is returned unchanged if it is too small to be meaningful.
    """
    pts = np.asarray(pcd.points)
    if pts.shape[0] < 4:
        return pcd
    v = np.asarray(view_axis, dtype=float)
    n = np.linalg.norm(v)
    if n < 1e-9:
        return pcd
    v = v / n

    d = pts @ v                       # projection of each point along the view axis
    d_ref = np.percentile(d, 95.0)    # far silhouette plane (robust to outliers)
    front = pts[d <= d_ref]           # visible front surface up to the silhouette
    if front.shape[0] < 4:
        return pcd
    df = front @ v
    mirrored = front + 2.0 * np.outer(d_ref - df, v)  # reflect front across silhouette
    completed = np.vstack([pts, mirrored])

    out = o3d.geometry.PointCloud()
    out.points = o3d.utility.Vector3dVector(completed)
    return out

def upright_box_from_cloud(
    pcd: o3d.geometry.PointCloud,
    up: np.ndarray,
    view_dir: np.ndarray = np.array([0.0, 0.0, 1.0]),
) -> Tuple[o3d.geometry.OrientedBoundingBox, o3d.geometry.TriangleMesh, o3d.geometry.PointCloud]:
    """Fit an UPRIGHT box to a tabletop object seen from a (possibly angled) view.

    Tabletop objects rest on a horizontal surface, so their box should stand
    vertically along gravity — not tilt to follow the camera's line of sight.
    Mirroring a partial cloud along the tilted view axis (see complete_partial_
    cloud) produces a box that leans over and punches through the table when the
    camera looks down at an angle. This instead separates the two directions:

      * VERTICAL (along ``up`` = gravity, expressed in the camera frame): the box
        spans from the lowest visible point (≈ where the object meets the table)
        up to the highest, so it sits ON the table and stays upright.
      * HORIZONTAL (the ground plane): fit a rectangle to the VISIBLE footprint
        (robust percentile extents). We do NOT fabricate the occluded back — a
        top-down-ish view already shows the full footprint (the rim), and
        fabricating it repeatedly over-sized the box.

    ``up`` and ``view_dir`` are unit vectors in the camera optical frame (the
    frame the cloud lives in); ``view_dir`` defaults to the optical +Z axis.

    Returns (oriented_bounding_box, mesh, completed_cloud), where completed_cloud
    is the point cloud actually used to fit the box (original points plus any
    ground-plane completion) — publish it to inspect what the fit sees. Falls
    back to the raw minimal OBB if there are too few points to fit reliably.
    """
    pts = np.asarray(pcd.points)
    up = np.asarray(up, dtype=float)
    up = up / (np.linalg.norm(up) + 1e-12)
    if pts.shape[0] < 8:
        obb = pcd.get_minimal_oriented_bounding_box()
        mesh = o3d.geometry.TriangleMesh.create_from_oriented_bounding_box(obb)
        return obb, mesh, pcd

    # Split each point into height (along gravity) and a horizontal component.
    h = pts @ up
    horiz = pts - np.outer(h, up)

    # Fit to the VISIBLE cloud only — do NOT fabricate the occluded back. Earlier
    # mirror-completion reliably over-sized the box (a partial cup rim reflected
    # into a much larger footprint), so we keep only what the camera actually saw
    # and let the robust percentile extents below reject stray points. A
    # top-down-ish view of a tabletop object already shows its full footprint (the
    # rim), so the box comes out object-sized without guessing.
    horiz_full = horiz
    h_full = h
    completed_pts = pts

    # Orthonormal ground basis (e1, e2) perpendicular to up.
    seed = np.array([1.0, 0.0, 0.0]) if abs(up[0]) < 0.9 else np.array([0.0, 1.0, 0.0])
    e1 = np.cross(up, seed); e1 /= np.linalg.norm(e1)
    e2 = np.cross(up, e1)

    # Fit a rectangle to the (completed) footprint via PCA in the ground plane.
    u = horiz_full @ e1
    v = horiz_full @ e2
    uv = np.c_[u, v]
    uv_c = uv.mean(axis=0)
    uvz = uv - uv_c
    _, evecs = np.linalg.eigh(uvz.T @ uvz)   # columns: ascending eigenvalue
    r_major = evecs[:, 1]
    r_minor = evecs[:, 0]

    # Robust extents via 1st/99th percentiles along each axis, so a few stray /
    # noisy points (e.g. a sliver of table left in the mask) can't balloon the
    # box. Returns (half_extent, center_offset) to also recenter for asymmetry.
    def _span(proj):
        lo, hi = np.percentile(proj, [1.0, 99.0])
        return (hi - lo) / 2.0, (hi + lo) / 2.0

    ha1, c1 = _span(uvz @ r_major)
    ha2, c2 = _span(uvz @ r_minor)
    hz, cz = _span(h_full)

    axis1 = r_major[0] * e1 + r_major[1] * e2
    axis2 = r_minor[0] * e1 + r_minor[1] * e2

    center_uv = uv_c + c1 * r_major + c2 * r_minor
    center = (center_uv[0] * e1 + center_uv[1] * e2) + cz * up
    R = np.column_stack([axis1, axis2, up])
    if np.linalg.det(R) < 0:
        R[:, 1] = -R[:, 1]
    extent = np.maximum(np.array([2.0 * ha1, 2.0 * ha2, 2.0 * hz]), 1e-3)

    obb = o3d.geometry.OrientedBoundingBox(center, R, extent)
    mesh = o3d.geometry.TriangleMesh.create_from_oriented_bounding_box(obb)

    completed = o3d.geometry.PointCloud()
    completed.points = o3d.utility.Vector3dVector(completed_pts)
    return obb, mesh, completed


def get_best_approx(
    pcd: o3d.geometry.PointCloud,
    up: np.ndarray = None,
) -> Tuple[o3d.geometry.TriangleMesh, int, np.ndarray, o3d.geometry.PointCloud]:
    """
    Returns the mesh, either a cuboid or sphere, that best approximates the point cloud

    Arguments:
        pcd: Point cloud to be approximated
        up: Optional gravity 'up' unit vector expressed in the point cloud's
            (camera) frame. When provided, the object is fit as an UPRIGHT box
            resting on the table (correct for tabletop objects seen at an angle);
            when None, falls back to a view-axis-symmetric minimal bounding box.

    Returns:
        shape_mesh: The best-approximation mesh as an o3d.geometry.TriangleMesh object
        shape_type: The type of shape as a shape_msgs.msg.SolidPrimitive type (either
                    SolidPrimitive.BOX or SolidPrimitive.SPHERE)
        quaternion: The orientation of the shape as a (4,) np array representing a quaternion (x, y, z, w)
        completed_cloud: The point cloud actually used to fit the shape (for viz/debug)

    """
    # Preferred: gravity is known, so fit an upright, table-resting box. This
    # avoids the tilted, table-penetrating box that view-axis mirroring produces
    # for angled views.
    if up is not None:
        obb, min_cuboid, completed = upright_box_from_cloud(pcd, up)
        min_cuboid.compute_vertex_normals()
        quaternion = R.from_matrix(np.asarray(obb.R).copy()).as_quat()
        return min_cuboid, SolidPrimitive.BOX, quaternion, completed
    # A single depth view only captures the near surface, so fit shapes to a
    # completed cloud that mirrors the visible points to the occluded far side
    # (one reasonable front/back-symmetry assumption). This makes the box/sphere
    # reflect the object's true extent instead of a too-shallow partial view.
    pcd_full = complete_partial_cloud(pcd)

    pcd_hull, _ = pcd_full.compute_convex_hull()
    pcd_hull.compute_vertex_normals()
    # o3d.visualization.draw_geometries([pcd, pcd_hull])

    orientated_bounding_box, min_cuboid = get_min_cuboid(pcd_full)
    avg_sphere = get_avg_sphere_2(pcd_full)[0]
    rotation_matrix = orientated_bounding_box.R.copy()
    
    print("Rotation matrix:\n", rotation_matrix)
    z_axis_rotation = R.from_euler('z', float(90), degrees=True)
    rotated_angle = R.from_matrix(rotation_matrix) * z_axis_rotation
    quaternion = rotated_angle.as_quat()
    print("Quaternion (x, y, z, w):", quaternion)

    # Accuracy is being taken as the difference in volume between pcd and approx...
    cuboid_accuracy = np.abs(pcd_hull.get_volume() - min_cuboid.get_volume())
    sphere_accuracy = np.abs(pcd_hull.get_volume() - avg_sphere.get_volume())

    min_cuboid.compute_vertex_normals()
    return min_cuboid, SolidPrimitive.BOX, quaternion, pcd_full

def crop_point_cloud(pcd: o3d.geometry.PointCloud, min_bounds: np.ndarray, max_bounds: np.ndarray) -> tuple[o3d.geometry.PointCloud, o3d.geometry.AxisAlignedBoundingBox]:
    """ "
    Crops a point cloud to a within a bounding box"
    
    Arguments:
        pcd: Point cloud to be cropped
        min_bounds: Lowest corner of bounding box
        max_bounds: Highest corner of bounding box

    Returns:
        (cropped_pcd, bounding_box): o3d.geometry.PointCloud object of cropped pcd, o3d.geometry.AxisAlignedBoundingBox object of cropping box 

    """  
    bounding_box = o3d.geometry.AxisAlignedBoundingBox(min_bounds, max_bounds)
    return (pcd.crop(bounding_box), bounding_box)

def print_point_cloud_info(pcd: o3d.geometry.PointCloud):
    """
    Prints the center and extents of a point cloud
    
    Arguments: 
        pcd: The point cloud to be described

    """
    print("Point cloud details")
    print("[x, y, z] center location: " + str(pcd.get_center()))
    print("[x, y, z] minimum corner: " + str(pcd.get_min_bound()))
    print("[x, y, z] maximum corner: " + str(pcd.get_max_bound()))
    print("[x, y, z] point cloud extents: " + str(pcd.get_axis_aligned_bounding_box().get_extent()))

def get_min_cuboid(pcd: o3d.geometry.PointCloud) -> tuple[o3d.geometry.OrientedBoundingBox, o3d.geometry.TriangleMesh]:
    """
    Returns the cuboid that best approximates the point cloud (the minimum volume bounding box) 
    
    Arguments: 
        pcd: The point cloud to be approximated

    Returns:
        (min_volume_bounding_box, min_volume_bounding_box_mesh): The best-approximation cuboid as an o3d.geometry.OrientedBoundingBox and o3d.geometry.TriangleMesh object respectively

    """
    # TODO @Tanay understand what this function returns and return the dimensions of the cuboid
    min_volume_bounding_box = pcd.get_minimal_oriented_bounding_box()
    # dimensions = min_volume_bounding_box.get_extent()
    # o3d.visualization.draw_geometries([pcd, min_volume_bounding_box])
    min_volume_bounding_box_mesh = o3d.geometry.TriangleMesh.create_from_oriented_bounding_box(min_volume_bounding_box)
    return (min_volume_bounding_box, min_volume_bounding_box_mesh)

# mean radius method
def get_avg_sphere_2(pcd: o3d.geometry.PointCloud) -> tuple[o3d.geometry.TriangleMesh, float]:
    """
    Returns the sphere that best approximates the point cloud (the sphere with radius equal to the average distance from the center) 
    
    Arguments: 
        pcd: The point cloud to be approximated

    Returns:
        (sphere, err_radius): The best-approximation sphere as a TriangleMesh and the error in the approximation respectively

    """
    center = pcd.get_center()
    points = np.asarray(pcd.points)
    r = np.zeros(len(points))
    for i in range(len(r)):
        point = points[i, :]
        r[i] = np.sqrt((point[0] - center[0])**2 + (point[1] - center[1])**2 + (point[2] - center[2])**2)

    radius = np.mean(r)
    err_radius = np.std(r)

    sphere = o3d.geometry.TriangleMesh.create_sphere(radius)
    sphere = sphere.translate(center, relative=False)
    return (sphere, err_radius)    

# def get_min_sphere(pcd: o3d.geometry.PointCloud) -> o3d.geometry.TriangleMesh:
#     #radius is max distance from 'center' - is that a centroid (mean)? or some other kind of center?
#     center = pcd.get_center()
#     points = np.asarray(pcd.points)
#     max_dist = 0
#     for point in points:
#         dist = np.sqrt((point[0] - center[0])**2 + (point[1] - center[1])**2 + (point[2] - center[2])**2)
#         if (dist > max_dist):
#             max_dist = dist
    
#     sphere = o3d.geometry.TriangleMesh.create_sphere(max_dist)
#     sphere = sphere.translate(center, relative=False)
#     return sphere

def get_xy_boundary_curve_of_pcd(pcd: o3d.geometry.PointCloud):
    """
    Returns a set of points along the boundary of the point cloud 'flattened' to the xy plane (the point cloud as seen from above)

    Arguments: 
        pcd: The point cloud to find the boundary of

    Returns:
        (pcd_boundary, (boundary_points, pcd_flat_center), pcd_flat): 
            pcd_boundary: the o3d.geometry.PointCloud object of the boundary of the flattened point cloud
            boundary_points: the (n, 3) np array containing coordinates of the points on the boundary of the flattened point cloud
            pcd_flat_center: the (1, 3) np array of the coordinates of the center point of the flattened point cloud
            pcd_flat: the o3d.geometry.PointCloud object of the flattened point cloud
    
    """
    # Get the np array of pcd points 
    points = np.asarray(pcd.points)
    # Copy the array of points
    points_flat = points.copy()
    # Set z coordinate of all copied points to 0
    points_flat[:, 2] = 0

    # Define new pcd with flattened points
    pcd_flat = o3d.geometry.PointCloud()
    pcd_flat.points = o3d.utility.Vector3dVector(points_flat)

    # Find the point on the flat pcd closest to the center 
    pcd_flat_center = pcd_flat.get_center()
    pcd_flat_center_point = points_flat[0]
    min_dist = np.linalg.norm(pcd_flat_center_point - pcd_flat_center)
    for point in points_flat:
        dist = np.linalg.norm(point - pcd_flat_center)
        if dist < min_dist:
            min_dist = dist
            pcd_flat_center_point = point

    radii = np.zeros(len(points_flat))
    thetas = np.zeros(len(points_flat))

    # Get the polar coord representation about the center point 
    # The theta values obtained from arctan2 are from -pi to pi
    for i in range(len(points_flat)):
        point = points_flat[i, :]
        x_coord = point[0] - pcd_flat_center_point[0]
        y_coord = point[1] - pcd_flat_center_point[1]
        radii[i] = np.linalg.norm(point - pcd_flat_center_point)
        thetas[i] = np.arctan2(y_coord, x_coord) * (180 / np.pi)

    boundary_points = np.array([[0,0,0]])         

    polar_axis = np.linspace(-180, 180, 150)
    for i in range(len(polar_axis)-1):
        theta_lower = polar_axis[i]
        theta_upper = polar_axis[i+1]
        mask = ((thetas > theta_lower) & (thetas < theta_upper))

        if (not mask.any()):
            continue
        else:
            ray_points = points_flat[mask]
            ray_radii = radii[mask]
            max_r = ray_radii[0] 
            boundary_point = ray_points[0]
            for j in range(len(ray_points)):
                if ray_radii[j] > max_r:
                    max_r = ray_radii[j]
                    boundary_point = ray_points[j]

            boundary_points = np.append(boundary_points, np.array([boundary_point]), axis=0) 
        
    boundary_points = boundary_points[1:]
    
    pcd_boundary = o3d.geometry.PointCloud()
    pcd_boundary.points = o3d.utility.Vector3dVector(boundary_points)            

    return (pcd_boundary, (boundary_points, pcd_flat_center), pcd_flat)


def find_closest_to_center_point(pcd: o3d.geometry.PointCloud) -> tuple[np.ndarray, int]:
    """
    Find the point on a pcd closest to its center

    Arguments: 
        pcd: The point cloud to find the point closest to the center of

    Returns:
        pcd_center_point: the (1, 3) np array with the coordinates of the point closest to the center
        center_i: the int index of the point closest to the center in np.asarray(pcd.points)
         

    """
    pcd_center = pcd.get_center()
    pcd_center_point = np.asarray(pcd.points)[0]
    min_dist = np.linalg.norm(pcd_center_point - pcd_center)
    center_i = 0
    for i in range(len(np.asarray(pcd.points))):
        point = np.asarray(pcd.points)[i]
        dist = np.linalg.norm(point - pcd_center)
        if dist < min_dist:
            min_dist = dist
            pcd_center_point = point
            center_i = i
    return pcd_center_point, center_i
    


