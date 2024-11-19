#!/bin/bash

# Exit on any error
set -e

# Update package lists
sudo apt-get update

# Install required development libraries
echo "Installing required development libraries..."
sudo apt-get install -y \
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
    libsdl2-dev

# Create a directory for SDL libraries
mkdir -p ~/sdl-libraries
cd ~/sdl-libraries

# SDL Libraries to download
SDL_LIBS=(
    "https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-2.6.3.tar.gz"
    "https://github.com/libsdl-org/SDL_mixer/archive/refs/tags/release-2.6.3.tar.gz"
    "https://github.com/libsdl-org/SDL_ttf/archive/refs/tags/release-2.20.2.tar.gz"
)

# Download and install each SDL library
for lib in "${SDL_LIBS[@]}"; do
    # Extract filename from URL
    filename=$(basename "$lib")
    libname="${filename%.*}"
    
    # Download
    wget "$lib"
    tar -xzvf "$filename"
    
    # Compile and install
    cd "$libname"
    mkdir -p build
    cd build
    cmake ..
    make -j4
    sudo make install
    
    # Return to parent directory and clean up
    cd ../..
    rm "$filename"
done

# Install Qt6
echo "Installing Qt6..."
sudo apt-get install -y qt6-base-dev qt6-declarative-dev

echo "SDL and Qt6 installation completed successfully!"
