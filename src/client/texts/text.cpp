
#include <SFML/Graphics.hpp>
#include <string>
#include "text.hpp"
#include "client/definitions/box.hpp"
#include "client/definitions/point.hpp"

using namespace std;
using namespace sf;


Text createText(Box box, string str, Font &font) {
    Text text;
    text.setFont(font);
    text.setString(str);
    text.setLetterSpacing(0.5);
    float st = 1, dr = 200;
    while(st <= dr) {
        float mij = (st+dr)/2;
        text.setCharacterSize(mij);
        if(text.getLocalBounds().width <= box.length && text.getLocalBounds().height <= box.height)
            st = mij+1;
        else
            dr = mij-1;
    }
    text.setCharacterSize(st-1);
    text.setPosition(box.x+(box.length-text.getLocalBounds().width)/2, box.y+(box.height-text.getLocalBounds().height)/2);
    text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);
    return text;
}

Text createTextForCode(Box box, string str, Font &font, Color color) {
    Text text;
    text.setFont(font);
    text.setString('1');
    if('A' <= str && str <= 'Z')
        text.setString('A');
    if('a' <= str && str <= 'z')
        text.setString('a');
    text.setCharacterSize(20);
    text.setLetterSpacing(0.5);
    float xheight = text.getLocalBounds().height;
    text.setString(str);
    if(str == 'g' || str == 'p' || str == 'q' || str == 'Q' || str == 'y')
        text.setPosition(box.x+(box.length-text.getLocalBounds().width)/2, box.y+box.height-xheight);
    else if(str == 'j')
        text.setPosition(box.x+(box.length-text.getLocalBounds().width)/2, box.y+box.height-xheight-(text.getLocalBounds().height-xheight)/2+0.5);
    else if(str == '7' || str == '9')
        text.setPosition(box.x+(box.length-text.getLocalBounds().width)/2, box.y+box.height-text.getLocalBounds().height+1);
    else if(str == '-' || str == '+' || str == '=' || str == '~')
        text.setPosition(box.x+(box.length-text.getLocalBounds().width)/2, box.y+box.height-xheight/2-text.getLocalBounds().height/2);
    else if(str == '^' || str == '`' || str == '\'' || str == '\"')
        text.setPosition(box.x+(box.length-text.getLocalBounds().width)/2, box.y+box.height-xheight);
    else
        text.setPosition(box.x+(box.length-text.getLocalBounds().width)/2, box.y+box.height-text.getLocalBounds().height);
    text.setFillColor(color);
    text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);
    return text;
}
