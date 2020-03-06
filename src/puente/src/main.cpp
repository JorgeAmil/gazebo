#include <ros/ros.h>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui/highgui.hpp>
#include <geometry_msgs/TwistStamped.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

using namespace std;
using namespace sensor_msgs;
using namespace message_filters;

void callback(const sensor_msgs::ImageConstPtr& msg, const geometry_msgs::TwistStampedConstPtr &image)
{
  std::cout << msg->header.stamp << ", " << image->header.stamp << "\n";
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;
  message_filters::Subscriber<sensor_msgs::Image> sub_1_;
  message_filters::Subscriber<geometry_msgs::TwistStamped> sub_2_;
  //ros::Subscriber sub = n.subscribe("chatter", 1000, callback);

  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, geometry_msgs::TwistStamped> MySyncPolicy;
  typedef message_filters::Synchronizer<MySyncPolicy> Sync;
  boost::shared_ptr<Sync> sync_;

/*
  message_filters::Subscriber<sensor_msgs::Image> image_sub(n1, "robot1/camera/rgb/image_raw", 1);
  message_filters::Subscriber<geometry_msgs::TwistStamped> info_sub(n2, "chatter", 1);
  TimeSynchronizer<Image, geometry_msgs::TwistStamped> sync(image_sub, info_sub, 10);
  sync.registerCallback(boost::bind(&callback, _1, _2));
*/

    sub_1_.subscribe(nh, "robot1/camera/rgb/image_raw", 1);
    sub_2_.subscribe(nh, "chatter", 1);
    sync_.reset(new Sync(MySyncPolicy(10), sub_1_, sub_2_));
    sync_->registerCallback(boost::bind(callback, _1, _2));

  ros::spin();

  return 0;
}