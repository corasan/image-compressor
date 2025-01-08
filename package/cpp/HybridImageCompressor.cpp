#include "HybridImageCompressor.hpp"
#include "ImageUtils.hpp"
#include <filesystem>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <stdexcept>
#ifdef __APPLE__
#include "ImageCompressor-Swift.h"
#endif

std::filesystem::path tmpImagePath;

namespace margelo::nitro::imagecompressor {

  CompressedImageAsset HybridImageCompressor::compress(const ImageAsset& image,
                                                       const std::optional<CompressionOptions>& options) {
    auto result = ImageUtils::compressImage(image, options);
    tmpImagePath = result.uri; // Store the path for later use
    return result;
  }

  bool HybridImageCompressor::saveImage(const std::string& uri) {
#ifdef __APPLE__
    if (tmpImagePath.empty()) {
      throw std::runtime_error("No compressed image available. Call compress() first.");
    }

    if (!std::filesystem::exists(tmpImagePath)) {
      throw std::runtime_error("Compressed image file no longer exists at path: " + tmpImagePath.string());
    }

    return ImageCompressor::saveImageToPhotos(tmpImagePath.string());
#else
    return false;
#endif
  }
} // namespace margelo::nitro::imagecompressor
