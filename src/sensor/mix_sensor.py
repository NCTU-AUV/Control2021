#! /usr/bin/env python

import serial
import glob
import rospy
import threading
from std_msgs.msg import Float64MultiArray, Float64
from sensor_msgs.msg import Imu
import time
import random
import math
from struct import unpack
from tf.transformations import euler_from_quaternion

#DEPTH_OFFSET = 10

class IMUAttitude:
    def __init__(self):
        #self.arduino_port = glob.glob('/dev/ttyACM1')[0]       
        
        #self.arduino = serial.Serial(self.arduino_port, 115200, timeout=1)
        self.arduino = serial.Serial('/dev/ttyACM0', 115200, timeout=1)

        while not self.arduino.is_open:
            self.arddfsdfuino.open()
            print("fail to open the arduino")

        rospy.on_shutdown(self.shutdown)
        rospy.init_node('Sensor_arduino', anonymous=True)

        #For PID
        self.arr_pub = rospy.Publisher('sensor_input_error', Float64MultiArray, queue_size=10) #[roll, pitch, yaw]
        self.depth_pub = rospy.Publisher('Depth', Float64, queue_size=10) 

        self.imu_t = threading.Thread(target=self.get_data)
        #self.imu_t = threading.Thread(target=self.get_data, daemon=True)
        self.imu_t.start()

        #self.rate = rospy.Rate(100)

        rospy.spin()
    
    def get_data(self):
        data = [0.1] * 7

        while self.arduino.is_open:
            try:
                raw_data = self.arduino.readline()
#                print(type(raw_data))
#                print('arduino raw data: ')
#                print(raw_data)
#                data = unpack('ffffc', raw_data)
#                print (data)
#                print (type(data))
                data = unpack('ffffffc', raw_data) 
#                print (data)
               
            except Exception as e:
#                print('oops')
#                print(e)
                 time.sleep(0.0001)
                #print(attitude)
           
            #attitude = euler_from_quaternion(data[0:4])
            #attitude = [x * 180 / math.pi for x in attitude] 
            attitude = list(data[0:6])
            pressure = data[3]
            
#            roll = attitude[0]
#            if roll > 0:
#                roll = roll - 180
#            elif roll < 0:
#                roll = roll + 180
#            attitude[0] = roll

            #roll, pitch, yaw
            for i in range(6):
                attitude[i] = round(attitude[i], 4)
#            pressure = round((pressure - 197.4) * 100 /12.5, 2)
#            print data
            #print "IMU: ",
            print (attitude)
            #print "  Depth: ",
            #print(pressure)
            self.arr_pub.publish(Float64MultiArray(data=attitude))
            self.depth_pub.publish(pressure)

            #self.rate.sleep() #100 hz

    def shutdown(self):
        self.arduino.close()
        print('\nbye')

def main():
    IMUAttitude()

if __name__ == '__main__':
    main()
