#ifndef __UI_WINDOW_BASE_H
#define __UI_WINDOW_BASE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "client/buttons/button.hpp"
#include "client/definitions/uiConfig.hpp"
#include "descriptors/socket.hpp"


class WindowBase {
    protected:
        bool ALIVE;
        std::vector <Button*> buttons;
    public:
        WindowBase(std::vector <Button*> buttons);
        virtual ~WindowBase();

        void run(sf::RenderWindow *window, sf::Font *font, UIConfig& config, Socket *clientFD);

    protected:
        virtual void pollEvents(sf::RenderWindow *window, UIConfig& config, Socket *clientFD)=0;
        virtual void buttonsMechanics(sf::RenderWindow *window, sf::Font *font, UIConfig& config, Socket *clientFD)=0;
        void resizeMechanics(sf::RenderWindow *window, UIConfig& config);
        virtual void updateWindow(sf::RenderWindow *window, sf::Font *font, UIConfig& config)=0;
        virtual void activateButton(sf::RenderWindow *window, Button *button, sf::Font *font, UIConfig& config, Socket *clientFD)=0;

        virtual void interfaceDraw(sf::RenderWindow *window, UIConfig& config)=0;
};

#endif
