#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

rm -rf external
mkdir external
cd external
git clone https://github.com/ggerganov/llama.cpp.git

