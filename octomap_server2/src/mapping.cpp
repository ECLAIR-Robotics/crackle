#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <octomap/octomap.h>
#include <octomap_msgs/msg/octomap.hpp>
#include <octomap_msgs/conversions.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>

class OctoMapping : public rclcpp::Node
{
public:
    std::shared_ptr<octomap::OcTree> tree;
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_;
    rclcpp::Publisher<octomap_msgs::msg::Octomap>::SharedPtr pub_;
    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::string target = "camera_depth_optical_frame";
    std::string source = "link_base";

    OctoMapping()
        : Node("octo_mapping"),
          tf_buffer_(this->get_clock()),
          tf_listener_(tf_buffer_)
    {
        tree = std::make_shared<octomap::OcTree>(0.1);
        sub_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "/camera/camera/depth/color/points", 10, std::bind(&OctoMapping::pointCloudCallback, this, std::placeholders::_1));

        pub_ = this->create_publisher<octomap_msgs::msg::Octomap>("/octomap", 10);
        
        try
        {
            // Optional: wait until transform is available (non-throwing)
            if (!tf_buffer_.canTransform(target, source, rclcpp::Time(0), tf2::durationFromSec(0.2)))
            {
                RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 5000,
                                     "Waiting for transform %s -> %s", target.c_str(), source.c_str());
                return;
            }
        }
        catch (const std::exception &e)
        {
            RCLCPP_WARN(this->get_logger(), "Exception while checking transforms: %s", e.what());
        }
    }

private:
    void pointCloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
    {

        std_msgs::msg::Header header = msg->header;

        std::vector<octomap::point3d> points;
        for (sensor_msgs::PointCloud2ConstIterator<float> it_x(*msg, "x"), it_y(*msg, "y"), it_z(*msg, "z");
             it_x != it_x.end();
             ++it_x, ++it_y, ++it_z)
        {
            if (!std::isnan(*it_x) && !std::isnan(*it_y) && !std::isnan(*it_z))
            {
                points.emplace_back(*it_x, *it_y, *it_z);
            }
        }

        if (points.empty())
        {
            RCLCPP_WARN(this->get_logger(), "No valid points in cloud");
            return;
        }

        // Convert points to a octomap::PointCloud
        octomap::Pointcloud pcl;
        for (octomap::point3d point: points) {
            pcl.push_back(point);
        }

        octomap::point3d sensor_origin;
        geometry_msgs::msg::TransformStamped tf =
            tf_buffer_.lookupTransform(target, source, rclcpp::Time(0)); // latest

        const auto &t = tf.transform.translation;
        const auto &q = tf.transform.rotation;

        RCLCPP_INFO(get_logger(),
            "Pose of %s in %s: p = [%.3f, %.3f, %.3f], q = [%.3f, %.3f, %.3f, %.3f]",
            source.c_str(), target.c_str(),
            t.x, t.y, t.z, q.x, q.y, q.z, q.w);
        octomap::pose6d origin(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

        tree->insertPointCloud(pcl, sensor_origin, origin);

        tree->writeBinary("octomap.bt");
        RCLCPP_INFO(this->get_logger(), "Octomap updated and saved to octomap.bt");

        octomap_msgs::msg::Octomap map_msg;
        map_msg.header.stamp = this->get_clock()->now();
        map_msg.header.frame_id = "map";

        octomap_msgs::msg::Octomap oct_msg;
        // oct_msg.data = octomap_msgs::fullMapToMsgData(tree, )

        if (octomap_msgs::fullMapToMsg(*tree, map_msg))
        {
            pub_->publish(map_msg);
            RCLCPP_INFO(this->get_logger(), "Published Octomap to /octomap");
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "Failed to convert OcTree to Octomap");
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<OctoMapping>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}