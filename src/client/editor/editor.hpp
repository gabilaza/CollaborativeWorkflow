#ifndef __EDITOR_H
#define __EDITOR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "client/definitions/point.hpp"
#include "client/definitions/uiConfig.hpp"


class Editor {
    private:
        std::vector <std::vector <char>> content;
        Point cursor;
    public:
        Editor();
        ~Editor();

        Point getCursorPos();
        void setContent(const char *content);
        std::string getContent();
        void putFile(const char *fileName);
        void insertChar(char character);
        void insertCharPos(char character, Point pos);
        void moveCursor(int direction); // 1 2 3 4 == < ^ > v
        void clear();

        void draw(sf::RenderWindow *window, sf::Font *font, UIConfig& config);
    private:
        std::string intToString(int value);
};

#endif
