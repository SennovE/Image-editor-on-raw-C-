#include "../../include/data/pixel.h"

#include <algorithm>

Pixel operator/(const Pixel &p, float a) {
    return Pixel{p.red / a, p.green / a, p.blue / a};
}

Pixel operator*(const Pixel &p, float a) {
    return Pixel{p.red * a, p.green * a, p.blue * a};
}

Pixel operator*(const Pixel &p, Pixel a) {
    return Pixel{p.red * a.red, p.green * a.green, p.blue * a.blue};
}

Pixel operator-(const Pixel &p, Pixel a) {
    return Pixel{p.red - a.red, p.green - a.green, p.blue - a.blue};
}

Pixel operator+(const Pixel &p, Pixel a) {
    return Pixel{p.red + a.red, p.green + a.green, p.blue + a.blue};
}

Pixel GetMinPixel(Pixel lhs, Pixel rhs) {
    Pixel result = lhs;
    result.red = std::min(result.red, rhs.red);
    result.green = std::min(result.green, rhs.green);
    result.blue = std::min(result.blue, rhs.blue);
    return result;
}

Pixel GetMaxPixel(Pixel lhs, Pixel rhs) {
    Pixel result = lhs;
    result.red = std::max(result.red, rhs.red);
    result.green = std::max(result.green, rhs.green);
    result.blue = std::max(result.blue, rhs.blue);
    return result;
}