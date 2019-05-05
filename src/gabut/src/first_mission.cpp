#include "../include/gabut/hoho.hpp"
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <stdio.h>
#include "gabut/image_value.h"
#include <string> 

using namespace std;
using namespace cv;

Mat rov_image, mini_image;

void miniProcessing(Mat input_image);

gabut::image_value image;
ros::Publisher pub_state_camera;

void rovCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
    rov_image = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    waitKey(10);
    // firstMissionProcessing(rov_image);
    imshow("first", rov_image);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert to image!");
  }
}

void miniCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
    mini_image = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    waitKey(10);
    miniProcessing(mini_image);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert to image!");
  }
}

int main(int argc, char **argv){
	ros::init(argc, argv, "videoRec");
	ros::NodeHandle nh;
	cv::startWindowThread();
	
	image_transport::ImageTransport it(nh);
	
	pub_state_camera 			= nh.advertise<gabut::image_value>("/mate/value/image", 1);
	ros::Subscriber sub_rov 	= nh.subscribe("/mate/image/rov/compressed", 1, rovCallback);
	ros::Subscriber sub_mini 	= nh.subscribe("/mate/image/mini/compressed", 1, miniCallback);
	
	namedWindow("panel", CV_WINDOW_AUTOSIZE);
	
	createTrackbar("LowH_red", "panel", &LowH_red, 255);
	createTrackbar("HighH_red", "panel", &HighH_red, 255);
	createTrackbar("LowS_red", "panel", &LowS_red, 255); 
	createTrackbar("HighS_red", "panel", &HighS_red, 255);
	createTrackbar("LowV_red", "panel", &LowV_red, 255);
	createTrackbar("HighV_red", "panel", &HighV_red, 255);
	createTrackbar("noise_red", "panel", &Noise_red, 255);

	createTrackbar("LowH_blue", "panel", &LowH_blue, 255);
	createTrackbar("HighH_blue", "panel", &HighH_blue, 255);
	createTrackbar("LowS_blue", "panel", &LowS_blue, 255); 
	createTrackbar("HighS_blue", "panel", &HighS_blue, 255);
	createTrackbar("LowV_blue", "panel", &LowV_blue, 255);
	createTrackbar("HighV_blue", "panel", &HighV_blue, 255);
	createTrackbar("noise_blue", "panel", &Noise_blue, 255);

	createTrackbar("x", "panel", &x_init, 700); //Hue (0 - 255)
	createTrackbar("y", "panel", &y_init, 700);
	createTrackbar("width", "panel", &width, 700); //Saturation (0 - 255)
	createTrackbar("hight", "panel", &height, 700);
	
	while (ros::ok()) {
		ros::spinOnce();
	}
}

void miniProcessing(Mat input_image){
	imshow("mini", input_image);
}
