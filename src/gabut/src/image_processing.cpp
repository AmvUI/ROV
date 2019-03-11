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

void rov_processing(Mat input_image);
void mini_processing(Mat input_image);

void rovCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
    rov_image = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    waitKey(10);
    rov_processing(rov_image);
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
    mini_processing(mini_image);
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
	
	ros::Subscriber sub_rov_image  = nh.subscribe("/mate/rov/image", 1, rovCallback);
	ros::Subscriber sub_mini_image = nh.subscribe("/mate/mini/image", 1, miniCallback);
	

	while (nh.ok()) {
		ros::spinOnce();
	}
}

void rov_processing(Mat input_image){
	imshow("rov", input_image);
}
void mini_processing(Mat input_image){
	imshow("mini", input_image);
}
