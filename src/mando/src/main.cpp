#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

class TeleopTurtle
{
public:
  TeleopTurtle();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  ros::NodeHandle nh_;

  int linear_, angular_;
  double l_scale_, a_scale_;
  ros::Publisher vel_pub_;
  ros::Subscriber joy_sub_;

  geometry_msgs::Twist twist;
};


TeleopTurtle::TeleopTurtle():
  linear_(1),
  angular_(2)
{
  twist.linear.y = twist.linear.x = twist.angular.z = 0;
  nh_.param("axis_linear", linear_, linear_);
  nh_.param("axis_angular", angular_, angular_);
  nh_.param("scale_angular", a_scale_, a_scale_);
  nh_.param("scale_linear", l_scale_, l_scale_);
  std::cout << "axis_linear" << linear_
 << "axis_angular"  << angular_
 << "scale_angular"  << a_scale_
 << "scale_linear"  << l_scale_ << "\n";

  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/robot1/mobile_base/commands/velocity", 1);
  
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopTurtle::joyCallback, this);
}

void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  twist.angular.z = /*a_scale_**/joy->axes[angular_];
  twist.linear.x =  /*l_scale_**/joy->axes[linear_];
  std::cout <<"a: "<< twist.angular.z  << "l: " << twist.linear.x << "\n";

  vel_pub_.publish(twist);
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_turtle");
  TeleopTurtle teleop_turtle;

  ros::spin();
}