#pragma once

#include "CompressionOptions.hpp"
#include "HybridCompressedImageAsset.hpp"
#include "ImageAsset.hpp"
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <string>
#include "FileUtils.hpp"

namespace margelo::nitro::imagecompressor {

  class ImageUtils {
  public:
    // Main compression function
    static std::shared_ptr<HybridCompressedImageAsset>
    compressImage(const ImageAsset& image, const std::optional<CompressionOptions>& options);

    // Helper functions
    static std::string formatFileSize(uintmax_t bytes);

  private:
    static cv::Mat loadImage(const std::string& path);
    static cv::Mat resizeImage(const cv::Mat& img, const std::optional<double>& maxWidth,
                               const std::optional<double>& maxHeight);
    static std::pair<std::vector<int>, std::string> getCompressionParams(double quality,
                                                                         const std::string& format);
    static std::filesystem::path generateTempPath(const std::string& originalPath,
                                                  const std::string& extension);

    static cv::Mat preprocessImg(const cv::Mat& img);
  };

} // namespace margelo::nitro::imagecompressor
