#include <DeepFake.hpp>
#include <Image.hpp>
#include <FaceDetector.hpp>
#include <Timer.hpp>
#include <Delaunay.hpp>

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

    std::vector<std::vector<cv::Point>> pointsBasic;
    faceDect.basic().points(pointsBasic);

    // On récupère des frames et on les traite jusqu'à ce que la fenêtre principale soit fermée
    while(!win.is_closed()) {
        time.start();

        // try {
            
            // On récupère une frame
            cv::Mat temp, output;
            if (faceDect >> temp) {
                break;
            }

            int defaultType = output.type();
            output = temp.clone();

            std::vector<std::vector<cv::Point>> points;
            faceDect.video().points(points);

            if (points.size() > 0) {
                /* Delaunay & Transformation affine */
                {
                    cv::Mat basic = faceDect.basic().toMat();
                    basic.convertTo(basic, CV_32F);
                    output.convertTo(output, CV_32F);

                    // On récupère les triangle de la triangulation de delaunay sur l'envelope convexe
                    std::vector<std::vector<int>> triangles;
                    const cv::Rect rect(0, 0, output.cols, output.rows);
                    calculateDelaunayTriangles(rect, points[0], triangles);

                    // On applique une transformation affine aux triangles que l'on viens de calculé
                    for(unsigned int i = 0; i < triangles.size(); i++) {
                        std::vector<cv::Point> t1(3), t2(3);

                        // On récupère les triangles
                        for(unsigned int j = 0; j < 3; j++) {
                            t1[j] = pointsBasic[0][triangles[i][j]];
                            t2[j] = points[0][triangles[i][j]];
                        }
                        
                        warpTriangle(basic, output, t1, t2);
                    }
                }

                //draw_delaunay(output, points[0]);

                /* Seamless Cloning */
                {
                    std::vector<cv::Mat> masks;
                    faceDect.video().masks(masks, points);

                    std::vector<cv::Point> centers;
                    faceDect.video().facesCenter(centers, points);

                    output.convertTo(output, defaultType);

                    cv::Mat temp_final;
                    cv::seamlessClone(output, temp, masks[0], centers[0], temp_final, cv::MIXED_CLONE);
                    faceDect.video().loadImage(temp_final);
                }
            }
        // } catch (...) {

        // }

        time.display();

        faceDect.video().display(win);
    }
}