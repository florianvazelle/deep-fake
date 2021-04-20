#pragma once

#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using image_type = dlib::matrix<dlib::rgb_pixel>;

class Image final {
    public:
        Image(const std::string&);

        inline image_type& frame() { return img; }
        inline const image_type& frame() const { return img; }

    private:
        // chemin de l'image
        const std::string& path;

        // représentation de l'image en opencv (matrice)
        image_type img;
};