#pragma once

#include <vector>
#include <cstdint>

namespace margelo::nitro::imagecompressor {

struct YCbCrPixel {
    uint8_t y;
    uint8_t cb;
    uint8_t cr;
};

class JPEGCompressor {
public:
    // Decode JPEG to RGB
    static std::vector<uint8_t> decodeJPEG(const std::vector<uint8_t>& jpegData);

    // Compress RGB data to JPEG format
    static std::vector<uint8_t> compressToJPEG(const std::vector<uint8_t>& rgb,
                                              int width,
                                              int height,
                                              double quality);

private:
    // Utility functions
    static bool isJPEGHeader(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> createJPEGHeader(int width, int height, int quality);

    // YCbCr conversion functions
    static std::vector<YCbCrPixel> convertToYCbCr(const std::vector<uint8_t>& rgb,
                                                 int width,
                                                 int height);
    static void subsample(std::vector<YCbCrPixel>& ycbcr, int width, int height);
    static void quantize(std::vector<YCbCrPixel>& ycbcr, double quality);

    static constexpr double KR = 0.299;
    static constexpr double KG = 0.587;
    static constexpr double KB = 0.114;
};

} // namespace margelo::nitro::imagecompressor
