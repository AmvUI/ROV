#include "../include/gabut/hoho.hpp"
#include "ros/ros.h"
#include "mavros_msgs/OverrideRCIn.h"
#include "mavros_msgs/SetMode.h"
#include <iostream>
#include <stdio.h>
#include <sensor_msgs/Joy.h>
#include "gabut/number_rc.h"

using namespace std;

int a, b, c, d, e, f;
int g, h, i, j, k, l, m, n, o, p, q;
int throttle, steering, sink, grip_high, grip_low;

int tempPwm;
int mode;

bool lastArmed = false;

void checkController();

void control_cb (const gabut::number_rc& msg);

mavros_msgs::OverrideRCIn rovRcIn;
mavros_msgs::SetMode flight;
	
ros::Publisher pub_override_rc;

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

int main(int argc, char** argv){
	ros::init(argc, argv, "subPixhawk");
	ros::NodeHandle nh;
	
	pub_override_rc 	= nh.advertise<mavros_msgs::OverrideRCIn>("/mavros/rc/override", 10);
	ros::Subscriber joy_sub 		= nh.subscribe<sensor_msgs::Joy>("joy", 8, &joyCallback);
	ros::Subscriber sub_pid_status 	= nh.subscribe("/mate/rov/number", 1, &control_cb);
	ros::ServiceClient client 		= nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
	
	flight.request.base_mode = 0;
	flight.request.custom_mode = "MANUAL";
	client.call(flight);
	
	flight.request.base_mode = 0;
	flight.request.custom_mode = "MANUAL";
	client.call(flight);
		
	system("rosrun mavros mavsafety arm");
	system("rosrun mavros mavsafety arm");
	system("rosrun mavros mavsafety arm");
		
	while( ros::ok() ){	
		ros::spinOnce();
		sleep(0.2);
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
	grip_high 	= joy->buttons[5];
	grip_low 	= joy->buttons[7];
	
	//checkController();
	
	if(mode==2){	
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
		else {
			rovRcIn.channels[MOTOR1] = middleStabil;
			rovRcIn.channels[MOTOR2] = middleMotor;
			rovRcIn.channels[MOTOR3] = middleMotor;		
		}
	
		//steering
		if (steering > 0){rovRcIn.channels[STEERING_PIN] = maxSteering;}//right
		else if (steering < 0){rovRcIn.channels[STEERING_PIN] = minSteering;}//left
		else {rovRcIn.channels[STEERING_PIN] = middleSteering;}
	
		//throttle
		if (throttle > 0){rovRcIn.channels[THROTTLE_PIN] = maxThrottle;}
		else if (throttle < 0){rovRcIn.channels[THROTTLE_PIN] = minThrottle;}
		else {rovRcIn.channels[THROTTLE_PIN] = middleThrottle;}
		
		
		if (grip_high > 0){
			//rovRcIn.channels[SERVO] = pwmServo + 100;
			pwmServo = pwmServo + change_servo;
			tempPwm = pwmServo;
			if(pwmServo > maxServo){pwmServo = maxServo;}
		}
		else if(grip_low > 0){ 
			//rovRcIn.channels[SERVO] = pwmServo - 100;
			pwmServo = pwmServo - change_servo;
			tempPwm = pwmServo;
			if(pwmServo < minServo){pwmServo = minServo;}
		}
		else{
			//rovRcIn.channels[SERVO] = pwmServo + 0;
			pwmServo = pwmServo + 0;
		}
		
		rovRcIn.channels[SERVO1] = pwmServo;
		rovRcIn.channels[SERVO2] = pwmServo;
		pub_override_rc.publish(rovRcIn);		
	}	
}



void checkController(){	
	cout<<a<<"\t"<<b<<"\t"<<c<<"\t"<<d<<"\t"<<e<<"\t"<<f<<endl;
	cout<<g<<"\t"<<h<<"\t"<<i<<"\t"<<j<<"\t"<<k<<"\t"<<l<<endl;
	cout<<m<<"\t"<<n<<endl;
	cout<<endl<<endl<<endl;
}


void control_cb (const gabut::number_rc& msg){
	mode=msg.rc_number;
}
