#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat rov_image, mini_image;

void rovProcessing(Mat input_image);
void miniProcessing(Mat input_image);

void rovCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
    rov_image = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    waitKey(10);
    rovProcessing(rov_image);
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
	
	ros::Subscriber sub_rov 	= nh.subscribe("/mate/image/rov/compressed", 1, rovCallback);
	ros::Subscriber sub_mini 	= nh.subscribe("/mate/image/mini/compressed", 1, miniCallback);
	
	while (ros::ok()) {
		ros::spinOnce();
	}
}

void rovProcessing(Mat input_image){
	imshow("rov", input_image);
}

void miniProcessing(Mat input_image){
	imshow("mini", input_image);
}
