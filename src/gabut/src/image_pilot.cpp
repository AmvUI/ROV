#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "../include/gabut/hoho.hpp"

using namespace std;
using namespace cv;
void imageProcessing(Mat image);

Mat imageROV;
Mat imageMini;

void rovCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
    imageROV = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    waitKey(10);
    imageProcessing(imageROV);
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
    imageMini = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    waitKey(10);
    imageProcessing(imageMini);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert to image!");
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  namedWindow("view");
  startWindowThread();
 
  image_transport::ImageTransport it(nh);
  ros::Subscriber sub_rov 	= nh.subscribe("camera/image/rov", 1, rovCallback);  
  ros::Subscriber sub_mini 	= nh.subscribe("camera/image/mini", 1, miniCallback);  
  
  while (nh.ok()) {
		ros::spinOnce();
	}
}

void imageProcessing(Mat image){
	int imageWidth;
	int imageHeight; 
  
	Size sz = image.size();
	imageWidth = sz.width;
	imageHeight = sz.height; 
  
	line( image, Point( imageWidth*1/3, 0 ), Point( imageWidth*1/3, imageHeight), Scalar( 255, 2255, 255 ),  thickness, 8 );		
	line( image, Point( imageWidth*2/3, 0 ), Point( imageWidth*2/3, imageHeight), Scalar( 255, 255, 255 ),  thickness, 8 );		
	
	line( image, Point( 0, imageHeight*1/3 ), Point( imageWidth, imageHeight*1/3), Scalar( 255, 255, 255 ),  thickness, 8 );		
	line( image, Point( 0, imageHeight*2/3 ), Point( imageWidth, imageHeight*2/3), Scalar( 255, 255, 255 ),  thickness, 8 );		
}
