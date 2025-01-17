import Photos
import Foundation
import UIKit

public func saveImageToPhotos(_ imagePath: String) -> Bool {
    let cleanPath = imagePath.replacingOccurrences(of: "file://", with: "")
    // check if file exists
    guard FileManager.default.fileExists(atPath: cleanPath) else {
        print("Image file does not exist at path: \(cleanPath)")
        return false
    }
        
    var success = false
    let semaphore = DispatchSemaphore(value: 0)
    
    // make sure it's using the main thread
    if !Thread.isMainThread {
        var result = false
        DispatchQueue.main.sync {
            result = saveImageToPhotos(cleanPath)
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
    
    // save photos to library
    func saveToPhotos() {
        if let data = try? Data(contentsOf: URL(fileURLWithPath: cleanPath)) {
            PHPhotoLibrary.shared().performChanges({
                let request = PHAssetCreationRequest.forAsset()
                request.addResource(with: .photo, data: data, options: nil)
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
}
