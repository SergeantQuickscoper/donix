#include <iostream>
#include <string>
#include "mcpServer.hpp"

int main(){
    // TODO figure out input these later
    std::string modelPath = "./external/models/DeepSeek-R1-0528-Qwen3-8B-UD-Q4_K_XL.gguf", sysPrompt = "You are a helpful assistant.";
    int gpuLayers = 37;
    
    MCPServer mainServ(modelPath, gpuLayers, sysPrompt);
   
    std::string prompt;
    std::cout << "Enter a prompt: ";
    std::getline(std::cin, prompt);
    mainServ.chatPrompt(prompt); 
     
}