
#include <SFML/Graphics.hpp>
#include <string>
#include <sys/ioctl.h>
#include "windowEditor.hpp"
#include "client/buttons/button.hpp"
#include "client/buttons/buttonType.hpp"
#include "client/definitions/uiConfig.hpp"
#include "client/definitions/point.hpp"
#include "client/shapes/rectangle.hpp"
#include "client/editor/editor.hpp"
#include "utils/logs.hpp"

using namespace logger;
using namespace std;
using namespace sf;

static const char *tag = "WindowEditor";


WindowEditor::WindowEditor(string fileName, string content)
    : WindowBase({new Button({MARGIN, MARGIN/4}, {MARGIN+BLOCK_BUTTON_WIDTH, MARGIN*(3.0/4)}, EXIT, Color(28, 28, 28), Color(255, 0, 0), Color(0, 255, 0), "EXIT"),
                  new Button({MARGIN+BLOCK_BUTTON_WIDTH+20, MARGIN/4}, {MARGIN+BLOCK_BUTTON_WIDTH*2+20, MARGIN*(3.0/4)}, HOME, Color(28, 28, 28), Color(255, 0, 0), Color(0, 255, 0), "HOME"),
                  new Button({MARGIN+BLOCK_BUTTON_WIDTH*2+20*2, MARGIN/4}, {MARGIN+BLOCK_BUTTON_WIDTH*3+20*2, MARGIN*(3.0/4)}, DOWNLOAD, Color(28, 28, 28), Color(255, 0, 0), Color(0, 255, 0), "DOWNLOAD"),
                  new Button({MARGIN+BLOCK_BUTTON_WIDTH*3+20*3, MARGIN/4}, {MARGIN+BLOCK_BUTTON_WIDTH*4+20*3, MARGIN*(3.0/4)}, SAVE, Color(28, 28, 28), Color(255, 0, 0), Color(0, 255, 0), "SAVE"),}) {
    this -> fileName = fileName;
    this -> editor = new Editor();
    this -> editor -> setContent(content.c_str());
}

WindowEditor::~WindowEditor() {
    delete this -> editor;
}

void WindowEditor::pollEvents(RenderWindow *window, UIConfig& config, Socket *clientFD) {

    int count = 0;
    ioctl(clientFD -> fd(), FIONREAD, &count);
    if(count > 0) {
        int line = -1, column = -1;
        char character = (char)-1;
        clientFD -> recv(&line, sizeof(int));
        clientFD -> recv(&column, sizeof(int));
        clientFD -> recv(&character, sizeof(char));
        Point pos;
        pos.y = line;
        pos.x = column;
        this -> editor -> insertCharPos(character, pos);
    }

    bool resizedEvent = false;
    Event event;
    while(window -> pollEvent(event)) {
        if(event.type == Event::Closed)
            window -> close();
        if(event.type == sf::Event::TextEntered) {

            Point cursorPos = this -> editor -> getCursorPos();
            int line = cursorPos.y,
                column = cursorPos.x;
            char character = static_cast<char>(event.text.unicode);
            int typeCommand = -3;
            clientFD -> send(&typeCommand, sizeof(int));

            clientFD -> send(&line, sizeof(int));
            clientFD -> send(&column, sizeof(int));
            clientFD -> send(&character, sizeof(char));

            this -> editor -> insertChar(static_cast<char>(event.text.unicode));

            //save
            typeCommand = -2;
            int command = 5;
            clientFD -> send(&typeCommand, sizeof(int));
            clientFD -> send(&command, sizeof(int));

            string fileName = this -> fileName;
            int lenFileName = fileName.size();
            clientFD -> send(&lenFileName, sizeof(int));
            clientFD -> send(fileName.c_str(), sizeof(char)*lenFileName);

            string str = this -> editor -> getContent();
            int lenStr = str.size();
            clientFD -> send(&lenStr, sizeof(int));
            if(lenStr > 0)
                clientFD -> send(str.c_str(), sizeof(char)*lenStr);
        }
        if(event.type == Event::KeyPressed) {
            if(event.key.code == Keyboard::Left)
                this -> editor -> moveCursor(1);
            if(event.key.code == Keyboard::Up)
                this -> editor -> moveCursor(2);
            if(event.key.code == Keyboard::Right)
                this -> editor -> moveCursor(3);
            if(event.key.code == Keyboard::Down)
                this -> editor -> moveCursor(4);
        }

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

void WindowEditor::updateWindow(RenderWindow *window, sf::Font *font, UIConfig& config) {
    window -> clear();

    interfaceDraw(window, config);

    for(Button *button : this -> buttons)
        button -> draw(window, font);

    this -> editor -> draw(window, font, config);

    window -> display();
}

void WindowEditor::buttonsMechanics(sf::RenderWindow *window, sf::Font *font, UIConfig& config, Socket *clientFD) {
    for(Button *button : this -> buttons)
        if(button -> isPressed(window))
            activateButton(window, button, font, config, clientFD);
}

void WindowEditor::activateButton(sf::RenderWindow *window, Button *button, sf::Font *font, UIConfig& config, Socket *clientFD) {
    buttonType typeButton = button -> getType();
    if(typeButton == EXIT) {
        logs(tag) << "EXIT button";
        window -> close();
    }
    if(typeButton == HOME) {
        logs(tag) << "HOME button";
        this -> ALIVE = false;
    }
    else if(typeButton == DOWNLOAD) {
        logs(tag) << "DOWNLOAD button";
        this -> editor -> putFile(this -> fileName.c_str());
    }
    else if(typeButton == SAVE) {
        logs(tag) << "SAVE button";
        int typeCommand = -2, command = 5;
        clientFD -> send(&typeCommand, sizeof(int));
        clientFD -> send(&command, sizeof(int));

        string fileName = this -> fileName;
        int lenFileName = fileName.size();
        clientFD -> send(&lenFileName, sizeof(int));
        clientFD -> send(fileName.c_str(), sizeof(char)*lenFileName);

        string str = this -> editor -> getContent();
        int lenStr = str.size();
        clientFD -> send(&lenStr, sizeof(int));
        if(lenStr > 0)
            clientFD -> send(str.c_str(), sizeof(char)*lenStr);
    }
}


void WindowEditor::interfaceDraw(sf::RenderWindow *window, UIConfig& config) {
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

