#include "ImageUtils.hpp"
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace margelo::nitro::imagecompressor {

  std::string ImageUtils::stripFilePrefix(const std::string& path) {
    if (path.find("file://") == 0) {
      return path.substr(7);
    }
    return path;
  }

  cv::Mat ImageUtils::loadImage(const std::string& path) {
    cv::Mat img = cv::imread(path);
    if (img.empty()) {
      throw std::runtime_error("Failed to load image: " + path);
    }
    return img;
  }

  cv::Mat ImageUtils::preprocessImg(const cv::Mat& img) {
    cv::Mat processed;

    cv::bilateralFilter(img, processed, 9, 75, 75);
    return processed;
  }

  cv::Mat ImageUtils::resizeImage(const cv::Mat& img, const std::optional<double>& maxWidth,
                                  const std::optional<double>& maxHeight) {
    if (!maxWidth && !maxHeight) {
      return img;
    }

    int targetWidth = maxWidth ? static_cast<int>(maxWidth.value()) : img.cols;
    int targetHeight = maxHeight ? static_cast<int>(maxHeight.value()) : img.rows;

    double widthScale = static_cast<double>(targetWidth) / img.cols;
    double heightScale = static_cast<double>(targetHeight) / img.rows;
    double scale = std::min(widthScale, heightScale);

    int newWidth = static_cast<int>(img.cols * scale);
    int newHeight = static_cast<int>(img.rows * scale);

    if (newWidth < img.cols || newHeight < img.rows) {
      cv::Mat resized;
      cv::resize(img, resized, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_AREA);
      return resized;
    }

    return img;
  }

  std::pair<std::vector<int>, std::string>
  ImageUtils::getCompressionParams(double quality, const std::string& format) {
    std::vector<int> compression_params;
    std::string extension;

    if (format == "jpg" || format == "jpeg") {
      compression_params = {cv::IMWRITE_JPEG_QUALITY, static_cast<int>(quality * 100)};
      extension = ".jpg";
    } else if (format == "png") {
      compression_params = {cv::IMWRITE_PNG_COMPRESSION, static_cast<int>((1.0 - quality) * 9)};
      extension = ".png";
    } else {
      throw std::runtime_error("Unsupported output format: " + format);
    }

    return {compression_params, extension};
  }

  std::filesystem::path ImageUtils::generateTempPath(const std::string& originalPath,
                                                     const std::string& extension) {
    return std::filesystem::temp_directory_path() /
           std::filesystem::path(std::to_string(std::hash<std::string>{}(originalPath)) +
                                 extension);
  }

  std::string ImageUtils::formatFileSize(uintmax_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024 && unitIndex < 3) {
      size /= 1024;
      unitIndex++;
    }

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << size << units[unitIndex];
    return ss.str();
  }

  std::shared_ptr<HybridCompressedImageAsset>
  ImageUtils::compressImage(const ImageAsset& image,
                            const std::optional<CompressionOptions>& options) {
    auto opts = options.value_or(CompressionOptions(
        std::optional<double>(0.8), std::nullopt, std::nullopt, std::optional<std::string>("jpg")));

    std::string path = stripFilePrefix(image.uri);
    cv::Mat img = loadImage(path);
    cv::Mat resizedImg = resizeImage(img, opts.maxWidth, opts.maxHeight);
    img = preprocessImg(resizedImg);

    double quality = opts.quality.value_or(0.8);
    std::string format = opts.outputFormat.value_or("jpg");
    auto [compression_params, extension] = getCompressionParams(quality, format);

    // Save the compressed image
    auto outputPath = generateTempPath(path, extension);

    if (!cv::imwrite(outputPath.string(), img, compression_params)) {
      throw std::runtime_error("Failed to compress and save image");
    }

    // uintmax_t compressedSize = std::filesystem::file_size(outputPath);
    auto compressedAsset = std::make_shared<HybridCompressedImageAsset>();
    uintmax_t compressedSize = std::filesystem::file_size(outputPath);
    compressedAsset->initialize("file://" + outputPath.string(), static_cast<double>(img.cols),
                                static_cast<double>(img.rows), formatFileSize(compressedSize));
    return compressedAsset;
  }

} // namespace margelo::nitro::imagecompressor
