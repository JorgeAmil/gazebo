#include <stdio.h>
#include <string>
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>
// procesar imagen
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <message_filters/subscriber.h>
// synchronizer
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
// 
#include <geometry_msgs/TwistStamped.h>

const std::string rutaImagenes = "/home/jorge/gazebo/imagen_data/";
const char* rutaMap = "/home/jorge/gazebo/imagen_data/imagen_twistCallocidad.data";

int contadorDatos = 0; 

void callback(const sensor_msgs::ImageConstPtr& imageCall, const geometry_msgs::TwistStampedConstPtr& twistStamped)
{
  // write : counter x y z
  /*
  FILE* imagenDataFile;
  
  imagenDataFile = fopen( rutaMap, "a" );

  fprintf( imagenDataFile, "%d %f %f\n",
    contadorDatos, twistStamped->twist.linear.x, twistStamped->twist.linear.z
  fprintf( imagenDataFile, "%u %f\n",
    contadorDatos, joy->axes[3]
    );
  //fprintf( imagenDataFile, "%u %f %f\n", contadorDatos, joy->axes[1] + joy->axes[0], joy->axes[3]);
  fclose( imagenDataFile );
  */

  // encode imageCall
  cv_bridge::CvImageConstPtr cv_ptr;
  try {
    cv_ptr = cv_bridge::toCvShare(imageCall, sensor_msgs::image_encodings::BGR8);
  }
  catch (cv_bridge::Exception& e) {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", imageCall->encoding.c_str());
  }

  if ( twistStamped->twist.angular.z == 0 ) {
    cv::imwrite( rutaImagenes + "avanzar/" + std::to_string(contadorDatos) + ".jpg", cv_ptr->image);
    std::cout << "[AVANZAR]";
  } else if ( twistStamped->twist.angular.z > 0 ) {
    cv::imwrite( rutaImagenes + "izquierda/" + std::to_string(contadorDatos) + ".jpg", cv_ptr->image);
    std::cout << "[IZQUIERDA]";
  } else {
    cv::imwrite( rutaImagenes + "derecha/" + std::to_string(contadorDatos) + ".jpg", cv_ptr->image);
    std::cout << "[DERECHA]";
  }

  std::cout << 
  " = [x: " << twistStamped->twist.linear.x  <<
  " z: " << twistStamped->twist.angular.z <<
  " stamp: " << twistStamped->header.stamp << "]\n";

  // save image
  //cv::imwrite( rutaImagenes + std::to_string(contadorDatos) + ".jpg", cv_ptr->image);

  std::cout << contadorDatos << "\n";

  ++contadorDatos;

  
}

/*
Al crear una instancia se subscribe al nodo que envia
el TwistStamp y a la imagen del turtlebot.
La sync_policies sirve para que al comparar los timestamp
se haga de forma aproximada.
*/
class ImageConverter
{
private:
  ros::NodeHandle nh;
  message_filters::Subscriber<sensor_msgs::Image> sub_image;
  message_filters::Subscriber<geometry_msgs::TwistStamped> sub_twist;
  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, geometry_msgs::TwistStamped> MySyncPolicy;
  typedef message_filters::Synchronizer<MySyncPolicy> Sync;
  boost::shared_ptr<Sync> sync_;

public:
  ImageConverter()
  {
    sub_image.subscribe(nh, "robot1/camera/rgb/image_raw", 1);
    sub_twist.subscribe(nh, "chatter", 1);
    sync_.reset(new Sync(MySyncPolicy(10), sub_image, sub_twist));
    sync_->registerCallback(boost::bind(callback, _1, _2));
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}