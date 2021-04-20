#include <DeepFake.hpp>

#include <iostream>
#include <vector>

#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

DeepFake* DeepFake::GetInstance() {
    // Not thread safe
    static DeepFake* instance = new DeepFake();
    return instance;
}

DeepFake::DeepFake() {
    dlib::deserialize(SHAPE_PREDICTOR_68_FACE_LANDMARKS) >> m_face_landmark;
}

void DeepFake::run(const std::string& filename) const {
        cv::VideoCapture cap("assets/toto.webm");

    // si cap n’est pas ouvert, quitter la fonction
    if (!cap.isOpened()) {
        std::cerr << "Unable to connect to camera" << std::endl;
        return;
    }

    dlib::image_window win;

    // Load face detection and pose estimation models.
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    double fps = 0;

    // Grab and process frames until the main window is closed by the user.
    while(!win.is_closed()) {
        auto time_start = cv::getTickCount();

        // Grab a frame
        cv::Mat temp;
        if (!cap.read(temp)) {
            break;
        }

        dlib::cv_image<dlib::bgr_pixel> cimg(temp);

        // Detect faces 
        std::vector<dlib::rectangle> faces = detector(cimg);

        // Find the pose of each face.
        std::vector<dlib::full_object_detection> shapes;
        for (unsigned long i = 0; i < faces.size(); ++i)
            shapes.push_back(m_face_landmark(cimg, faces[i]));

        auto time_end = cv::getTickCount();
        auto time_per_frame = (time_end - time_start) / cv::getTickFrequency();

        fps = (15 * fps + (1 / time_per_frame)) / 16;

        printf("Total time: %3.5f | FPS: %3.2f\n", time_per_frame, fps);

        // Display it all on the screen
        win.clear_overlay();
        win.set_image(cimg);
        win.add_overlay(dlib::render_face_detections(shapes));
    }
}