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

using image_type = dlib::matrix<dlib::bgr_pixel>;

/**
 * @brief Une Image est l'objet qui gére nos image dlib
 * 
 * On pourra trouver dans cette classe une méthode de détection,
 * de calcule de points ...
 * On stockera aussi les visages détectés et les points d'intérêts correspondant
 */
class Image final {
    public:
        /**
         * @brief Permet de construire un nouvel objet Image
         * 
         * @param num_faces Le nombre de visage que l'on veut détecter dans l'image
         */
        Image(int num_faces);
        
        /**
         * @brief Permet de charger une image depuis une image OpenCV
         * 
         * @param mat L'image OpenCV que l'on veut charger
         */
        void loadImage(const cv::Mat& mat);

        /**
         * @brief Permet de charger une image depuis un chemin vers un fichier
         *
         * @param path Le chemin vers un fichier image que l'on veut charger
         */
        void loadImage(const std::string& path);

        /**
         * @brief Permet de détecter les visages et les points d'intérêt correspondant
         *
         * @param face_landmark Le modèle de repérage de visage que l'on veut appliquer
         * @param on_small Est-ce que l'on fait la détection sur l'image plus petite (pour améliorer les perf)
         */
        void detect(const dlib::shape_predictor& face_landmark, bool on_small = true);

        /**
         * @brief Permet de récupèrer la liste des points d'intérêt pour chaque visage détecté
         * 
         * @param points La liste des points que l'on va remplir
         */
        void points(std::vector<std::vector<cv::Point>>& points) const;

        /**
         * @brief Permet de récupèrer la liste des mask pour chaque visage détecté
         * 
         * Pour chaque visage :
         * 1. On applique une enveloppe convexe sur la liste des points
         * 2. On déssine une image blanc/noire représentant l'enveloppe convexe
         *
         * @param masks La liste des mask que l'on va remplir
         * @param points La liste des points que l'on utilise pour déterminer le mask à produire
         */
        void masks(std::vector<cv::Mat>& masks, const std::vector<std::vector<cv::Point>>& points) const;

        /**
         * @brief Permet de récupèrer le point central pour chaque visage détecté
         *
         * @param centers La liste des centres que l'on va remplir
         */
        void facesCenter(std::vector<cv::Point>& centers) const;

        /**
         * @brief Permet de dessiner l'image sur une fenêtre
         *
         * @param win La fenêtre sur laquelle on va dessiner l'image
         */
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

        // Quelque données utile sur l'image
        int m_rows, m_cols, m_depth;

        // Représentation de l'image avec dlib / une version plus petite de l'image
        image_type m_img, m_img_small;

        // Liste des visages détectés sur l'image
        std::vector<dlib::rectangle> m_faces;

        // Liste des points d'intérét détectés sur chaque visage
        std::vector<dlib::full_object_detection> m_shapes;

        /**
         * @brief Permet lors du chargement d'une image, de pré-calculé certaines informations
         * tels que :
         * - le nombre de lignes/colonnes
         * - l'information de profondeur
         * - l'image plus petite
         */
        void computeInfo();
};