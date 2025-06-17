#ifndef MCP_CLIENT
#define MCP_CLIENT

#include <string>
#include <mcpServer.hpp>
#include <filesystem>

class MCPClient{
    public:
        /*
        @returns 1 if server is live, else 0
        */
        int detectServerRunning();

        /*
        @returns 1 if success, else 0
        */
        int startServer(std::string modelPath, int gpuLayers, std::string sysPrompt);

        /*
        @returns 1 if success, else 0
        */
        int killServer();

        /*
        @brief prompts the server in chat and streams response from within the function.
        @returns void
        */
        void chatPrompt(std::string);
    private:
        std::filesystem::path baseDir = "/tmp/donix";

};






#endif