#include "JPEGCompressor.hpp"

namespace margelo::nitro::imagecompressor {
    std::vector<uint8_t> JPEGCompressor::decodeJPEG(const std::vector<uint8_t>& jpegData) {
        if (!isJPEGHeader(jpegData)) {
            throw std::runtime_error("Invalid JPEG data");
        }

        // For now, just extract the raw image data after JPEG headers
        // Skip JPEG markers until we find the start of scan (SOS) marker
        size_t pos = 0;
        while (pos < jpegData.size() - 1) {
            if (jpegData[pos] == 0xFF) {
                uint8_t marker = jpegData[pos + 1];
                if (marker == 0xDA) { // Start of Scan
                    pos += 2;
                    break;
                }
                // Skip marker segment
                if (marker != 0x00 && marker != 0xFF) {
                    pos += 2 + ((jpegData[pos + 2] << 8) | jpegData[pos + 3]);
                } else {
                    pos++;
                }
            } else {
                pos++;
            }
        }

        // Extract image data
        std::vector<uint8_t> imageData(jpegData.begin() + pos, jpegData.end());
        return imageData;
    }

    std::vector<uint8_t> JPEGCompressor::compressToJPEG(const std::vector<uint8_t>& rgb,
                                                       int width,
                                                       int height,
                                                       double quality) {
        // Create JPEG header
        std::vector<uint8_t> jpegData = createJPEGHeader(width, height, static_cast<int>(quality * 100));

        // Convert to YCbCr and compress
        std::vector<YCbCrPixel> ycbcr = convertToYCbCr(rgb, width, height);
        subsample(ycbcr, width, height);
        quantize(ycbcr, quality);

        // Add compressed data
        for (const auto& pixel : ycbcr) {
            jpegData.push_back(pixel.y);
            jpegData.push_back(pixel.cb);
            jpegData.push_back(pixel.cr);
        }

        // Add EOI marker
        jpegData.push_back(0xFF);
        jpegData.push_back(0xD9);

        return jpegData;
    }

    bool JPEGCompressor::isJPEGHeader(const std::vector<uint8_t>& data) {
        // Check for JPEG SOI marker
        return data.size() >= 2 && data[0] == 0xFF && data[1] == 0xD8;
    }

    std::vector<uint8_t> JPEGCompressor::createJPEGHeader(int width, int height, int quality) {
        std::vector<uint8_t> header;

        // SOI marker
        header.push_back(0xFF);
        header.push_back(0xD8);

        // JFIF header
        const uint8_t jfif[] = {
            0xFF, 0xE0,                   // JFIF marker
            0x00, 0x10,                   // length
            0x4A, 0x46, 0x49, 0x46, 0x00, // "JFIF\0"
            0x01, 0x01,                   // version
            0x00,                         // units
            0x00, 0x01,                   // X density
            0x00, 0x01,                   // Y density
            0x00, 0x00                    // thumbnails
        };

        header.insert(header.end(), jfif, jfif + sizeof(jfif));

        return header;
    }

    std::vector<YCbCrPixel> JPEGCompressor::convertToYCbCr(
        const std::vector<uint8_t>& rgb,
        int width,
        int height
    ) {
        std::vector<YCbCrPixel> ycbcr(width * height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int idx = (y * width + x) * 3;
                double r = rgb[idx];
                double g = rgb[idx + 1];
                double b = rgb[idx + 2];

                ycbcr[y * width + x].y = static_cast<uint8_t>(
                    KR * r + KG * g + KB * b);
                ycbcr[y * width + x].cb = static_cast<uint8_t>(
                    128 + (-0.168736 * r - 0.331264 * g + 0.5 * b));
                ycbcr[y * width + x].cr = static_cast<uint8_t>(
                    128 + (0.5 * r - 0.418688 * g - 0.081312 * b));
            }
        }
        return ycbcr;
    }

    void JPEGCompressor::subsample(std::vector<YCbCrPixel>& ycbcr, int width, int height) {
        for (int y = 0; y < height; y += 2) {
            for (int x = 0; x < width; x += 2) {
                if (y + 1 < height && x + 1 < width) {
                    uint16_t avgCb = 0;
                    uint16_t avgCr = 0;

                    for (int dy = 0; dy < 2; dy++) {
                        for (int dx = 0; dx < 2; dx++) {
                            avgCb += ycbcr[(y + dy) * width + (x + dx)].cb;
                            avgCr += ycbcr[(y + dy) * width + (x + dx)].cr;
                        }
                    }

                    avgCb /= 4;
                    avgCr /= 4;

                    for (int dy = 0; dy < 2; dy++) {
                        for (int dx = 0; dx < 2; dx++) {
                            ycbcr[(y + dy) * width + (x + dx)].cb = avgCb;
                            ycbcr[(y + dy) * width + (x + dx)].cr = avgCr;
                        }
                    }
                }
            }
        }
    }

    void JPEGCompressor::quantize(std::vector<YCbCrPixel>& ycbcr, double quality) {
        int quantizationFactor = static_cast<int>((1.0 - quality) * 50);

        for (auto& pixel : ycbcr) {
            pixel.cb = (pixel.cb / (quantizationFactor + 1)) * (quantizationFactor + 1);
            pixel.cr = (pixel.cr / (quantizationFactor + 1)) * (quantizationFactor + 1);
            pixel.y = (pixel.y / ((quantizationFactor / 2) + 1)) * ((quantizationFactor / 2) + 1);
        }
    }

} // namespace margelo::nitro::imagecompressor
