#pragma once

#include "filter.h"

class GrayScale : public Filter {
public:
    Image Apply(Image &image) override;
};

class Crop : public Filter {
public:
    Crop(size_t width, size_t height) : width_(width), height_(height){};

    Image Apply(Image &image) override;

private:
    size_t width_ = 0;
    size_t height_ = 0;
};

class Negative : public Filter {
public:
    Image Apply(Image &image) override;
};

class Sharpening : public Filter {
public:
    Image Apply(Image &image) override;
};

class EdgeDetection : public Filter {
public:
    explicit EdgeDetection(float threshold) : threshold_(threshold){};

    Image Apply(Image &image) override;

private:
    float threshold_ = 0.f;
};

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(float sigma) : sigma_(sigma){};

    Image Apply(Image &image) override;

private:
    float sigma_ = 0.f;
};

class Crystallize : public Filter {
public:
    Image Apply(Image &image) override;

private:
    Image Crystal(Image &image, float min_distance, float max_distance, float crystalize_amount, size_t radius,
                  int shift_x, int shift_y);
};