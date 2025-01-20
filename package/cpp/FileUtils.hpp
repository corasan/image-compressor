#pragma once

#include <filesystem>

namespace margelo::nitro::imagecompressor {
  class FileUtils {
  public:
    static std::string stripFilePrefix(const std::string& path);
    static void removeTmpFile(const std::string& path);
  };
} // namespace margelo::nitro::imagecompressor
