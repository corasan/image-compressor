#!/bin/bash

ANDROID_NDK_PATH=$ANDROID_NDK
ANDROID_SDK_PATH=$ANDROID_SDK_ROOT
OPENCV_SOURCE_DIR=$OPENCV_SOURCE_DIR
ANDROID_ABI="arm64-v8a,x86_64"
ANDROID_MIN_SDK=21

rm -rf package/android/OpenCV

python3 ${OPENCV_SOURCE_DIR}/platforms/android/build_sdk.py \
    --ndk_path ${ANDROID_NDK_PATH} \
    --sdk_path ${ANDROID_SDK_PATH} \
    --modules_list "core,imgproc,imgcodecs,videoio" \
    --no_samples_build \
    --disable video \
    --disable highgui \
    --disable features2d \
    --disable objdetect \
    --disable dnn \
    --disable ml \
    --disable photo \
    --disable flann \
    --disable gapi \
    --disable calib3d

cp -r OpenCV-android-sdk/ package/android/OpenCV
