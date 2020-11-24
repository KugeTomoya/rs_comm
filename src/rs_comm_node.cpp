#include <ros/ros.h>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <librealsense2/rsutil.h>
#include "util.hpp"
#include "source_pub.hpp"
#include "edited_sub.hpp"
using namespace cv;
using namespace std;
#define TARGET_W 10
#define TARGET_H 300

int main(int argc, char **argv)
{
  ros::init(argc,argv,"hoge");
  ROS_INFO("Hello!!!!!");
  SourcePub src_pub;

  EditedSub edt_sub;
  Mat sub_img;


  rs2::colorizer color_map;
  rs2::config cfg;
  cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
  cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
  rs2::pipeline pipe;
  rs2::pipeline_profile profile = pipe.start(cfg);
  rs2::video_stream_profile depth_stream = profile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();
  rs2_intrinsics depth_intr = depth_stream.get_intrinsics();
  //pipe.start(cfg);
  int64_t cycleT=millis();

  while(ros::ok()) {
    rs2::frameset frames = pipe.wait_for_frames();
    // align
    rs2::align align(RS2_STREAM_COLOR);
    auto aligned_frames = align.process(frames);
    rs2::video_frame color_frame = aligned_frames.first(RS2_STREAM_COLOR);
    rs2::depth_frame depth_frame = aligned_frames.get_depth_frame();
    rs2::depth_frame depth_h = frames.get_depth_frame();
    cv::Mat color(cv::Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);
    cv::Mat depth(cv::Size(640, 480), CV_8UC3, (void*)depth_frame.get_data(), cv::Mat::AUTO_STEP);
    float width = depth_h.get_width();
    float height = depth_h.get_height();
    float dist_to_center = depth_h.get_distance(TARGET_W,TARGET_H);
    // get 3d-coordinate info from specified pixel position.
    float pixel[2] = {float(TARGET_W), float(TARGET_H)};
    float depth_fix = depth_frame.get_distance(pixel[0], pixel[1]);
    float point[3];   
    rs2_deproject_pixel_to_point(point, &depth_intr, pixel, depth_fix);
    //printf("dist:%f depth:%f\n",dist_to_center,depth_fix);
    //circle(color, Point(TARGET_W,TARGET_H), 10, Scalar(255, 0,0), -1, 8);
    src_pub.input(color);
    src_pub.cycle();

    edt_sub.cycle();

    if(edt_sub.retCommFlag()==1){
      //画像が送られてくるまで画像を読み込まない
      sub_img = edt_sub.ret_img().clone();
      cv::imshow("edited",sub_img);
    }
    //sub_img = edt_sub.ret_img().clone();
    cv::namedWindow("source", cv::WINDOW_AUTOSIZE);
    cv::imshow("source", color);
    //cv::imshow("edited",sub_img);
    cv::waitKey(1);

    if(millis()-cycleT>100){
        cycleT = millis();
        ROS_INFO("flag:%d",edt_sub.retCommFlag());
        //ROS_INFO("(x,y,z)=(%.4f,%.4f,%.4f)",point[0],point[1],point[2]);

    }
  }
  return 0;
}