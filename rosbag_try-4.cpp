#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <string>
#include <iostream>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <boost/foreach.hpp>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

namespace enc = sensor_msgs::image_encodings;
using namespace cv;

void print_status(const sensor_msgs::ImageConstPtr& l_img)
{
     //ROS_INFO_STREAM("Iam here");
     cv_bridge::CvImagePtr cv_ptr;
     Mat cv_clone;
     try
     {
          cv_ptr = cv_bridge::toCvCopy(l_img, enc::BGR8);
          cv_clone = cv_ptr->image.clone();
     }
     catch (cv_bridge::Exception& e)
     {
	ROS_ERROR("There is an error : %s", e.what());
     }
     Mat image;
     cv_clone.copyTo(image);
     if (!image.data)
	ROS_INFO_STREAM("It is empty");
         
 
     // Display the image      
     // imshow("Original Image",image);
    //  cvWaitKey(33);
     

     Mat contours;
     Canny(image,contours,125,350);
     imshow("Canny",contours);
      cvWaitKey(33);        
}

int main(int argc, char**argv)
{
  ros::init(argc, argv, "try");
  rosbag::Bag bag1;
  bag1.open("/home/nikhil2510/Desktop/workspace/test.bag", rosbag::bagmode::Read);
  std::vector<std::string> topics;
  topics.push_back(std::string("/camera/color/image_raw"));
  cv::namedWindow("Original Image");
  rosbag::View view(bag1, rosbag::TopicQuery(topics));
  ros::NodeHandle nh;
 // image_transport::ImageTransport it(nh);
 // image_transport::Subscriber sub = it.subscribe("/camera/color/image_raw", 1, print_status);


 BOOST_FOREACH(rosbag::MessageInstance const m, view)
    {
        sensor_msgs::Image::ConstPtr l_img = m.instantiate<sensor_msgs::Image>();
        if (l_img != NULL)
           print_status(l_img);
    }

    bag1.close();
}

