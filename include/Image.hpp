#pragma once

#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using image_type = dlib::matrix<dlib::rgb_pixel>;

class Image final {
    public:
        Image(const std::string&);

        inline image_type& frame() { return m_img; }
        inline const image_type& frame() const { return m_img; }

    private:
        // Chemin de l'image
        const std::string& m_path;

        // Représentation de l'image avec dlib
        image_type m_img;
};