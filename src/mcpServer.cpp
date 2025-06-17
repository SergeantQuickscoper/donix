#include <iostream>
#include <fstream>
#include <filesystem>
#include "mcpServer.hpp"
#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/system.hpp>
#include <boost/process/v2/posix/fork_and_forget_launcher.hpp>

MCPServer::MCPServer(std::string modelPath, int gpuLayers, std::string sysPrompt){
    // Setting the priv vars
    std::cout << "\nInitializing model parameters...." << std::endl;
    this->modelPath = modelPath;
    std::cout << "Model path used: " << modelPath << std::endl;
    this->gpuLayers = gpuLayers;
    std::cout << "Layers used: " << gpuLayers << std::endl;
    this->sysPrompt = sysPrompt;
    std::cout << "System prompt: " << sysPrompt << std::endl;

    // Creating application directories and filepaths
    if(!std::filesystem::exists("/tmp/donix")) std::filesystem::create_directory("/tmp/donix");
    std::filesystem::path pidPath = "/tmp/donix/mcpserver.pid";
    std::filesystem::path socketPath = "/tmp/donix/mcpserver.sock";
    
    // Starting the llama.cpp subprocess
    boost::asio::io_context context;
    boost::asio::readable_pipe outputPipe{context}; 
    boost::asio::readable_pipe errorPipe{context};

    std::cout << "Starting the MCP Server...." << std::endl;
    boost::process::v2::process llmProcess(
            context,
            "./external/llama.cpp/bin/llama-cli", {
            "-m", this->modelPath, 
            "--jinja", 
            "--prompt", "Hello, Please introduce yourself in short.", 
            "--gpu-layers", std::to_string(gpuLayers), 
            "-sys", this->sysPrompt},
            boost::process::v2::process_stdio{{}, outputPipe, errorPipe}
        );
    
    std::cout << "Server Started. Awaiting requests...." << std::endl;
    std::cout << llmProcess.running() << " " << llmProcess.id() << std::endl;
    
    sleep(5);
    std::cout << "Writing PID" << std::endl;

    pid_t serverPID = llmProcess.id();
    std::ofstream write(pidPath);
    write << serverPID;
    write.close();

    // Save a socket file for the client to acess


    // Detach the process
    std::cout << "Detaching Process...." << std::endl;
    llmProcess.detach();

    // TODO: maybe put some server parameters here
}
