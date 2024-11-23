const fs = require('fs');
const path = require('path');

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
  items.forEach(item => {
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
  });

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

// Path to xcframework
const xcframeworkPath = path.join(__dirname, '..', 'ios', 'opencv2.xcframework');

// Process both device and simulator frameworks
['ios-arm64', 'ios-arm64-simulator'].forEach(arch => {
  const frameworkPath = path.join(xcframeworkPath, arch, 'opencv2.framework');
  if (fs.existsSync(frameworkPath)) {
    console.log(`Creating symlinks for ${arch}...`);
    createSymlinks(frameworkPath);
  } else {
    console.error(`Framework not found at ${frameworkPath}`);
  }
});
