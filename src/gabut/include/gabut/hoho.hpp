#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>

using namespace std;

float kp = 1.2;//1.5
float ki = 0.2;//1
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

#define minTrim 		1100
#define maxTrim 		1700

#define maxRoll			1700
#define minRoll			1100

#define maxStabil 		1900
#define maxMotor 		1900
#define maxSteering 	1900
#define maxThrottle 	1900


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
#define autoThrottle	1700

float pwmServo =	1500;
float maxServo =	1900;
float minServo =	1100;

int change_servo = 100;

int mode_number=2;
int manual_number =2;
int auto_number =1;

int x_init=0;
int y_init=240;
int width=640; //width 400 for simple
int height=180;
							//nuc  pc
int LowH_red 	= 0; 		//0  
int HighH_red 	= 184;		//184 
int LowS_red 	= 130;      //130  65
int HighS_red 	= 248;      //248  246
int LowV_red 	= 49;		//49   242
int HighV_red 	= 230;		//230  255
int Noise_red 	= 15;

int LowH_blue 	= 88; 		//0  
int HighH_blue 	= 170;		//184 
int LowS_blue 	= 40;      //130  65
int HighS_blue 	= 197;      //248  246
int LowV_blue 	= 34;		//49   242
int HighV_blue 	= 133;		//230  255
int Noise_blue 	= 15;

int LowH_black 	= 0; 		//0  
int HighH_black = 184;		//184 
int LowS_black	= 130;      //130  65
int HighS_black = 248;      //248  246
int LowV_black	= 49;		//49   242
int HighV_black = 230;		//230  255
int Noise_black = 50;
int min_radius_black=100;
int max_radius_black=150;

int red_setpoint = 320;
int blue_setpoint = 320;

int strip_x 	= 500;
int strip_y1 	= 50;
int strip_y2	= 100;
int circle_x 	= 500;
int circle_y	= 180;
int square_x	= 480;
int square_y 	= 250;
int tri_x1 		= 500;
int tri_y1		= 350;
int tri_x2		= 530;
int tri_y2		= 400;
int tri_x3 		= 470;
int tri_y3		= 400;

int strip_x_text = 550;
int strip_y_text = 100;
int circle_x_text = 550;
int circle_y_text = 200;
int square_x_text = 550;
int square_y_text = 300;
int tri_x_text =  550;
int tri_y_text = 400;

string strip_text="2";
string circle_text="2";
string square_text="1";
string tri_text="1";

int thickness = 2;

int ph_x = 20;
int ph_y = 50;
int heat_x = 20;
int heat_y = 100;

string ph_text = "Ph :";
string heat_text = "Heat :";

int ph_x_value = 100;
int ph_y_value = 50;
int heat_x_value = 120;
int heat_y_value = 100;
