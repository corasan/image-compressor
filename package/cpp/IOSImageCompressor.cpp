#include "IOSImageCompressor.hpp"
#include "ImageIOCompressor.hpp"
#include <stdexcept>

namespace margelo::nitro::imagecompressor {

std::string IOSImageCompressor::compress(const std::string& path, double quality) {
#ifdef __APPLE__
    try {
        return ImageIOCompressor::compress(path, quality);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Image compression failed: ") + e.what());
    }
#else
    throw std::runtime_error("IOSImageCompressor is only available on iOS/macOS");
#endif
}

} // namespace margelo::nitro::imagecompressor
