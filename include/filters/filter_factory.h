#pragma once

#include <memory>
#include <string>
#include <vector>

#include "filter.h"

class FilterFactory {
public:
    enum FilterType { GRAYSCALE, CROP, NEGATIVE, SHARPENING, EDGE_DETECTION, GAUSSIAN_BLUR, CRYSTALLIZE };

    FilterFactory() = default;

    explicit FilterFactory(const std::vector<std::string>& flags);

    std::unique_ptr<Filter> CreateFilter(FilterType type, const std::vector<float>& args = {});

    Image ApplyAll(const Image& image);

private:
    std::vector<std::unique_ptr<Filter>> filters_;
    std::vector<std::string> flags_;

    std::vector<float> GetArgs(size_t start, size_t amount);
};