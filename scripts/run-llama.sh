# !bin/bash 

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."
cd external

# This is for my system using the the default model specified in the README
./llama.cpp/build/bin/llama-cli -m models/DeepSeek-R1-0528-Qwen3-8B-UD-Q4_K_XL.gguf --gpu-layers 37

# Storing this for the future:
# This will be the simple chat command of the mcp server
# ./llama.cpp/build/bin/llama-cli -m models/DeepSeek-R1-0528-Qwen3-8B-UD-Q4_K_XL.gguf --jinja --single-turn --prompt "tell me about mao zedong in one sentence" --gpu-layers 37 -sys "You are a helpful assistant."
