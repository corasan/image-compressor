// ImageUtils.cpp
#include "ImageUtils.hpp"
#include <stdexcept>

namespace margelo::nitro::imagecompressor {

ImageDimensions ImageUtils::getJPEGDimensions(const std::vector<unsigned char>& buffer) {
    // JPEG marker for Start Of Frame (dimensions)
    const unsigned char SOF_MARKERS[] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC5, 0xC6, 0xC7, 0xC9, 0xCA, 0xCB, 0xCD, 0xCE, 0xCF};
    
    size_t i = 0;
    while (i < buffer.size()) {
        // Find marker prefix (0xFF)
        if (buffer[i] != 0xFF) {
            i++;
            continue;
        }
        
        // Check next byte for SOF marker
        i++;
        if (i >= buffer.size()) break;
        
        // Check if current marker is an SOF marker
        bool isSOF = false;
        for (unsigned char marker : SOF_MARKERS) {
            if (buffer[i] == marker) {
                isSOF = true;
                break;
            }
        }
        
        if (isSOF) {
            // Skip segment length
            i += 3;
            if (i + 4 >= buffer.size()) break;
            
            // Height and width are stored in big-endian format
            int height = (buffer[i] << 8) | buffer[i + 1];
            int width = (buffer[i + 2] << 8) | buffer[i + 3];
            
            return {width, height};
        }
        i++;
    }
    
    throw std::runtime_error("Could not determine JPEG dimensions");
}

} // namespace margelo::nitro::imagecompressor