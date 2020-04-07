#!/usr/bin/env python

# links de referencia
# http://projectsfromtech.blogspot.com/2017/10/visual-object-recognition-in-ros-using.html
# https://github.com/kungfrank/ros_inception_v3/blob/master/classify_image.py
# https://www.pyimagesearch.com/2019/01/21/regression-with-keras/

from __future__ import print_function
import roslib
roslib.load_manifest('thinker')
import rospy
import cv2
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from geometry_msgs.msg import Twist
import numpy as np
from tensorflow.python.keras.applications.mobilenet import MobileNet
from tensorflow.python.keras.models import Model
from tensorflow.python.keras.layers import Dense, Input
from tensorflow.python.keras import backend as K

tam_entrada = (224, 224, 3)
tensor_entrada = Input(shape=tam_entrada)
K.set_image_data_format('channels_last')

def obtenerModelo(tam_entrada):
    modelo_preentrenado = MobileNet(input_shape=tam_entrada,
                                input_tensor=tensor_entrada,
                                pooling='avg',
                                include_top=False,
                                weights=None)
    salida = modelo_preentrenado.output
    prediccion = Dense(3, activation="softmax")(salida)
    
    modelo_modificado = Model(inputs=tensor_entrada, outputs=prediccion)
    return modelo_modificado

modelo = obtenerModelo(tam_entrada)

class RosTensorFlow:
  def __init__(self):
    self.bridge = CvBridge()
    self.image_sub = rospy.Subscriber("/robot1/camera/rgb/image_raw",Image, self.imageCallback)
    self.cmd_vel = rospy.Publisher('/robot1/mobile_base/commands/velocity', Twist, queue_size=1)
    self.lastImage = None
    self.process = False

  def imageCallback(self,data):
    try:
      imagen_opencv = cv2.resize(self.bridge.imgmsg_to_cv2(data, "rgb8"), (224, 224), interpolation = cv2.INTER_CUBIC)
      self.lastImage = imagen_opencv
      self.process = True
    except CvBridgeError as excepcion:
      print(excepcion)

  def run(self):
    move_cmd = Twist()
    move_cmd.linear.x = 0
    move_cmd.angular.z = 0
    while True:
      if self.process:
        ImageList_ = []
        ImageList_.append(np.asarray(self.lastImage).astype('float32')/255)
        predicciones = modelo.predict(np.asarray(ImageList_).reshape(1, 224, 224, 3))
        cv2.imshow("Image window", self.lastImage)
        cv2.waitKey(3)
        move_cmd.linear.x = move_cmd.linear.y = move_cmd.angular.z = 0;   
        print(predicciones[-1])
        if (np.argmax(predicciones[-1])) == 0:
          move_cmd.angular.z += 0.0
          move_cmd.linear.x += 0.25
        elif (np.argmax(predicciones[-1])) == 2:
          move_cmd.angular.z += -0.75
          move_cmd.linear.x += 0.25
        else:
          move_cmd.angular.z += 0.75
          move_cmd.linear.x += 0.25

      self.cmd_vel.publish(move_cmd)
    cv2.destroyAllWindows()

def main():
  modelo.compile(loss='mse', optimizer='adam', metrics=['accuracy'])
  modelo.load_weights('./ros.hdf5')
  navegacion = RosTensorFlow()
  rospy.init_node('robotia', anonymous=True)
  navegacion.run()

if __name__ == '__main__':
	main()