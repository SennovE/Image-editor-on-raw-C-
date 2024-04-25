#pragma once

#include "../data/image.h"

class Filter {
public:
    virtual ~Filter() = default;

    virtual Image Apply(Image &image) = 0;
};