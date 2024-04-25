#include "../../include/filters/all_filters.h"

#include <numbers>
#include <cmath>
#include <random>

Image GrayScale::Apply(Image &image) {
    Image result = image;
    static const Pixel GRAY_SCALE = Pixel{0.299f, 0.587f, 0.114f};
    for (size_t row = 0; row != image.GetHeight(); ++row) {
        for (size_t col = 0; col != image.GetWidth(); ++col) {
            auto current = image.GetPixel(col, row);
            Pixel weighted_color = current * GRAY_SCALE;
            float brightness = weighted_color.red + weighted_color.green + weighted_color.blue;
            result.SetPixel(col, row, Pixel{brightness, brightness, brightness});
        }
    }
    return result;
}

Image Crop::Apply(Image &image) {
    Image result{std::min(width_, image.GetWidth()), std::min(height_, image.GetHeight())};
    for (size_t row = 0; row < result.GetHeight(); ++row) {
        for (size_t col = 0; col < result.GetWidth(); ++col) {
            result.SetPixel(col, result.GetHeight() - row - 1, image.GetPixel(col, image.GetHeight() - row - 1));
        }
    }
    return result;
}

Image Negative::Apply(Image &image) {
    Image result = image;
    Pixel pix{1, 1, 1};
    for (size_t row = 0; row < image.GetHeight(); ++row) {
        for (size_t col = 0; col < image.GetWidth(); ++col) {
            result.SetPixel(col, row, pix - image.GetPixel(col, row));
        }
    }
    return result;
}

Pixel MatrixMul(const std::vector<float> &matrix, const Image &image, int row, int col,
                const std::vector<std::pair<int, int>> &idx) {
    static const Pixel WHITE{1, 1, 1};
    static const Pixel BLACK{0, 0, 0};
    Pixel result{0, 0, 0};
    int height = static_cast<int>(image.GetHeight()) - 1;
    int width = static_cast<int>(image.GetWidth()) - 1;
    size_t i = 0;
    for (const auto &k : idx) {
        result = result + image.GetPixel(std::min(width, std::max(0, col + k.first)),
                                         std::min(height, std::max(0, row + k.second))) *
                              matrix[i++];
    }
    result = GetMinPixel(WHITE, GetMaxPixel(BLACK, result));
    return result;
}

Image Sharpening::Apply(Image &image) {
    Image result = image;
    static const std::vector<std::pair<int, int>> IDX = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0},
                                                         {0, 1},   {1, -1}, {1, 0},  {1, 1}};
    static const std::vector<float> MATRIX{0, -1, 0, -1, 5, -1, 0, -1, 0};
    for (size_t row = 0; row < image.GetHeight(); ++row) {
        for (size_t col = 0; col < image.GetWidth(); ++col) {
            result.SetPixel(col, row, MatrixMul(MATRIX, image, static_cast<int>(row), static_cast<int>(col), IDX));
        }
    }
    return result;
}

Image EdgeDetection::Apply(Image &image) {
    GrayScale gs;
    Image gs_image = gs.Apply(image);
    Image result = image;
    static const std::vector<std::pair<int, int>> IDX = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0},
                                                         {0, 1},   {1, -1}, {1, 0},  {1, 1}};
    static const std::vector<float> MATRIX{0, -1, 0, -1, 4, -1, 0, -1, 0};
    for (size_t row = 0; row < image.GetHeight(); ++row) {
        for (size_t col = 0; col < image.GetWidth(); ++col) {
            Pixel pix = MatrixMul(MATRIX, gs_image, static_cast<int>(row), static_cast<int>(col), IDX);
            if (pix.red > this->threshold_) {
                result.SetPixel(col, row, {1, 1, 1});
            } else {
                result.SetPixel(col, row, {0, 0, 0});
            }
        }
    }
    return result;
}

double GaussX(size_t x, float sigma) {
    return (1 / std::sqrt(2 * std::numbers::pi * std::pow(sigma, 2))) *
           std::pow(std::numbers::e, -(std::pow(x, 2) / (2 * std::pow(sigma, 2))));
}

double GaussXY(size_t x, size_t y, float sigma) {
    return (1 / (2 * std::numbers::pi * std::pow(sigma, 2))) *
           std::pow(std::numbers::e, -((std::pow(x, 2) + std::pow(y, 2))) / (2 * std::pow(sigma, 2)));
}

/*
Image GaussianBlur::Apply(Image &image) {
    Image result = image;
    std::vector<float> matrix;
    std::vector<std::pair<int, int>> idx;
    static const int RADIUS = 7;
    float sum = 0;
    for (int x = 0; x < (RADIUS * 2 + 1); ++x) {
        for (int y = 0; y < (RADIUS * 2 + 1); ++y) {
            float gauss_xy = static_cast<float>(GaussXY(std::abs(x - RADIUS), std::abs(y - RADIUS), this->sigma_));
            matrix.push_back(gauss_xy);
            idx.push_back({x - RADIUS, y - RADIUS});
            sum += gauss_xy;
        }
    }
    for (size_t x = 0; x < (RADIUS * 2 + 1) * (RADIUS * 2 + 1); ++x) {
        matrix[x] /= sum;
    }
    for (size_t row = 0; row < image.GetHeight(); ++row) {
        for (size_t col = 0; col < image.GetWidth(); ++col) {
            result.SetPixel(col, row, MatrixMul(matrix, image, static_cast<int>(row), static_cast<int>(col), idx));
        }
    }
    return result;
}*/

Image GaussianBlur::Apply(Image &image) {
    Image result = image;
    std::vector<float> matrix;
    std::vector<std::pair<int, int>> idx;
    static const int RADIUS = 12;
    float sum = 0;
    for (int x = 0; x < (RADIUS * 2 + 1); ++x) {
        float gauss_x = static_cast<float>(GaussX(std::abs(x - RADIUS), this->sigma_));
        matrix.push_back(gauss_x);
        idx.push_back({x - RADIUS, 0});
        sum += gauss_x;
    }
    for (size_t x = 0; x < (RADIUS * 2 + 1); ++x) {
        matrix[x] /= sum;
    }
    for (size_t row = 0; row < image.GetHeight(); ++row) {
        for (size_t col = 0; col < image.GetWidth(); ++col) {
            result.SetPixel(col, row, MatrixMul(matrix, image, static_cast<int>(row), static_cast<int>(col), idx));
        }
    }
    image = result;
    for (size_t x = 0; x < (RADIUS * 2 + 1); ++x) {
        idx[x] = {idx[x].second, idx[x].first};
    }
    for (size_t row = 0; row < image.GetHeight(); ++row) {
        for (size_t col = 0; col < image.GetWidth(); ++col) {
            result.SetPixel(col, row, MatrixMul(matrix, image, static_cast<int>(row), static_cast<int>(col), idx));
        }
    }
    return result;
}

Image Crystallize::Crystal(Image &image, float min_distance, float max_distance, float crystalize_amount, size_t radius,
                           int shift_x, int shift_y) {
    Image result = image;
    static const float TWO = 2.0;
    for (size_t row = 0; row < image.GetHeight(); ++row) {
        for (size_t col = 0; col < image.GetWidth(); ++col) {
            size_t cell_index_row = row / radius;
            size_t cell_index_col = col / radius;

            float center_row = static_cast<float>(cell_index_row * radius) + static_cast<float>(radius) / TWO;
            float center_col = static_cast<float>(cell_index_col * radius) + static_cast<float>(radius) / TWO;

            float offset_row = std::abs(center_row - static_cast<float>(row));
            float offset_col = std::abs(center_col - static_cast<float>(col));
            float distance_to_center = std::sqrt(offset_row * offset_row + offset_col * offset_col);
            if (distance_to_center >= min_distance && distance_to_center <= max_distance) {
                float crystalize_factor = crystalize_amount / distance_to_center;
                int new_row = static_cast<int>(row) + shift_x * static_cast<int>(crystalize_factor * offset_row);
                int new_col = static_cast<int>(col) + shift_y * static_cast<int>(crystalize_factor * offset_col);

                new_row = std::max(0, std::min(static_cast<int>(image.GetWidth()) - 1, new_row));
                new_col = std::max(0, std::min(static_cast<int>(image.GetHeight()) - 1, new_col));

                result.SetPixel(new_col, new_row, result.GetPixel(col, row));
            }
        }
    }
    return result;
}

Image Crystallize::Apply(Image &image) {
    Image result = image;
    static const float TEN = 10.0;
    result = this->Crystal(result, 0.0, static_cast<float>(image.GetWidth()),
                           static_cast<float>(image.GetWidth()) / TEN, image.GetWidth() / 4, 1, 1);
    result = this->Crystal(result, 0.0, static_cast<float>(image.GetWidth()),
                           static_cast<float>(image.GetWidth()) / (TEN * 2), image.GetWidth() / 2, -1, -1);
    return result;
}