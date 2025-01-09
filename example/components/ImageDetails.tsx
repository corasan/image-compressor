import { type CompressedImageAsset, ImageCompressor } from '@corasan/image-compressor'
import { Image } from 'expo-image'
import type * as ImagePicker from 'expo-image-picker'
import { Alert, StyleSheet, Text, TouchableOpacity, View } from 'react-native'
import { formatBytes } from '../utils/formatBytes'

type ImageDetailsProps = {
  image: CompressedImageAsset | ImagePicker.ImagePickerAsset
}

export function ImageDetails({ image }: ImageDetailsProps) {
  const saveImage = async () => {
    const filePath = image.uri.replace('file://', '')
    const wasSaved = ImageCompressor.saveImage(filePath)
    if (wasSaved) {
      Alert.alert('Image saved successfully')
    }
  }

  return (
    <View style={styles.container}>
      <Image
        source={image.uri}
        style={styles.image}
        contentFit="contain"
        transition={200}
      />
      <View style={styles.detailsContainer}>
        <View>
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
        {typeof image.fileSize === 'string' ? (
          <TouchableOpacity onPress={() => saveImage()}>
            <Text style={styles.buttonText}>Save</Text>
          </TouchableOpacity>
        ) : null}
      </View>
    </View>
  )
}

const styles = StyleSheet.create({
  container: {
    flexDirection: 'column',
    width: '100%',
    gap: 8,
  },
  detailsContainer: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    padding: 8,
    borderRadius: 6,
    backgroundColor: '#eee',
    marginHorizontal: 24,
  },
  image: {
    height: 200,
  },
  buttonText: {
    fontWeight: 'bold',
    padding: 4,
    color: '#67A',
  },
})
