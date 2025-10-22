#include <iostream>
#include "server.h"

int main(int argc, char* argv[]) {
    int port = 0;
    if (argc != 2) {
        std::cout << "Missing 1 argument to program: port\n";
        return 1;
    }
    if ((port = std::atoi(argv[1])) <= 0) {
        std::cout << "Provided argument is not a port: " << argv[1] << "\n";
        return 1;
    }
    std::cout << port << "\n";
    Server S(port);
    S.ListenAndServe();
}