#include <DeepFake.hpp>
#include <Image.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

#define MIN_CORNERS 100

DeepFake* DeepFake::GetInstance() {
    // Not thread safe
    static DeepFake* instance = new DeepFake();
    return instance;
}

DeepFake::DeepFake() {}

std::vector<cv::Point2f> purgePoints(const std::vector<cv::Point2f>& points,
                                     const std::vector<uchar>& status) {
  std::vector<cv::Point2f> result;
  for (int i = 0; i < points.size(); ++i) {
    if (status[i] > 0) result.push_back(points[i]);
  }
  return result;
}


void DeepFake::detectPoints(const cv::Rect& area, cv::Mat& img) {
  if (area.area() < 10) return;

  // on crée une nouvelle image a partir du roi
  cv::Mat mask(img, area);

  // on utilise goodFeaturesToTrack pour detecter les points d’interet
  // dans roi et les stocker dans prevPoints
  cv::goodFeaturesToTrack(mask, prevPoints, 500, 0.01, 10);

  // on décale les points d'interet, pour qu'ils correspondent à la selection
  for (int i = 0; i < prevPoints.size(); ++i) {
    prevPoints[i] = cv::Point2f(prevPoints[i].x + area.x, prevPoints[i].y + area.y);
  }
}

void DeepFake::trackPoints(const cv::Rect& area) {
  // si prevInput n’est pas vide
  if (!prevInput.empty()) {
    // on met le contenu de nextPoints dans prevPoints
    prevPoints.resize(nextPoints.size());
    for (int i = 0; i < nextPoints.size(); ++i) {
      prevPoints[i] = nextPoints[i];
    }

    // si prevPoints ne contient pas assez de points
    //if (prevPoints.size() < MIN_CORNERS) {
      // on detecte les points sur prevInput
      detectPoints(area, prevInput);
    //}

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

void DeepFake::analyse(cv::Mat& img) {
    cv::CascadeClassifier faceCascade;
    faceCascade.load("assets/models/haarcascade_frontalface_default.xml");

    if(faceCascade.empty()) { std::cout<< "xml file not loaded" << std::endl; }

    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(img, faces, 1.1, 10);

    for(int i = 0; i < faces.size(); i++) {
        trackPoints(faces[i]);
        rectangle(img, faces[i].tl(), faces[i].br(), cv::Scalar(255,0,255), 2);
    }
}

void DeepFake::draw(cv::Mat& img) const {
  for (int i = 0; i < nextPoints.size(); ++i) {
    cv::line(img, prevPoints[i], nextPoints[i], cv::Scalar(0, 255, 0), 2);
    cv::circle(img, nextPoints[i], 5, cv::Scalar(255, 0, 0), 2);
  }

  // cv::rectangle(img, roi, cv::Scalar(0, 0, 255), 3);

  cv::imshow("input", img);
}

void DeepFake::run(const std::string& videoname) {
    Image img("assets/therock.jpg");
    DeepFake::analyse(img.frame());
    img.show();

    cv::VideoCapture cap;
    cv::Mat frame;

    // si videoname n’est pas null, ouvrir la video dans cap, sinon ouvrir la camera
    if (videoname.size() > 0) {
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
        DeepFake::analyse(frame);

        // on dessine
        draw(frame);

        // on recupere une nouvelle image et la stocker dans nextInput
        cap >> frame;
        cv::cvtColor(frame, nextInput, cv::COLOR_RGB2GRAY);

        if ((char)27 == (char)cv::waitKey(10)) break;
    }
}