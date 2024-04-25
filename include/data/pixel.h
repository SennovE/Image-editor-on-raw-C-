#pragma once

struct Pixel {
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;

    static const int BITS_PER_PIXEL = 24;
};

Pixel GetMinPixel(Pixel lhs, Pixel rhs);
Pixel GetMaxPixel(Pixel lhs, Pixel rhs);

Pixel operator/(const Pixel &p, float a);
Pixel operator*(const Pixel &p, float a);
Pixel operator*(const Pixel &p, Pixel a);
Pixel operator-(const Pixel &p, Pixel a);
Pixel operator+(const Pixel &p, Pixel a);