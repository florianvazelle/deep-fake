#include <dlib/cmd_line_parser.h>
#include <opencv2/core/types_c.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace dlib;

Mat openImg(std::string path) {
  Mat img = imread(path, 0);
  if(img.empty())
    std::cout << "Could not read the image: " << path << std::endl;
  return img;
}

int main(int argc, char** argv) {
  /* Define the command line options */
  command_line_parser parser;

  parser.add_option("h", "Display this help message.");
  parser.add_option("in", "The input image", 1);

  parser.parse(argc, argv);

  if (parser.option("h")) {
    std::cout << "Usage: deep_fake --in input_file\n";
    parser.print_options();
    return 0;
  }

  /* Get the input image */
  std::string in_file;

  if (parser.option("in")) {
    in_file = parser.option("in").argument();
  } else {
    std::cout << "Error in command line:\n   You must specify an input file.\n";
    std::cout << "\nTry the -h option for more information." << std::endl;
    return 0;
  }

  // create a gui window:
  namedWindow("Output", 1);

  //std::string image_path = samples::findFile("assets/therock.jpg");
  Mat img = openImg(in_file);

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