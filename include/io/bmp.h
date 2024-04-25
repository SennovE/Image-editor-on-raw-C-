#pragma once

#include "binary_reader.h"
#include "binary_writer.h"
#include "image_format.h"

#include <algorithm>

class BMP : ImageFormat {
public:
    Image Load(const std::string &filename) override;
    void Save(const Image &image, const std::string &filename) override;

private:
    static const uint8_t BYTE = 8;
    static const uint16_t NORMAL_DPI = 300;

    struct Header {
        char signature[2] = {'B', 'M'};
        uint32_t file_size = 0;
        uint16_t reserved[2] = {0, 0};
        uint32_t offset = BYTE * (BYTE - 1) - 2;
    };

    struct InfoHeader {
        uint32_t header_size = BYTE * (BYTE - 3);

        struct Size {
            int32_t width = 0;
            int32_t height = 0;
        } size = {};

        uint16_t planes = 1;
        uint16_t bits_per_pixel = BYTE * 3;
        uint32_t compression = 0;
        uint32_t image_size = 0;

        struct DPI {
            int32_t hor = NORMAL_DPI;
            int32_t ver = NORMAL_DPI;
        } dpi = {};

        uint32_t total_colors = 0;
        uint32_t important = 0;
    };

private:
    static auto Tie(Header &header);
    static auto Tie(InfoHeader &info_header);
    static auto ReadRow(Image &image, BinaryReader &input, size_t row);
    static auto WriteRow(const Image &image, BinaryWriter &output, size_t row);
    static size_t GetRowSize(size_t width, size_t bits_per_pixel);
    static void TryValidateHeader(const Header &header);
    static void TryValidateInfoHeader(const InfoHeader &header);
};