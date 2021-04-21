#pragma once

#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/gui_widgets.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <opencv2/opencv.hpp>
#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_transforms.h>

using image_type = dlib::matrix<dlib::rgb_pixel>;

class Image final {
    public:
        Image(int);

        void loadImage(const cv::Mat&);
        void loadImage(const std::string&);

        void detect(const dlib::shape_predictor&, bool = true);

        void convexHull(std::vector<std::vector<cv::Point>>&) const;
        void masks(std::vector <cv::Mat>&, const std::vector<std::vector<cv::Point>>&) const;
        void facesCenter(std::vector<cv::Point>&) const;

        void display(dlib::image_window&) const;

        inline image_type& frame() { return m_img; }
        inline const image_type& frame() const { return m_img; }

        inline int& rows() { return m_rows; }
        inline const int& rows() const { return m_rows; }

        inline int& cols() { return m_cols; }
        inline const int& cols() const { return m_cols; }

        inline cv::Mat toMat() { return dlib::toMat(m_img); } // Not const because dlib::toMat is not const

    private:
        // Le nombre de visage que l'on veut identifié
        int num_faces;

        int m_rows, m_cols, m_depth;

        // Représentation de l'image avec dlib / une version plus petite de l'image
        image_type m_img, m_img_small;

        // Liste des visages détectés sur l'image
        std::vector<dlib::rectangle> m_faces;

        // Liste des points d'intérét détectés sur chaque visage
        std::vector<dlib::full_object_detection> m_shapes;

        void computeInfo();
};