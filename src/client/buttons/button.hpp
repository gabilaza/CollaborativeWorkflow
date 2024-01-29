#ifndef __UI_BUTTON_H
#define __UI_BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include "client/definitions/point.hpp"
#include "client/buttons/buttonType.hpp"

class Button {
    private:
        Point topLeft, bottomRight;
        buttonType type;
        sf::Color colorFill, colorLine, colorOnPressFill;
        bool press, prepForPress;
        std::string str;
    public:
        Button(Point topLeft, Point bottomRight, buttonType type, sf::Color colorFill, sf::Color colorLine, sf::Color colorOnPressFill, std::string str);
        ~Button();
        bool mouseOnButton(float x, float y);

        bool isPressed(sf::RenderWindow *window);
        buttonType getType();
        std::string getString();

        void draw(sf::RenderWindow *window, sf::Font *font);
};

#endif
