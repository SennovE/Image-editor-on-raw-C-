#pragma once

#include <fstream>
#include <string>

class BinaryWriter {
public:
    explicit BinaryWriter(const std::string &filename);
    ~BinaryWriter();

    template <typename T>
    BinaryWriter &operator<<(T value) {
        using U = std::make_unsigned_t<T>;
        U bits = static_cast<U>(value);

        unsigned char buffer[sizeof(T)];
        for (unsigned char &byte : buffer) {
            byte = bits & U{BYTE * BYTE * 4 - 1};
            bits = bits >> U{BYTE};
        }

        file_.write(reinterpret_cast<char *>(buffer), sizeof(T));
        return *this;
    }

private:
    std::ofstream file_;
    static const uint8_t BYTE = 8;
};