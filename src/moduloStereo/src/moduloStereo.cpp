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
    matriz1 = cv_bridge::toCvShare(trasera1, "mono8")->image;//mono8
    cv::Mat matriz2, salida2;
    matriz2 = cv_bridge::toCvShare(trasera2, "mono8")->image;//bgr8
    //cout << "ajuste" << endl;
    //cv::cvtColor(matriz1, salida1, CV_8UC1);
    //cv::cvtColor(matriz2, salida2, CV_8UC1);
    cout << "disparidad" << endl;
    cv::Mat disparidad;
    cout << "compute" << endl;
//    Ptr<StereoBM> emparejador = StereoBM::create();
    Ptr<StereoSGBM> emparejador = StereoSGBM::create(-55,32,11,120,2000,32,0,15,1000,16,StereoSGBM::MODE_HH);
    emparejador->compute(matriz1, matriz2, disparidad);
//    emparejador->compute(salida1, salida2, disparity_out);
    cout << "normalizacion" << endl;
    Mat disparidad_normalizada;
    normalize(disparidad, disparidad_normalizada, 0, 255, CV_MINMAX, CV_8U);
    cout << "reprojectar" << endl;
    float matriz_transformacion[4][4] = {
      {1,  0, 0, -matriz1.cols / 2.0},
      {0, -1, 0, matriz1.cols / 2.0},
      {0,  0, 0, -0.8 * matriz1.cols},
      {0,  0, 1, 0}
    };
    Mat nube, conversion(4, 4, CV_32FC1, &matriz_transformacion);
    reprojectImageTo3D(disparidad_normalizada, nube, conversion);
    cout << "show" << endl;
    imshow("view",nube * 16);
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
