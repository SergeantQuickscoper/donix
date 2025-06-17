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
Basically use this for now:
##### This will be the simple chat command of the mcp server
`./llama.cpp/build/bin/llama-cli -m models/DeepSeek-R1-0528-Qwen3-8B-UD-Q4_K_XL.gguf --jinja --single-turn --prompt "tell me about mao zedong in one sentence" --gpu-layers 37 -sys "You are a helpful assistant."`

So if im not wrong as long as i have the llama.cpp binaries in my build, and the model file, everything should work out great. The copying is kind of scuffed but it should work for now, luckily have good disk space haha. 

Ok we are basically a llama.cpp wrapper for now but this seems promisingly useful.

### TODOs

- Prompt Engineering :nerd: (have it stop hallucinating, replying to itself, temperature controls etc.)
- Result Styling (make it look cooler and not just the raw output)
- Support for other endpoints (also standardize the chat endpoint)
- Run LLM or llama-cli as a daemon or in the background for quick inference
- Stream responses 
- have the app be a cli tool with arguments instead of using iostream funcs

Ok problem, seems like llama-cli doesnt have a setting for no chat history while the model is loaded so for context management, so ill have to make up an
arbitrary set of rules when it comes to context while maintaining a separate context manager
rules like:
if user clears context simply restart the cli and clear the context manager
if user switches to a different tool or cli config, the context manager cleanly transfers context between them.
have multiple profiles etc. etc.
This still feels super messy though so im gonna wait on implementing this.


For now though the goal is to turn llama.cpp into a background process (maybe using llama server??? but i feel like it would be too much). Gotta learn how to that in CPP first off. 

### Architecture

The Architecture of the project will contain 2 main components:

- The MCP Server 
- The MCP Client (cli tool)

#### MCP Server
The MCP Server process will contain the LLM inference engine (llama.cpp) and have the capability to communicate with Resources and Tools (will define these later) and accepts data in JSON (doing this cuz of the MCP Standard). It would be a detached process and when started will save its pid in a pid file so the client can have access to it. It will also be able to accept socket connections so would need to save a .sock file for access.

#### MCP Client 
The MCP Client will be run using the cli tool. Using the aforementioned pid file, it will be able to start or kill the detached MCP Server process. It contains arguments and flags that will generate JSON data to pass to the Server, and then asynchronously stream the response from the server. The context allotted will be managed by the client and will be sent with each request to the server, the reasoning being that i wanted to include profiles and separate contexts, which would be easier to do client side. Besides UDP should be able to handle it well so I am not worried. The context manager component will be a subpart of the client and will need to :

- Remember context across multiple profiles (use persistant file storage?)
- Clear or flush context for a profile
- Provide user friendly context control methods other than flushing, though i think flushing would control most use cases
- Somehow be aware when the LLM in the server restarts so as to redfine "active context"

Passive context refers to all the context of the current conversation of the current profile the user is in. Flushing will clear this context for that profile. Active context refers to the context actually sent to the LLM (information such as previous messages would not be here, while they would still be in the passive context). When the llm stops or restarts, the client should be made aware (possibily through a simple file write?) and have a one time push of the passive context instead of the active context. This seems to be the best approach for now, though clearly having some level of redundancy.


