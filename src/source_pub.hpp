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

class SourcePub{
public:
    SourcePub();
    void input(Mat input_img);
    void cycle();
    void imageCb(const sensor_msgs::ImageConstPtr& msg);

private:
    //送信するデータの宣言
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub;
    image_transport::Publisher image_pub;
    sensor_msgs::ImagePtr msg;

    Mat pub_img;
};