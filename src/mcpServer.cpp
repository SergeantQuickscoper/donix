#include <iostream>
#include "mcpServer.hpp"
#include <string>

MCPServer::MCPServer(std::string modelPath, int gpuLayers, std::string sysPrompt){
    // Setting the priv vars
    std::cout << "\nInitializing model parameters...." << std::endl;
    this->modelPath = modelPath;
    std::cout << "Model path used: " << modelPath << std::endl;
    this->gpuLayers = gpuLayers;
    std::cout << "Layers used: " << gpuLayers << std::endl;
    this->sysPrompt = sysPrompt;
    std::cout << "System prompt: " << sysPrompt << std::endl;

    // TODO: maybe put some server parameters here
}

void MCPServer::chatPrompt(std::string prompt){
    // Let me see how the build progresses for now
    std::cout << "You prompted: " << prompt << std::endl; 
}