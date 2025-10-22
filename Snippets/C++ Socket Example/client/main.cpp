#include <iostream>
#include "client.h"

int main(int argc, char* argv[]) {
    std::string textName {""};
    int port {0};
    int timeout {0};
    if (argc != 4) {
        std::cout << "Usage: program text_name port timeout\n";
        return 1;
    }
    textName = argv[1];
    if ((port = std::atoi(argv[2])) <= 0) {
        std::cout << "Provided argument is not a port: " << argv[2] << "\n";
        return 1;
    }
    if ((timeout = std::atoi(argv[3])) <= 0) {
        std::cout << "Provided argument is not a timeout: " << argv[3] << "\n";
        return 1;
    }
    
    Client C(textName.c_str(), port, timeout);
    C.SendRequests();
}