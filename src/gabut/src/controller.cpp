#include "../include/gabut/hoho.hpp"
#include "ros/ros.h"
#include "mavros_msgs/OverrideRCIn.h"
#include "mavros_msgs/SetMode.h"
#include <iostream>
#include <stdio.h>
#include <sensor_msgs/Joy.h>
#include "gabut/number_rc.h"
#include "gabut/image_value.h"
#include "pid/plant_msg.h"
#include "pid/controller_msg.h"
#include "pid/pid_const_msg.h"
#include <mavros_msgs/State.h>

using namespace std;

int a, b, c, d, e, f;
int g, h, i, j, k, l, m, n, o, p, q;
int throttle, steering, sink, grip_high, grip_low, trim, roll;
int counter;

int tempPwm;

bool lastArmed = false;

mavros_msgs::OverrideRCIn rovRcIn;
mavros_msgs::SetMode flight;

pid::plant_msg  pid_in;
pid::pid_const_msg pid_const;

int state_red;
int state_blue;
int control_effort;
int autoSteering;

void image_process_cb(const gabut::image_value& image);
void pid_receiver_cb(const pid::controller_msg& control);
void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
void control_cb (const gabut::number_rc& msg);
void checkController();
void armPixhawk(const mavros_msgs::State::ConstPtr& state);

ros::Publisher pub_pid_in; 
ros::Publisher pub_pid_const;
ros::Publisher pub_override_rc;

int main(int argc, char** argv){
	ros::init(argc, argv, "subPixhawk");
	ros::NodeHandle nh;
	
	pub_override_rc = nh.advertise<mavros_msgs::OverrideRCIn>("/mavros/rc/override", 10);
	pub_pid_in		= nh.advertise<pid::plant_msg>("/mate/pid/in", 1);
	pub_pid_const 	= nh.advertise<pid::pid_const_msg>("/mate/pid/const", 1,true);

	ros::Subscriber sub_pid_x_out 		= nh.subscribe("/mate/pid/out", 10, pid_receiver_cb );
	ros::Subscriber joy_sub 			= nh.subscribe<sensor_msgs::Joy>("joy", 8, &joyCallback);
	ros::ServiceClient client 			= nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
	ros::Subscriber state = nh.subscribe<mavros_msgs::State>("/mavros/state", 8, &armPixhawk);

	flight.request.base_mode = 0;
	flight.request.custom_mode = "MANUAL";
	client.call(flight);
		
	pid_const.p = kp;
	pid_const.i = ki;
	pid_const.d = kd;
	pub_pid_const.publish(pid_const);
	
	pid_in.t = initial_time;
		
	while( ros::ok() ){	
		ros::spinOnce();
		sleep(0.2);
	}
}
void armPixhawk(const mavros_msgs::State::ConstPtr& state) {
	if (!state->armed) {
		system("rosrun mavros mavsafety arm");
	}
}

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy){	
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
	
	
	throttle 	= joy->axes[1];
	steering 	= joy->axes[3];
	sink 		= joy->axes[2];
	trim 		= joy->axes[5];
	roll		= joy->axes[4];	
	grip_high 	= joy->buttons[5];
	grip_low 	= joy->buttons[7];
	//checkController();
	
	for(int i=0; i < 8; i++) rovRcIn.channels[i] = 0;	//Releases all Channels First
	
	//up 
	if (sink > 0){
		rovRcIn.channels[MOTOR1] = minStabil;
		rovRcIn.channels[MOTOR2] = minMotor;
		rovRcIn.channels[MOTOR3] = minMotor;
	}
	//down
	else if (sink < 0){
		rovRcIn.channels[MOTOR1] = maxStabil;
		rovRcIn.channels[MOTOR2] = maxMotor;
		rovRcIn.channels[MOTOR3] = maxMotor;
	}
	
	else if (trim < 0){
		rovRcIn.channels[MOTOR1] = minTrim;
		rovRcIn.channels[MOTOR2] = maxTrim;
		rovRcIn.channels[MOTOR3] = maxTrim;
	}
	else if (trim > 0){
		rovRcIn.channels[MOTOR1] = maxTrim;
		rovRcIn.channels[MOTOR2] = minTrim;
		rovRcIn.channels[MOTOR3] = minTrim;
	}
	else if (roll < 0){//left
		rovRcIn.channels[MOTOR1] = middleStabil;//middle
		rovRcIn.channels[MOTOR2] = maxRoll;//right
		rovRcIn.channels[MOTOR3] = minRoll;//left
	}
	else if (roll > 0){//right
		rovRcIn.channels[MOTOR1] = middleStabil;
		rovRcIn.channels[MOTOR2] = minRoll;
		rovRcIn.channels[MOTOR3] = maxRoll;
	}
	else {
		rovRcIn.channels[MOTOR1] = middleStabil;
		rovRcIn.channels[MOTOR2] = middleMotor;
		rovRcIn.channels[MOTOR3] = middleMotor;		
	}

	//steering
	if (steering < 0){rovRcIn.channels[STEERING_PIN] = maxSteering;}//right
	else if (steering > 0){rovRcIn.channels[STEERING_PIN] = minSteering;}//left
	else {rovRcIn.channels[STEERING_PIN] = middleSteering;}

	//throttle
	if (throttle > 0){rovRcIn.channels[THROTTLE_PIN] = maxThrottle;}
	else if (throttle < 0){rovRcIn.channels[THROTTLE_PIN] = minThrottle;}
	else {rovRcIn.channels[THROTTLE_PIN] = middleThrottle;}
	
	
	if (grip_high > 0){
		tempPwm = 1660;

		rovRcIn.channels[SERVO1] = tempPwm;
		rovRcIn.channels[SERVO2] = tempPwm;
	}
	else if (grip_low > 0){
		tempPwm = 1100;

		rovRcIn.channels[SERVO1] = tempPwm;
		rovRcIn.channels[SERVO2] = tempPwm;

	} else {
		rovRcIn.channels[SERVO1] = tempPwm;
		rovRcIn.channels[SERVO2] = tempPwm;
	}
	
	pub_override_rc.publish(rovRcIn);
}

void checkController(){	
	cout<<a<<"\t"<<b<<"\t"<<c<<"\t"<<d<<"\t"<<e<<"\t"<<f<<endl;
	cout<<g<<"\t"<<h<<"\t"<<i<<"\t"<<j<<"\t"<<k<<"\t"<<l<<endl;
	cout<<m<<"\t"<<n<<endl;
	cout<<endl<<endl<<endl;
}

void pid_receiver_cb(const pid::controller_msg& control){
	control_effort = control.u;
}
