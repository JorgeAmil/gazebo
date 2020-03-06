#include <ros/ros.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>

#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>
#include <iostream>


using namespace sensor_msgs;
using namespace message_filters;

void callback(const ImageConstPtr& image, const geometry_msgs::TwistStamped::ConstPtr& vel)
{
  std::cout << "entra\n";
 ROS_INFO("Synchronization successful");
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "vision_node");

  ros::NodeHandle nh;

  message_filters::Subscriber<Image> image_sub(nh, "image", 1);
  message_filters::Subscriber<geometry_msgs::TwistStamped> info_sub(nh, "velocity", 1);
  
  TimeSynchronizer<Image, geometry_msgs::TwistStamped> sync(image_sub, info_sub, 10);
  sync.registerCallback(boost::bind(&callback, _1, _2));

  ros::spin();

  return 0;
}
