#include <ros/ros.h>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui/highgui.hpp>
#include <geometry_msgs/TwistStamped.h>

using namespace std;


void imageCallback(const geometry_msgs::TwistStampedPtr &image)
{
    std::cout<< " x: " << image->twist.linear.x << "\n";
 }

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("chatter", 1000, imageCallback);
  ros::spin();

  return 0;
}