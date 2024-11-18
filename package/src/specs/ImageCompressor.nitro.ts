import type { HybridObject } from 'react-native-nitro-modules'

export interface CompressionOptions {
  quality?: number
  maxWidth?: number
  maxHeight?: number
  outputFormat?: string
}

export interface CompressionResult {
  uri: string
  width: number
  height: number
  fileSize: number
}

export interface ImageAsset {
  uri: string
  width: number
  height: number
  fileSize: number
}

export interface ImageCompressor extends HybridObject<{ ios: 'c++'; android: 'c++' }> {
  compress(image: ImageAsset, options?: CompressionOptions): string
}
