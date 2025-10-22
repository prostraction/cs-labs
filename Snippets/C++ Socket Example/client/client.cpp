#include "client.h"

Client::Client(const char* _name, int _port, int _timeout) : name(_name), port(_port), timeout(_timeout) {
    //std::cout << this->name << " " << this->port << " " << this->timeout << "\n";
    this->cSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );
    if (cSocket < 0) {
        /* */
    }
    sockAddress.sin_family = AF_INET;
    sockAddress.sin_port = htons(this->port);
    sockAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    //timeout_sec = _timeout;
}

int Client::SendRequests() {

    

    while (true) {
        /*int err = 1;
        if (setsockopt(this->cSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&err, sizeof(err)) < 0) {
            fprintf(stderr, "setsockopt() failed. Error: %d\n", errno);
        }
        if ((err = bind(this->cSocket, (struct sockaddr*)(&sockAddress), sizeof(sockAddress))) < 0) {
            std::cout << "err: " << err << " " << errno << "\n";
        }*/

        connect(this->cSocket, (struct sockaddr*)(&sockAddress), sizeof(sockAddress));

        std::string payLoad = [](const char* name){
            const auto clock = std::chrono::system_clock::now();
            const auto in_ctime = std::chrono::system_clock::to_time_t(clock);
            const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(clock.time_since_epoch()) % 1000;
            std::stringstream time_convert;
            time_convert << std::put_time(std::localtime(&in_ctime), "[%Y-%m-%d %X:") << std::setfill('0') << std::setw(3) << ms.count() << "] " << name << "\n";
            time_convert.flush();
            return time_convert.str();
        }(this->name);

        send(this->cSocket, payLoad.c_str(), payLoad.size(), MSG_NOSIGNAL);
        //recv(this->cSocket, 0, 0, MSG_NOSIGNAL);
        //shutdown(this->cSocket, SHUT_RDWR);
        //close(this->cSocket);
        //recv(this->cSocket, 0, 0, MSG_NOSIGNAL);

        /*
        shutdown(socket, SHUT_RDWR);
        close(socket);
        */

        std::this_thread::sleep_for(std::chrono::seconds(this->timeout));
        
        //std::string temp = ;
        std::cout << payLoad;
    }

    /*char buff[] = ...
    send(socket, buff, 4, MSG_NOSIGNAL);
    recv(socket, buff, 4, MSG_NOSIGNAL);
    shutdown(socket, SHUT_RDWR);
    close(socket);*/

    return 0;
}