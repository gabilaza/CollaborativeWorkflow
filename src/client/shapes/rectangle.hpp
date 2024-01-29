#ifndef __UI_RECTANGLE_H
#define __UI_RECTANGLE_H

#include <SFML/Graphics.hpp>
#include "client/definitions/point.hpp"


sf::RectangleShape createRect(Point topLeft, Point bottomRight, sf::Color colorFill, sf::Color colorLine);

#endif
