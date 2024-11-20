#include "HybridImageCompressor.hpp"
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include "ImageUtils.hpp"
#include "ImageCompressor-Swift.h"

std::filesystem::path tmpImagePath;

namespace margelo::nitro::imagecompressor {

    CompressedImageAsset HybridImageCompressor::compress(const ImageAsset& image, const std::optional<CompressionOptions>& options) {
        auto result = ImageUtils::compressImage(image, options);
        tmpImagePath = result.uri; // Store the path for later use
        return result;
    }

    void HybridImageCompressor::saveImage(const std::string& uri) {
        if (tmpImagePath.empty()) {
            throw std::runtime_error("No compressed image available. Call compress() first.");
        }
        
        if (!std::filesystem::exists(tmpImagePath)) {
            throw std::runtime_error("Compressed image file no longer exists at path: " + tmpImagePath.string());
        }
        
        bool success = ImageUtils::saveImageToPhotos(tmpImagePath.string());
        if (!success) {
            throw std::runtime_error("Failed to save image to Photos Library. Check logs for details.");
        }
    }

} // namespace margelo::nitro::imagecompressor
