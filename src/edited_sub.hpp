#pragma once
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32.h>
#include <ros/ros.h>
#include "util.hpp"
#include <time.h>
#include <iostream>
#include <thread>
#include <exception>
using namespace cv;
using namespace std;

class EditedSub{
public:
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    ros::NodeHandle flag_nh_;
    image_transport::Subscriber image_sub;
    image_transport::Publisher image_pub;
    ros::Subscriber int_sub;

    EditedSub();
    void imageCb(const sensor_msgs::ImageConstPtr& msg);
    void flagCb(const std_msgs::Int32& msg);
    Mat ret_img();
    void cycle();
    int retCommFlag();
    Mat retImg;
    int ret_flag;

private:
};
