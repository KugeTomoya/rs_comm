#include "source_pub.hpp"
SourcePub::SourcePub()
: it_(nh_)
{
    // カラー画像をサブスクライブ                                                                
    //image_sub_ = it_.subscribe("/camera/rgb/image_raw", 1,
    //&SourcePub::imageCb, this);
    // 処理した画像をパブリッシュ                                                                                          
    image_pub = it_.advertise("/source_img", 1);
    
}
void SourcePub::input(Mat input_img){
    pub_img = input_img.clone();
    msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", pub_img).toImageMsg();
    image_pub.publish(msg);
}
void SourcePub::cycle(){
    ros::spinOnce();
}