#include <DeepFake.hpp>

#include <iostream>
#include <vector>

#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>

#define FACE_DOWNSAMPLE_RATIO 4
#define SKIP_FRAMES 2

#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <fstream>

DeepFake* DeepFake::GetInstance() {
    // Not thread safe
    static DeepFake* instance = new DeepFake();
    return instance;
}

DeepFake::DeepFake() {
    dlib::deserialize(SHAPE_PREDICTOR_68_FACE_LANDMARKS) >> m_face_landmark;
}

void DeepFake::run(const std::string& filename) const {
    //cv::VideoCapture cap("assets/toto.webm");
    cv::VideoCapture cap(0);
    // si cap n’est pas ouvert, quitter la fonction
    if (!cap.isOpened()) {
        std::cerr << "Unable to connect to camera" << std::endl;
        return;
    }

    dlib::image_window win;

    // On chargement des modèles de détection des visages et d'estimation des faces
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    double fps = 0;
    int count = 0;

    const int num_faces = 1;

    std::vector<dlib::rectangle> faces(num_faces);
    std::vector<dlib::full_object_detection> shapes(num_faces);

    // On récupère des frames et on les traite jusqu'à ce que la fenêtre principale soit fermée
    while(!win.is_closed()) {
        auto time_start = cv::getTickCount();

        // On récupère une frame
        cv::Mat temp, temp_small;
        if (!cap.read(temp)) {
            break;
        }

        // On redimensionne l'image pour la détection des faces
        cv::resize(temp, temp_small, cv::Size(), 1.0 / FACE_DOWNSAMPLE_RATIO, 1.0 / FACE_DOWNSAMPLE_RATIO);

        // Passage au format d'image de dlib
        // Aucune mémoire n'est copiée
        dlib::cv_image<dlib::bgr_pixel> cimg_small(temp_small);
        dlib::cv_image<dlib::bgr_pixel> cimg(temp);

        // On detecte les faces sur l'image redimenssionnée
        if (count % SKIP_FRAMES == 0) {
            faces.clear();
            faces = detector(cimg_small);
        }

        count++;

        // On trouve la position de chaque face
        for (unsigned int i = 0; i < faces.size(); ++i) {
            // On redimensionne le rectangle obtenu pour obtenir une image en pleine résolution
            dlib::rectangle r(
                (long)(faces[i].left() * FACE_DOWNSAMPLE_RATIO),
                (long)(faces[i].top() * FACE_DOWNSAMPLE_RATIO),
                (long)(faces[i].right() * FACE_DOWNSAMPLE_RATIO),
                (long)(faces[i].bottom() * FACE_DOWNSAMPLE_RATIO)
            );

            // On détection les points de repère sur l'image en taille réelle
            shapes[i] = m_face_landmark(cimg, r);
        }

        // On calcule les fps pour
        auto time_end = cv::getTickCount();
        auto time_per_frame = (time_end - time_start) / cv::getTickFrequency();

        fps = (15 * fps + (1 / time_per_frame)) / 16;

        printf("Total time: %3.5f | FPS: %3.2f\n", time_per_frame, fps);

        // On affiche tout sur l'écran
        win.clear_overlay();
        win.set_image(cimg);
        win.add_overlay(dlib::render_face_detections(shapes));
    }
}