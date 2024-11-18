// IOSImageCompressor.hpp
#pragma once
#include <string>

namespace margelo::nitro::imagecompressor {

class IOSImageCompressor {
public:
    static std::string compress(const std::string& path, double quality);
};

} // namespace margelo::nitro::imagecompressor
