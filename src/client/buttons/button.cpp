
#include <SFML/Graphics.hpp>
#include <string>
#include "button.hpp"
#include "client/definitions/box.hpp"
#include "client/texts/text.hpp"
#include "client/shapes/rectangle.hpp"
#include "client/buttons/buttonType.hpp"

using namespace sf;


Button::Button(Point topLeft, Point bottomRight, buttonType type, sf::Color colorFill, sf::Color colorLine, sf::Color colorOnPressFill, std::string str) {
    this -> topLeft = topLeft;
    this -> bottomRight = bottomRight;
    this -> type = type;
    this -> colorFill = colorFill;
    this -> colorLine = colorLine;
    this -> colorOnPressFill = colorOnPressFill;
    this -> press = false;
    this -> prepForPress = false;
    this -> str = str;
}

Button::~Button() {
}

bool Button::mouseOnButton(float x, float y) {
    return (this -> topLeft.x < x && x < this -> bottomRight.x && this -> topLeft.y < y && y < this -> bottomRight.y);
}

bool Button::isPressed(RenderWindow *window) {
    Vector2i positionMouse = Mouse::getPosition(*window);
    if(this -> mouseOnButton(positionMouse.x, positionMouse.y)) {
        if(!this -> prepForPress && !Mouse::isButtonPressed(Mouse::Left)) {
            this -> prepForPress = true;
        }
        else if(this -> prepForPress && Mouse::isButtonPressed(Mouse::Left))
            this -> press = true;
        else if(this -> press) {
            this -> prepForPress = false;
            this -> press = false;
            return true;
        }
    }
    else {
        this -> prepForPress = false;
        this -> press = false;
    }
    return false;
}

buttonType Button::getType() {
    return this -> type;
}

std::string Button::getString() {
    return this -> str;
}

void Button::draw(RenderWindow *window, Font *font) {
    if(!this -> press)
        window -> draw(createRect(this -> topLeft, this -> bottomRight, this -> colorFill, this -> colorLine));
    else
        window -> draw(createRect(this -> topLeft, this -> bottomRight, this -> colorOnPressFill, this -> colorLine));
    Box box;
    box.x = this -> topLeft.x+(this -> bottomRight.x-this -> topLeft.x)/8;
    box.y = this -> topLeft.y+(this -> bottomRight.y-this -> topLeft.y)/8;
    box.length = this -> bottomRight.x-this -> topLeft.x-(this -> bottomRight.x-topLeft.x)/4;
    box.height = this -> bottomRight.y-this -> topLeft.y-(this -> bottomRight.y-this -> topLeft.y)/4;
    window -> draw(createText(box, this -> str, *font));
}
