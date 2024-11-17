#pragma once

#include <string>
#include "HybridImageCompressorSpec.hpp"
#include "CompressionOptions.hpp"

namespace margelo::nitro::imagecompressor {
    class HybridImageCompressor: public HybridImageCompressorSpec {
    public:
        HybridImageCompressor(): HybridObject(TAG) {}

        std::string compress(const std::string& uri, const std::optional<CompressionOptions>& options) override;
    };
} // namespace margelo::nitro::imagecompressor
