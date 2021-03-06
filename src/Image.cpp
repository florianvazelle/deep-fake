#include <Image.hpp>

#define FACE_DOWNSAMPLE_RATIO 4 // on peu descendre jusqu'a 6

Image::Image(int num_faces) : num_faces(num_faces), m_shapes(num_faces) {}

void Image::loadImage(const cv::Mat& mat) {
    // Passage au format d'image de dlib
    // Aucune mémoire n'est copiée
    dlib::cv_image<dlib::bgr_pixel> image(mat);
    dlib::assign_image(m_img, image);

    computeInfo();
}

void Image::loadImage(const std::string& path) {
    dlib::load_image(m_img, path);

    computeInfo();
}

void Image::computeInfo() {
    cv::Mat img = toMat(); 

    // On redimensionne l'image pour la détection des faces
    cv::Mat img_small;
    cv::resize(img, img_small, cv::Size(), 1.0 / FACE_DOWNSAMPLE_RATIO, 1.0 / FACE_DOWNSAMPLE_RATIO);

    // Passage au format d'image de dlib
    // Aucune mémoire n'est copiée
    dlib::cv_image<dlib::bgr_pixel> image(img_small);
    dlib::assign_image(m_img_small, image);

    // pre-compute some informations
    m_rows = img.rows;
    m_cols = img.cols;
    m_depth = img.depth();
}

void Image::detect(const dlib::shape_predictor& face_landmark, bool on_small) {
    // On chargement des modèles de détection des visages et d'estimation des faces
    static dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    
    m_faces.clear();
    m_shapes.clear();

    if (on_small) {
        // On detecte les visages sur l'image redimenssionnée
        m_faces = detector(m_img_small);

        if (m_faces.size() > num_faces)
            m_faces.resize(num_faces);
        m_shapes.resize(m_faces.size());

        // On trouve la position de chaque face
        for (unsigned int i = 0; i < m_faces.size(); ++i) {
            // On redimensionne le rectangle obtenu pour obtenir une image en pleine résolution
            dlib::rectangle r(
                (long)(m_faces[i].left() * FACE_DOWNSAMPLE_RATIO),
                (long)(m_faces[i].top() * FACE_DOWNSAMPLE_RATIO),
                (long)(m_faces[i].right() * FACE_DOWNSAMPLE_RATIO),
                (long)(m_faces[i].bottom() * FACE_DOWNSAMPLE_RATIO)
            );

            // On détecte les points de repère sur l'image en taille réelle
            m_shapes[i] = face_landmark(m_img, r);
        }
    } else {
        // On detecte les faces
        m_faces = detector(m_img);

        if (m_faces.size() > num_faces)
            m_faces.resize(num_faces);
        m_shapes.resize(m_faces.size());

        // On trouve la position de chaque face
        for (unsigned int i = 0; i < m_faces.size(); ++i) {
            // On détecte les points de repère sur l'image en taille réelle
            m_shapes[i] = face_landmark(m_img, m_faces[i]);
        }
    }
}

void Image::points(std::vector<std::vector<cv::Point>>& points) const {
    points.resize(m_faces.size());

    for (unsigned int i = 0; i < m_faces.size(); ++i) {
        // On convertit tout les points d'un visage en point opencv
        points[i].resize(m_shapes[i].num_parts());
        for (unsigned int j = 0; j < m_shapes[i].num_parts(); ++j) {
            points[i][j] = cv::Point(m_shapes[i].part(j).x(), m_shapes[i].part(j).y());
        }
    }
}

void Image::masks(std::vector <cv::Mat>& masks, const std::vector<std::vector<cv::Point>>& points) const {
    masks.resize(m_faces.size());

    // On calcule le mask de chaque visage
    for (unsigned int i = 0; i < m_faces.size(); ++i) {
        // On applique une enveloppe convexe
        std::vector<int> hullIndex;

        if (points[i].size()) {
            cv::convexHull(points[i], hullIndex, false, false);

            // On récupère les points du mask
            std::vector<cv::Point> hulls(hullIndex.size());
            for (unsigned int j = 0; j < hullIndex.size(); ++j) {
                hulls[j] = points[i][hullIndex[j]];
            }

            // On dessine le mask
            masks[i] = cv::Mat::zeros(m_rows, m_cols, m_depth);

            if (hulls.size() > 0) {
                cv::fillConvexPoly(masks[i], &hulls[0], hulls.size(), cv::Scalar(255, 255, 255));
            }
        }
    }
}

void Image::facesCenter(std::vector<cv::Point>& centers, const std::vector<std::vector<cv::Point>>& points) const {
    centers.resize(m_faces.size());

    // On calcule le centre de chaque visage
    for (unsigned int i = 0; i < m_faces.size(); ++i) {
        cv::Rect rect = cv::boundingRect(points[i]);
        centers[i] = (rect.tl() + rect.br()) / 2;
    }
}

void Image::display(dlib::image_window& win) const {
    win.clear_overlay();
    win.set_image(m_img);
    //win.add_overlay(dlib::render_face_detections(m_shapes));
}