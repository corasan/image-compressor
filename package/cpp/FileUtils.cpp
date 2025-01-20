#include "FileUtils.hpp"

namespace margelo::nitro::imagecompressor {
  std::string FileUtils::stripFilePrefix(const std::string& path) {
    if (path.find("file://") == 0) {
      return path.substr(7);
    }
    return path;
  }

  void FileUtils::removeTmpFile(const std::string& path) {
    std::string strippedPath = stripFilePrefix(path);
    if (std::filesystem::exists(strippedPath)) {
      std::filesystem::remove(strippedPath);
    }
  }
} // namespace margelo::nitro::imagecompressor
