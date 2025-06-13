#ifndef MCP_SERVER_HPP
#define MCP_SERVER_HPP

#include <string>

class MCPServer {
    public:
        MCPServer(std::string modelPath, int gpuLayers, std::string sysPrompt);
        void chatPrompt(std::string prompt);
    private:
        std::string modelPath;
        std::string sysPrompt;
        int gpuLayers;
        
};

#endif