#include "HybridImageCompressor.hpp"
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <filesystem>
#include <sstream>
#include <iomanip>

namespace margelo::nitro::imagecompressor {

std::string formatFileSize(uintmax_t bytes) {
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

CompressedImageAsset HybridImageCompressor::compress(const ImageAsset& image, const std::optional<CompressionOptions>& options) {
    // Parse options with defaults
    auto opts = options.value_or(
        CompressionOptions(
            std::optional<double>(0.8),
            std::nullopt,
            std::nullopt,
            std::optional<std::string>("jpg")
        )
    );
    
    // Strip file:// prefix if present
    std::string path = image.uri;
    if (path.find("file://") == 0) {
        path = path.substr(7);
    }
    
    // Load image
    cv::Mat img = cv::imread(path);
    if (img.empty()) {
        throw std::runtime_error("Failed to load image: " + path);
    }
    
    // Original dimensions
    int originalWidth = img.cols;
    int originalHeight = img.rows;
    
    // Calculate new dimensions if maxWidth or maxHeight is specified
    if (opts.maxWidth || opts.maxHeight) {
        int targetWidth = opts.maxWidth ? static_cast<int>(opts.maxWidth.value()) : img.cols;
        int targetHeight = opts.maxHeight ? static_cast<int>(opts.maxHeight.value()) : img.rows;
        
        // Calculate scaling factors
        double widthScale = static_cast<double>(targetWidth) / img.cols;
        double heightScale = static_cast<double>(targetHeight) / img.rows;
        
        // Use the smaller scale to maintain aspect ratio
        double scale = std::min(widthScale, heightScale);
        
        // Calculate new dimensions
        int newWidth = static_cast<int>(img.cols * scale);
        int newHeight = static_cast<int>(img.rows * scale);
        
        // Only resize if the new dimensions are smaller
        if (newWidth < originalWidth || newHeight < originalHeight) {
            cv::Mat resized;
            cv::resize(img, resized, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_AREA);
            img = resized;
        }
    }
    
    // Prepare compression parameters
    std::vector<int> compression_params;
    std::string extension;
    
    double quality = opts.quality.value_or(0.8);
    std::string format = opts.outputFormat.value_or("jpg");
    
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
    
    // Generate output path in temp directory
    auto outputPath = std::filesystem::temp_directory_path() /
    std::filesystem::path(std::to_string(std::hash<std::string>{}(path)) + extension);
    
    // Compress and save
    if (!cv::imwrite(outputPath.string(), img, compression_params)) {
        throw std::runtime_error("Failed to compress and save image");
    }
    
    // Get compressed file size
    uintmax_t compressedSize = std::filesystem::file_size(outputPath);
    CompressedImageAsset properties = CompressedImageAsset(outputPath, img.cols, img.rows, formatFileSize(compressedSize));

    return properties;
}

} // namespace margelo::nitro::imagecompressor
