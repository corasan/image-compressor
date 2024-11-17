import { ImageCompressor } from '@corasan/image-compressor'
import { Image } from 'expo-image'
import * as ImagePicker from 'expo-image-picker'
import { useState } from 'react'
import { StyleSheet, Text, TouchableOpacity, View } from 'react-native'

export default function App() {
  const [image, setImage] = useState<string | null>(null)

  const pickImage = async () => {
    const result = await ImagePicker.launchImageLibraryAsync({
      mediaTypes: ['images'],
      allowsEditing: false,
      quality: 1,
    })

    console.log(ImageCompressor.compress(result.assets[0].uri, { quality: 0.5 }))

    if (!result.canceled) {
      setImage(result.assets[0].uri)
    }
  }

  return (
    <View style={styles.container}>
      <Text style={styles.title}>Welcome to Nitro!</Text>
      <View style={styles.separator} />
      <TouchableOpacity onPress={() => pickImage()}>
        <View style={styles.button}>
          <Text>Pick an image</Text>
        </View>
      </TouchableOpacity>

      <View style={styles.separator} />

      {!image ? null : (
        <Image
          source={{ uri: image }}
          style={styles.image}
          contentFit="contain"
          transition={200}
        />
      )}
    </View>
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
  },
  separator: {
    marginVertical: 10,
    height: 1,
    width: '80%',
  },
  button: {
    backgroundColor: '#d5d5ff',
    padding: 10,
    borderRadius: 5,
  },
  image: {
    height: 300,
    width: '100%',
  },
})
