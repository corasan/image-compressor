#pragma once

#include "HybridImageCompressorSpec.hpp"
#include <string>
#include "FileUtils.hpp"

namespace margelo::nitro::imagecompressor {
  class HybridImageCompressor : public HybridImageCompressorSpec {
  public:
    HybridImageCompressor() : HybridObject(TAG) {}

    std::shared_ptr<margelo::nitro::imagecompressor::HybridCompressedImageAssetSpec>
    compress(const ImageAsset& image, const std::optional<CompressionOptions>& options) override;
  };
} // namespace margelo::nitro::imagecompressor
