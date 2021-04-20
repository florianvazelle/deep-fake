#include <Image.hpp>
#include <exception>

Image::Image(const std::string& path) : path(path) {
    img = cv::imread(path, 0);
    if (img.empty()) {
        throw std::runtime_error("Could not read the image: " + path);
    }
}

void Image::show() const {
    imshow("Output", img);
}