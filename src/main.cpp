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

using namespace std;
using namespace cv;

#define MIN_CORNERS 100

// images extraite du flux vidéos
cv::Mat prevInput;
cv::Mat nextInput;

// les points détectés
std::vector<cv::Point2f> prevPoints;
std::vector<cv::Point2f> nextPoints;

cv::Rect roi;  // region of interest
cv::Point start(-1, -1);

std::vector<cv::Point2f> purgePoints(const std::vector<cv::Point2f>& points,
                                     const std::vector<uchar>& status) {
  std::vector<cv::Point2f> result;
  for (int i = 0; i < points.size(); ++i) {
    if (status[i] > 0) result.push_back(points[i]);
  }
  return result;
}

// Mouse Callback
void CallBackFunc(int event, int x, int y, int flags, void* userdata) {
  if (event == cv::EVENT_LBUTTONDOWN) {
    start = cv::Point(x, y);
    roi = cv::Rect();
    prevPoints.clear();
    nextPoints.clear();
  } else if (event == cv::EVENT_MOUSEMOVE) {
    if (start.x >= 0) {
      cv::Point end(x, y);
      roi = cv::Rect(start, end);
    }

  } else if (event == cv::EVENT_LBUTTONUP) {
    cv::Point end(x, y);
    roi = cv::Rect(start, end);
    start = cv::Point(-1, -1);
  }
}

void updateROI() {
  // si roi est vide ou si une selection est en cours, on quitte direct
  if (roi.empty() || start.x >= 0) return;

  // sinon, on calcule le rectangle englobant notre ensemble de points stockés
  // dans nextPoints, et on l’assigne a roi
  roi = cv::boundingRect(nextPoints);
}

void detectPoints(cv::Mat& img) {
  if (roi.area() < 10 || start.x >= 0) return;

  // on crée une nouvelle image a partir du roi
  cv::Mat mask(img, roi);

  // on utilise goodFeaturesToTrack pour detecter les points d’interet
  // dans roi et les stocker dans prevPoints
  cv::goodFeaturesToTrack(mask, prevPoints, 500, 0.01, 10);

  // on décale les points d'interet, pour qu'ils correspondent à la selection
  for (int i = 0; i < prevPoints.size(); ++i) {
    prevPoints[i] = cv::Point2f(prevPoints[i].x + roi.x, prevPoints[i].y + roi.y);
  }
}

void trackPoints() {
  // si prevInput n’est pas vide
  if (!prevInput.empty()) {
    // on met le contenu de nextPoints dans prevPoints
    prevPoints.resize(nextPoints.size());
    for (int i = 0; i < nextPoints.size(); ++i) {
      prevPoints[i] = nextPoints[i];
    }

    // si prevPoints ne contient pas assez de points
    if (prevPoints.size() < MIN_CORNERS) {
      // on detecte les points sur prevInput
      detectPoints(prevInput);
    }

    // si prevPoints ne contient toujours pas assez de points, on quitte
    if (prevPoints.size() < MIN_CORNERS) return;

    // a l'aide de calcOpticalFlowPyrLK, on calcule les nouvelles positions (nextPoints) dans la
    // nouvelle image (nextImage)
    std::vector<uchar> status;
    std::vector<float> error;
    cv::calcOpticalFlowPyrLK(prevInput, nextInput, prevPoints, nextPoints, status, error);

    // on supprime les points non suivis des deux listes de points
    prevPoints = purgePoints(prevPoints, status);
    nextPoints = purgePoints(nextPoints, status);
  }

  // on clone nextInput dans prevInput
  prevInput = nextInput.clone();
}

void draw(cv::Mat& img) {
  for (int i = 0; i < nextPoints.size(); ++i) {
    cv::line(img, prevPoints[i], nextPoints[i], cv::Scalar(0, 255, 0), 2);
    cv::circle(img, nextPoints[i], 5, cv::Scalar(255, 0, 0), 2);
  }

  cv::rectangle(img, roi, cv::Scalar(0, 0, 255), 3);

  cv::imshow("input", img);
  cv::setMouseCallback("input", CallBackFunc, &img);
}

void video(char* videoname = NULL) {
  cv::VideoCapture cap;
  cv::Mat frame;

  // si videoname n’est pas null, ouvrir la video dans cap, sinon ouvrir la camera
  if (videoname) {
    cap.open(videoname);
  } else {
    cap.open(0);  // open the default camera
  }

  // si cap n’est pas ouvert, quitter la fonction
  if (!cap.isOpened()) return;

  // on recupere une image depuis cap et la stocker dans nextInput
  cap >> frame;  // get a new frame from camera
  cv::cvtColor(frame, nextInput, cv::COLOR_RGB2GRAY);

  // tant que nextinput n’est pas vide
  while (!nextInput.empty()) {
    // on fait les traitements sur l’image
    trackPoints();
    updateROI();

    // on dessine
    draw(frame);

    // on recupere une nouvelle image et la stocker dans nextInput
    cap >> frame;
    cv::cvtColor(frame, nextInput, cv::COLOR_RGB2GRAY);

    if ((char)27 == (char)cv::waitKey(10)) break;
  }

  // the camera will be deinitialized automatically in VideoCapture destructor
}

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

  
  // display the image
  //img.show();
  imshow("Image", img);
  // wait for the user to press any key:
  cv::waitKey(0);

  return 0;
}