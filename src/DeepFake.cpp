#include <DeepFake.hpp>
#include <Image.hpp>
#include <FaceDetector.hpp>
#include <Timer.hpp>

#include <iostream>
#include <vector>

#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/gui_widgets.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>

#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <fstream>

DeepFake* DeepFake::GetInstance() {
    // Not thread safe
    static DeepFake* instance = new DeepFake();
    return instance;
}

DeepFake::DeepFake() {}

void DeepFake::run(const std::string& filename) const {
    // Nombre de visage que l'on veut par image
    const int num_faces = 1;

    FaceDetector faceDect(filename, num_faces);
    Timer time;

    dlib::image_window win, picture;

    faceDect.basic().display(picture);

    std::vector<cv::Mat> masks;
    faceDect.basic().masks(masks);
    
    std::vector<cv::Point> centers;
    faceDect.basic().facesCenter(centers);

    // On récupère des frames et on les traite jusqu'à ce que la fenêtre principale soit fermée
    while(!win.is_closed()) {
        time.start();

        // On récupère une frame
        cv::Mat temp, output;
        if (faceDect >> temp) {
            break;
        }

        seamlessClone(faceDect.basic().toMat(), temp, masks[0], centers[0], output, cv::NORMAL_CLONE);
        faceDect.video().loadImage(output);

        time.display();

        faceDect.video().display(win);
    }
}