#ifndef __CLIENT_HANDLER_H
#define __CLIENT_HANDLER_H

#include "server/handlers/base/clientHandlerBase.hpp"
#include "descriptors/socket.hpp"
#include "server/supervisor.hpp"


class ClientHandler : public ClientHandlerBase {
    private:
        Supervisor *supervisor;
    public:
        ClientHandler(int clientID, Socket *clientFD, Supervisor *supervisor);
        ~ClientHandler();

        void start();
    private:
        void handler();
};

#endif
