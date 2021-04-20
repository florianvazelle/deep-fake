#include <DeepFake.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

DeepFake* DeepFake::GetInstance() {
    // Not thread safe
    static DeepFake* instance = new DeepFake();
    return instance;
}

DeepFake::DeepFake() {}

void DeepFake::draw(cv::Mat& img) const {
  for (int i = 0; i < nextPoints.size(); ++i) {
    cv::line(img, prevPoints[i], nextPoints[i], cv::Scalar(0, 255, 0), 2);
    cv::circle(img, nextPoints[i], 5, cv::Scalar(255, 0, 0), 2);
  }

  // cv::rectangle(img, roi, cv::Scalar(0, 0, 255), 3);

  cv::imshow("input", img);
}

void DeepFake::run(const std::string& videoname) {
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
        cv::CascadeClassifier faceCascade;
        faceCascade.load("assets/models/haarcascade_frontalface_default.xml");

        if(faceCascade.empty()) { std::cout<< "xml file not loaded" << std::endl; }

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(frame, faces, 1.1, 10);

        for(int i = 0; i < faces.size(); i++){
            rectangle(frame, faces[i].tl(), faces[i].br(), cv::Scalar(255,0,255), 2);
        }

        // on dessine
        draw(frame);

        // on recupere une nouvelle image et la stocker dans nextInput
        cap >> frame;
        cv::cvtColor(frame, nextInput, cv::COLOR_RGB2GRAY);

        if ((char)27 == (char)cv::waitKey(10)) break;
    }
}