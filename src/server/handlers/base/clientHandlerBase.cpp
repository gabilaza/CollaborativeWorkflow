
#include <string>
#include "descriptors/socket.hpp"
#include "clientHandlerBase.hpp"
#include "utils/logs.hpp"

using namespace logger;
using namespace std;

static const char *tag = "ClientHandlerBase";


ClientHandlerBase::ClientHandlerBase(int clientID, Socket *clientFD) {
    this -> clientFD = clientFD;

    this -> sessionID.clear();
    this -> clientID = clientID;
}

ClientHandlerBase::~ClientHandlerBase() {
    if(this -> th.joinable())
        this -> th.join();
    delete this -> clientFD;
    this -> sessionID.clear();
}

string ClientHandlerBase::getSessionID() {
    return this -> sessionID;
}

int ClientHandlerBase::getClientID() {
    return this -> clientID;
}

void ClientHandlerBase::setSessionID(string sessionID) {
    this -> sessionID = sessionID;
}

void ClientHandlerBase::clearSessionID() {
    this -> sessionID.clear();
}

void ClientHandlerBase::sendSession(int line, int column, char character) {
    logs(tag) << "sendSession()";
    this -> clientFD -> send(&line, sizeof(int));
    this -> clientFD -> send(&column, sizeof(int));
    this -> clientFD -> send(&character, sizeof(char));
}

