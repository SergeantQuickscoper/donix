#include "mcpServer.hpp"
#include <iostream>

MCPServer::MCPServer(int number){
    this->num = number;
}

void MCPServer::printNumber(){
    std::cout << "THE NUMBER IS: " << this->num;
}