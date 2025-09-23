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

def get_best_approx(pcd: o3d.geometry.PointCloud) -> Tuple[o3d.geometry.TriangleMesh, int, np.ndarray]:
    """
    Returns the mesh, either a cuboid or sphere, that best approximates the point cloud
    
    Arguments: 
        pcd: Point cloud to be approximated

    Returns:
        shape_mesh: The best-approximation mesh as an o3d.geometry.TriangleMesh object
        shape_type: The type of shape as a shape_msgs.msg.SolidPrimitive type (either
                    SolidPrimitive.BOX or SolidPrimitive.SPHERE)
        quaternion: The orientation of the shape as a (4,) np array representing a quaternion (x, y, z, w)        

    """
    pcd_hull, _ = pcd.compute_convex_hull()
    pcd_hull.compute_vertex_normals()
    # o3d.visualization.draw_geometries([pcd, pcd_hull])

    orientated_bounding_box, min_cuboid = get_min_cuboid(pcd)
    avg_sphere = get_avg_sphere_2(pcd)[0]
    rotation_matrix = orientated_bounding_box.R.copy()
    
    print("Rotation matrix:\n", rotation_matrix)
    z_axis_rotation = R.from_euler('z', float(90), degrees=True)
    rotated_angle = R.from_matrix(rotation_matrix) * z_axis_rotation
    quaternion = rotated_angle.as_quat()
    print("Quaternion (x, y, z, w):", quaternion)

    # Accuracy is being taken as the difference in volume between pcd and approx...
    cuboid_accuracy = np.abs(pcd_hull.get_volume() - min_cuboid.get_volume())
    sphere_accuracy = np.abs(pcd_hull.get_volume() - avg_sphere.get_volume())

    if (cuboid_accuracy < sphere_accuracy):
        min_cuboid.compute_vertex_normals()
        return min_cuboid, SolidPrimitive.BOX, quaternion
    else:
        avg_sphere.compute_vertex_normals()
        return avg_sphere, SolidPrimitive.SPHERE, np.array([0, 0, 0, 1])  # Identity quaternion

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
    


