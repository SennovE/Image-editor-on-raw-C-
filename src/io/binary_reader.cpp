#include "../../include/io/binary_reader.h"

#include <cstring>

BinaryReader::BinaryReader(const std::string &filename) {
    file_.open(filename, std::ios::in | std::ios::binary);
    if (!file_.is_open()) {
        throw std::invalid_argument("Error opening file: " + filename + ' ' + std::strerror(errno));
    }
    if (errno == EACCES) {
        throw std::invalid_argument("No permission to read file: " + filename);
    }
}

BinaryReader::~BinaryReader() {
    file_.close();
}