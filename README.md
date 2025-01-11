# @corasan/image-compressor

`@corasan/image-compressor` is a React Native module that allows you to compress images using OpenCV. Built with [Nitro Modules](https://github.com/mrousavy/nitro).

## Installation

You will need to install `react-native-nitro-modules` in your app.

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

const wasSaved = await compressedImage.save()
```
