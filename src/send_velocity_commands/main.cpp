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
  ros::Publisher cmd_vel_pub_;
  ros::Publisher cmd_stamp_pub_;


public:
  //! ROS node initialization
  RobotDriver(ros::NodeHandle &nh)
  {
    nh_ = nh;
    //set up the publisher for the cmd_vel topic
    ///robot1/mobile_base/commands/velocity
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::TwistStamped>("chatter", 1);
    cmd_stamp_pub_ = nh_stamp_.advertise<geometry_msgs::Twist>("/robot1/mobile_base/commands/velocity", 1);
  }

  //! Loop forever while sending drive commands based on keyboard input
  bool driveKeyboard()
  {
    std::cout << "Type a command and then press enter.  "
      "Use '+' to move forward, 'l' to turn left, "
      "'r' to turn right, '.' to exit.\n";

    //we will be sending commands of type "twist"
    geometry_msgs::TwistStamped base_stamp_cmd;  
    geometry_msgs::Twist base_cmd;  


    char cmd[50];
    while(nh_.ok() && nh_stamp_.ok()){

      std::cin.getline(cmd, 50);
      if(cmd[0]!='+' && cmd[0]!='l' && cmd[0]!='r' && cmd[0]!='.')
      {
        std::cout << "unknown command:" << cmd << "\n";
        continue;
      }
      base_stamp_cmd.twist.linear.x = base_stamp_cmd.twist.linear.y = base_stamp_cmd.twist.angular.z = 0;
      base_stamp_cmd.header.stamp = ros::Time::now();  
      base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;   
    
      //move forward
      if(cmd[0]=='+'){
        base_stamp_cmd.twist.linear.x = base_cmd.linear.x = 0.25;
      } 
      //turn left (yaw) and drive forward at the same time
      else if(cmd[0]=='l'){
        base_stamp_cmd.twist.angular.z = base_cmd.angular.z = 0.75;
        base_stamp_cmd.twist.linear.x = base_cmd.linear.x = 0.25;
      } 
      //turn right (yaw) and drive forward at the same time
      else if(cmd[0]=='r'){
        base_stamp_cmd.twist.angular.z = base_cmd.angular.z = -0.75;
        base_stamp_cmd.twist.linear.x = base_cmd.linear.x = 0.25;
      } 
      //quit
      else if(cmd[0]=='.'){
        break;
      }

      //publish the assembled command
      std::cout << base_stamp_cmd.twist.linear.x  << 
        ",  " << base_stamp_cmd.twist.linear.y  <<
        ",  " << base_stamp_cmd.twist.angular.z << "\n"
        ",  " << base_cmd.linear.x  << 
        ",  " << base_cmd.linear.y  << "\n";
      cmd_vel_pub_.publish(base_stamp_cmd);
      cmd_stamp_pub_.publish(base_cmd);
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
