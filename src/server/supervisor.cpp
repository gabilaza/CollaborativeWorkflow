
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include "descriptors/socket.hpp"
#include "server/handlers/base/clientHandlerBase.hpp"
#include "supervisor.hpp"
#include "utils/logs.hpp"

using namespace logger;
using namespace std;
using namespace std::__fs;

static const char *tag = "Supervisor";
static const string pathWorkDir = "work";


Supervisor::Supervisor() {
    this -> maxClientID = 1;
}

Supervisor::~Supervisor() {
    this -> clients.clear();
}

void Supervisor::addClient(ClientHandlerBase *client) {
    this -> clients.push_back(client);
}

int Supervisor::getNewClientID() {
    return (this -> maxClientID)++;
}

void Supervisor::sendSession(int line, int column, char character, std::string sessionID, int clientID) {
    logs(tag) << "sendSession() " << line << " " << column << " " << character << " " << sessionID;
    for(ClientHandlerBase* client : this -> clients) {
        //logs(tag) << "client:: " << client -> getSessionID() << ' ' << client -> getClientID();
        if(sessionID == client -> getSessionID() && clientID != client -> getClientID())
            client -> sendSession(line, column, character);
    }
}

char* Supervisor::getFilesAvailable() {
    logs(tag) << "getFilesAvailable()";
    const filesystem::path path(pathWorkDir);
    error_code ec;
    if(!filesystem::is_directory(path, ec))
        mkdir(pathWorkDir.c_str(), 0777);
    int lenMsg = 0;
    for(const auto& entry : filesystem::directory_iterator(path))
        if(!entry.is_directory(ec))
            lenMsg += strlen(entry.path().filename().string().c_str())+1;
    char *msg = new char[lenMsg+1];
    msg[0] = '\0';
    for(const auto& entry : filesystem::directory_iterator(path))
        if(!entry.is_directory(ec)) {
            strcat(msg, entry.path().filename().string().c_str());
            strcat(msg, "\n");
        }
    return msg;
}

void Supervisor::createFile(const char *fileName) {
    logs(tag) << "createFile() \"" << fileName << "\"";
    const filesystem::path path(pathWorkDir);
    error_code ec;
    if(!filesystem::is_directory(path, ec))
        mkdir(pathWorkDir.c_str(), 0777);
    ofstream file(pathWorkDir+"/"+fileName);
    file.close();
}

string Supervisor::getContentFile(const char *fileName) {
    logs(tag) << "getContentFile() \"" << fileName << "\"";
    ifstream file(pathWorkDir+"/"+fileName);
    string data;
    if(file) {
        ostringstream ss;
        ss << file.rdbuf();
        data = ss.str();
    }
    file.close();
    return data;
}

void Supervisor::setContentFile(const char *fileName, string data) {
    logs(tag) << "setContentFile() \"" << fileName << "\"";
    ofstream file(pathWorkDir+"/"+fileName);
    if(file)
        file << data;
    file.close();
}

