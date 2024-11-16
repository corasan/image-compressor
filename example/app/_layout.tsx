import { SplashScreen, Stack } from 'expo-router'
import { useEffect } from 'react'

SplashScreen.preventAutoHideAsync()
export default function RootLayout() {
  useEffect(() => {
    SplashScreen.hideAsync()
  }, [])

  return <Stack screenOptions={{ headerShown: false }} />
}
