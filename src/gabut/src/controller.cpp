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

using namespace std;

int a, b, c, d, e, f;
int g, h, i, j, k, l, m, n, o, p, q;
int throttle, steering, sink, grip_high, grip_low;

int tempPwm;
int mode;

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
	ros::Subscriber sub_image_process 	= nh.subscribe("/mate/value/image", 1, image_process_cb);
	ros::Subscriber joy_sub 			= nh.subscribe<sensor_msgs::Joy>("joy", 8, &joyCallback);
	ros::Subscriber sub_mode_rc 		= nh.subscribe("/mate/rov/number", 1, &control_cb);
	ros::ServiceClient client 			= nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
	
	flight.request.base_mode = 0;
	flight.request.custom_mode = "MANUAL";
	client.call(flight);
	
	flight.request.base_mode = 0;
	flight.request.custom_mode = "MANUAL";
	client.call(flight);
		
	system("rosrun mavros mavsafety arm");
	system("rosrun mavros mavsafety arm");
	system("rosrun mavros mavsafety arm");
		
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
		if (steering < 0){rovRcIn.channels[STEERING_PIN] = maxSteering;}//right
		else if (steering > 0){rovRcIn.channels[STEERING_PIN] = minSteering;}//left
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

void image_process_cb(const gabut::image_value& image){
	state_red 		= image.state_red;
	state_blue 		= image.state_blue;
	
	if(mode==1){//mode auto	
		for(int i=0; i < 8; i++) rovRcIn.channels[i] = 0;	//Releases all Channels First
		
		rovRcIn.channels[MOTOR1] = autoStabil;
		rovRcIn.channels[MOTOR2] = autoMotor;
		rovRcIn.channels[MOTOR3] = autoMotor;		
		
		rovRcIn.channels[SERVO1] = pwmServo;
		
		pid_in.x = state_red;
		pid_in.t = pid_in.t+delta_t;
		pid_in.setpoint = red_setpoint;
		pub_pid_in.publish(pid_in);
			
		ros::spinOnce();
		if(state_blue > 0){
			mode==3;
		}
		
		autoSteering = middleSteering - control_effort;
		
		if(state_red==0){rovRcIn.channels[STEERING_PIN] = middleSteering;}
		else if(autoSteering >= maxSteering){rovRcIn.channels[STEERING_PIN] = maxSteering;}
		else if(autoSteering <= minSteering){rovRcIn.channels[STEERING_PIN] = minSteering;}
		else{rovRcIn.channels[STEERING_PIN] = autoSteering;}
		
		rovRcIn.channels[THROTTLE_PIN] = autoThrottle;
		
		//cout<<"1"<<endl;
		pub_override_rc.publish(rovRcIn);		
	}
	else if(mode==3){//mode blue	
		for(int i=0; i < 8; i++) rovRcIn.channels[i] = 0;	//Releases all Channels First
		
		rovRcIn.channels[MOTOR1] = autoStabil;
		rovRcIn.channels[MOTOR2] = autoMotor;
		rovRcIn.channels[MOTOR3] = autoMotor;
		
		rovRcIn.channels[SERVO1] = pwmServo;
		
		//maju
		rovRcIn.channels[STEERING_PIN] = middleSteering;
		rovRcIn.channels[THROTTLE_PIN] = autoThrottle;
		pub_override_rc.publish(rovRcIn);	
		sleep(1);
		
		//kanan
		rovRcIn.channels[STEERING_PIN] = maxSteering;
		rovRcIn.channels[THROTTLE_PIN] = middleThrottle;
		pub_override_rc.publish(rovRcIn);
		sleep(1);
		
		//diam
		rovRcIn.channels[STEERING_PIN] = middleSteering;
		rovRcIn.channels[THROTTLE_PIN] = middleThrottle;
		pub_override_rc.publish(rovRcIn);
		sleep(5);
		
		//kiri
		rovRcIn.channels[STEERING_PIN] = minSteering;
		rovRcIn.channels[THROTTLE_PIN] = middleThrottle;
		pub_override_rc.publish(rovRcIn);
		sleep(1);
		
		//maju
		rovRcIn.channels[STEERING_PIN] = middleSteering;
		rovRcIn.channels[THROTTLE_PIN] = autoThrottle;
		pub_override_rc.publish(rovRcIn);
		sleep(1);
		
		mode == 2;
		ros::spinOnce();			
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

void pid_receiver_cb(const pid::controller_msg& control){
	control_effort = control.u;
}
