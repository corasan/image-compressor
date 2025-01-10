#pragma once

#include <jni.h>
#include <string>

namespace margelo::nitro::imagecompressor {
  class ImageCompressor_jni {
  public:
    static bool saveImageToPhotos(const std::string& uri);

  private:
    static jclass getImageCompressorClass();
  };
} // namespace margelo::nitro::imagecompressor