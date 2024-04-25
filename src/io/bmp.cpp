#include <limits>
#include <stdexcept>
#include <tuple>

#include "../../include/io/bmp.h"

auto BMP::Tie(BMP::Header &header) {
    return std::tie(header.signature[0], header.signature[1], header.file_size, header.reserved[0], header.reserved[1],
                    header.offset);
}

auto BMP::Tie(BMP::InfoHeader &info_header) {
    return std::tie(info_header.header_size, info_header.size.width, info_header.size.height, info_header.planes,
                    info_header.bits_per_pixel, info_header.compression, info_header.image_size, info_header.dpi.hor,
                    info_header.dpi.ver, info_header.total_colors, info_header.important);
}

auto BMP::ReadRow(Image &image, BinaryReader &input, const size_t row) {
    const float upper = std::numeric_limits<uint8_t>::max();
    for (size_t col = 0; col != image.GetWidth(); ++col) {

        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;

        input >> blue >> green >> red;

        Pixel pixel = Pixel{static_cast<float>(red), static_cast<float>(green), static_cast<float>(blue)} / upper;

        image.SetPixel(col, row, pixel);
    }

    size_t row_size = GetRowSize(image.GetWidth(), Pixel::BITS_PER_PIXEL);
    uint8_t reserved = 0;
    for (size_t bytes_read = Pixel::BITS_PER_PIXEL / BYTE * image.GetWidth(); bytes_read < row_size; ++bytes_read) {
        input >> reserved;
    }
}

auto BMP::WriteRow(const Image &image, BinaryWriter &output, size_t row) {
    const float upper = std::numeric_limits<uint8_t>::max();

    for (size_t col = 0; col != image.GetWidth(); ++col) {
        Pixel pixel = image.GetPixel(col, row) * upper;
        output << static_cast<uint8_t>(std::clamp(pixel.blue, 0.0f, upper));
        output << static_cast<uint8_t>(std::clamp(pixel.green, 0.0f, upper));
        output << static_cast<uint8_t>(std::clamp(pixel.red, 0.0f, upper));
    }

    size_t row_size = GetRowSize(image.GetWidth(), Pixel::BITS_PER_PIXEL);
    uint8_t reserved{};
    for (size_t bytes_written = Pixel::BITS_PER_PIXEL / BYTE * image.GetWidth(); bytes_written < row_size;
         ++bytes_written) {
        output << reserved;
    }
}

size_t BMP::GetRowSize(size_t width, size_t bits_per_pixel) {
    return (bits_per_pixel * width + (BYTE * 4) - 1) / (BYTE * 4) * 4;
}

Image BMP::Load(const std::string &filename) {
    BinaryReader input(filename);
    Header header{};
    InfoHeader info_header{};

    std::apply([&input](auto &...args) { (input >> ... >> args); }, Tie(header));

    TryValidateHeader(header);

    std::apply([&input](auto &...args) { (input >> ... >> args); }, Tie(info_header));

    TryValidateInfoHeader(info_header);

    Image image(static_cast<size_t>(info_header.size.width), static_cast<size_t>(info_header.size.height));

    if (image.GetWidth() == 0 || image.GetHeight() == 0) {
        return image;
    }

    for (size_t row = 0; row != image.GetHeight(); ++row) {
        ReadRow(image, input, row);
    }

    return image;
}

void BMP::Save(const Image &image, const std::string &filename) {
    BinaryWriter output(filename);
    Header header{};
    InfoHeader info_header{};

    info_header.size.width = static_cast<int32_t>(image.GetWidth());
    info_header.size.height = static_cast<int32_t>(image.GetHeight());
    info_header.image_size = GetRowSize(image.GetWidth(), Pixel::BITS_PER_PIXEL) * image.GetHeight();

    header.file_size = info_header.image_size + header.offset;

    std::apply([&output](auto &...args) { (output << ... << args); }, Tie(header));

    std::apply([&output](auto &...args) { (output << ... << args); }, Tie(info_header));

    for (size_t row = 0; row != image.GetHeight(); ++row) {
        WriteRow(image, output, row);
    }
}

void BMP::TryValidateHeader(const BMP::Header &header) {
    std::string bmp_signature = {header.signature[0], header.signature[1]};
    if ((bmp_signature != "BM") || (header.file_size < header.offset)) {
        throw std::runtime_error("Incorrect file format: invalid bitmap header data!");
    }
}

void BMP::TryValidateInfoHeader(const BMP::InfoHeader &header) {
    if ((header.header_size != BYTE * (BYTE - 3)) || (header.planes != 1) || (header.bits_per_pixel != BYTE * 3) ||
        (header.compression != 0)) {
        throw std::runtime_error("Incorrect file format: unsupported DIB header value!");
    }
}
