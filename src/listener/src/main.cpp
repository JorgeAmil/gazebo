#include <ros/ros.h>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui/highgui.hpp>

using namespace std;

static const string OPENCV_WINDOW = "view";

class RobotSense {
  ros::NodeHandle nh;
  image_transport::ImageTransport it;
  image_transport::Subscriber sub;

public:
  RobotSense() : it(nh) {
    sub = it.subscribe("robot1/camera/rgb/image_raw", 1, &RobotSense::imageCallback, this);
    cv::namedWindow(OPENCV_WINDOW);
    cv::startWindowThread();
  }

  ~RobotSense() {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
    cv_bridge::CvImageConstPtr cv_ptr;
    try {
      cv_ptr = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e) {
      ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(30);
  }

  void ver() {
    ros::Rate rate(10.0);
    while(ros::ok()) {
      ros::spinOnce();
      rate.sleep();
    }
    ros::spin();
    ros::shutdown();
  }
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "image_listener");
  RobotSense vista;
  vista.ver();
  return 0;
}