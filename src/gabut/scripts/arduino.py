#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Joy
import serial
import time

def callback(data):
    throttle = data.buttons[0] # triangle
    right = data.buttons[1] # circle
    no = data.buttons[2] # cross
    left = data.buttons[3] # square

    if throttle == 1:
        ser.write('A')
    elif right == 1:
        ser.write('B')
    elif left == 1:
        ser.write('C')
    elif no == 1:
        ser.write('D')
    else:
        ser.write('E')
    time.sleep(0.01)

    
def listener():

    rospy.init_node('arduino', anonymous=True)

    rospy.Subscriber("joy", Joy, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    global ser
    #try:
    #	ser = serial.Serial('/dev/ttyUSB0', 9600)
    #except:
    #    ser = serial.Serial('/dev/ttyUSB1', 9600)
    ser = serial.Serial('/dev/ttyUSB0', 9600)
    listener()
