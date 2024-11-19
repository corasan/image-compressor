// TODO: Export all HybridObjects here
import { NitroModules } from 'react-native-nitro-modules'
import type { ImageCompressor as ImageCompressorType, ImageAsset, CompressedImageAsset, CompressionOptions } from './specs/ImageCompressor.nitro'

export const ImageCompressor = NitroModules.createHybridObject<ImageCompressorType>('ImageCompressor')

export type { ImageAsset, CompressedImageAsset, CompressionOptions }
