#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
//#include <stdio.h>

#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>

class Client{
    public:
        Client(const char* _name, int _port, int _timeout);
        int SendRequests();
    private:
        const char* name {""};
        int port {0};
        int timeout {0};

        int cSocket {0};
        struct sockaddr_in sockAddress {};
};