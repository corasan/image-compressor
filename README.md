# @corasan/image-compressor

`@corasan/image-compressor` is a React Native module that allows you to compress images using OpenCV. Built with C++ and Swift with the power of [Nitro Modules](https://github.com/mrousavy/nitro).

## ⚠️ WORK IN PROGRESS ⚠️

> [!WARNING]
> This package is still in development and very much an experiment. It's not ready for production use yet. Has only been tested on iOS using Expo.

## Installation

You will need to install `react-native-nitro-modules` in your project.

```sh
yarn add @corasan/image-compressor react-native-nitro-modules
npm install @corasan/image-compressor react-native-nitro-modules
bun add @corasan/image-compressor react-native-nitro-modules
```

Add the plugin to your `app.json` file:
```json
{
  "expo": {
    "plugins": [
      "@corasan/image-compressor"
    ]
  }
}
```

If you're using Expo, you will need to run `expo prebuild` before building your app.

**Note:** You might need to add the right permissions to your `app.json`.
```json
{
  "expo": {
    "plugins": [
      "@corasan/image-compressor",
      [
        "expo-media-library",
        {
          "photosPermission": "Allow $(PRODUCT_NAME) to access your photos.",
          "savePhotosPermission": "Allow $(PRODUCT_NAME) to save photos.",
          "isAccessMediaLocationEnabled": true
        }
      ]
    ]
  }
}
```

## Usage

```ts
import { ImageCompressor } from '@corasan/image-compressor'

const compressedImage = ImageCompressor.compress(image, {
  quality: 0.8,
  maxWidth: 800,
})

const wasSaved = compressedImage.save(compressedImage.uri)
```
