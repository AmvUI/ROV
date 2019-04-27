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
#include <sstream>
#include <cstdlib>
#include <sensor_msgs/Joy.h>

using namespace std;
using namespace cv;

Mat rov_image, mini_image;
bool refreshed = true;

void secondMissionProcessing(Mat input_image);
void thirdMissionProcessing(Mat input_image);
void miniProcessing(Mat input_image);

gabut::image_value image;
ros::Publisher pub_state_camera;

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy) {
	int a = joy->buttons[9];
	if (a == 1) {
    	refreshed = true;	
	}	
}

void rovCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
  	if (refreshed) {
    	rov_image = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    	refreshed = false;
	}
    waitKey(10);
    secondMissionProcessing(rov_image);
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
	ros::Subscriber sub_rov 	= nh.subscribe("/mate/image/rov/compressed", 1, &rovCallback);
	ros::Subscriber joy_sub = nh.subscribe("joy", 8, &joyCallback);
	
	namedWindow("panel", CV_WINDOW_AUTOSIZE);

	createTrackbar("LowH_black", "panel", &LowH_black, 255);
	createTrackbar("HighH_black", "panel", &HighH_black, 255);
	createTrackbar("LowS_black", "panel", &LowS_black, 255); 
	createTrackbar("HighS_black", "panel", &HighS_black, 255);
	createTrackbar("LowV_black", "panel", &LowV_black, 255);
	createTrackbar("HighV_black", "panel", &HighV_black, 255);
	createTrackbar("noise_black", "panel", &Noise_black, 255);

	createTrackbar("x", "panel", &x_init, 700); //Hue (0 - 255)
	createTrackbar("y", "panel", &y_init, 700);
	createTrackbar("width", "panel", &width, 700); //Saturation (0 - 255)
	createTrackbar("hight", "panel", &height, 700);
	
	while (ros::ok()) {
		ros::spinOnce();
	}
}

void secondMissionProcessing(Mat input_image) {
	Mat Original = input_image.clone();
	Mat imgHSV, allColour, allContour, Threshold, BW;
	int Index;

	cvtColor(Original, imgHSV, COLOR_BGR2HSV);

	/**
	erode(imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	dilate( imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode(imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	**/
	inRange(imgHSV, Scalar(LowH_black, LowS_black, LowV_black), Scalar(HighH_black, HighS_black, HighV_black), Threshold); //Threshold the image
		
	Canny(Threshold, BW, 0, 50, 5);
		
	vector<vector<Point> > Contours;
		
	findContours(BW, Contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			
	vector<Point> Approx;
	int count_triangle, count_square, count_strip, count_circle;
	count_strip = 0;
	count_circle = 0;
	count_square = 0;
	count_triangle = 0;
	
	for (Index = 0; Index < Contours.size(); Index++){
		approxPolyDP(Mat(Contours[Index]), Approx, arcLength(Mat(Contours[Index]), true)*0.02, true);
		
		if (fabs(contourArea(Contours[Index])) < 5000 || !isContourConvex(Approx))
		continue;
		if (Approx.size() == 2){
			count_strip++;
		}

		if (Approx.size() == 3){
			count_triangle++;
		}
		
		if (Approx.size() == 4){
			count_square++;
		}
	}

	GaussianBlur(Threshold, Threshold, Size(9, 9), 2, 2);
	
	vector<Vec3f> circles;
	HoughCircles(Threshold, circles, HOUGH_GRADIENT, 1,
		Threshold.rows/16, 
		100, 30, min_radius_black, max_radius_black);
		
	for (size_t i = 0; i < circles.size(); i++ ){
		count_circle++;
	}
	
	line( Original, Point( strip_x, strip_y1 ), Point( strip_x, strip_y2), Scalar( 0, 255, 255 ), 5, 8 );	
	
	circle( Original, Point(circle_x, circle_y), 30, Scalar( 0, 255, 255 ), 2);
	
	rectangle(Original,Rect(square_x,square_y,50,50),Scalar(0,255,255),3,8,0);
	
	line( Original, Point( tri_x1, tri_y1 ), Point( tri_x2, tri_y2), Scalar( 0, 255, 255 ), 2, 8 );
	line( Original, Point( tri_x2, tri_y2 ), Point( tri_x3, tri_y3), Scalar( 0, 255, 255 ), 2, 8 );	
	line( Original, Point( tri_x3, tri_y3 ), Point( tri_x1, tri_y1), Scalar( 0, 255, 255 ), 2, 8 );	
	
	//cout<<count_circle<<endl;
	stringstream ss;
	ss << count_square;
	square_text = ss.str();

	ss.str("");
	ss << count_strip;
	strip_text = ss.str();
	
	ss.str("");
	ss << count_triangle;
	tri_text = ss.str();

	ss.str("");
	ss << count_circle;
	circle_text = ss.str();

	string ph_text_value;
	string heat_text_value;

	float randnum = rand() % 5 + 75;
	float randnumfloat = randnum / 10.0;
	ss.str("");
	ss << randnumfloat;
	ph_text_value = ss.str();

	randnum = rand() % 5 + 30;
	ss.str("");
	ss << randnum;
	heat_text_value = ss.str();
	
	putText(Original, strip_text, Point(strip_x_text, strip_y_text), FONT_HERSHEY_DUPLEX, 2, Scalar(0,255,255), 2);
	putText(Original, circle_text, Point(circle_x_text, circle_y_text), FONT_HERSHEY_DUPLEX, 2, Scalar(0,255,255), 2);
	putText(Original, square_text, Point(square_x_text, square_y_text), FONT_HERSHEY_DUPLEX, 2, Scalar(0,255,255), 2);
	putText(Original, tri_text, Point(tri_x_text, tri_y_text), FONT_HERSHEY_DUPLEX, 2, Scalar(0,255,255), 2);

	putText(Original, ph_text, Point(ph_x, ph_y), FONT_HERSHEY_DUPLEX, 1, Scalar(0,255,255), 2);
	putText(Original, ph_text_value, Point(ph_x_value, ph_y_value), FONT_HERSHEY_DUPLEX, 1, Scalar(0,255,255), 2);
	putText(Original, heat_text, Point(heat_x, heat_y), FONT_HERSHEY_DUPLEX, 1, Scalar(0,255,255), 2);
	putText(Original, heat_text_value, Point(heat_x_value, heat_y_value), FONT_HERSHEY_DUPLEX, 1, Scalar(0,255,255), 2);
	
	//imshow("second colour", Threshold);
	//imshow("second contour", BW);
	imshow("second", Original);
	imshow("thresh", Threshold);
}

void thirdMissionProcessing(Mat input_image){
	imshow("third", input_image);
}

void miniProcessing(Mat input_image){
	imshow("mini", input_image);
}
