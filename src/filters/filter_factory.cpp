#include "../../include/filters/filter_factory.h"
#include "../../include/filters/all_filters.h"

std::unique_ptr<Filter> FilterFactory::CreateFilter(FilterType type, const std::vector<float>& args) {
    switch (type) {
        case GRAYSCALE:
            return std::make_unique<GrayScale>();
        case CROP:
            return std::make_unique<Crop>(args[0], args[1]);
        case NEGATIVE:
            return std::make_unique<Negative>();
        case SHARPENING:
            return std::make_unique<Sharpening>();
        case EDGE_DETECTION:
            return std::make_unique<EdgeDetection>(args[0]);
        case GAUSSIAN_BLUR:
            return std::make_unique<GaussianBlur>(args[0]);
        case CRYSTALLIZE:
            return std::make_unique<Crystallize>();
        default:
            throw std::invalid_argument("No such filter");
    }
    return nullptr;
}

Image FilterFactory::ApplyAll(const Image& image) {
    Image result = image;
    for (const auto& filter : this->filters_) {
        result = filter->Apply(result);
    }
    return result;
}

FilterFactory::FilterFactory(const std::vector<std::string>& flags) {
    size_t i = 0;
    this->flags_ = flags;
    while (i < this->flags_.size()) {
        if (this->flags_[i][0] != '-') {
            throw std::invalid_argument("The flag should start with '-'");
        } else if (this->flags_[i] == "-gs") {
            this->filters_.push_back(this->CreateFilter(FilterFactory::GRAYSCALE));
        } else if (this->flags_[i] == "-crop") {
            this->filters_.push_back(this->CreateFilter(FilterFactory::CROP, this->GetArgs(i, 2)));
            i += 2;
        } else if (this->flags_[i] == "-neg") {
            this->filters_.push_back(this->CreateFilter(FilterFactory::NEGATIVE));
        } else if (this->flags_[i] == "-sharp") {
            this->filters_.push_back(this->CreateFilter(FilterFactory::SHARPENING));
        } else if (this->flags_[i] == "-edge") {
            this->filters_.push_back(this->CreateFilter(FilterFactory::EDGE_DETECTION, this->GetArgs(i, 1)));
            i++;
        } else if (this->flags_[i] == "-blur") {
            this->filters_.push_back(this->CreateFilter(FilterFactory::GAUSSIAN_BLUR, this->GetArgs(i, 1)));
            i++;
        } else if (this->flags_[i] == "-cryst") {
            this->filters_.push_back(this->CreateFilter(FilterFactory::CRYSTALLIZE));
            i++;
        } else {
            throw std::invalid_argument("No such filter: " + this->flags_[i]);
        }
        ++i;
    }
}

std::vector<float> FilterFactory::GetArgs(size_t start, size_t amount) {
    if (this->flags_.size() - (start + 1) < amount) {
        throw std::invalid_argument("Too few arguments for filter " + this->flags_[start]);
    }
    std::vector<float> params;
    try {
        for (size_t i = 0; i < amount; ++i) {
            params.push_back(std::stof(this->flags_[i + 1]));
        }
    } catch (...) {
        throw std::invalid_argument("The filter must be followed by " + std::to_string(amount) + " parameter(s)");
    }
    return params;
}