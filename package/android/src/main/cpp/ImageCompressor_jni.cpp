#include "ImageCompressor_jni.hpp"
#include "JNIUtils.hpp"
#if __has_include("android/log.h")
#include <android/log.h>
#endif

#define LOG_TAG "ImageCompressor"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace margelo::nitro::imagecompressor {
  bool ImageCompressor_jni::saveImageToPhotos(const std::string& uri) {
    jclass clazz = getImageCompressorClass();
    JNIEnv* env = getEnv();

    if (clazz == nullptr)
      return false;

    jmethodID methodId =
        env->GetStaticMethodID(clazz, "saveImageToGallery", "(Ljava/lang/String;)Z");
    if (methodId == nullptr) {
      return false;
    }

    jstring jUri = env->NewStringUTF(uri.c_str());
    jboolean result = env->CallStaticBooleanMethod(clazz, methodId, jUri);
    env->DeleteLocalRef(jUri);
    return result;
  };

  jclass ImageCompressor_jni::getImageCompressorClass() {
    if (g_jvm->GetEnv(reinterpret_cast<void**>(&g_env), JNI_VERSION_1_6) != JNI_OK) {
      LOGE("Failed to get the environment");
      return nullptr;
    }
    jclass localRef = g_env->FindClass("com/margelo/imagecompressor/PhotoLibrary");
    if (localRef == nullptr) {
      LOGE("Failed to find ImageCompressor class");
      return nullptr;
    }

    jclass globalRef = static_cast<jclass>(g_env->NewGlobalRef(localRef));
    g_env->DeleteLocalRef(localRef);

    return globalRef;
  }
} // namespace margelo::nitro::imagecompressor