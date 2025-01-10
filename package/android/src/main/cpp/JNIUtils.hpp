#pragma once

#include <jni.h>

namespace margelo::nitro::imagecompressor {
  // Declare the globals as extern
  extern JavaVM* g_jvm;
  extern JNIEnv* g_env;
  extern jclass g_class;

  // Utility function to get env
  JNIEnv* getEnv();
} // namespace margelo::nitro::imagecompressor