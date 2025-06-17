#include <iostream>
#include <string>
#include <mcpClient.hpp>

int main(int argc, char* argv[]){
    // TODO figure out input these later
    std::string modelPath = "./external/models/DeepSeek-R1-0528-Qwen3-8B-UD-Q4_K_XL.gguf", sysPrompt = "You are a helpful assistant.";
    int gpuLayers = 37;

    // Shitty approach: TODO make this scalable later (design the cli args too)
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [-i | -k | -s]\n";
        return 1;
    }

    std::string command = argv[1];
    MCPClient cli;
    if(command == "-i") cli.startServer(modelPath, gpuLayers, sysPrompt);
    else if(command == "-k") cli.killServer();
    else if(command == "-s") {
        if(cli.detectServerRunning()) std::cout << "Status: Server is currently running" << std::endl;
        else std::cout << "Status: Server is not running" << std::endl;
    }

    return 0; 
   
     
}