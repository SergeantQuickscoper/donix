#ifndef MCP_SERVER_HPP
#define MCP_SERVER_HPP

#include <string>

class MCPServer {
    public:
        MCPServer(std::string modelPath, int gpuLayers, std::string sysPrompt);
        static void onExit();
        static void signalHandler(int signalCode);
    private:
        std::string modelPath;
        std::string sysPrompt;
        int gpuLayers;
        
};

#endif