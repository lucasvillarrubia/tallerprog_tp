#!/bin/bash

# Exit on any error
set -e

if [ "$EUID" -ne 0 ]; then 
    echo "Please run as root (sudo)"
    exit 1
fi

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Initial dependencies
echo "Installing initial dependencies..."
apt-get update
apt-get install -y \
    libjpeg-dev \
    libpng-dev \
    libfreetype-dev \
    libopusfile-dev \
    libflac-dev \
    libxmp-dev \
    libfluidsynth-dev \
    libwavpack-dev \
    cmake \
    libmodplug-dev \
    libsdl2-dev \
    build-essential \
    wget \
    qt6-base-dev \
    qt6-tools-dev

# Create temp directory
TEMP_DIR=$(mktemp -d)
cd "$TEMP_DIR"

# Function to download, build and install SDL libraries
install_sdl_lib() {
    local NAME=$1
    local VERSION=$2
    local URL=$3
    local DIR_NAME=$4

    echo "Installing $NAME $VERSION..."
    echo "Downloading from $URL..."
    wget "$URL" -O "$NAME.zip"
    
    echo "Extracting $NAME.zip..."
    unzip -o "$NAME.zip"
    
    echo "Entering directory $DIR_NAME..."
    cd "$DIR_NAME"
    
    echo "Building..."
    mkdir -p build
    cd build
    cmake ..
    make -j$(nproc)
    make install
    cd "$TEMP_DIR"
}

# Install SDL libraries
install_sdl_lib "SDL2_image" "2.6.3" "https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.3/SDL2_image-2.6.3.zip" "SDL2_image-2.6.3"
install_sdl_lib "SDL2_mixer" "2.6.3" "https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.6.3/SDL2_mixer-2.6.3.zip" "SDL2_mixer-2.6.3"
install_sdl_lib "SDL2_ttf" "2.20.2" "https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.2/SDL2_ttf-2.20.2.zip" "SDL2_ttf-2.20.2"

echo "Compiling the project..."
cd "$SCRIPT_DIR"
make compile-debug

# Run tests
echo "Running tests..."
make run-tests

# Cleanup
cd /
rm -rf "$TEMP_DIR"
ldconfig

echo "SDL libraries installed successfully!"
echo ""
echo "¡Seguí los otros pasos del manual de usuario!"