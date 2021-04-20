#pragma once

#include <opencv2/opencv.hpp>

class Image final {
    public:
        Image(const std::string&);

        void show() const;

    private:
        // chemin de l'image
        const std::string& path;

        // représentation de l'image en opencv (matrice)
        cv::Mat img;
};