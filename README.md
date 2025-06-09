# donix

This project will start out as an AI agent CLI tool for linux. I will attempt to write an MCP server in C++. I will initally use the latest DeepSeek R1 model as my LLM. Here's to learning as we go...

### MCP Server requirements
 
 1. Use a structured MCP format
 2. Prompt Building for the LLM
 3. Support a set of resources (lets start with CLI ig)
 4. parses LLM output an
 5. executes actions (commands for now)
 
 #### Dependency List (for now):

 1. llama.cpp (to run the model locally)
 2. nlohmann/json (for mcp formatting)
 3. unsloth/DeepSeek-R1-0528-Qwen3-8B-GGUF (LLM Model)


 #### Testing Models for my personal use:
 1.  unsloth/DeepSeek-R1-0528-Qwen3-8B-GGUF ([Q4 K_M](https://huggingface.co/unsloth/DeepSeek-R1-0528-Qwen3-8B-GGUF/resolve/main/DeepSeek-R1-0528-Qwen3-8B-Q4_K_M.gguf) and [Q4_UD_K_XL](https://huggingface.co/unsloth/DeepSeek-R1-0528-Qwen3-8B-GGUF/resolve/main/DeepSeek-R1-0528-Qwen3-8B-UD-Q4_K_XL.gguf) versions)
 2.  unsloth/DeepSeek-R1-Distill-Qwen-7B-GGUF ([Q4_K_M](https://huggingface.co/unsloth/DeepSeek-R1-Distill-Qwen-7B-GGUF/resolve/main/DeepSeek-R1-Distill-Qwen-7B-Q4_K_M.gguf))


### Phase 1
Building an MCP server that recieves a prompt through the CLI and returns the LLM response.

I have no idea what im doing rn but ill figure this out.
