#include "HybridImageCompressor.hpp"
#include "CompressionOptions.hpp"
#include "HybridCompressedImageAsset.hpp"
#include "ImageUtils.hpp"
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <stdexcept>

std::filesystem::path tmpImagePath;

namespace margelo::nitro::imagecompressor {

  std::shared_ptr<margelo::nitro::imagecompressor::HybridCompressedImageAssetSpec>
  HybridImageCompressor::compress(const ImageAsset& image, const std::optional<CompressionOptions>& options) {
    FileUtils::removeTmpFile(tmpImagePath);
    auto result = ImageUtils::compressImage(image, options);
    tmpImagePath = result.get()->getUri(); // Store the path for later use
    return result;
  }
} // namespace margelo::nitro::imagecompressor
