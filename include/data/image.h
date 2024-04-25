#pragma once

#include "pixel.h"

#include <vector>
#include <stddef.h>

class Image {
public:
    Image();

    Image(size_t width, size_t height, const Pixel &fill_value = {});

    size_t GetWidth() const;
    size_t GetHeight() const;

    void SetPixel(size_t col, size_t row, const Pixel &pixel);
    Pixel GetPixel(size_t col, size_t row) const;

private:
    size_t width_;
    size_t height_;
    std::vector<std::vector<Pixel>> data_;
};