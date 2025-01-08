project(ImageCompressor)
cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME ImageCompressor)
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 20)

# Set OpenCV path
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/OpenCV/sdk/native/jni")
# Add OpenCV includes
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/OpenCV/sdk/native/jni/include")

# Find OpenCV package
find_package(OpenCV REQUIRED)


# Define C++ library and add all sources
add_library(${PACKAGE_NAME} SHARED
        src/main/cpp/cpp-adapter.cpp
        ../cpp/HybridImageCompressor.cpp
        ../cpp/ImageUtils.cpp
)

# Add Nitrogen specs :)
include(${CMAKE_SOURCE_DIR}/../nitrogen/generated/android/ImageCompressor+autolinking.cmake)

# Set up local includes
include_directories(
        "src/main/cpp"
        "../cpp"
        ${OpenCV_INCLUDE_DIRS}
)

find_library(LOG_LIB log)
find_package(OpenCV REQUIRED)

# Link all libraries together
target_link_libraries(
        ${PACKAGE_NAME}
        ${OpenCV_LIBS}                            # <-- Link OpenCV libraries
        ${LOG_LIB}
        android                                   # <-- Android core
)
