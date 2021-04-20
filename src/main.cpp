#include <DeepFake.hpp>

int main(int argc, char** argv) {
  DeepFake* app = DeepFake::GetInstance();
  app->run("assets/toto.webm");

  cv::waitKey(0);
  
  return 0;
}