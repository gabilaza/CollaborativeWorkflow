#ifndef __CLIENT_H
#define __CLIENT_H

#include <SFML/Graphics.hpp>
#include <netinet/in.h>
#include <unordered_map>
#include "descriptors/socket.hpp"
#include "client/definitions/uiConfig.hpp"
#include "client/windows/base/windowBase.hpp"


class Client {
    private:
        Socket *clientFD;
        sockaddr_in address;

        UIConfig config;
        sf::RenderWindow *window;
        sf::Font *font;

        WindowBase* windowHome;
    public:
        Client(const char *host, const char *port);
        Client(const char *host, const int port);
        ~Client();

        void run();
};

#endif
