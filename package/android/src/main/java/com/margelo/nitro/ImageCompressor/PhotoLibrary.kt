package com.margelo.imagecompressor

import android.content.ContentValues
import android.content.Context
import android.os.Build
import android.os.Environment
import android.provider.MediaStore
import java.io.File
import java.io.FileInputStream
import java.io.IOException
import android.util.Log
import com.margelo.nitro.NitroModules
import com.facebook.react.bridge.ReactApplicationContext
import android.app.Activity

object PhotoLibrary {
    @JvmStatic
    fun saveImageToGallery(imagePath: String): Boolean {
        val cleanPath = imagePath.removePrefix("file:/")    
        val imageFile = File(cleanPath)
        val context = NitroModules.applicationContext?: return false
        Log.d("ImageCompressor","PhotoLibrary: context was found")

        val activity = context.currentActivity?: return false
        Log.d("ImageCompressor","PhotoLibrary: activity was found")

        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            saveImageApi29AndAbove(context, imageFile)
        } else {
            saveImageLegacy(context, imageFile)
        }
    }

    @JvmStatic
    private fun saveImageApi29AndAbove(context: ReactApplicationContext, imageFile: File): Boolean {
        val contentValues = ContentValues().apply {
            put(MediaStore.MediaColumns.DISPLAY_NAME, imageFile.nameWithoutExtension)
            put(MediaStore.MediaColumns.MIME_TYPE, getMimeType(imageFile.extension))
            put(MediaStore.MediaColumns.RELATIVE_PATH, Environment.DIRECTORY_PICTURES)
            put(MediaStore.Images.Media.DATE_ADDED, System.currentTimeMillis() / 1000)
            put(MediaStore.Images.Media.DATE_TAKEN, System.currentTimeMillis())
        }

        val resolver = context.contentResolver
        val uri = resolver.insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, contentValues) ?: return false

        return try {
            resolver.openOutputStream(uri)?.use { outputStream ->
                FileInputStream(imageFile).use { inputStream ->
                    inputStream.copyTo(outputStream)
                }
            }
            Log.d("ImageCompressor","PhotoLibrary: Image saved successfully")
            true
        } catch (e: IOException) {
            // Clean up on failure
            resolver.delete(uri, null, null)
            Log.d("ImageCompressor","PhotoLibrary: Failed to save image")
            Log.d("ImageCompressor","PhotoLibrary - Error: ${e.message}")
            false
        }
    }
    
    @JvmStatic
    private fun saveImageLegacy(context: ReactApplicationContext, imageFile: File): Boolean {
        try {
            // Ensure the Pictures directory exists
            val pictures = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES)
            if (!pictures.exists() && !pictures.mkdirs()) {
                return false
            }

            // Copy the file to Pictures directory
            val destination = File(pictures, imageFile.name)
            FileInputStream(imageFile).use { input ->
                destination.outputStream().use { output ->
                    input.copyTo(output)
                }
            }

            // Notify the system about the new image
            context.sendBroadcast(android.content.Intent(android.content.Intent.ACTION_MEDIA_SCANNER_SCAN_FILE).apply {
                data = android.net.Uri.fromFile(destination)
            })

            Log.d("ImageCompressor","PhotoLibrary: Image saved successfully")
            return true
        } catch (e: Exception) {
            Log.d("ImageCompressor", "PhotoLibrary - Error in legacy save: ${e.message}")
            Log.d("ImageCompressor","PhotoLibrary - Error in legacy save: Failed to save image")
            return false
        }
    }

    private fun getMimeType(extension: String): String {
        return when (extension.lowercase()) {
            "jpg", "jpeg" -> "image/jpeg"
            "png" -> "image/png"
            "gif" -> "image/gif"
            "webp" -> "image/webp"
            else -> "image/jpeg"  // default to JPEG
        }
    }

    private fun getCurrentActivity(): Activity? {
        return MyContext.getCurrentActivity()
    }

    private fun stripFilePrefix(path: String): String {
        return when {
            path.startsWith("file:///") -> path.substring(7)
            path.startsWith("file:/") -> path.substring(5)
            path.startsWith("file:") -> path.substring(5)
            else -> path
        }
    }
}

object MyContext {
    private var reactContext: ReactApplicationContext? = null

    fun setContext(context: ReactApplicationContext) {
        reactContext = context
    }

    fun getCurrentActivity(): Activity? {
        return reactContext?.currentActivity
    }
}