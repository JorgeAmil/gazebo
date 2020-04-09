# clonar y compilar
```sh
git clone --recursive https://github.com/JorgeAmil/gazebo.git
catkin_make
```

# packages
send_velocity_commands
```sh
Dado un caracter (w, a, d) envia un mensaje de tipo Twist al robot
Además publica un mensaje de tipo TwistStamped al nodo "chatter" para
que pueda ser obtenido por otro nodo
```
mando
```sh
Misma funcion al send_velocity_commands pero con el mando de la ps3
se suscribe al nodo joy del cual obtiene las movimientos del mando
además publica el mensaje de tipo Joy al nodo "chatter"
```
puente
```sh
Permite crear el dataset.
Se suscribe a la camara del turtlebot y al nodo chatter de forma sincronizada.
Con la politica aplicada los timestamp son aproximados.
```
autopilot
```sh
rosrun autopilot run.py
el archivo run.py se subscribe a la camara del turtlebot, dada la imagen 
obtenida llama al modelo para calcular la predicción del angulo a girar 
```
# tensorflow
Para usar los modulos con tensorflow lanzar:
```sh
source ~/tensorflow/bin/activate
```
donde tensorflow es sustituido por el nombre del entorno virtual de python con el que se instala tensorflow
```sh
http://projectsfromtech.blogspot.com/2017/10/visual-object-recognition-in-ros-using.html
https://github.com/osrf/tensorflow_object_detector/tree/master/src/object_detection
```

# ps3joy
Configurar PS3Joy http://wiki.ros.org/ps3joy/Tutorials
https://wiki.ros.org/joy/Tutorials
http://library.isr.ist.utl.pt/docs/roswiki/ps3joy.html
```sh
roslaunch turtlebot_gazebo_multiple create_multi_robot.launch
rosrun joy joy_node
rostopic echo joy
rosrun mando mando
rosrun puente puente
```

# calibrar
rosrun camera_calibration cameracalibrator.py --approximate 0.1 --size 8x6
--square 0.108 right:=/robot1/trasera2/trasera2/rgb/image_raw
left:=/robot1/trasera1/trasera1/rgb/image_raw right_camera:=/robot1/trasera2/
left_camera:=/robot1/trasera1
