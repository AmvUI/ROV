#include "../include/gabut/hoho.hpp"
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

void firstMissionProcessing(Mat input_image);
void secondMissionProcessing(Mat input_image);
void thirdMissionProcessing(Mat input_image);
void miniProcessing(Mat input_image);

void rovCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
    rov_image = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    waitKey(10);
    firstMissionProcessing(rov_image);
    secondMissionProcessing(rov_image);
    thirdMissionProcessing(rov_image);
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
	
	namedWindow("panel", CV_WINDOW_AUTOSIZE);
	
	createTrackbar("LowH_red", "panel_red", &LowH_red, 255);
	createTrackbar("HighH_red", "panel_red", &HighH_red, 255);
	createTrackbar("LowS_red", "panel_red", &LowS_red, 255); 
	createTrackbar("HighS_red", "panel_red", &HighS_red, 255);
	createTrackbar("LowV_red", "panel_red", &LowV_red, 255);
	createTrackbar("HighV_red", "panel_red", &HighV_red, 255);
	createTrackbar("noise_red", "panel_red", &Noise_red, 255);

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


void firstMissionProcessing(Mat input_image){
	Rect region_of_interest = Rect(x_init, y_init, width, height);
	Mat Original = input_image(region_of_interest);
	
	Size sz = Original.size();
	int original_height = sz.height; 
	int original_width	= sz.width;
	
	Size sx = input_image.size();
	int input_height = sx.height;
	int input_width = sx.width; 
	
	//red image processing
	Mat imgOriginal_red, imgHSV_red, imgThresholded_red, imgErode_red, imgDilate_red, imgDebug_red;
	int sum_x_red = 0;
	int sum_y_red = 0;
	
	//debug for all red
	imgDebug_red = input_image.clone();
	medianBlur(imgDebug_red, imgDebug_red, 5);
	cvtColor(imgDebug_red, imgDebug_red, COLOR_BGR2HSV);
	inRange(imgDebug_red, Scalar(LowH_red, LowS_red, LowV_red), Scalar(HighH_red, HighS_red, HighV_red), imgDebug_red);//range threshold
	erode(imgDebug_red, imgDebug_red, getStructuringElement(MORPH_ELLIPSE, Size(Noise_red, Noise_red)) );
	dilate( imgDebug_red, imgDebug_red, getStructuringElement(MORPH_ELLIPSE, Size(Noise_red, Noise_red)) ); 
	dilate(imgDebug_red, imgDebug_red, getStructuringElement(MORPH_ELLIPSE, Size(Noise_red, Noise_red)) ); 
	erode(imgDebug_red, imgDebug_red, getStructuringElement(MORPH_ELLIPSE, Size(Noise_red, Noise_red)) ); 
	
	//ROI for red
	imgOriginal_red = Original.clone();
	medianBlur(imgOriginal_red, imgOriginal_red, 5);
	cvtColor(imgOriginal_red, imgHSV_red, COLOR_BGR2HSV);
	inRange(imgHSV_red, Scalar(LowH_red, LowS_red, LowV_red), Scalar(HighH_red, HighS_red, HighV_red), imgThresholded_red);//range threshold	
	erode(imgThresholded_red, imgThresholded_red, getStructuringElement(MORPH_ELLIPSE, Size(Noise_red, Noise_red)) );
	dilate( imgThresholded_red, imgThresholded_red, getStructuringElement(MORPH_ELLIPSE, Size(Noise_red, Noise_red)) ); 
	dilate(imgThresholded_red, imgThresholded_red, getStructuringElement(MORPH_ELLIPSE, Size(Noise_red, Noise_red)) ); 
	erode(imgThresholded_red, imgThresholded_red, getStructuringElement(MORPH_ELLIPSE, Size(Noise_red, Noise_red)) ); 
	
	Moments mu_red=moments(imgThresholded_red);
	int area_red = mu_red.m00; // sum of zero'th moment is area
	int posX_red = mu_red.m10/area_red; // center of mass = w*x/weight
	int posY_red = mu_red.m01/area_red;// center of mass = w*y/high
	area_red /= 255; // scale from bytes to pixels	
	
	//blue image processing
	Mat imgOriginal_blue, imgHSV_blue, imgThresholded_blue, imgErode_blue, imgDilate_blue, imgDebug_blue;
	int sum_x_blue = 0;
	int sum_y_blue = 0;
	
	//debug for all blue
	imgDebug_blue = input_image.clone();
	medianBlur(imgDebug_blue, imgDebug_blue, 5);
	cvtColor(imgDebug_blue, imgDebug_blue, COLOR_BGR2HSV);
	inRange(imgDebug_blue, Scalar(LowH_blue, LowS_blue, LowV_blue), Scalar(HighH_blue, HighS_blue, HighV_blue), imgDebug_blue);//range threshold
	erode(imgDebug_blue, imgDebug_blue, getStructuringElement(MORPH_ELLIPSE, Size(Noise_blue, Noise_blue)) );
	dilate( imgDebug_blue, imgDebug_blue, getStructuringElement(MORPH_ELLIPSE, Size(Noise_blue, Noise_blue)) ); 
	dilate(imgDebug_blue, imgDebug_blue, getStructuringElement(MORPH_ELLIPSE, Size(Noise_blue, Noise_blue)) ); 
	erode(imgDebug_blue, imgDebug_blue, getStructuringElement(MORPH_ELLIPSE, Size(Noise_blue, Noise_blue)) ); 
	
	//ROI for blue
	imgOriginal_blue = Original.clone();
	medianBlur(imgOriginal_blue, imgOriginal_blue, 5);
	cvtColor(imgOriginal_blue, imgHSV_blue, COLOR_BGR2HSV);
	inRange(imgHSV_blue, Scalar(LowH_blue, LowS_blue, LowV_blue), Scalar(HighH_blue, HighS_blue, HighV_blue), imgThresholded_blue);//range threshold	
	erode(imgThresholded_blue, imgThresholded_blue, getStructuringElement(MORPH_ELLIPSE, Size(Noise_blue, Noise_blue)) );
	dilate( imgThresholded_blue, imgThresholded_blue, getStructuringElement(MORPH_ELLIPSE, Size(Noise_blue, Noise_blue)) ); 
	dilate(imgThresholded_blue, imgThresholded_blue, getStructuringElement(MORPH_ELLIPSE, Size(Noise_blue, Noise_blue)) ); 
	erode(imgThresholded_blue, imgThresholded_blue, getStructuringElement(MORPH_ELLIPSE, Size(Noise_blue, Noise_blue)) ); 
	
	Moments mu_blue=moments(imgThresholded_blue);
	int area_blue = mu_blue.m00; // sum of zero'th moment is area
	int posX_blue = mu_blue.m10/area_blue; // center of mass = w*x/weight
	int posY_blue = mu_blue.m01/area_blue;// center of mass = w*y/high
	area_blue /= 255; // scale from bytes to pixels	
	
	int state_red = posX_red;
	int state_blue = posX_blue;
	
	//red
	line( input_image, Point( red_setpoint, 0 ), Point( red_setpoint, input_height), Scalar( 50, 50, 50 ), 2, 8 );
	line( input_image, Point( state_red, 0 ), Point( state_red, input_height), Scalar( 150, 150, 150 ), 2, 8 );
	
	//blue
	line( input_image, Point( blue_setpoint, 0 ), Point( blue_setpoint, input_height), Scalar( 50, 50, 50 ), 2, 8 );
	line( input_image, Point( state_blue, 0 ), Point( state_blue, input_height), Scalar( 150, 150, 150 ), 2, 8 );
	
	//roi
	line( input_image, Point( x_init, y_init ), Point( x_init+original_width, y_init), Scalar( 100, 100, 100 ), 2, 8 );
	line( input_image, Point( x_init, y_init+original_height ), Point( x_init+original_width, y_init+original_height), Scalar( 100, 100, 100 ), 2, 8 );	
	line( input_image, Point( x_init, y_init ), Point( x_init, y_init+original_height), Scalar( 100, 100, 100 ), 2, 8 );
	line( input_image, Point( x_init+original_width, y_init ), Point( x_init+original_width, y_init+original_height), Scalar( 100, 100, 100 ), 2, 8 );
	
	
	//image.state_red = state;
	//pub_state_camera.publish(image);
	
	imshow("Threshold_Red", imgThresholded_red);
	imshow("All_Red", imgDebug_red);
	
	//imshow("Roi", Original);	
	imshow("Input_Red", input_image);
}

void secondMissionProcessing(Mat input_image){
	imshow("second", input_image);
}

void thirdMissionProcessing(Mat input_image){
	imshow("third", input_image);
}

void miniProcessing(Mat input_image){
	imshow("mini", input_image);
}
