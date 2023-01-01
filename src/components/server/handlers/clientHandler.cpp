
#include <thread>
#include <string>
#include <cstring>
#include "clientHandler.hpp"
#include "descriptors/socket.hpp"
#include "server/supervisor.hpp"
#include "utils/logs.hpp"
#define ever ;;

using namespace logger;
using namespace std;

static const char *tag = "ClientHandler";


ClientHandler::ClientHandler(int clientID, Socket *clientFD, Supervisor *supervisor) : ClientHandlerBase(clientID, clientFD) {
    this -> supervisor = supervisor;
}

ClientHandler::~ClientHandler() {
}

void ClientHandler::start() {
    this -> th = thread(&ClientHandler::handler, this);
}

void ClientHandler::handler() {
    logs(tag) << "Client handler Started";

    for(ever) {
        int typeCommand = -1;
        if(this -> clientFD -> recv(&typeCommand, sizeof(int)) == 0)
            break;
        if(typeCommand == -2) {
            int command = -1;

            this -> clientFD -> recv(&command, sizeof(int));

            if(command == 1) { // get files
                char *msg = this -> supervisor -> getFilesAvailable();
                int lenMsg = strlen(msg);
                this -> clientFD -> send(&lenMsg, sizeof(int));
                if(lenMsg > 0)
                    this -> clientFD -> send(msg, sizeof(char)*lenMsg);
                delete msg;
            }
            else if(command == 2) { // create file
                char *fileName = nullptr;
                int lenMsg = -1;
                this -> clientFD -> recv(&lenMsg, sizeof(int));
                fileName = new char[lenMsg+1];
                this -> clientFD -> recv(fileName, sizeof(char)*lenMsg);
                fileName[lenMsg] = '\0';

                this -> supervisor -> createFile(fileName);
                delete fileName;
            }
            else if(command == 3) { // create session (get file and make session)
                char *fileName = nullptr;
                int lenMsg = -1;
                this -> clientFD -> recv(&lenMsg, sizeof(int));
                fileName = new char[lenMsg+1];
                this -> clientFD -> recv(fileName, sizeof(char)*lenMsg);
                fileName[lenMsg] = '\0';

                string data = this -> supervisor -> getContentFile(fileName);

                string sessionID = fileName;
                delete fileName;

                int lenData = data.size();
                this -> clientFD -> send(&lenData, sizeof(int));
                if(lenData > 0)
                    this -> clientFD -> send(data.c_str(), sizeof(char)*lenData);


                setSessionID(sessionID);
            }
            else if(command == 4) { // exit session
                clearSessionID();
            }
            else if(command == 5) {
                char *fileName = nullptr;
                int lenMsg = -1;
                this -> clientFD -> recv(&lenMsg, sizeof(int));
                fileName = new char[lenMsg+1];
                this -> clientFD -> recv(fileName, sizeof(char)*lenMsg);
                fileName[lenMsg] = '\0';

                int lenData = -1;
                this -> clientFD -> recv(&lenData, sizeof(int));
                string data;
                if(lenData > 0) {
                    int remainData = lenData,
                        recvBytes = 0;
                    char buffer[BUFSIZ+1];
                    while(remainData > 0 && (recvBytes = this -> clientFD -> recv(buffer, sizeof(char)*BUFSIZ)) > 0) {
                        buffer[recvBytes] = '\0';
                        data += buffer;
                        remainData -= recvBytes;
                    }
                    this -> supervisor -> setContentFile(fileName, data);
                }
                delete fileName;
            }
        }
        else if(typeCommand == -3) {
            int line = -1, column = -1;
            char character = (char)-1;

            this -> clientFD -> recv(&line, sizeof(int));
            this -> clientFD -> recv(&column, sizeof(int));
            this -> clientFD -> recv(&character, sizeof(char));

            if(line != -1 && column != -1 && character != -1)
                this -> supervisor -> sendSession(line, column, character, getSessionID(), getClientID());
        }
    }

    clearSessionID();
    logs(tag) << "Client handler Finished";
}

