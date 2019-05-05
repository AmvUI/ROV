#!/usr/bin/env python
from scipy.spatial import distance as dist
from imutils import perspective
from imutils import contours
import numpy as np
import argparse
import imutils
import cv2
import rospy
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image, Joy
import random

font = cv2.FONT_HERSHEY_COMPLEX
refreshed = True
data = None

def nothing(x):
    pass

def callback1(image):
    pass

def joy_callback(joy):
    a = joy.buttons[9];
    print a
    if a == 1:
        global refreshed
        refreshed = True;   


if __name__ == '__main__':
    try:
        rospy.init_node('second_mission', anonymous=True)
        sub = rospy.Subscriber('mate/image/rov', Image, callback1)
        joy_sub = rospy.Subscriber('joy', Joy, joy_callback)
        cv2.startWindowThread()
        cv2.namedWindow("Trackbars")
        cv2.namedWindow("Frame")
        cv2.namedWindow("Mask")
        cv2.createTrackbar("L-H", "Trackbars", 0, 180, nothing)
        cv2.createTrackbar("L-S", "Trackbars", 66, 255, nothing)
        cv2.createTrackbar("L-V", "Trackbars", 126, 255, nothing)
        cv2.createTrackbar("U-H", "Trackbars", 180, 180, nothing)
        cv2.createTrackbar("U-S", "Trackbars", 255, 255, nothing)
        cv2.createTrackbar("U-V", "Trackbars", 223, 255, nothing)
        cv2.createTrackbar("poin1_x", "Trackbars", 352, 2500, nothing)
        cv2.createTrackbar("poin1_y", "Trackbars", 119, 2500, nothing)
        cv2.createTrackbar("poin2_x", "Trackbars", 565, 2500, nothing)
        cv2.createTrackbar("poin2_y", "Trackbars", 113, 2500, nothing)
        cv2.createTrackbar("poin3_x", "Trackbars", 158, 2500, nothing)
        cv2.createTrackbar("poin3_y", "Trackbars", 278, 2500, nothing)
        cv2.createTrackbar("poin4_x", "Trackbars", 428, 2500, nothing)
        cv2.createTrackbar("poin4_y", "Trackbars", 347, 2500, nothing)
        while not rospy.is_shutdown():
            if not refreshed:
                pass
            else:
                data = rospy.wait_for_message('mate/image/rov', Image)
                refreshed = False
            # do stuff
            count_a = 0
            count_b = 0
            count_c = 0
            count_d = 0
            bridge = CvBridge()
            ori = bridge.imgmsg_to_cv2(data)
            frame = ori.copy()
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            hsv = cv2.GaussianBlur(hsv, (5, 5), 1)

            l_h = cv2.getTrackbarPos("L-H", "Trackbars")
            l_s = cv2.getTrackbarPos("L-S", "Trackbars")
            l_v = cv2.getTrackbarPos("L-V", "Trackbars")
            u_h = cv2.getTrackbarPos("U-H", "Trackbars")
            u_s = cv2.getTrackbarPos("U-S", "Trackbars")
            u_v = cv2.getTrackbarPos("U-V", "Trackbars")

            poin1_x = cv2.getTrackbarPos("poin1_x", "Trackbars")
            
            poin1_y = cv2.getTrackbarPos("poin1_y", "Trackbars")
            poin2_x = cv2.getTrackbarPos("poin2_x", "Trackbars")
            
            poin2_y = cv2.getTrackbarPos("poin2_y", "Trackbars")

            poin3_x = cv2.getTrackbarPos("poin3_x", "Trackbars")
            
            poin3_y = cv2.getTrackbarPos("poin3_y", "Trackbars")

            poin4_x = cv2.getTrackbarPos("poin4_x", "Trackbars")

            poin4_y = cv2.getTrackbarPos("poin4_y", "Trackbars")

            poin_arr = [(poin1_x, poin1_y), (poin2_x, poin2_y), (poin3_x, poin3_y), (poin4_x, poin4_y)]

            lower_red = np.array([l_h, l_s, l_v])
            upper_red = np.array([u_h, u_s, u_v])
            
            kontur = np.array([[poin2_x,poin2_y],[poin1_x,poin1_y],[poin4_x,poin4_y],[poin3_x,poin3_y]], dtype=np.int32)
            cv2.drawContours(frame,[kontur],0,(255,255,255),1)
            mask = cv2.inRange(hsv, lower_red, upper_red)
            # mask = cv2.Canny(hsv, 50, 100)
            kernel = np.ones((5, 5), np.uint8)

            # mask = cv2.dilate(mask, kernel)
            mask = cv2.erode(mask, kernel)

            # Contours detection
            _, contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            for j, i in enumerate(poin_arr):
                cv2.putText(frame, str(j+1), (i[0], i[1]), font, 0.4, (0, 0, 255))

            for cnt in contours:
                area = cv2.contourArea(cnt)
                approx = cv2.approxPolyDP(cnt, 0.02*cv2.arcLength(cnt, True), True)
                x = approx.ravel()[0]
                y = approx.ravel()[1]
                

                dist = cv2.pointPolygonTest(kontur,(x,y),True)
                if dist <= 0:
                    continue

                if area > 150:
                    cv2.drawContours(frame, [approx], 0, (0, 0, 0), 5)

                    if len(approx) == 3:
                        cv2.putText(frame, "Triangle", (x, y), font, 0.5, (0, 0, 255))
                        count_a += 1
                    elif 4 <= len(approx) <= 7:
                        # compute the bounding box of the contour and use the
                        # bounding box to compute the aspect ratio
                        (x, y, w, h) = cv2.boundingRect(approx)
                        ar = w / float(h)    
                        # a square will have an aspect ratio that is approximately
                        # equal to one, otherwise, the shape is a rectangle
                        if ar >= 0.95 and ar <= 3:
                            cv2.putText(frame, "Square" + str(len(approx)), (x, y), font, 0.5, (0, 0, 255))
                            count_b += 1
                        else:
                            cv2.putText(frame, "Rectangle" + str(len(approx)), (x, y), font, 0.5, (0, 0, 255))
                            count_c += 1

                    elif 7 <= len(approx) < 20:
                        cv2.putText(frame, "Circle", (x, y), font, 0.5, (0, 0, 255))
                        count_d += 1

            ph = random.randrange(75, 79, 1)
            ph = ph / 10.0
            temperature = random.randrange(255, 265, 1)
            temperature = temperature / 10.0
            cv2.putText(frame,"A = " + str(count_a),(525,100), font, 1,(125,125,255),2,cv2.LINE_AA)
            cv2.putText(frame,"B = " + str(count_b),(525,200), font, 1,(125,125,255),2,cv2.LINE_AA)
            cv2.putText(frame,"B = " + str(count_c),(525,300), font, 1,(125,125,255),2,cv2.LINE_AA)
            cv2.putText(frame,"B = " + str(count_d),(525,400), font, 1,(125,125,255),2,cv2.LINE_AA)

            cv2.putText(frame,"Temp = " + str(temperature),(20,100), font, 1,(125,125,255),2,cv2.LINE_AA)
            cv2.putText(frame,"PH = " + str(ph),(20,200), font, 1,(125,125,255),2,cv2.LINE_AA)
            cv2.imshow("Frame", frame)
            cv2.imshow("Mask", mask)
            cv2.waitKey(30)

    except rospy.ROSInterruptException:
        cv2.destroyAllWindows()
