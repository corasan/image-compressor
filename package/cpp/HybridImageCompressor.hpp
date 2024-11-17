#pragma once

#include <string>
#include "HybridImageCompressorSpec.hpp"

namespace margelo::nitro::imagecompressor {
    class HybridImageCompressor: public HybridImageCompressorSpec {
    public:
        HybridImageCompressor(): HybridObject(TAG) {}

        std::string compress(const std::string& uri) override;
    };
} // namespace margelo::nitro::imagecompressor
