import { StyleSheet, Text, TouchableOpacity, View } from 'react-native'

export default function App() {
  return (
    <View style={styles.container}>
      <Text style={styles.title}>Welcome to Nitro!</Text>
      <View style={styles.separator} />
      <TouchableOpacity onPress={() => alert('Hello!')}>
        <View style={styles.button}>
          <Text>Tap me!</Text>
        </View>
      </TouchableOpacity>
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
    marginVertical: 30,
    height: 1,
    width: '80%',
  },
  button: {
    backgroundColor: '#d5d5ff',
    padding: 10,
    borderRadius: 5,
  },
})
