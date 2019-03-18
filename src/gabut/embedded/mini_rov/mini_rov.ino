#include <ros.h>
#include <Servo.h>
#include <sensor_msgs/Joy.h>
#include <ros.h>

ros::NodeHandle nh;

int motor_1=6;
int motor_2=7;

Servo motor1;
Servo motor2;  

float a, b, c, d, e, f;
float g, h, i, j, k, l, m, n;

void joyCb( const sensor_msgs::Joy& joy){
    a = joy.axes[0];   //-1  < right left  < 0 < left left < 1
    b = joy.axes[1];   //-1  < down left < 0 < up left   < 1
    c = joy.axes[2];   //-1  <   down right  < 0 < up right  < 1
    d = joy.axes[3];   //-1  <   right right <   0   <   left right  <   1
    e = joy.axes[4];   //-1  <   right     <   0   <   left    <   1
    f = joy.axes[5];   //-1  <   down    < 0 < up      < 1
    g = joy.buttons[0];  //triangle
    h = joy.buttons[1];  //circle
    i = joy.buttons[2];  //cross
    j = joy.buttons[3];  //square
    k = joy.buttons[4];  //l1
    l = joy.buttons[5];  //r1
    m = joy.buttons[6];  //l2
    n = joy.buttons[7];  //r2

    if(g>0){
      motor1.write(180);
      motor2.write(180);
    }
    else if(i>0){
      motor1.write(180);
      motor2.write(180);
    }
    else{
      motor1.write(90);
      motor2.write(90);
    }
    delay(100);

    if(j>0){
      motor1.write(0);
      motor2.write(180);
    }
    else if(h>0){
      motor1.write(180);
      motor2.write(0);
    }
    delay(100);
}

ros::Subscriber<sensor_msgs::Joy> sub("joy", &joyCb );

void setup(){
  nh.initNode();
  motor1.attach(motor_1); 
  motor2.attach(motor_2);   
  nh.subscribe(sub);
}
  
void loop(){
  nh.spinOnce();
  delay(10);
}
