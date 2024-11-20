import Photos
import Foundation
import UIKit

public func saveImageToPhotos(_ imagePath: String) -> Bool {
    // Validate file exists
    guard FileManager.default.fileExists(atPath: imagePath) else {
        print("Image file does not exist at path: \(imagePath)")
        return false
    }
    
    // Validate image can be loaded
    guard let image = UIImage(contentsOfFile: imagePath) else {
        print("Could not load image from path: \(imagePath)")
        return false
    }
    
    let url = URL(fileURLWithPath: imagePath)
    
    var success = false
    let semaphore = DispatchSemaphore(value: 0)
    
    // Ensure we're on the main thread
    if !Thread.isMainThread {
        var result = false
        DispatchQueue.main.sync {
            result = saveImageToPhotos(imagePath)
        }
        return result
    }
    
    let status = PHPhotoLibrary.authorizationStatus(for: .addOnly)
    
    switch status {
    case .notDetermined:
        PHPhotoLibrary.requestAuthorization(for: .addOnly) { newStatus in
            if newStatus == .authorized {
                saveToPhotos()
            } else {
                print("Photo library access denied")
                semaphore.signal()
            }
        }
    case .authorized:
        saveToPhotos()
    case .denied:
        print("Photo library access denied")
        semaphore.signal()
        return false
    case .restricted:
        print("Photo library access restricted")
        semaphore.signal()
        return false
    default:
        print("Photo library access not available")
        semaphore.signal()
        return false
    }
    
    semaphore.wait()
    return success
    
    func saveToPhotos() {
        PHPhotoLibrary.shared().performChanges({
            let request = PHAssetChangeRequest.creationRequestForAsset(from: image)
            request.creationDate = Date()
        }) { savedSuccess, error in
            defer {
                semaphore.signal()
            }
            
            if savedSuccess {
                success = true
            } else {
                if let error = error {
                    if let underlying = (error as NSError).userInfo[NSUnderlyingErrorKey] as? Error {
                        print("Underlying error: \(underlying)")
                    }
                } else {
                    print("Unknown error saving photo")
                }
            }
        }
    }
}
