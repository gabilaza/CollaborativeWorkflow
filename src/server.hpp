#ifndef __SERVER_H
#define __SERVER_H

#include <vector>
#include <netinet/in.h>
#include "descriptors/socket.hpp"
#include "server/supervisor.hpp"
#include "server/handlers/clientHandler.hpp"


class Server {
    private:
        Socket *serverFD;
        sockaddr_in address;
        Supervisor *supervisor;
        std::vector < ClientHandler* > clients;
    public:
        Server(const char *host, const char *port);
        Server(const char *host, const int port);
        ~Server();

        void run();
};

#endif
