#pragma once

#include "HybridImageCompressorSpec.hpp"
#include <string>

namespace margelo::nitro::imagecompressor {
  class HybridImageCompressor : public HybridImageCompressorSpec {
  public:
    HybridImageCompressor() : HybridObject(TAG) {}

    CompressedImageAsset compress(const ImageAsset& image, const std::optional<CompressionOptions>& options) override;
    bool saveImage(const std::string& uri) override;
  };
} // namespace margelo::nitro::imagecompressor
