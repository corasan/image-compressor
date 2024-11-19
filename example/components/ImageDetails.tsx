import type { CompressedImageAsset } from '@corasan/image-compressor'
import { Image } from 'expo-image'
import type * as ImagePicker from 'expo-image-picker'
import { Text, View } from 'react-native'
import { formatBytes } from '../utils/formatBytes'

export function ImageDetails({
  image,
}: { image: CompressedImageAsset | ImagePicker.ImagePickerAsset }) {
  return (
    <View
      style={{
        flexDirection: 'column',
        width: '100%',
        gap: 8,
      }}
    >
      <Image
        source={{ uri: image.uri }}
        style={{ height: 240, width: '100%' }}
        contentFit="contain"
        transition={200}
      />
      <View
        style={{
          padding: 8,
          borderRadius: 6,
          backgroundColor: '#eee',
          marginHorizontal: 8,
        }}
      >
        <Text>
          Dimensions: {image.height} x {image.width}
        </Text>
        <Text>
          File Size:{' '}
          {typeof image.fileSize === 'string'
            ? image.fileSize
            : formatBytes(image.fileSize)}
        </Text>
      </View>
    </View>
  )
}
