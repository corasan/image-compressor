#include <jni.h>
#include "ImageCompressorOnLoad.hpp"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
  return margelo::nitro::imagecompressor::initialize(vm);
}
