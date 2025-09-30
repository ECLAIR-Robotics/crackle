#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <octomap/octomap.h>
#include <octomap_msgs/msg/octomap.hpp>
#include <octomap_msgs/conversions.h>

class OctoMapping : public rclcpp::Node
{
public:
    OctoMapping()
    : Node("octo_mapping")
    {
        tree = std::make_shared<octomap::OcTree>(0.1);

        sub_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "/camera/camera/depth/camera_info", 10, std::bind(&OctoMapping::pointCloudCallback, this, std::placeholders::_1)
        );
        pub_ = this->create_publisher<octomap_msgs::msg::OctoMap>("/octomap", 10);
    } 

private:
    void pointCloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg){
        std::vector<octomap::point3d> points;
        for(sensor_msgs::PointCloud2ConstIterator<float> it_x(*msg, "x"), it_y(*msg, "y"), it_z(*msg, "z");
            it_x != it_x.end();
            ++it_x, ++it_y, ++it_z)
        {
            if(!std::isnan(*it_x) && !std::isnan(*it_y) && !std::isnan(*it_z)){
                points.emplace_back(*it_x, *it_y, *it_z);
            }
        }

        if(points.empty()){
            RCLCPP_WARN(this->get_logger(), "No valid points in cloud");
            return;
        }

        octomap::point3d origin(0.0f, 0.0f, 0.0f);

        tree->insertPointCloud(points, origin);

        tree->writeBinary("octomap.bt");
        RCLCPP_INFO(this->get_logger(), "Octomap updated and saved to octomap.bt");

        octomap_msgs::msg::Octomap map_msg;
        map_msg.header.stamp = this->get_clock()->now();
        map_msg.header.frame_id = "map";

        if(octomap_msgs::fullMapToMsg(*tree_, map_msg)){
            pub_->publish(map_msg);
            RCLCPP_INFO(this->get_logger(), "Published Octomap to /octomap");
        } else {
            RCLCPP_WARN(this->get_logger(), "Failed to convert OcTree to Octomap");
        }
        
    }

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_;
    rclcpp::Publisher<octomap_msgs::msg::OctoMap>::SharedPtr pub_;
    std::shared_ptr<octomap::OcTree> tree_;
};

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<OctoMapping>();
    rclcpp::spin(node);
    rclcpp:;shutdown();
    return 0;
}