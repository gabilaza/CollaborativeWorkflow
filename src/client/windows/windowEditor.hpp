#ifndef __UI_WINDOW_EDITOR_H
#define __UI_WINDOW_EDITOR_H

#include <SFML/Graphics.hpp>
#include <string>
#include "base/windowBase.hpp"
#include "client/buttons/button.hpp"
#include "client/definitions/uiConfig.hpp"
#include "client/editor/editor.hpp"
#include "descriptors/socket.hpp"


class WindowEditor : public WindowBase {
    protected:
        std::string fileName;
        Editor *editor;
    public:
        WindowEditor(std::string fileName, std::string content);
        ~WindowEditor();

    protected:
        void pollEvents(sf::RenderWindow *window, UIConfig& config, Socket *clientFD);
        void buttonsMechanics(sf::RenderWindow *window, sf::Font *font, UIConfig& config, Socket *clientFD);
        void updateWindow(sf::RenderWindow *window, sf::Font *font, UIConfig& config);
        void activateButton(sf::RenderWindow *window, Button *button, sf::Font *font, UIConfig& config, Socket *clientFD);

        void interfaceDraw(sf::RenderWindow *window, UIConfig& config);
};

#endif
