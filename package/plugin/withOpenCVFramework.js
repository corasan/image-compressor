const { withDangerousMod } = require('@expo/config-plugins');
const fs = require('fs');
const path = require('path');

function findPackagePath(startPath, packageName) {
  // Possible node_modules locations to check
  const possiblePaths = [
    // Standard node_modules
    path.join(startPath, '..', 'node_modules', packageName),
    // Monorepo root node_modules
    path.join(startPath, '..', '..', 'node_modules', packageName),
  ];

  // Find the first path that exists
  const foundPath = possiblePaths.find(p => fs.existsSync(p));

  if (!foundPath) {
    throw new Error(`Could not find package ${packageName} in node_modules. Searched paths:\n${possiblePaths.join('\n')}`);
  }

  return foundPath;
}

function createSymlinks(frameworkPath) {
  const versionsPath = path.join(frameworkPath, 'Versions');
  const currentPath = path.join(versionsPath, 'A');

  // Create directories if they don't exist
  if (!fs.existsSync(versionsPath)) {
    fs.mkdirSync(versionsPath);
  }
  if (!fs.existsSync(currentPath)) {
    fs.mkdirSync(currentPath);
  }

  // Move Headers and binary to Versions/A if they exist at root
  const items = ['Headers', 'opencv2'];
  for (item of items) {
    const rootItem = path.join(frameworkPath, item);
    const versionedItem = path.join(currentPath, item);

    if (fs.existsSync(rootItem) && !fs.existsSync(versionedItem)) {
      // Move the item to Versions/A
      fs.renameSync(rootItem, versionedItem);
    }

    // Create symlink in root if it doesn't exist
    if (!fs.existsSync(rootItem)) {
      try {
        fs.symlinkSync(
          path.join('Versions', 'A', item),
          rootItem,
          'dir'
        );
      } catch (error) {
        console.error(`Failed to create symlink for ${item}:`, error);
      }
    }
  }

  // Create "Current" symlink in Versions if it doesn't exist
  const currentSymlink = path.join(versionsPath, 'Current');
  if (!fs.existsSync(currentSymlink)) {
    try {
      fs.symlinkSync('A', currentSymlink, 'dir');
    } catch (error) {
      console.error('Failed to create Current symlink:', error);
    }
  }
}

const archs = ['ios-arm64', 'ios-arm64_x86_64-simulator'];

const withOpenCVFramework = (config) => {
  return withDangerousMod(config, [
    'ios',
    async (config) => {
      try {
        const packageName = '@corasan/image-compressor';
        const packagePath = findPackagePath(config.modRequest.platformProjectRoot, packageName);
        console.log(`Found package at: ${packagePath}`);

        const xcframeworkPath = path.join(packagePath, 'ios', 'opencv2.xcframework');

        if (!fs.existsSync(xcframeworkPath)) {
          throw new Error(`opencv2.xcframework not found at ${xcframeworkPath}`);
        }

        // Process both device and simulator frameworks
        for (const arch of archs) {
          const frameworkPath = path.join(xcframeworkPath, arch, 'opencv2.framework');
          if (fs.existsSync(frameworkPath)) {
            console.log(`Creating opencv2 symlinks for ${arch}...`);
            createSymlinks(frameworkPath);
          } else {
            console.error(`Framework not found at ${frameworkPath}`);
          }
        }
      } catch (error) {
        console.error('Error in withOpenCVFramework:', error);
        throw error;
      }

      return config;
    },
  ]);
};

module.exports = withOpenCVFramework;
