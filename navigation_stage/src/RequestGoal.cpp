/*
 * SendGoals.cpp
 *
 *  Created on: Dec 7, 2013
 *      Author: roiyeho
 */

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_datatypes.h>
#include <nav_msgs/Odometry.h>
#include <time.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

//declare subscriber node
ros::Subscriber SubGoal;

//variable declarations
double theta;
static double xpos;
static double ypos;

//function declarations.
void poseCallback(const nav_msgs::Odometry::ConstPtr& poseprey);

//Callback function
void poseCallback(const nav_msgs::Odometry::ConstPtr& poseprey)
{
    xpos = poseprey->pose.pose.position.x;
    ypos = poseprey->pose.pose.position.y;
}

//Main Function
int main(int argc, char** argv)
{
    ros::init(argc, argv, "RequestGoal");
    ros::NodeHandle nh;

    //get Parameter from launch file
    nh.getParam("goal_x", xpos);
    nh.getParam("goal_y", ypos);

    /*Subscribe to the position of robot_1, that is, the prey, and call the
    poseCallback function to execute*/
    SubGoal = nh.subscribe("/robot_1/base_pose_ground_truth", 100, & poseCallback);

    //create Action CLient ac
    MoveBaseClient ac("robot_0/move_base", true);

    // Wait max 5 seconds for the action server to become available
    while (!ac.waitForServer(ros::Duration(5)))
    {
       ROS_INFO("Waiting for the move_base action server");
    }

    ROS_INFO("Connected to move base server, sending goal");

    //goal message is created, the goal values are set
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = xpos;
    goal.target_pose.pose.position.y = ypos;
    ROS_INFO("first pose: x = %f, y = %f", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);

     // Convert the Euler angle to quaternion
    double radians = theta * (M_PI/180);
    tf::Quaternion quaternion;
    quaternion = tf::createQuaternionFromYaw(radians);
    geometry_msgs::Quaternion qMsg;
    tf::quaternionTFToMsg(quaternion, qMsg);
    goal.target_pose.pose.orientation = qMsg;

    ROS_INFO("You have set goal parameters!");

    // Send goal message to the action server
    ac.sendGoal(goal);
    ROS_INFO("You have sent the goal to the server!");
    // Send the goal command
    ROS_INFO("Sending robot to: x = %f, y = %f", xpos, ypos);

    /*Wait 1 sec for the action to return. Here the server provides
    the optimal path, along which the hunter has to drive*/
    ac.waitForResult(ros::Duration(1.0));
    //call the Callback function once.(For receiving after 1 sec new prey coordinates)
    ros::spinOnce();
    ROS_INFO("first pose: x = %f, y = %f", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
    ROS_INFO("second pose 1s later: x = %f, y = %f",xpos, ypos);

    //while the goal is not yet reached
    while (ac.getState() != actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      // while the new retrieved coordinates are not the same as the old ones
      while ((goal.target_pose.pose.position.x != xpos) && (goal.target_pose.pose.position.y != ypos) )
      {
        //set new retrieved coordinates as current goal location
        goal.target_pose.pose.position.x = xpos;
        goal.target_pose.pose.position.y = ypos;
        double radians = theta * (M_PI/180);
        tf::Quaternion quaternion;
        quaternion = tf::createQuaternionFromYaw(radians);
        geometry_msgs::Quaternion qMsg;
        tf::quaternionTFToMsg(quaternion, qMsg);
        goal.target_pose.pose.orientation = qMsg;

        //send new goal
        ac.sendGoal(goal);
        ROS_INFO("You have sent a new goal to the server! It is x= %f, y = %f", xpos, ypos);

        /*follow the prey for the specified update time (here 0.2s) and call the callback function once again
        to retrieve new prey coordinates*/
        ac.waitForResult(ros::Duration(0.2));
        ros::spinOnce();
      }
      /*while old and new coordinates are the same
      follow last depicted path for the specifies update time and
      call the callback function once again*/
      ac.waitForResult(ros::Duration(0.2));
      ros::spinOnce();
    }
       //If action state returns SUCCEEDED, display "you have reached the goal"
    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      ROS_INFO("You have reached the goal!");
    }

    else
    {
      ROS_INFO("The base failed for some reason");
    }
    return 0;
}












// typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
// // Get the goal's x, y and angle from the launch file
//
// double xpos, ypos, theta;
// void poseCallback(const nav_msgs::Odometry::ConstPtr& poseprey);
// void RequestGoal()
//
// int main(int argc, char** argv) {
//     ros::init(argc, argv, "RequestGoal");
//
//     ros::NodeHandle nh;
//
//     //nh.getParam("goal_x", x);
//     //nh.getParam("goal_y", y);
//     //nh.getParam("goal_theta", theta);
//
//     ros::Subscriber SubGoal = nh.subscribe("/robot_1/base_pose_ground_truth", 100, & poseCallback);
//
//     return 0;
// }
//
//
// void poseCallback(const nav_msgs::Odometry::ConstPtr& poseprey)
// {
// double theta;
// double xpos = poseprey->pose.pose.position.x;
// double ypos = poseprey->pose.pose.position.y;
// }
// ROS_INFO("x= %f, y= %f",xpos, ypos);
//
// //tell the action client that we want to spin a thread by default
// //by connecting us to the server name "move_base"
//  MoveBaseClient ac("robot_0/move_base", true);
//
//  // Wait 10 seconds for the action server to become available
//  while (!ac.waitForServer(ros::Duration(10))) {
//    ROS_INFO("Waiting for the move_base action server");
//  };
//  ROS_INFO("Connected to move base server, sending goal");
//
//
// //goal message is created, the goal value is set
// move_base_msgs::MoveBaseGoal goal;
// goal.target_pose.header.frame_id = "map";
// goal.target_pose.header.stamp = ros::Time::now();
//
// goal.target_pose.pose.position.x = xpos;
// goal.target_pose.pose.position.y = ypos;
//
//     // Convert the Euler angle to quaternion
// double radians = theta * (M_PI/180);
// tf::Quaternion quaternion;
// quaternion = tf::createQuaternionFromYaw(radians);
//
// geometry_msgs::Quaternion qMsg;
// tf::quaternionTFToMsg(quaternion, qMsg);
//
// goal.target_pose.pose.orientation = qMsg;
//
// // Send the goal command
// //ROS_INFO("Sending robot to: x = %f, y = %f, theta = %f", x, y, theta);
//
// // Goal message is sent to the action server "move_base"
// ac.sendGoal(goal);
//
// // Wait for the action to return
// ac.waitForResult(ros::Duration(10.0));
//
// if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
//   {
//   ROS_INFO("You have reached the goal!");
//   //return true;
//   }
// else
//   {
//   ROS_INFO("The base failed for some reason");
//   //return false;
//   }
//}
