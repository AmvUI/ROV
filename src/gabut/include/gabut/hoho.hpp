#include <iostream>
#include <stdio.h>

int camera_rov = 0;
int camera_mini = 1;

#define STEERING_PIN 		0
#define THROTTLE_PIN 		2
#define MOTOR1 				1
#define MOTOR2  			3
#define MOTOR3				4
#define SERVO1				5
#define SERVO2				6

#define maxStabil 		1720
#define maxMotor 		1720
#define maxSteering 	1720
#define maxThrottle 	1720


#define minStabil 		1120
#define minMotor		1120
#define minSteering 	1120
#define minThrottle 	1120

#define middleStabil  	1520
#define middleMotor  	1520
#define middleSteering  1520
#define middleThrottle 	1520

float pwmServo =	1500;
float maxServo =	1900;
float minServo =	1100;

int change_servo = 100;

int mode_number=2;
int manual_number =2;
int auto_number =1;

int x_init=0;
int y_init=180;
int width=280; //width 400 for simple
int height=180;
							//nuc  pc
int LowH_red 	= 0; 		//0  
int HighH_red 	= 184;		//184 
int LowS_red 	= 130;      //130  65
int HighS_red 	= 248;      //248  246
int LowV_red 	= 49;		//49   242
int HighV_red 	= 230;		//230  255
int Noise_red 	= 15;

int LowH_blue 	= 0; 		//0  
int HighH_blue 	= 184;		//184 
int LowS_blue 	= 130;      //130  65
int HighS_blue 	= 248;      //248  246
int LowV_blue 	= 49;		//49   242
int HighV_blue 	= 230;		//230  255
int Noise_blue 	= 15;

int red_setpoint = 320;
int blue_setpoint = 320;
