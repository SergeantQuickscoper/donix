#include <mcpClient.hpp>
#include <mcpServer.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <signal.h>
int MCPClient::detectServerRunning(){
    // very simplistic for now, open to edge cases so needs refining
    // i wonder if this could be enough if somehow crashes of the server
    // ensures that the pid file gets deleted
    return std::filesystem::exists(this->baseDir / "mcpserver.pid");
}

int MCPClient::startServer(std::string modelPath, int gpuLayers, std::string sysPrompt){
    if(this->detectServerRunning()) std::cout << "Server is already running." << std::endl;
    
    // i think this should be fine as the object is destroyed when the client dies
    MCPServer main(modelPath, gpuLayers, sysPrompt);
    return 1;
}

int MCPClient::killServer(){
    if(!this->detectServerRunning()){
        std::cout << "Server is not running." << std::endl;
        return 0;
    }
    // Get PID
    std::ifstream pidFile(this->baseDir / "mcpserver.pid");
    std::string pid_str;
    std::getline(pidFile, pid_str);
    pidFile.close();

    pid_t serverPID;

    try{
        serverPID = static_cast<pid_t>(std::stoi(pid_str));
    } catch (...) {
        std::cerr << "Invalid PID: " << pid_str << std::endl;
        return 0;
    }

    // Kill the server
    if(kill(serverPID, SIGTERM) != 0) {
        perror("Failed to kill server process");
        std::cout << std::endl;
        return 0;
    }

    std::cout << "Successfully killed server." << std::endl;;
    return 1;

}
