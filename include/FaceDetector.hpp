#pragma once

#include <Image.hpp>
#include <exception>

/**
 * @brief Un Face Detector est l'objet qui possède les images et qui va lancer les detections dessus
 *
 * L'objet possède deux images :
 * - une capturé par la caméra que l'on update, on va l'appeler "video"
 * - une autre qui est static et que l'on obtient via un nom de fichier, on va l'appeler "basic"
 */
class FaceDetector final {
    public:
        /**
         * @brief Permet de construire un nouvel objet Face Detector
         * 
         * @param filename Le chemin vers une image qui sera l'image static
         * @param num_faces Le nombre de visage que l'on veut détecter dans les images
         */
        FaceDetector(const std::string& filename, int num_faces);

        /**
         * @brief On récupère une frame de la caméra et on detecte directement les visages dessus
         * 
         * Cette fonction va permettre d'update l'image vidéo
         * 
         * @param frame Une image OpenCV dans laquelle on va placer la frame
         * @return true Si il a toujours une image
         * @return false Si il n'y a plus d'image
         */
        bool operator>>(cv::Mat& frame);

        inline Image& video() { return m_imgVideo; }
        inline const Image& video() const { return m_imgVideo; }

        inline Image& basic() { return m_imgBasic; }
        inline const Image& basic() const { return m_imgBasic; }

    private:
        // Le nombre d'update que l'on a réalisé
        int m_count;

        // Nos deux images
        Image m_imgVideo, m_imgBasic;

        // Caméra ou vidéo
        cv::VideoCapture m_camera;

        // Modèle de repérage de visage
        dlib::shape_predictor m_face_landmark;
};