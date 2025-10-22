#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>

#include <thread>
#include <mutex>

class Server {
    public:
        Server(const int &_port);
        void ListenAndServe();
    private:
        void handleSocket(int socket);
        //static void writeLog(const );

        int msSocket {0};
        struct sockaddr_in sockAddress{};
        int port {0};
        std::mutex mutex{};
};