#!/bin/bash

# Define the pattern to search for the ZIP file and the destination directory
PATTERN="zoom-meeting-sdk-linux*.tar.xz"
DEST_DIR="./zoom-sdk-temp/"

# Check if the destination directory exists, if not, create it
if [ ! -d "$DEST_DIR" ]; then
    mkdir -p "$DEST_DIR"
fi

# Find the file matching the pattern
ZIP_FILE=$(find . -name "$PATTERN" -print -quit)

# Check if the file was found
if [ -z "$ZIP_FILE" ]; then
    echo "No file matching the pattern '$PATTERN' found."
    exit 1
fi

# Unzip the file
tar -xvf "$ZIP_FILE" -C "$DEST_DIR"

# Create include and qt_libs directories in the rzbcpp_copy directory two levels up
PARENT_DIR="../../rzbcpp_copy"
INCLUDE_DIR="$PARENT_DIR/include"
QT_LIBS_DIR="$PARENT_DIR/lib/qt_libs"
LIBS_DIR="$PARENT_DIR/lib"

mkdir -p "$INCLUDE_DIR"
mkdir -p "$QT_LIBS_DIR"

# Move the h/ folder to rzbcpp_copy/include
if [ -d "$DEST_DIR/h" ]; then
    mv "$DEST_DIR/h"/* "$INCLUDE_DIR"
fi

# Move the qt_libs/ folder to rzbcpp_copy/qt_libs
if [ -d "$DEST_DIR/qt_libs" ]; then
    mv "$DEST_DIR/qt_libs"/* "$QT_LIBS_DIR"
fi

# Move all .so files to rzbcpp_copy/qt_libs
find "$DEST_DIR" -name "*.so" -exec mv {} "$LIBS_DIR" \;

# Delete the zoom-sdk-temp directory
rm -rf "$DEST_DIR"

echo "Files have been moved successfully and the temporary directory has been deleted."