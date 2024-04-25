#include "../../include/data/image.h"

Image::Image() {
}

Image::Image(size_t width, size_t height, const Pixel &fill_value)
    : width_(width), height_(height), data_(height_, {width_, fill_value}) {
}

size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    return height_;
}

void Image::SetPixel(size_t col, size_t row, const Pixel &pixel) {
    data_[row][col] = pixel;
}

Pixel Image::GetPixel(size_t col, size_t row) const {
    return data_[row][col];
}
