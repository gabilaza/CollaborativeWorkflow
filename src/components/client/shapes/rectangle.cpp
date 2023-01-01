
#include <SFML/Graphics.hpp>
#include "rectangle.hpp"
#include "client/definitions/point.hpp"

using namespace sf;


RectangleShape createRect(Point topLeft, Point bottomRight, Color colorFill, Color colorLine) {
    RectangleShape rectangle;
    rectangle.setSize(Vector2f(bottomRight.x-topLeft.x, bottomRight.y-topLeft.y));
    rectangle.setFillColor(colorFill);
    rectangle.setOutlineColor(colorLine);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(topLeft.x, topLeft.y);
    return rectangle;
}
