#include <Image.hpp>

Image::Image(const std::string& path) : path(path) {
    dlib::load_image(img, path);
}