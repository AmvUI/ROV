#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include "../include/gabut/hoho.hpp"
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <stdio.h>
#include "gabut/image_value.h"
#include <cstdlib>
#include <sensor_msgs/Joy.h>
using namespace cv;
using namespace std;

bool refreshed = true;

Mat Original;

Mat rov_image, mini_image;
gabut::image_value image;
ros::Publisher pub_state_camera;

//convert double to string
namespace patch
{
    template<typename T> std::string to_string(const T& n)
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/**
 * Helper function to display text in the center of a contour
 */
void setLabel(Mat& im, const std::string label, std::vector<Point>& contour)
{
	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	Size text = getTextSize(label, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(im, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

void ngode(Mat Original)
{		
	int countA = 0;
	int countB = 0;
	int countC = 0;
	int countD = 0;
	Mat gray,bw,gray2;
	Mat dst = Original.clone();
	erode(dst, dst, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	// GaussianBlur(Original, Original, Size(5, 5), 2, 2);
	cvtColor(dst, gray, COLOR_BGR2GRAY);
	threshold( gray, bw,0, 115,3 );
	Canny(bw, gray2, 0, 50, 5);
	vector<vector<Point> > contours;
	findContours(gray2.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	vector<Point> approx;

    for (int i = 0; i < contours.size(); i++)
    {
	    // Approximate contour with accuracy proportional
	    // to the contour perimeter
	    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

	    // Skip small or non-convex objects 
	    if (std::fabs(contourArea(contours[i])) < 1000 || !isContourConvex(approx))
		    continue;

	    if (approx.size() == 3)
	    {
	    	/**
		    if (std::fabs(contourArea(contours[i])) < 10000 || !isContourConvex(approx))
		    {
			    continue;
			
		    }
		    */
		    setLabel(dst, "TRI", contours[i]);
		    countA++;

	    }
	     else if (approx.size() >= 4 && approx.size() <= 6)
	    {
		    // Number of vertices of polygonal curve
		    int vtc = approx.size();

		    // Get the cosines of all corners
		    std::vector<double> cos;
		    for (int j = 2; j < vtc+1; j++)
			    cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

		    // Sort ascending the cosine values
		    std::sort(cos.begin(), cos.end());

		    // Get the lowest and the highest cosine
		    double mincos = cos.front();
		    double maxcos = cos.back();

		    // Use the degrees obtained above and the number of vertices
		    // to determine the shape of the contour
		    if (vtc == 4)// && mincos >= -0.1 && maxcos <= 0.3)
			    {
			    double area = contourArea(contours[i]);
			    double keliling = arcLength( contours[i], true );
			    double ganteng = (keliling*keliling)/(16*area);
			
			    if (ganteng >= 0.8 and ganteng <= 1.15)
				    {	
				    setLabel(dst, "SQR", contours[i]);
				    countB++;
				    }
			    else 
			    {
				    setLabel(dst, "RECT", contours[i]);
				    countC++;
			    }
			 }
		    else if (vtc == 5)// && mincos >= -0.34 && maxcos <= -0.27)
			    setLabel(dst, "PENTA", contours[i]);
		    else if (vtc == 6)// && mincos >= -0.55 && maxcos <= -0.45)
			    setLabel(dst, "HEXA", contours[i]);
	    }
	    else
	    {
		    // Detect and label circles
		    double area = contourArea(contours[i]);
		    Rect r = boundingRect(contours[i]);
		    int radius = r.width / 2;

		    if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
		        std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
			     { setLabel(dst, "CIR", contours[i]); countD++; }

	    }
    }
    stringstream ss;
    ss << countA;
    putText(dst, ss.str(), Point(550, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 1);
    ss.str("");
    ss << countB;
    putText(dst, ss.str(), Point(550, 200), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 1);
    ss.str("");
    ss << countC;
    putText(dst, ss.str(), Point(550, 300), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 1);
    ss.str("");
    ss << countD;
    putText(dst, ss.str(), Point(550, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 1);
	
	imshow("Circle Detector", dst);
	waitKey(30);
}

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
    ngode(rov_image);
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
	
	namedWindow("Circle Detector", CV_WINDOW_AUTOSIZE);

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

