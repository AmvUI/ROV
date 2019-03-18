#include "../include/gabut/hoho.hpp"
#include <iostream>
#include <stdio.h>
#include <ros/ros.h>
#include "pid/controller_msg.h"
#include "mavros_msgs/State.h"
#include "mavros_msgs/OverrideRCIn.h"

using namespace std;

int effort;

int out_channel[8];

string armed;
string mode;

void pid_receiver_cb(const pid::controller_msg& control);
void override_motor_cb (const mavros_msgs::OverrideRCIn& motor);
void rc_state_cb (const mavros_msgs::State& state);

int main(int argc, char **argv)
{
	ros::init(argc, argv, "debug_makara");
	ros::NodeHandle nh;
	
	ros::Subscriber sub_override_motor 	= nh.subscribe("/mavros/rc/override", 8, override_motor_cb);
	ros::Subscriber sub_state_rc 		= nh.subscribe("/mavros/state", 8, rc_state_cb);
	ros::Subscriber sub_pid_x_out 		= nh.subscribe("/mate/pid/out", 10, pid_receiver_cb );

	ROS_WARN("NC : debug_makara.cpp active");

	while( ros::ok() ){	
		ros::spinOnce();
		
		ROS_WARN("NC : topic override");
		ROS_INFO("effort:%d", effort);
		ROS_INFO(" ");
		
		ROS_WARN("NC : topic motor");
		ROS_INFO("%d, %d, %d, %d, %d, %d, %d, %d", out_channel[0], out_channel[1], out_channel[2], out_channel[3], out_channel[4], out_channel[5], out_channel[6], out_channel[7]);		
		ROS_INFO(" ");
		
		ROS_WARN("NC : topic state");
		ROS_INFO("armed:%s mode:%s", armed.c_str(), mode.c_str());
		ROS_INFO(" ");
		
		sleep(1);
		system("clear");
	}
}

void override_motor_cb (const mavros_msgs::OverrideRCIn& motor){
	int x;
	for(x=0;x<8;x++){ 
		out_channel[x] = motor.channels[x]; 
	}
}

void rc_state_cb (const mavros_msgs::State& state){
	if(state.armed == true){
		armed = "true";
	}
	else{armed = "false";}
	mode = state.mode;	
}
	

void pid_receiver_cb(const pid::controller_msg& control){
	effort = control.u;
}
