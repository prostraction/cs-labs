#include "server.h"

Server::Server(const int &_port) : port(_port) {
    this->msSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );
    this->sockAddress.sin_family = AF_INET;
    this->sockAddress.sin_port = htons(this->port);
    this->sockAddress.sin_addr.s_addr = htonl(INADDR_ANY);   // 0.0.0.0
}

void Server::handleSocket(int socket) {
    while (true) {
        std::cout << socket << "\n";
        char buf[128];
        std::string load;
        int bufLen {0};

        while (true) {
            bufLen = recv(socket, buf, 127, 0);
            load.append(buf, bufLen);
            if (bufLen < 127) {
                break;
            }
        }

        for (size_t i = 0; i < load.size(); i++) {
            if (load[i] == '\n') {
                load = load.substr(0, i);
            }
        }

        std::cout << load << "\n";
    
        this->mutex.lock();
        std::ofstream logFile;
        logFile.open("log.txt", std::ios::app);
        logFile << load << "\n";
        logFile.flush();
        logFile.close();
        this->mutex.unlock();

        //recv(socket, buf, 1024, 0);//MSG_NOSIGNAL);
        
        //printf("%s\n", buff);
    }
    shutdown(socket, SHUT_RDWR);
    close(socket);
    
}

void Server::ListenAndServe() {
    int err = 1;
    if (setsockopt(msSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&err, sizeof(err)) < 0) {
        fprintf(stderr, "setsockopt() failed. Error: %d\n", errno);
    }
    if ((err = bind(msSocket, (struct sockaddr*)(&sockAddress), sizeof(sockAddress))) < 0) {
        std::cout << "err: " << err << " " << errno << "\n";
    }
    std::cout << "Server called with port "<< port << ", socket = " << this->msSocket << "\n";
    // if
    listen(msSocket, SOMAXCONN);
    while (true) {
        int slSocket = accept(this->msSocket, nullptr, nullptr);
        std::cout << "socket = " << slSocket << "\n";
        std::thread(&Server::handleSocket, this, std::ref(slSocket)).detach();
        std::cout << "scope here\n";
    }
}