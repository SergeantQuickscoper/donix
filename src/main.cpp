#include <iostream>
#include <string>
#include "mcpServer.hpp"

int main(){
    MCPServer mainServ("./this/is/a/test", 37, "You are a helpful assistant");
    std::string prompt;
    std::cout << "Enter a prompt: ";
    std::cin >> prompt;
    mainServ.chatPrompt(prompt); 
}