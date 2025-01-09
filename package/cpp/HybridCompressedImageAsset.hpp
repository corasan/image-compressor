// HybridCompressedImageAsset.hpp
#pragma once

#include "HybridCompressedImageAssetSpec.hpp"
#include <string>

namespace margelo::nitro::imagecompressor {
  class HybridCompressedImageAsset : public HybridCompressedImageAssetSpec {
  private:
    std::string _uri;
    double _width;
    double _height;
    std::string _fileSize;

  public:
    HybridCompressedImageAsset() : HybridObject(TAG) {}

    void initialize(std::string uri, double width, double height, std::string fileSize) {
      _uri = std::move(uri);
      _width = width;
      _height = height;
      _fileSize = std::move(fileSize);
    }

    // Property implementations
    std::string getUri() override {
      return _uri;
    }
    void setUri(const std::string& uri) override {
      _uri = uri;
    }

    double getWidth() override {
      return _width;
    }
    void setWidth(double width) override {
      _width = width;
    }

    double getHeight() override {
      return _height;
    }
    void setHeight(double height) override {
      _height = height;
    }

    std::string getFileSize() override {
      return _fileSize;
    }
    void setFileSize(const std::string& fileSize) override {
      _fileSize = fileSize;
    }

    // Method implementations
    std::shared_ptr<Promise<bool>> save() override;
  };
} // namespace margelo::nitro::imagecompressor
