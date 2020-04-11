#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <sensor_msgs/Image.h>

using namespace std;
using namespace cv;
using namespace sensor_msgs;

void imageCallback(const ImageConstPtr& trasera1, const ImageConstPtr& trasera2) {
  try {
    cout << "matrices" << endl;
    cv::Mat matriz1, salida1;
    matriz1 = cv_bridge::toCvShare(trasera1, "mono8")->image;
    cv::Mat matriz2, salida2;
    matriz2 = cv_bridge::toCvShare(trasera2, "mono8")->image;
    //cout << "ajuste" << endl;
    //cv::cvtColor(matriz1, salida1, CV_8UC1);
    //cv::cvtColor(matriz2, salida2, CV_8UC1);
    cout << "disparidad" << endl;
    cv::Mat disparity_out;
    cout << "compute" << endl;
    Ptr< StereoBM > match = StereoBM::create();
    match->compute(matriz1, matriz2, disparity_out);
//    match->compute(salida1, salida2, disparity_out);
    cout << "show" << endl;
    imshow("test",disparity_out * 16);
    cout << "wait" << endl;
    cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e) {

  }
}

int main(int argc, char **argv) {
  cout << "inicio" << endl;
  ros::init(argc, argv, "moduloStereo_node");
  ros::NodeHandle nh;
  cout << "hilo" << endl;
  cv::namedWindow("view");
  cv::startWindowThread();
  cout << "camaras" << endl;
  message_filters::Subscriber<Image> trasera1(nh, "robot1/trasera1/trasera1/rgb/image_raw", 1);
  message_filters::Subscriber<Image> trasera2(nh, "robot1/trasera2/trasera2/rgb/image_raw", 1);
  cout << "sincro" << endl;
  message_filters::TimeSynchronizer<Image, Image> sincronizador(trasera1, trasera2, 2);
  sincronizador.registerCallback(boost::bind(&imageCallback, _1, _2));
  cout << "bucle" << endl;
  ros::Rate rate(10.0);
  while(nh.ok()) {
    ros::spinOnce();
    rate.sleep();
  }
  ros::spin();
  ros::shutdown();
  cv::destroyWindow("view");
}
