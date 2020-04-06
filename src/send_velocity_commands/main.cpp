#include <iostream>

#include <ros/ros.h>

#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/Twist.h>


class RobotDriver
{
private:
  //! The node handle we'll be using
  ros::NodeHandle nh_;
  ros::NodeHandle nh_stamp_;
  //! We will be publishing to the "/base_controller/command" topic to issue commands
  ros::Publisher cmd_stamp_pub_;
  ros::Publisher cmd_pub_;
  // teclas
  char teclas_ [4];

public:
  //! ROS node initialization
  RobotDriver(ros::NodeHandle &nh)
  {
    nh_ = nh;
    //set up the publisher for the cmd_vel topic
    ///robot1/mobile_base/commands/velocity
    cmd_stamp_pub_ = nh_.advertise<geometry_msgs::TwistStamped>("chatter", 1);
    cmd_pub_ = nh_stamp_.advertise<geometry_msgs::Twist>("/robot1/mobile_base/commands/velocity", 1);
    teclas_[0] = 'w'; // delante
    teclas_[1] = 'a'; // izquierda
    teclas_[2] = 'd'; // derecha
    teclas_[3] = 'p'; // parar bucle infinito
  }

  //! Loop forever while sending drive commands based on keyboard input
  bool driveKeyboard()
  {
    std::cout << 
      "forward: " << teclas_[0] << 
      " left: "   << teclas_[1] <<
      " right: "  << teclas_[2] <<
      " exit: "   << teclas_[3] << "\n";

    //sending commands type
    geometry_msgs::TwistStamped base_stamp_cmd;  
    geometry_msgs::Twist base_cmd;

    char cmd[50];
    while(nh_.ok() && nh_stamp_.ok()){

      std::cin.getline(cmd, 50);
      if(cmd[0]!=teclas_[0] && cmd[0]!=teclas_[1] && cmd[0]!=teclas_[2] && cmd[0]!=teclas_[3] && cmd[0]!=teclas_[4])
      {
        std::cout << "unknown command:" << cmd << "\n";
        continue;
      }
      base_stamp_cmd.twist.linear.x = base_stamp_cmd.twist.linear.y = base_stamp_cmd.twist.angular.z = 0;
      base_stamp_cmd.header.stamp = ros::Time::now();
      base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;   
    
      //move forward
      if(cmd[0]==teclas_[0]){
        base_stamp_cmd.twist.linear.x = base_cmd.linear.x = 0.25;
      } 
      //turn left (yaw) and drive forward at the same time
      else if(cmd[0]==teclas_[1]){
        base_stamp_cmd.twist.angular.z = base_cmd.angular.z = 0.75;
        base_stamp_cmd.twist.linear.x = base_cmd.linear.x = 0.25;
      } 
      //turn right (yaw) and drive forward at the same time
      else if(cmd[0]==teclas_[2]){
        base_stamp_cmd.twist.angular.z = base_cmd.angular.z = -0.75;
        base_stamp_cmd.twist.linear.x = base_cmd.linear.x = 0.25;
      } 
      //quit
      else if(cmd[0]==teclas_[3]){
        break;
      }

      //publish the assembled command
      std::cout << 
        "x: " << base_stamp_cmd.twist.linear.x  << 
        " y: " << base_stamp_cmd.twist.linear.y  <<
        " z: " << base_stamp_cmd.twist.angular.z <<
        " stamp: " << base_stamp_cmd.header.stamp << "\n";

      cmd_stamp_pub_.publish(base_stamp_cmd);
      cmd_pub_.publish(base_cmd);
    }
    return true;
  }
};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "robot_driver");
  ros::NodeHandle nh;

  RobotDriver driver(nh);
  driver.driveKeyboard();
}
