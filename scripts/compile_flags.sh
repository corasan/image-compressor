#!/bin/bash
CURRENT_DIR=$(pwd)

cat > compile_flags.txt << EOF
-std=c++20
-Wall
-Wextra
-I/usr/include
-I/usr/local/include
-I.
-I${CURRENT_DIR}/package/
-I${CURRENT_DIR}/package/cpp/
-I${CURRENT_DIR}/package/ios/
-I${CURRENT_DIR}/package/nitrogen/generated/shared/c++/
-I${CURRENT_DIR}/node_modules/react-native-nitro-modules/ios/
-I${CURRENT_DIR}/node_modules/react-native-nitro-modules/android/
-I${CURRENT_DIR}/node_modules/react-native-nitro-modules/cpp/
-I${CURRENT_DIR}/node_modules/react-native-nitro-modules/cpp/core/
-I${CURRENT_DIR}/node_modules/react-native-nitro-modules/cpp/core/detail/
--include-directory=${CURRENT_DIR}/package/
EOF
