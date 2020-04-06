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

# Ejecutar
```sh
Seguir el tutorial para configurar PS3Joy http://wiki.ros.org/ps3joy/Tutorials
```
```sh
catkin_make
roscore
roslaunch turtlebot_gazebo_multiple create_multi_robot.launch
rosrun joy joy_node
rosrun mando mando
rosrun puente puente

```
# joy
Tutorial instalacion
```sh
https://wiki.ros.org/joy/Tutorials
http://library.isr.ist.utl.pt/docs/roswiki/ps3joy.html
```
Una vez detecta el mando levantar el nodo
```sh
rosrun joy joy_node
```
Mostrar valores del nodo
```sh
rostopic echo joy
```

# Teclado
```sh
catkin_make
roslaunch turtlebot_gazebo_multiple create_multi_robot.launch
rosrun send_velocity_commands send_velocity_commands_node
rosrun listener listener
rosrun puente puente
```
# calibrar

rosrun camera_calibration cameracalibrator.py --approximate 0.1 --size 8x6
--square 0.108 right:=/robot1/trasera2/trasera2/rgb/image_raw
left:=/robot1/trasera1/trasera1/rgb/image_raw right_camera:=/robot1/trasera2/
left_camera:=/robot1/trasera1
