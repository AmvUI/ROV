#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>

using namespace std;

float kp = 1.65;//1.5
float ki = 1;//1
float kd = 0.5;//0.5

float update_time=0.01;
float delta_t = 0.01;
float initial_time = 0; 

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

#define autoStabil 		1520
#define autoMotor		1520
#define autoThrottle	1520

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

int LowH_black 	= 0; 		//0  
int HighH_black = 184;		//184 
int LowS_black	= 130;      //130  65
int HighS_black = 248;      //248  246
int LowV_black	= 49;		//49   242
int HighV_black = 230;		//230  255
int Noise_black = 15;
int min_radius_black=10;
int max_radius_black=15;

int red_setpoint = 320;
int blue_setpoint = 320;

int strip_x 	= 100;
int strip_y1 	= 100;
int strip_y2	= 150;
int circle_x 	= 100;
int circle_y	= 100;
int square_x	= 200;
int square_y 	= 200;
int tri_x1 		= 100;
int tri_x2		= 150;
int tri_x3 		= 300;
int tri_y1		= 230;
int tri_y2		= 320;
int tri_y3		= 260;

int tri_x_text =  100;
int tri_y_text = 100;
int square_x_text = 200;
int square_y_text = 200;
int circle_x_text = 300;
int circle_y_text = 300;
int strip_x_text = 400;
int strip_y_text = 400;

string strip_text;
string circle_text;
string square_text;
string tri_text;
