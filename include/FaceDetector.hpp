#pragma once

#include <Image.hpp>
#include <exception>

class FaceDetector final {
    public:
        FaceDetector(const std::string& filename, int num_faces);

        bool operator>>(cv::Mat& frame);

        inline Image& video() { return m_imgVideo; }
        inline const Image& video() const { return m_imgVideo; }

        inline Image& basic() { return m_imgBasic; }
        inline const Image& basic() const { return m_imgBasic; }

    private:
        int m_count;

        Image m_imgVideo, m_imgBasic;

        // Caméra ou vidéo
        cv::VideoCapture m_camera;

        // Modèle de repérage de visage
        dlib::shape_predictor m_face_landmark;
};