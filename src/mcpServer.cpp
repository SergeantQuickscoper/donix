#include <iostream>
#include <fstream>
#include <filesystem>
#include <pty.h>
#include <mcpServer.hpp>
#include <sys/socket.h>
#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/system.hpp>
#include <boost/process/v2/posix/fork_and_forget_launcher.hpp>

#define bufferSize 128

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
    if (pid > 0) return;         // Parent exits

    if (setsid() < 0) std::exit(1);  // Become session leader (create a new session for the child)

    // This is a second fork after the first 
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
    
    int ptyMasterFd; // master (server process)
    pid_t childPid = forkpty(&ptyMasterFd, nullptr, nullptr, nullptr); // llama.cpp pty process
    if (childPid < 0) {
        perror("forkpty failed");
        exit(1);
    }

    int logFd = open("/tmp/donix/mcpserver.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    // Temporarly setting stdout as the logfile
    dup2(logFd, STDOUT_FILENO);
    dup2(logFd, STDERR_FILENO);
    close(logFd);

    // Register Signal Handlers and clean up function.
    atexit(onExit);
    std::signal(SIGINT, signalHandler);   // Ctrl+C
    std::signal(SIGTERM, signalHandler);  // kill
    std::signal(SIGHUP, signalHandler);   // terminal closed / hangup

    // llama.cpp terminal process
    if (childPid == 0) {
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

    // TODO: save the master process instead of this
    pid_t serverPID = childPid;
    std::ofstream write(pidPath);
    write << serverPID;
    write.close();

    // Server Socket setup
    //
    std::filesystem::remove(socketPath);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path) - 1);

    // Socket Sys Call
    int socketFD = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socketFD < 0){
        perror("socket call failed");
        exit(1);
    }
    int bindRes = bind(socketFD, (struct sockaddr *) &addr, sizeof(sockaddr_un));
    if(bindRes < 0){
        perror("bind call failed");
        exit(1);
    }
    
    int listenRes = listen(socketFD, 2); // 2 connections backlog but needs experimentation
    if(listenRes < 0){
        perror("listen call failed");
        exit(1);
    }

    while(true){
        std::cout << "Waiting to recieve connection..." << std::endl;
        
        int connectionFD = accept(socketFD, nullptr, nullptr);
        std::cout << "Socket connection accepted on: " << connectionFD << std::endl;

        // implement client connection and streaming here (buffer async??)
    }

    exit(0);
}
