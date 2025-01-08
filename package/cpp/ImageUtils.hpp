#pragma once

#include "CompressedImageAsset.hpp"
#include "CompressionOptions.hpp"
#include "ImageAsset.hpp"
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <string>

namespace margelo::nitro::imagecompressor {

  class ImageUtils {
  public:
    // Main compression function
    static CompressedImageAsset compressImage(const ImageAsset& image,
                                              const std::optional<CompressionOptions>& options);

    // Helper functions
    static std::string stripFilePrefix(const std::string& path);
    static std::string formatFileSize(uintmax_t bytes);

  private:
    static cv::Mat loadImage(const std::string& path);
    static cv::Mat resizeImage(const cv::Mat& img, const std::optional<double>& maxWidth,
                               const std::optional<double>& maxHeight);
    static std::pair<std::vector<int>, std::string> getCompressionParams(double quality, const std::string& format);
    static std::filesystem::path generateTempPath(const std::string& originalPath, const std::string& extension);
  };

} // namespace margelo::nitro::imagecompressor
