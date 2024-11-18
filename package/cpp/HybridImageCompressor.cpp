#include "HybridImageCompressor.hpp"
#include <ImageUtils.hpp>
#include <fstream>
#include <ios>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace margelo::nitro::imagecompressor {

std::string HybridImageCompressor::compress(const std::string& uri, const std::optional<CompressionOptions>& options) {
  auto opts = options.value_or(CompressionOptions(
    std::optional<double>(0.8),  // default quality
    std::nullopt,                // default maxWidth
    std::nullopt,                // default maxHeight
    std::optional<std::string>("jpg")  // default format
  ));

  // Strip file:// prefix if present
    std::string path = uri;
    if (path.find("file://") == 0) {
        path = path.substr(7);
    }

    // Read file into buffer
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }

    // Get file size and read into buffer
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + path);
    }
    file.close();

    auto dimensions = ImageUtils::getJPEGDimensions(buffer);

    // For now just return debug info
    return "Loaded image " + path +
           "\nSize: " + std::to_string(size) + " bytes" +
           "\nDimensions: " + std::to_string(dimensions.width) + "x" + std::to_string(dimensions.height);
}

} // namespace margelo::nitro::imagecompressor
