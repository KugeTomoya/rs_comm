#include "edited_sub.hpp"
EditedSub::EditedSub():it_(nh_),flag_nh_(""){
  ret_flag = 0;
   // カラー画像をサブスクライブ                                                                
    image_sub = it_.subscribe("/detectron_img", 100,
      &EditedSub::imageCb, this);
    // 処理した画像をパブリッシュ                                                                                          
    image_pub = it_.advertise("/image_topic", 1);
    int_sub = flag_nh_.subscribe("/detectron_flag",100,&EditedSub::flagCb,this);
}
void EditedSub::imageCb(const sensor_msgs::ImageConstPtr& msg){
  try
    {
      // ROSからOpenCVの形式にtoCvCopy()で変換。cv_ptr->imageがcv::Matフォーマット。
      retImg  = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
      //cv_ptr3   = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
    }
  catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
}

void EditedSub::flagCb(const std_msgs::Int32& msg){
  ret_flag = msg.data;
}
Mat EditedSub::ret_img(){
  return retImg;
}
int EditedSub::retCommFlag(){
  return ret_flag;
}
void EditedSub::cycle(){
  ros::spinOnce();
}