
#include <arpa/inet.h>
#include <cstring>
#include <thread>
#include "server.hpp"
#include "server/supervisor.hpp"
#include "utils/logs.hpp"
#define ever ;;

using namespace logger;

static const char *tag = "Server";


Server::Server(const char *host, const char *port) : Server(host, atoi(port)) {
}

Server::Server(const char *host, const int port) {
    logs(INFO) << "Server init";
    logs(tag) << "Supervisor init";
    this -> supervisor = new Supervisor();
    logs(tag) << "Address init";
    this -> address.sin_family = AF_INET;
    this -> address.sin_port = htons(port);

    if(inet_pton(AF_INET, host, &this -> address.sin_addr) <= 0)
        raiseError("Invalid address");

    logs(tag) << "Socket create";
    this -> serverFD = new Socket(AF_INET, SOCK_STREAM, 0);

    logs(tag) << "Socket bind";
    this -> serverFD -> bind((sockaddr*)&this -> address, sizeof(this -> address));

    logs(tag) << "Socket listen";
    this -> serverFD -> listen();
}

Server::~Server() {
    for(ClientHandler* client : this -> clients)
        delete client;
    this -> clients.clear();
    delete this -> serverFD;
    this -> serverFD = nullptr;
    delete this -> supervisor;
    this -> supervisor = nullptr;
}

void Server::run() {
    logs(INFO) << "Server Started";
    try {
        for(ever) {
            Socket *client = this -> serverFD -> accept();
            logs(tag) << "Client accepted";
            ClientHandler *clientH = new ClientHandler(this -> supervisor -> getNewClientID(), client, this -> supervisor);
            this -> clients.push_back(clientH);
            this -> supervisor -> addClient(clientH);
            logs(tag) << "Added a client";
            clientH -> start();
        }
    }
    catch (...) {
        logs(INFO) << "Server restart";
    }
    logs(INFO) << "Sever Finished";
}
