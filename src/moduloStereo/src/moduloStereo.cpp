#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std;

void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
    try { 
         cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
         cv::waitKey(30);
     }
     catch (cv_bridge::Exception& e) {
         ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
     }
}

int main(int argc, char **argv) {
     ros::init(argc, argv, "moduloStereo_node");
     ros::NodeHandle nh;

     cv::namedWindow("view");
     cv::startWindowThread();
     image_transport::ImageTransport it(nh);
     image_transport::Subscriber sub1 = it.subscribe("robot1/camera/rgb/image_raw", 1, imageCallback);
     image_transport::Subscriber sub2 = it.subscribe("robot1/camera/rgb/image_raw", 1, imageCallback);
     ros::Rate rate(10.0);
     while(nh.ok()) {
       ros::spinOnce();
       rate.sleep();
     }
     ros::spin();
     ros::shutdown();
     cv::destroyWindow("view");
}

/*
/robot1/trasera1/parameter_descriptions
/robot1/trasera1/parameter_updates
/robot1/trasera1/trasera1/depth/camera_info
/robot1/trasera1/trasera1/depth/image_raw
/robot1/trasera1/trasera1/depth/points
/robot1/trasera1/trasera1/rgb/camera_info
/robot1/trasera1/trasera1/rgb/image_raw
/robot1/trasera1/trasera1/rgb/image_raw/compressed
/robot1/trasera1/trasera1/rgb/image_raw/compressed/parameter_descriptions
/robot1/trasera1/trasera1/rgb/image_raw/compressed/parameter_updates
/robot1/trasera1/trasera1/rgb/image_raw/compressedDepth
/robot1/trasera1/trasera1/rgb/image_raw/compressedDepth/parameter_descriptions
/robot1/trasera1/trasera1/rgb/image_raw/compressedDepth/parameter_updates
/robot1/trasera1/trasera1/rgb/image_raw/theora
/robot1/trasera1/trasera1/rgb/image_raw/theora/parameter_descriptions
/robot1/trasera1/trasera1/rgb/image_raw/theora/parameter_updates
*/