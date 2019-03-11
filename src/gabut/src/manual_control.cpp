#include "ros/ros.h"
#include "mavros_msgs/OverrideRCIn.h"
#include <iostream>
#include <stdio.h>
#include "gabut/mode.h"
#include <sensor_msgs/Joy.h>

using namespace std;

#define STEERING_PIN 0
#define THROTTLE_PIN 2

#define MOTOR1 		1

#define MOTOR2 		3
#define MOTOR3		4

#define SERVO1		5
#define SERVO2		6

#define max_throttle 	1720
#define max_steering  	1720
#define max_motor1 		1720
#define max_motor2 		1720
#define max_motor3 		1720

#define min_throttle 	1120
#define min_steering  	1120
#define min_motor1 		1120
#define min_motor2 		1120
#define min_motor3 		1120

#define middle_throttle 1520
#define middle_steering 1520
#define middle_motor1 	1520
#define middle_motor2 	1520
#define middle_motor3 	1520

float middle_servo 	=	1500;
float max_servo 	=	1900;
float min_servo	 	=	1100;

int a, b, c, d, e, f;
int g, h, i, j, k, l, m, n, o, p, q;

int pwm_servo;

int mode;

int throttle, steering, sink, grip_high, grip_low;

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
void control_cb (const gabut::mode& msg);

mavros_msgs::OverrideRCIn rovRcIn;

int main(int argc, char** argv){
	ros::init(argc, argv, "subPixhawk");
	ros::NodeHandle n;
	
	ros::Publisher pub_override_rc 	= n.advertise<mavros_msgs::OverrideRCIn>("/mavros/rc/override", 8);
	ros::Subscriber joy_sub 		= n.subscribe<sensor_msgs::Joy>("joy", 8, &joyCallback);
	ros::Subscriber rc_in_sub 		= n.subscribe("/mate/rov/mode", 8, control_cb);
		
	while( ros::ok() ){	
		ros::spinOnce();
		while(mode==2){	
			for(int i=0; i < 8; i++) rovRcIn.channels[i] = 0;	//Releases all Channels First
		
			//up 
			if (sink > 0){
				rovRcIn.channels[MOTOR2] = min_motor2;
				rovRcIn.channels[MOTOR3] = min_motor3;
			}
			//down
			else if (sink < 0){
				rovRcIn.channels[MOTOR2] = max_motor2;
				rovRcIn.channels[MOTOR3] = max_motor3;
			}
			
			else {
				rovRcIn.channels[MOTOR2] = middle_motor2;
				rovRcIn.channels[MOTOR3] = middle_motor3;
			}
	
			//steering
			if (steering > 0){rovRcIn.channels[STEERING_PIN] = max_steering;}//right
			else if (steering < 0){rovRcIn.channels[STEERING_PIN] = min_steering;}//left
			else {rovRcIn.channels[STEERING_PIN] = middle_steering;}
	
			//throttle
			if (throttle > 0){rovRcIn.channels[THROTTLE_PIN] = max_throttle;}
			else if (throttle < 0){rovRcIn.channels[THROTTLE_PIN] = min_throttle;}
			else {rovRcIn.channels[THROTTLE_PIN] = middle_throttle;}
		
			sleep(0.2);
		
			if (grip_high > 0){
				//rovRcIn.channels[SERVO] = pwmServo + 100;
				pwm_servo = pwm_servo + 0.1;
				if(pwm_servo > max_servo){pwm_servo = max_servo;}
			}
			else if(grip_low > 0){ 
				//rovRcIn.channels[SERVO] = pwmServo - 100;
				pwm_servo = pwm_servo - 0.1;
				if(pwm_servo < min_servo){pwm_servo = min_servo;}
			}
			else{
				//rovRcIn.channels[SERVO] = pwmServo + 0;
				pwm_servo = pwm_servo + 0;
			}
		
			rovRcIn.channels[SERVO1] 	= pwm_servo;
			rovRcIn.channels[SERVO2] 	= pwm_servo;
			
			pub_override_rc.publish(rovRcIn);	
		}
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
	
	throttle 	= joy->axes[1];
	steering 	= joy->axes[3];
	sink 		= joy->axes[2];
	grip_high 	= joy->buttons[5];
	grip_low 	= joy->buttons[7];
	
	//checkController();
}


void checkController(){	
	cout<<a<<"\t"<<b<<"\t"<<c<<"\t"<<d<<"\t"<<e<<"\t"<<f<<endl;
	cout<<g<<"\t"<<h<<"\t"<<i<<"\t"<<j<<"\t"<<k<<"\t"<<l<<endl;
	cout<<m<<"\t"<<n<<endl;
	cout<<endl<<endl<<endl;
}

void control_cb (const gabut::mode& msg){
	mode=msg.mode;
}
