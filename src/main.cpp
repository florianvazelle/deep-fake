#include <opencv2/core/types_c.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat openImg(std::string path) {
  Mat img = imread(path, 0);
  if(img.empty())
    std::cout << "Could not read the image: " << path << std::endl;
  return img;
}

int main(int argc, char** argv) {
  // create a gui window:
  namedWindow("Output", 1);

  //std::string image_path = samples::findFile("assets/therock.jpg");
  Mat img = openImg("assets/therock.jpg");

  // initialize a 120X350 matrix of black pixels:
  Mat output = Mat::zeros(120, 350, CV_8UC3);

  // write text on the matrix:
  putText(output, "Hello World :)", cvPoint(15, 70), FONT_HERSHEY_PLAIN, 3, cvScalar(0, 255, 0), 4);

  // display the image:
  //imshow("Output", output);
  imshow("Output", img);

  // wait for the user to press any key:
  waitKey(0);

  return 0;
}