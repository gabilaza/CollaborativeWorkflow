#ifndef __UI_WINDOW_HOME_H
#define __UI_WINDOW_HOME_H

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "base/windowBase.hpp"
#include "client/buttons/button.hpp"
#include "client/definitions/uiConfig.hpp"
#include "descriptors/socket.hpp"


class WindowHome : public WindowBase {
    protected:
        std::unordered_map<std::string, Button*> extraButtons;
    public:
        WindowHome();
        ~WindowHome();

    protected:
        void pollEvents(sf::RenderWindow *window, UIConfig& config, Socket *clientFD);
        void buttonsMechanics(sf::RenderWindow *window, sf::Font *font, UIConfig& config, Socket *clientFD);
        void updateWindow(sf::RenderWindow *window, sf::Font *font, UIConfig& config);
        void activateButton(sf::RenderWindow *window, Button *button, sf::Font *font, UIConfig& config, Socket *clientFD);

        void interfaceDraw(sf::RenderWindow *window, UIConfig& config);
};

#endif
