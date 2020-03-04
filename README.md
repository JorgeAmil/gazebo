# gazebo
```sh
git clone --recursive https://github.com/JorgeAmil/gazebo.git

catkin_make

roslaunch turtlebot_gazebo_multiple create_multi_robot.launch

rosrun send_velocity_commands send_velocity_commands_node

rosrun listener listener
```

# gazebo old
```sh
git clone https://github.com/JorgeAmil/gazebo.git

cd ~/gazebo/src

git clone https://github.com/turtlebot/turtlebot_simulator

cd ~/gazebo/doc

unzip turtlebot_gazebo_multiple.zip -d ~/gazebo/src/

cd ..
```

* [E1] - Si no funciona el comando anterior desde "~/gazebo/src/" escribir "catkin_init_workspace"
* [E2] - En cada nueva terminal desde "~/gazebo/" escribir "source devel/setup.bash" y despues

catkin_make

roslaunch turtlebot_gazebo_multiple create_multi_robot.launch

rosrun send_velocity_commands send_velocity_commands_node

rosrun listener listener

Para usar los modulos con tensorflow lanzar:

source ~/tensorflow/bin/activate

donde tensorflow es sustituido por el nombre del entorno virtual de python con el que se instala tensorflow

http://projectsfromtech.blogspot.com/2017/10/visual-object-recognition-in-ros-using.html
https://github.com/osrf/tensorflow_object_detector/tree/master/src/object_detection

