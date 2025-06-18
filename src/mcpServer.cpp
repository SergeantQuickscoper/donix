#include <iostream>
#include <fstream>
#include <filesystem>
#include <pty.h>
#include "mcpServer.hpp"
#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/system.hpp>
#include <boost/process/v2/posix/fork_and_forget_launcher.hpp>

void MCPServer::onExit(){
    std::ofstream log("/tmp/donix/mcpserver.log", std::ios::app);
    log << "Server shutting down.\n";
    std::filesystem::remove("/tmp/donix/mcpserver.pid");
}

void MCPServer::signalHandler(int signalCode){
    std::ofstream log("/tmp/donix/mcpserver.log", std::ios::app);
    log << "Server encountered signal: " << signalCode << std::endl;
    exit(0);
}

MCPServer::MCPServer(std::string modelPath, int gpuLayers, std::string sysPrompt){
    // Setting the priv vars
    std::cout << "\nInitializing model parameters...." << std::endl;
    this->modelPath = modelPath;
    std::cout << "Model path used: " << modelPath << std::endl;
    this->gpuLayers = gpuLayers;
    std::cout << "Layers used: " << gpuLayers << std::endl;
    this->sysPrompt = sysPrompt;
    std::cout << "System prompt: " << sysPrompt << std::endl;
   
    std::cout << "Starting the MCP Server...." << std::endl;

    pid_t pid = fork();
    if (pid < 0) std::exit(1);
    if (pid > 0) return;         // Parent exits, returns child PID

    if (setsid() < 0) std::exit(1);  // Become session leader (create a new session for the child)

    pid = fork();
    if (pid < 0) std::exit(1);
    if (pid > 0) std::exit(0);       // First child exits

    umask(0);
    
  
    // Clear inherited STD I/O
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    open("/dev/null", O_RDONLY); // stdin
    open("/dev/null", O_WRONLY); // stdout
    open("/dev/null", O_RDWR);   // stderr


    // Creating application directories and filepaths
    if(!std::filesystem::exists("/tmp/donix")) std::filesystem::create_directory("/tmp/donix");
    std::filesystem::path pidPath = "/tmp/donix/mcpserver.pid";
    std::filesystem::path socketPath = "/tmp/donix/mcpserver.sock";
    
    int pty_master_fd;
    pid_t child_pid = forkpty(&pty_master_fd, nullptr, nullptr, nullptr);
    if (child_pid < 0) {
        perror("forkpty failed");
        exit(1);
    }

    int log_fd = open("/tmp/donix/mcpserver.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    dup2(log_fd, STDOUT_FILENO);
    dup2(log_fd, STDERR_FILENO);
    close(log_fd);

    // Register Signal Handlers and clean up function.
    atexit(onExit);
    std::signal(SIGINT, signalHandler);   // Ctrl+C
    std::signal(SIGTERM, signalHandler);  // kill
    std::signal(SIGHUP, signalHandler);   // terminal closed / hangup

    if (child_pid == 0) {
        const char* program = "./external/llama.cpp/bin/llama-cli";
        std::string gpuStr = std::to_string(this->gpuLayers);

        const char* argv[] = {
            "llama-cli",
            "-m", this->modelPath.c_str(),
            "--jinja",
            "--prompt", "Hello, Please introduce yourself in short.",
            "--gpu-layers", gpuStr.c_str(),
            "-sys", this->sysPrompt.c_str(),
            nullptr
        };

        execvp(program, (char* const*)argv);
        perror("execvp failed");
        _exit(1);
    }
    
    
    std::cout << "Writing PID" << std::endl;

    pid_t serverPID = child_pid;
    std::ofstream write(pidPath);
    write << serverPID;
    write.close();

    // Save a socket file for the client to acesss and add blocking statements



    sleep(30);
    exit(0);
}
