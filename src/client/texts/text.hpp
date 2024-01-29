#ifndef __UI_TEXT_H
#define __UI_TEXT_H

#include <SFML/Graphics.hpp>
#include <string>
#include "client/definitions/box.hpp"
#include "client/definitions/point.hpp"

sf::Text createText(Box box, std::string str, sf::Font &font);
sf::Text createTextForCode(Box box, std::string str, sf::Font &font, sf::Color color);

#endif
