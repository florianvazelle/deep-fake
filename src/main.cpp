#include <dlib/cmd_line_parser.h>
#include <opencv2/core/types_c.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <Image.hpp>

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

  // create an image
  Image img(in_file);

  // display the image
  img.show();

  // wait for the user to press any key:
  waitKey(0);

  return 0;
}