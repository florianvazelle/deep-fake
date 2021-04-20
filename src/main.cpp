#include <opencv2/core/types_c.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <Image.hpp>

cv::Mat openImg(std::string path) {
  cv::Mat img = cv::imread(path, 0);
  if(img.empty())
    std::cout << "Could not read the image: " << path << std::endl;
  return img;
}

int main(int argc, char** argv) {
  // create an image
  Image img("assets/therock.jpg");

  // display the image
  img.show();

  // wait for the user to press any key:
  cv::waitKey(0);

  return 0;
}