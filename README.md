# gazebo

git clone https://github.com/JorgeAmil/gazebo.git

cd ~/gazebo/src

catkin_init_workspace

git clone https://github.com/turtlebot/turtlebot_simulator

cd ~/gazebo/doc

unzip turtlebot_gazebo_multiple.zip -d ~/gazebo/src/turtlebot_simulator/

cd ..

catkin_make

