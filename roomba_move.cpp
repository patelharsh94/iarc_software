#include "ros/ros.h"
#include <math.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include <time.h>


/**
 * This tutorial demonstrates simple sending of velocity commands to the IRobot Create in Gazebo.
   Some of the code was taken from: http://guitarpenguin.is-programmer.com/posts/58100.html
   by, Harsh Patel
   Date: 07/25/2015
 */

int main(int argc, char **argv)
{
  /* initialize random seed: */
  srand (time(NULL)); 

  double radConverter = (M_PI/180);
  double currDeg = 0.0;         // variable used to calculate degrees
  double finalRad = 0.0;        // the final degree
  double randVal = 0.0;         // the random degree of change.

  int noiseDir = 0;             // will be 1 if positive noise, else
                                // negative noise, so turn direction will
                                // change.
  bool isFive;                  // to check if 5 secs have passed.
  bool isTwenty;                // to check if 20 secs have passed.

  ros::init(argc, argv, "CreateController");

  ros::NodeHandle n;

  double sec = ros::Time::now().toSec();

  ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

  ros::Rate loop_rate(1);
 
  // setting the last check time.
  ros::Time lastChecked_5 = ros::Time::now();
  ros::Time lastChecked_20 = ros::Time::now();

  while (ros::ok())
  {
    // to write messages
    geometry_msgs::Twist cmd_vel;
    ROS_INFO("current degrees: %f",currDeg);
    
    // printing to console.
    ROS_INFO("current time  : %f",ros::Time::now().toSec());
    cmd_vel.linear.x = .33;
    // as we cross the 5 sec mark..
    if(ros::Time::now().toSec() - lastChecked_5.toSec() >= 5.0)
    {
      isFive = true;
    }
    // if we cross the twenty second mark, only run the twenty second 
    if(ros::Time::now().toSec() - lastChecked_20.toSec() >= 20.0)
    {
      isFive = false;
      isTwenty = true;
    }

    if(isFive)
    {
      // picks a number from 0-2
      noiseDir = rand() % 2; 
      // get random value from 0-20
      randVal = rand() % 21;
      
      // move counter clock wise
      if(noiseDir < 1)
      {
        currDeg = -1*randVal;
      }
      else    // move clock wise
      {
        
        currDeg = randVal;
      }

      ROS_INFO("\n\n\nFive seconds, degrees: %f\n\n\n",currDeg);
      // conver to radians
      finalRad = currDeg * radConverter;
      // spin
      cmd_vel.angular.z = finalRad;
      
      // reset.
      //currDeg = 0.0;
      lastChecked_5 = ros::Time::now();   
      // resetting the five sec boolean.
      isFive = false;
    }
    else if (isTwenty)
    {
      // 180 deg change
      currDeg += 180;
      // for the wrap around.
      //currDeg %= 360;
      ROS_INFO("\n\n\nTwenty seconds, degrees: %f\n\n\n",currDeg);
      // conver to radians
      finalRad = currDeg * radConverter;
      // spin
      cmd_vel.angular.z = finalRad;

      // reset
      lastChecked_20 = ros::Time::now();  
      lastChecked_5 = ros::Time::now();
      isTwenty = false;
    }
    else
    {
      // keep moving straight.
      cmd_vel.linear.x = .33;
    }
    
    vel_pub.publish(cmd_vel);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
