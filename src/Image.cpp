#include <Image.hpp>

Image::Image(const std::string& path) : m_path(path) {
    dlib::load_image(m_img, path);
}