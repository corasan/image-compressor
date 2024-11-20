#include "ImageUtils.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

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

    cv::Mat ImageUtils::resizeImage(const cv::Mat& img, const std::optional<double>& maxWidth, const std::optional<double>& maxHeight) {
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

    std::pair<std::vector<int>, std::string> ImageUtils::getCompressionParams(double quality, const std::string& format) {
        std::vector<int> compression_params;
        std::string extension;
        
        if (format == "jpg" || format == "jpeg") {
            compression_params = {
                cv::IMWRITE_JPEG_QUALITY,
                static_cast<int>(quality * 100)
            };
            extension = ".jpg";
        } else if (format == "png") {
            compression_params = {
                cv::IMWRITE_PNG_COMPRESSION,
                static_cast<int>((1.0 - quality) * 9)
            };
            extension = ".png";
        } else {
            throw std::runtime_error("Unsupported output format: " + format);
        }
        
        return {compression_params, extension};
    }

    std::filesystem::path ImageUtils::generateTempPath(const std::string& originalPath, const std::string& extension) {
        return std::filesystem::temp_directory_path() /
        std::filesystem::path(std::to_string(std::hash<std::string>{}(originalPath)) + extension);
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

    CompressedImageAsset ImageUtils::compressImage(const ImageAsset& image, const std::optional<CompressionOptions>& options) {
        // Parse options with defaults
        auto opts = options.value_or(
            CompressionOptions(
                std::optional<double>(0.8),
                std::nullopt,
                std::nullopt,
                std::optional<std::string>("jpg")
            )
        );
        
        // Process image
        std::string path = stripFilePrefix(image.uri);
        cv::Mat img = loadImage(path);
        img = resizeImage(img, opts.maxWidth, opts.maxHeight);
        
        // Get compression settings
        double quality = opts.quality.value_or(0.8);
        std::string format = opts.outputFormat.value_or("jpg");
        auto [compression_params, extension] = getCompressionParams(quality, format);
        
        // Save compressed image
        auto outputPath = generateTempPath(path, extension);
        
        if (!cv::imwrite(outputPath.string(), img, compression_params)) {
            throw std::runtime_error("Failed to compress and save image");
        }
        
        // Create result
        uintmax_t compressedSize = std::filesystem::file_size(outputPath);
        return CompressedImageAsset(outputPath, img.cols, img.rows, formatFileSize(compressedSize));
    }

} // namespace margelo::nitro::imagecompressor
