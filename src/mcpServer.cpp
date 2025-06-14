#include <iostream>
#include "mcpServer.hpp"
#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/system.hpp>

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
    boost::asio::io_context context;
    boost::asio::readable_pipe outputPipe{context}; 
    boost::asio::readable_pipe errorPipe{context};

    std::cout << "Processing...." << std::endl;
    boost::process::v2::process llmProcess (
            context, 
            "./external/llama.cpp/bin/llama-cli", {
            "-m", this->modelPath, 
            "--jinja", 
            "--single-turn", 
            "--prompt", prompt, 
            "--gpu-layers", std::to_string(gpuLayers), 
            "-sys", this->sysPrompt},
            boost::process::v2::process_stdio{{}, outputPipe, errorPipe}
        );

    
    std::string rawResult;
    boost::system::error_code errorCode;
    boost::asio::read(outputPipe, boost::asio::dynamic_buffer(rawResult), errorCode);

    // This is error code might be redundant! 
    if(errorCode && errorCode != boost::asio::error::eof) {
        std::cout << "ERROR" << errorCode << std::endl;
        return;
    }
    llmProcess.wait();
    std::cout << "RAW DATA OUTPUT:\n" << rawResult << std::endl;
}