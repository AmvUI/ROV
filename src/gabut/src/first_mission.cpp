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

int main(int argc, char **argv){
	ros::init(argc, argv, "videoRec");
	ros::NodeHandle nh;
	cv::startWindowThread();
	
	image_transport::ImageTransport it(nh);
	
	pub_state_camera 			= nh.advertise<gabut::image_value>("/mate/value/image", 1);
	ros::Subscriber sub_rov 	= nh.subscribe("/mate/image/rov/compressed", 1, rovCallback);
	
	while (ros::ok()) {
		ros::spinOnce();
	}
}
