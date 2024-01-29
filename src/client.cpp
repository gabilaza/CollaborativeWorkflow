
#include <SFML/Graphics.hpp>
#include <arpa/inet.h>
#include "client.hpp"
#include "client/definitions/uiConfig.hpp"
#include "client/windows/base/windowBase.hpp"
#include "client/windows/windowHome.hpp"
#include "client/windows/windowEditor.hpp"
#include "utils/logs.hpp"

using namespace logger;
using namespace std;
using namespace sf;

static const char *tag = "Client";


Client::Client(const char *host, const char *port) : Client(host, atoi(port)) {
}

Client::Client(const char *host, const int port) {
    logs(INFO) << "Client init";
    logs(tag) << "Address init";
    this -> address.sin_family = AF_INET;
    this -> address.sin_port = htons(port);

    if(inet_pton(AF_INET, host, &this -> address.sin_addr) <= 0)
        raiseError("Invalid address");

    logs(tag) << "Socket create";
    this -> clientFD = new Socket(AF_INET, SOCK_STREAM, 0);

    logs(tag) << "Socket connect";
    this -> clientFD -> connect(this -> address);
    logs(INFO) << "Client connected";


    logs(tag) << "UI init";
    this -> window = new RenderWindow(VideoMode(this -> config.SCREEN_WIDTH, this -> config.SCREEN_HEIGHT), "CollaborativeWorkflow");
    logs(tag) << "UI Font init";
    this -> font = new Font;
    if(!this ->font -> loadFromFile("./assets/font.ttf"))
        raiseError("Not found file");
    logs(tag) << "UI Create windows";
    this -> windowHome = new WindowHome();
}

Client::~Client() {
    if(this -> window -> isOpen())
        this -> window -> close();
    delete this -> window;
    delete this -> windowHome;
    delete this -> clientFD;
    this -> clientFD = nullptr;
}

void Client::run() {
    logs(INFO) << "Client Started";

    this -> windowHome -> run(this -> window, this -> font, this -> config, this -> clientFD);

    logs(INFO) << "Client Finished";
}

