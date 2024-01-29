#ifndef __SUPERVISOR_H
#define __SUPERVISOR_H

#include <vector>
#include <string>
#include "server/handlers/base/clientHandlerBase.hpp"


class Supervisor {
    private:
        int maxClientID;
        std::vector <ClientHandlerBase* > clients;
    public:
        Supervisor();
        ~Supervisor();

        void addClient(ClientHandlerBase *client);
        int getNewClientID();

        void sendSession(int line, int column, char character, std::string sessionID, int clientID);

        char* getFilesAvailable();
        void createFile(const char *fileName);
        std::string getContentFile(const char *fileName);
        void setContentFile(const char *fileName, std::string data);
};

#endif
