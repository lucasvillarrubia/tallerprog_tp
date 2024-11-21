#!/bin/bash

# Exit on any error
set -e

echo "Installing DuckGame dependencies..."

# Check if running as root
if [ "$EUID" -ne 0 ]; then 
    echo "Please run as root (sudo)"
    exit 1
fi

# Install dependencies
echo "Installing dependencies..."
apt-get update
apt-get install -y \
    build-essential \
    cmake \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev \
    qt6-base-dev \
    qt6-tools-dev

echo "Dependencies installed successfully!"
echo ""
echo "To build the project:"
echo "1. mkdir -p build && cd build"
echo "2. cmake .."
echo "3. make -j$(nproc)"
