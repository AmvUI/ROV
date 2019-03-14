#include "../include/gabut/hoho.hpp"
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <iostream>
#include <stdio.h>
#include "gabut/number_rc.h"

using namespace std;

void checkController();
void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

int a, b, c, d, e, f;
int g, h, i, j, k, l, m, n;

int auto_mode, manual_mode; 

gabut::number_rc number; 

int main(int argc, char** argv){
	ros::init(argc, argv, "remoteServer");
	ros::NodeHandle nh;
	
	ros::Publisher pub_rc_flag 	= nh.advertise<gabut::number_rc>("/mate/rov/number", 1,true);
	ros::Subscriber joy_sub 	= nh.subscribe<sensor_msgs::Joy>("joy", 8, &joyCallback);
	
	while( ros::ok() ){
		ros::spinOnce();
		if(auto_mode ==1 && mode_number ==manual_number){
			mode_number = auto_number;
		}
		else if(manual_mode ==1 && mode_number==auto_number){
			mode_number = manual_number;
		}
		number.rc_number=mode_number;
		pub_rc_flag.publish(number);
		//cout<<mode_number<<endl;
	}
}
 

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)	{
	a = joy->axes[0];		//-1	<	right left 	<	0	<	left left	<	1
    b = joy->axes[1];		//-1	<	down left	<	0	<	up left		<	1
    c = joy->axes[2];		//-1 	< 	down right	<	0	<	up right	<	1
    d = joy->axes[3];		//-1 	< 	right right < 	0 	< 	left right 	< 	1
    e = joy->axes[4];		//-1 	< 	right 		< 	0 	< 	left 		< 	1
    f = joy->axes[5];		//-1 	< 	down		<	0	<	up			<	1
    g = joy->buttons[0];	//triangle
    h = joy->buttons[1];	//circle
    i = joy->buttons[2];	//cross
    j = joy->buttons[3];	//square
    k = joy->buttons[4];	//l1
    l = joy->buttons[5];	//r1
    m = joy->buttons[6];	//l2
    n = joy->buttons[7];	//r2
    
    auto_mode	= joy->buttons[4];
	manual_mode = joy->buttons[6];
	
	//checkController();
}


void checkController(){	
	cout<<a<<"\t"<<b<<"\t"<<c<<"\t"<<d<<"\t"<<e<<"\t"<<f<<endl;
	cout<<g<<"\t"<<h<<"\t"<<i<<"\t"<<j<<"\t"<<k<<"\t"<<l<<endl;
	cout<<m<<"\t"<<n<<endl;
	cout<<endl<<endl<<endl;
}

