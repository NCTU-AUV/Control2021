#! /usr/bin/env python3

import rospy
from std_msgs.msg import String, Float64MultiArray

import math
import time
#from control_pkg.srv import PidControl, PidControlResponse
#from msg import tracking_command
#import attitude_pid
#import depth_pid

class Controller():
    def __init__(self, command = 'stop'):
        rospy.init_node('Controller', anonymous=True)
        self.pub_motor = rospy.Publisher('command_force', Float64MultiArray, queue_size=10)

        self.command = command
        self.motor = [0.0]*6
        
        print('ready to controll vehicle')

        self.listener_joystick()
        self.listener_CV()
    
    def listener_joystick(self):
        rospy.Subscriber('cmd', String, self.callback)
        rospy.spin()
    
    def listener_CV(self):
        rospy.Subscriber('tracking_command', tracking_command, self.callback_CV)
        rospy.spin()

    def callback(self, data):
        self.command = data.data
        self.switch(data.data)
        self.talker()

    def callback_CV(self, data):
        self.command = data.yaw_cmd

    def switch(self, command):
        return{
            'stop': self.stop,
            'up': self.move_vertical,
            'down': self.move_vertical,
            'forward': self.move_horizontal,
            'backward': self.move_horizontal,
            'left': self.move_horizontal,
            'right': self.move_horizontal
        }[command](command)

    def stop(self, command):
        for i in range(4,6):
            if self.motor[i] > 0.2:
                self.motor[i] -= 0.1
            elif self.motor[i] < -0.2:
                self.motor[i] += 0.1
            else:
                self.motor[i] = 0

    
    def move_vertical(self, command):
        if command == 'up':
            for i in range(4):
                self.motor[i] -= 0.1
        elif command == 'down':
            for i in range(4):
                self.motor[i] += 0.1
    def move_horizontal(self, command):
        if command == 'forward' or command == 'right':
             self.motor[4] = 1
        else:
             self.motor[4] = -1
        if command == 'forward' or command == 'left':
            self.motor[5] = 1
        else:
            self.motor[5] = -1

    def talker(self):
        rospy.loginfo(self.motor)
        self.pub_motor.publish(Float64MultiArray(data = self.motor))
    
if __name__ == '__main__':
    controller = Controller()
