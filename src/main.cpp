#include <opencv2/core/types_c.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <Image.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <DeepFake.hpp>

using namespace std;
using namespace cv;

cv::Mat openImg(std::string path) {
  cv::Mat img = cv::imread(path, 0);
  if(img.empty())
    std::cout << "Could not read the image: " << path << std::endl;
  return img;
}

int main(int argc, char** argv) {
  // create an image
  //Image img("assets/therock.jpg");

  string path ="assets/therock.jpg";
  Mat img = imread(path);
  CascadeClassifier faceCascade;
  faceCascade.load("assets/models/haarcascade_frontalface_default.xml");

  if(faceCascade.empty()){cout<<"xml file not loaded"<<endl;}

  vector<Rect> faces;
  faceCascade.detectMultiScale(img,faces,1.1,10);

  for(int i =0; i< faces.size(); i++){
    rectangle(img,faces[i].tl(), faces[i].br(),Scalar(255,0,255),2);
  }

  DeepFake* app = DeepFake::GetInstance();
  app->run("assets/toto.webm");
  
  // display the image
  //img.show();
  imshow("Image", img);
  // wait for the user to press any key:
  cv::waitKey(0);

  return 0;
}