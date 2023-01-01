
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "windowHome.hpp"
#include "client/buttons/button.hpp"
#include "client/buttons/buttonType.hpp"
#include "client/definitions/uiConfig.hpp"
#include "client/definitions/point.hpp"
#include "client/shapes/rectangle.hpp"
#include "scripts/exec.hpp"
#include "client/windows/windowEditor.hpp"
#include "utils/logs.hpp"

using namespace logger;
using namespace std;
using namespace sf;

static const char *tag = "WindowHome";


WindowHome::WindowHome()
    : WindowBase({new Button({MARGIN, MARGIN/4}, {MARGIN+BLOCK_BUTTON_WIDTH, MARGIN*(3.0/4)}, EXIT, Color(28, 28, 28), Color(255, 0, 0), Color(0, 255, 0), "EXIT"),
                  new Button({MARGIN+BLOCK_BUTTON_WIDTH+20, MARGIN/4}, {MARGIN+BLOCK_BUTTON_WIDTH*2+20, MARGIN*(3.0/4)}, CREATE, Color(28, 28, 28), Color(255, 0, 0), Color(0, 255, 0), "CREATE"),
                  new Button({MARGIN+BLOCK_BUTTON_WIDTH*2+20*2, MARGIN/4}, {MARGIN+BLOCK_BUTTON_WIDTH*3+20*2, MARGIN*(3.0/4)}, LIST, Color(28, 28, 28), Color(255, 0, 0), Color(0, 255, 0), "LIST"),}) {
}

WindowHome::~WindowHome() {
    for(auto& it: this -> extraButtons)
        delete it.second;
    this -> extraButtons.clear();
}

void WindowHome::pollEvents(RenderWindow *window, UIConfig& config, Socket *clientFD) {
    bool resizedEvent = false;
    Event event;
    while(window -> pollEvent(event)) {
        if(event.type == Event::Closed)
            window -> close();

        if(event.type == Event::Resized)
            resizedEvent = true;
    }

    if(resizedEvent)
        config.oneTimeResize = false;
    if(config.sizeScreen) {
        config.oneTimeResize = true;
        config.sizeScreen = false;
    }
    if(!config.oneTimeResize && !resizedEvent && !Mouse::isButtonPressed(Mouse::Left))
        config.sizeScreen = true;
}

void WindowHome::updateWindow(RenderWindow *window, sf::Font *font, UIConfig& config) {
    window -> clear();

    interfaceDraw(window, config);

    for(Button *button : this -> buttons)
        button -> draw(window, font);

    for(auto& it: this -> extraButtons)
        it.second -> draw(window, font);

    window -> display();
}

void WindowHome::buttonsMechanics(sf::RenderWindow *window, sf::Font *font, UIConfig& config, Socket *clientFD) {
    for(Button *button : this -> buttons)
        if(button -> isPressed(window))
            activateButton(window, button, font, config, clientFD);
    for(auto& it: this -> extraButtons)
        if(it.second -> isPressed(window))
            activateButton(window, it.second, font, config, clientFD);
}

void WindowHome::activateButton(sf::RenderWindow *window, Button *button, sf::Font *font, UIConfig& config, Socket *clientFD) {
    buttonType typeButton = button -> getType();
    if(typeButton == EXIT) {
        logs(tag) << "EXIT button";
        window -> close();
    }
    else if(typeButton == CREATE) {
        logs(tag) << "CREATE button";
        string fileName = exec("./src/getStringWindow/getStringWindow");
        if(fileName.size() > 0) {
            logs(tag) << fileName;

            int typeCommand = -2, command = 2;
            clientFD -> send(&typeCommand, sizeof(int));
            clientFD -> send(&command, sizeof(int));
            int lenFileName = fileName.size();
            clientFD -> send(&lenFileName, sizeof(int));
            clientFD -> send(fileName.c_str(), sizeof(char)*lenFileName);

            // no reponse
        }

    }
    else if(typeButton == LIST) {
        logs(tag) << "LIST button";
        int typeCommand = -2, command = 1;
        clientFD -> send(&typeCommand, sizeof(int));
        clientFD -> send(&command, sizeof(int));

        char *msg = nullptr;
        int lenMsg = -1;
        clientFD -> recv(&lenMsg, sizeof(int));
        if(lenMsg > 0) {
            msg = new char[lenMsg+1];
            clientFD -> recv(msg, sizeof(char)*lenMsg);
            msg[lenMsg] = '\0';

            logs(tag) << '\n' << msg;

            string fileName;
            for(int i = 0; i < lenMsg; i++) {
                if(msg[i] == '\n') {
                    if(this -> extraButtons.find(fileName) == this -> extraButtons.end()) {
                        //TODO
                        float line = (int)(this -> extraButtons.size()/7);
                        float column = this -> extraButtons.size()%7;
                        this -> extraButtons[fileName] = new Button({MARGIN+column*BLOCK_BUTTON_WIDTH+column*20+10, (line+1)*MARGIN+10+MARGIN/4}, {MARGIN+(column+1)*BLOCK_BUTTON_WIDTH+column*20+10, (line+1)*MARGIN+10+MARGIN*(3.0f/4)}, OPEN, Color(28, 28, 28), Color(255, 0, 0), Color(0, 255, 0), fileName);
                    }
                    fileName.clear();
                }
                else
                    fileName += msg[i];
            }

            delete msg;
        }
    }
    else if(typeButton == OPEN) {
        logs(tag) << "OPEN button";

        int typeCommand = -2, command = 3;
        clientFD -> send(&typeCommand, sizeof(int));
        clientFD -> send(&command, sizeof(int));
        string fileName = button -> getString();
        int lenFileName = fileName.size();
        clientFD -> send(&lenFileName, sizeof(int));
        clientFD -> send(fileName.c_str(), sizeof(char)*lenFileName);


        int lenStr = -1;
        clientFD -> recv(&lenStr, sizeof(int));

        string contentFile;

        if(lenStr > 0) {
            int remainData = lenStr,
                recvBytes = 0;
            char buffer[BUFSIZ+1];
            while(remainData > 0 && (recvBytes = clientFD -> recv(buffer, sizeof(char)*BUFSIZ)) > 0) {
                buffer[recvBytes] = '\0';
                contentFile += buffer;
                remainData -= recvBytes;
            }
        }

        WindowEditor *windowEditor = new WindowEditor(button -> getString(), contentFile);
        logs(tag) << "Enter Editor: " << button -> getString();
        windowEditor -> run(window, font, config, clientFD);
        logs(tag) << "Exit Editor";
        delete windowEditor;


        typeCommand = -2, command = 4;
        clientFD -> send(&typeCommand, sizeof(int));
        clientFD -> send(&command, sizeof(int));

    }
}


void WindowHome::interfaceDraw(sf::RenderWindow *window, UIConfig& config) {
    Point topLeft, bottomRight;
    Color colorFill(28, 28,28), colorLine(255, 255, 255);

    topLeft = {0, 0};
    bottomRight = {config.SCREEN_WIDTH, config.SCREEN_HEIGHT};
    window -> draw(createRect(topLeft, bottomRight, colorFill, colorLine));


    topLeft = {MARGIN, MARGIN};
    bottomRight = {config.SCREEN_WIDTH-MARGIN, config.SCREEN_HEIGHT-MARGIN};
    colorFill = Color(0, 0, 0);
    colorLine = Color(255, 0, 0);
    window -> draw(createRect(topLeft, bottomRight, colorFill, colorLine));
}

