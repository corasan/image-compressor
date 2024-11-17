// TODO: Export all HybridObjects here
import { NitroModules } from 'react-native-nitro-modules'
import type { ImageCompressor as ImageCompressorType } from './specs/ImageCompressor.nitro'

export const ImageCompressor = NitroModules.createHybridObject<ImageCompressorType>('ImageCompressor')
