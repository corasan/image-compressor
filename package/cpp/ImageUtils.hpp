// ImageUtils.hpp
#pragma once

#include <vector>
#include <string>

namespace margelo::nitro::imagecompressor {

struct ImageDimensions {
    int width;
    int height;
};

class ImageUtils {
public:
    static ImageDimensions getJPEGDimensions(const std::vector<unsigned char>& buffer);
};

} // namespace margelo::nitro::imagecompressor
