#include "HybridImageCompressor.hpp"

namespace margelo::nitro::imagecompressor {

std::string HybridImageCompressor::compress(const std::string& uri) {
  // std::filesystem::path path(uri);
  // auto start = std::chrono::high_resolution_clock::now();
  // auto result = compress(path, options);
  // auto end = std::chrono::high_resolution_clock::now();
  // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  // return "Compressed " + std::to_string(result.size) + " bytes in " + std::to_string(duration) + "ms";
  return "The URI is " + uri;
}

} // namespace margelo::nitro::imagecompressor
