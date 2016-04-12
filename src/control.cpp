#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h> 
#include <turtlesim/Pose.h>
#include <math.h>
#include <sstream>

  float angular_velocity;
  float linear_velocity;
  float theta;
  float x;
  float y;
  float X;
  float Y;

  float alpha;
  float new_angle;
  float distance=0;

void Pose_Callback(const turtlesim::Pose::ConstPtr& Pose)
{
  angular_velocity=Pose->angular_velocity;
  linear_velocity=Pose->linear_velocity;
  theta=Pose->theta;
  x=Pose->x;
  y=Pose->y;
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "control");

  ros::NodeHandle n;

  ros::Publisher turtle_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
  ros::Subscriber turtle_sub = n.subscribe("/turtle1/pose", 1000, Pose_Callback);  

  ros::Rate loop_rate(50);
  
  geometry_msgs::Twist g_msgs;

 
  while (ros::ok())
  {
    if(distance<0.05) distance=0;

    if(!distance){
    scanf("%f",&X);
    scanf("%f",&Y);
    }

    alpha= atan((Y-y)/(X-x));
    new_angle=theta-alpha;
    distance= sqrtf(pow((Y-y),2) + pow((X-x),2));
    
    if(distance>0.02 && distance<1) distance=1;
    if((X<x && Y<y) || (X<x && Y>y)) new_angle= new_angle+M_PI;

    g_msgs.linear.x = distance/3;
    g_msgs.angular.z = -new_angle*5;

    ROS_INFO("x=        %f", x);
    ROS_INFO("y=        %f", y);
    ROS_INFO("X=        %f", X);
    ROS_INFO("Y=        %f", Y);
    ROS_INFO("alpha=    %f", alpha);
    ROS_INFO("theta=    %f", theta);
    ROS_INFO("new_angle %f", new_angle);
    ROS_INFO("distance  %f", distance);

    turtle_pub.publish(g_msgs);
  

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}
