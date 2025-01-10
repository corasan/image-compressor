#include "ImageCompressorOnLoad.hpp"
#include "JNIUtils.hpp"
#include <jni.h>

namespace margelo::nitro::imagecompressor {
  JavaVM* g_jvm = nullptr;
  JNIEnv* g_env = nullptr;

  JNIEnv* getEnv() {
    return g_env;
  }
} // namespace margelo::nitro::imagecompressor

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
  using namespace margelo::nitro::imagecompressor;

  g_jvm = vm;

  return margelo::nitro::imagecompressor::initialize(vm);
}
