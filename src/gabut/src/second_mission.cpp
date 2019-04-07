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

void firstMissionProcessing(Mat input_image);
void secondMissionProcessing(Mat input_image);
void thirdMissionProcessing(Mat input_image);
void miniProcessing(Mat input_image);

gabut::image_value image;
ros::Publisher pub_state_camera;

void rovCallback(const sensor_msgs::CompressedImageConstPtr& msg)
{
  try
  {
    rov_image = cv::imdecode(cv::Mat(msg->data),1);//convert compressed image data to cv::Mat
    waitKey(10);
    //firstMissionProcessing(rov_image);
    secondMissionProcessing(rov_image);
    //thirdMissionProcessing(rov_image);
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


void firstMissionProcessing(Mat input_image){
	Mat first_all = input_image.clone();
	
	Rect region_of_interest = Rect(x_init, y_init, width, height);
	Mat Original = first_all(region_of_interest);
	
	Size sz = Original.size();
	int original_height = sz.height; 
	int original_width	= sz.width;
	
	Size sx = first_all.size();
	int input_height = sx.height;
	int input_width = sx.width; 
	
	//red image processing
	Mat imgOriginal_red, imgHSV_red, imgThresholded_red, imgErode_red, imgDilate_red, imgDebug_red;
	int sum_x_red = 0;
	int sum_y_red = 0;
	
	//debug for all red
	imgDebug_red = first_all.clone();
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
	imgDebug_blue = first_all.clone();
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
	
	//red state
	line( first_all, Point( red_setpoint, 0 ), Point( red_setpoint, input_height), Scalar( 50, 50, 50 ), 2, 8 );
	line( first_all, Point( state_red, 0 ), Point( state_red, input_height), Scalar( 150, 150, 150 ), 2, 8 );
	
	//blue state
	line( first_all, Point( blue_setpoint, 0 ), Point( blue_setpoint, input_height), Scalar( 200, 200, 200 ), 2, 8 );
	line( first_all, Point( state_blue, 0 ), Point( state_blue, input_height), Scalar( 100, 100, 100 ), 2, 8 );
	
	//roi
	line( first_all, Point( x_init, y_init ), Point( x_init+original_width, y_init), Scalar( 100, 100, 100 ), 2, 8 );
	line( first_all, Point( x_init, y_init+original_height ), Point( x_init+original_width, y_init+original_height), Scalar( 100, 100, 100 ), 2, 8 );	
	line( first_all, Point( x_init, y_init ), Point( x_init, y_init+original_height), Scalar( 100, 100, 100 ), 2, 8 );
	line( first_all, Point( x_init+original_width, y_init ), Point( x_init+original_width, y_init+original_height), Scalar( 100, 100, 100 ), 2, 8 );
	
	image.state_red 	= state_red;
	image.state_blue 	= state_blue;
	
	pub_state_camera.publish(image);
	
	//imshow("first_th_Red", imgThresholded_red);
	//imshow("first_th_blue", imgThresholded_blue);
	
	//imshow("fisrt_Red", imgDebug_red);
	//imshow("first_blue", imgDebug_blue);
	
	imshow("first", first_all);
}

void secondMissionProcessing(Mat input_image){
	Mat Original = input_image.clone();
	Mat imgHSV, allColour, allContour, Threshold, BW;
	int Index;
	cvtColor(Original, imgHSV, COLOR_BGR2HSV);
			
	erode(imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	dilate( imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode(imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		
	inRange(imgHSV, Scalar(LowH_black, LowS_black, LowV_black), Scalar(HighH_black, HighS_black, HighV_black), Threshold); //Threshold the image
		
	Canny(Threshold, BW, 0, 50, 5);
		
	vector<vector<Point> > Contours;
		
	findContours(BW, Contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			
	vector<Point> Approx;
	int count_triangle, count_square, count_strip, count_circle;
	
	for (Index = 0; Index < Contours.size(); Index++){
		approxPolyDP(Mat(Contours[Index]), Approx, arcLength(Mat(Contours[Index]), true)*0.02, true);
		
		if (fabs(contourArea(Contours[Index])) < 5000 || !isContourConvex(Approx))
		continue;

		if (Approx.size() == 3){
			count_triangle++;
		}
		
		if (Approx.size() == 4){
			Moments mu_black=moments(Threshold);
			int area_black = mu_black.m00; // sum of zero'th moment is area
			int posX_black = mu_black.m10/area_black; // center of mass = w*x/weight
			int posY_black = mu_black.m01/area_black;// center of mass = w*y/high
			area_black /= 255; // scale from bytes to pixels	
			if(mu_black.m10<0.5*mu_black.m01){
				count_strip++;
			}
			else{
				count_square++;
			}
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
	string strip_text="0";
	string circle_text="0";
	string square_text="0";
	string tri_text="0";
	
	//square_text = to_string(count_square);
	//strip_text = to_string(count_strip);
	//tri_text = to_string(count_triangle);
	//circle_text = to_string(count_circle);
	
	putText(Original, strip_text, Point(strip_x_text, strip_y_text), FONT_HERSHEY_DUPLEX, 2, Scalar(0,255,255), 2);
	putText(Original, circle_text, Point(circle_x_text, circle_y_text), FONT_HERSHEY_DUPLEX, 2, Scalar(0,255,255), 2);
	putText(Original, square_text, Point(square_x_text, square_y_text), FONT_HERSHEY_DUPLEX, 2, Scalar(0,255,255), 2);
	putText(Original, tri_text, Point(tri_x_text, tri_y_text), FONT_HERSHEY_DUPLEX, 2, Scalar(0,255,255), 2);

	//imshow("second colour", Threshold);
	//imshow("second contour", BW);
	imshow("second", Original);
}

void thirdMissionProcessing(Mat input_image){
	imshow("third", input_image);
}

void miniProcessing(Mat input_image){
	imshow("mini", input_image);
}
