#include "HybridImageCompressor.hpp"
#include <stdexcept>
#include "ImageCompressor-Swift.h"

namespace margelo::nitro::imagecompressor {
    
    std::string HybridImageCompressor::compress(const ImageAsset& image, const std::optional<CompressionOptions>& options) {
        auto opts = options.value_or(CompressionOptions(
            std::optional<double>(0.8),
            std::nullopt,
            std::nullopt,
            std::optional<std::string>("jpg")
        ));

        // Strip file:// prefix if present
        std::string path = image.uri;
        if (path.find("file://") == 0) {
            path = path.substr(7);
        }

        double quality = opts.quality.value_or(0.8);

        try {
            return ImageCompressor::ImageIOCompressor::hello();
        } catch (const std::exception& e) {
            throw std::runtime_error("Image compression failed: " + std::string(e.what()));
        }

        // For now just return debug info
        // return "Loaded image " + image.uri +
        //     "\nSize: " + std::to_string(image.fileSize) + " bytes" +
        //     "\nDimensions: " + std::to_string(width) + "x" + std::to_string(height) +
        //     "\nQuality: " + std::to_string(quality) +
        //     "\nCompressed size: " + std::to_string(compressed.size()) + " bytes";
    }
} // namespace margelo::nitro::imagecompressor
