import Foundation
import UniformTypeIdentifiers
import ImageIO
import CoreGraphics

public class ImageIOCompressor {
    public static func hello() -> String {
        return "Hello, World!"
    }
    public static func compress(path: String, quality: Double) -> String {
        return "Hello, World!"
//        guard let sourceURL = URL(string: path) else {
//            throw ImageError.invalidPath
//        }
//        
//        guard let source = CGImageSourceCreateWithURL(sourceURL as CFURL, nil) else {
//            throw ImageError.cannotCreateSource
//        }
//        
//        let outputPath = sourceURL.deletingPathExtension().path + "_compressed.jpg"
//        let outputURL = URL(fileURLWithPath: outputPath)
//        
//        let destProperties = [
//            kCGImageDestinationLossyCompressionQuality: quality,
//            kCGImageDestinationOptimizeColorForSharing: true
//        ] as [CFString : Any]
//        
//        guard let destination = CGImageDestinationCreateWithURL(outputURL as CFURL,
//                                                                UTType.jpeg as! CFString,
//                                                                1, nil) else {
//            throw ImageError.cannotCreateDestination
//        }
//        
//        CGImageDestinationSetProperties(destination, destProperties as CFDictionary)
//        
////        if !CGImageDestinationAddImageFromSource(destination, source, 0, destProperties as CFDictionary) {
////            throw ImageError.cannotAddImage
////        }
//        
//        if !CGImageDestinationFinalize(destination) {
//            throw ImageError.cannotFinalizeImage
//        }
//        
//        return outputPath
    }
}

enum ImageError: Error {
    case invalidPath
    case cannotCreateSource
    case cannotCreateDestination
    case cannotAddImage
    case cannotFinalizeImage
}
