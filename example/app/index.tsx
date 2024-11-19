import { type CompressedImageAsset, ImageCompressor } from '@corasan/image-compressor'
import { Image } from 'expo-image'
import * as ImagePicker from 'expo-image-picker'
import { useState } from 'react'
import { ScrollView, StyleSheet, Text, TouchableOpacity, View } from 'react-native'
import { ImageDetails } from '../components/ImageDetails'
import { formatBytes } from '../utils/formatBytes'

export default function App() {
  const [originalImg, setImage] = useState<ImagePicker.ImagePickerAsset | null>(null)
  const [compressedImage, setCompressedImage] = useState<CompressedImageAsset | null>(
    null,
  )
  const [timeToCompress, setTimeToCompress] = useState<number | null>(null)

  const pickImage = async () => {
    const result = await ImagePicker.launchImageLibraryAsync({
      mediaTypes: ['images'],
      allowsEditing: false,
      quality: 1,
    })
    const img = {
      uri: result.assets[0].uri,
      width: result.assets[0].width,
      height: result.assets[0].height,
      fileSize: result.assets[0].fileSize,
    }

    if (!result.canceled) {
      setImage(img)
      setCompressedImage(null)
    }

    const startTime = performance.now()
    const compressedImg = ImageCompressor.compress(img, { quality: 0.2 })
    const endTime = performance.now()
    setTimeToCompress(endTime - startTime)
    setCompressedImage(compressedImg)
  }

  if (compressedImage) {
    console.log(
      'It tooks',
      timeToCompress.toFixed(2),
      'ms to compress the image. From',
      formatBytes(originalImg.fileSize),
      'to',
      compressedImage.fileSize,
    )
  }

  return (
    <ScrollView
      style={{ backgroundColor: 'white' }}
      contentContainerStyle={{ padding: 16, paddingTop: 60 }}
    >
      <View style={styles.container}>
        <Text style={styles.title}>Welcome to Nitro compressor!</Text>
        <TouchableOpacity onPress={() => pickImage()}>
          <View style={styles.button}>
            <Text>Pick a photo</Text>
          </View>
        </TouchableOpacity>

        <View style={styles.separator} />

        {!originalImg ? null : (
          <>
            <Text style={styles.title}>Original Image</Text>
            <ImageDetails image={originalImg} />
          </>
        )}
        <View style={styles.separator} />

        {!compressedImage ? null : (
          <>
            <Text style={styles.title}>Compressed Image</Text>
            <ImageDetails image={compressedImage} />
          </>
        )}
      </View>
    </ScrollView>
  )
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  title: {
    fontSize: 20,
    fontWeight: 'bold',
    paddingVertical: 8,
  },
  separator: {
    marginVertical: 6,
    height: 1,
    width: '80%',
  },
  button: {
    backgroundColor: '#d5d5ff',
    padding: 10,
    borderRadius: 5,
    alignSelf: 'flex-start',
  },
})
