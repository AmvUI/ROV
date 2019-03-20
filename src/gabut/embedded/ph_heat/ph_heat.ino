#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

ros::NodeHandle nh;
std_msgs::Float32MultiArray haha;
ros::Publisher chatter("/mate/sensor/value", &haha);

float ph, heat;

void setup(){
  nh.initNode();
  nh.advertise(chatter);
}
  
void loop(){
  nh.spinOnce();
  haha.data[0]=heat;
  haha.data[1]=ph;
  chatter.publish( &haha );
  delay(1);
}
