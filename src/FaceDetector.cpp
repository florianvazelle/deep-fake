#include <FaceDetector.hpp>

#define SKIP_FRAMES 2

FaceDetector::FaceDetector(const std::string& filename, int num_faces) : m_camera("assets/toto.webm"), m_count(0), m_imgVideo(num_faces), m_imgBasic(num_faces) {
    // Si la caméra n’est pas ouvert, quitter la fonction
    if (!m_camera.isOpened()) {
        throw std::runtime_error("Unable to connect to camera.");
    }

    dlib::deserialize(SHAPE_PREDICTOR_68_FACE_LANDMARKS) >> m_face_landmark;

    m_imgBasic.loadImage(filename);
    m_imgBasic.detect(m_face_landmark, false);

    if (!m_imgBasic.facesAreDetected()) {
        throw std::runtime_error("No face is detected in the base image.");
    }
}

bool FaceDetector::operator>>(cv::Mat& frame) {
    m_camera >> frame;

    if (frame.empty()) return true;

    m_imgVideo.loadImage(frame);

    // if (m_count % SKIP_FRAMES == 0) {
        m_imgVideo.detect(m_face_landmark);
    // }

    m_count++;

    return false;
}