#ifndef __CLIENT_HANDLER_BASE_H
#define __CLIENT_HANDLER_BASE_H

#include <thread>
#include <string>
#include "descriptors/socket.hpp"


class ClientHandlerBase {
    protected:
        Socket *clientFD;

        std::thread th;
        int clientID;
        std::string sessionID;
    public:
        ClientHandlerBase(int clientID, Socket *clientFD);
        ~ClientHandlerBase();

        std::string getSessionID();
        int getClientID();
        void setSessionID(std::string sessionID);
        void clearSessionID();

        void sendSession(int line, int column, char character);

        virtual void start()=0;
    private:
        virtual void handler()=0;
};

#endif
