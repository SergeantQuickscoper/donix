#include <iostream>
#include "mcpServer.hpp"

int main(){
    std::cout << "Hello World!";
    MCPServer test(10);
    test.printNumber();
    return 0;
}