#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

echo "Creating build directory..."

rm -rf build

echo "Copying external dependencies...."

mkdir build && mkdir build/external && mkdir build/external/llama.cpp

# This assumes you've built llama.cpp and have your models saved in the respective external directories
cp -r external/llama.cpp/build/bin/ build/external/llama.cpp 
cp -r external/models/ build/external/models

cd build

echo "Building source..."

cmake ..

make

./donix