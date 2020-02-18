# gazebo

git clone https://github.com/JorgeAmil/gazebo.git

cd ~/gazebo/src

git clone https://github.com/turtlebot/turtlebot_simulator

cd ~/gazebo/doc

unzip turtlebot_gazebo_multiple.zip -d ~/gazebo/src/turtlebot_simulator/

cd ..

catkin_make

Si no funciona el comando anterior desde ~/gazebo/src/ escribir catkin_init_workspace

En cada nueva terminal desde ~/gazebo/ escribir source devel/setup.bash y despues

roslaunch turtlebot_gazebo_multiple create_multi_robot.launch

rosrun send_velocity_commands send_velocity_commands_node

rosrun listener listener

