
#include <SFML/Graphics.hpp>
#include <vector>
#include "windowBase.hpp"
#include "client/buttons/button.hpp"
#include "client/buttons/buttonType.hpp"
#include "client/definitions/uiConfig.hpp"
#include "descriptors/socket.hpp"

using namespace std;


WindowBase::WindowBase(vector <Button*> buttons) {
    this -> buttons = buttons;
    this -> ALIVE = true;
}

WindowBase::~WindowBase() {
    for(Button* button : this -> buttons)
        delete button;
    this -> buttons.clear();
}

void WindowBase::run(sf::RenderWindow *window, sf::Font *font, UIConfig& config, Socket *clientFD) {
    while(window -> isOpen() && this -> ALIVE) {
        pollEvents(window, config, clientFD);
        buttonsMechanics(window, font, config, clientFD);
        resizeMechanics(window, config);
        updateWindow(window, font, config);
    }
}


void WindowBase::resizeMechanics(sf::RenderWindow *window, UIConfig& config) {
    sf::View view = window -> getView();
    bool correctWidth = true;
    bool correctHeight = true;
    if(window -> getSize().x < config.MINSCR_WIDTH)
        correctWidth = false;
    if(window -> getSize().y < config.MINSCR_HEIGHT)
        correctHeight = false;
    if(!config.oneTimeResize && (config.sizeScreen || (correctWidth && correctHeight))) {
        if(!correctWidth && !correctHeight) {
            window -> setSize(sf::Vector2u(config.MINSCR_WIDTH, config.MINSCR_HEIGHT));
            config.sizeScreen = false;
        }
        else if(!correctWidth) {
            window -> setSize(sf::Vector2u(config.MINSCR_WIDTH, window -> getSize().y));
            config.sizeScreen = false;
        }
        else if(!correctHeight) {
            window -> setSize(sf::Vector2u(window -> getSize().x, config.MINSCR_HEIGHT));
            config.sizeScreen = false;
        }
        view = window -> getView();
        config.SCREEN_WIDTH = window -> getSize().x;
        config.SCREEN_HEIGHT = window -> getSize().y;
        config.CODE_WIDTH = config.SCREEN_WIDTH-MARGIN*2;
        config.CODE_HEIGHT = config.SCREEN_HEIGHT-MARGIN*2;
        config.LIMIT_COLUMN_CODE = (config.CODE_WIDTH-config.CODEEDIT_MARGIN_WIDTH*2)/BLOCK_CODE_WIDTH-1;
        config.LIMIT_LINE_CODE = (config.CODE_HEIGHT-config.CODEEDIT_MARGIN_HEIGHT*2)/BLOCK_CODE_HEIGHT;
        view.setSize(config.SCREEN_WIDTH, config.SCREEN_HEIGHT);
        view.setCenter(config.SCREEN_WIDTH/2, config.SCREEN_HEIGHT/2);
    }
    window -> setView(view);
}

