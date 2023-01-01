
#include <SFML/Graphics.hpp>
#include <string>
#include <cstring>
#include "editor.hpp"
#include "client/definitions/box.hpp"
#include "client/definitions/point.hpp"
#include "client/texts/text.hpp"
#include "utils/logs.hpp"

using namespace logger;
using namespace std;
using namespace sf;

static const char *tag = "Editor";


Editor::Editor() {
    this -> cursor = {0, 0};
    clear();
    this -> content.push_back(vector<char>());
}

Editor::~Editor() {
    for(int i = 0; i < (int)this -> content.size(); i++)
        this -> content[i].clear();
    this -> content.clear();
}

Point Editor::getCursorPos() {
    return this -> cursor;
}

void Editor::setContent(const char *content) {
    clear();
    string line;
    for(int i = 0; i < strlen(content); i++) {
        if(content[i] == '\n') {
            this -> content.push_back(vector<char>());
            for(int j = 0; j < (int)line.size(); j++)
                this -> content[this -> content.size()-1].push_back(line[j]);
            line.clear();
        }
        else
            line += content[i];
    }
    if(!line.empty()) {
        this -> content.push_back(vector<char>());
        for(int j = 0; j < (int)line.size(); j++)
            this -> content[this -> content.size()-1].push_back(line[j]);
        line.clear();
    }

    if(this -> content.empty())
        this -> content.push_back(vector<char>());
    else {
        if(this -> content[this -> content.size() - 1].size() > 0)
            this -> content.push_back(vector<char>());
    }
}

std::string Editor::getContent() {
    string str;
    if(this -> content[this -> content.size() - 1].size() > 0)
        this -> content.push_back(vector<char>());

    for(int line = 0; line < (int)this -> content.size(); line++) {
        for(int column = 0; column < (int)this -> content[line].size(); column++)
            str += this -> content[line][column];
        str += '\n';
    }
    return str;
}

void Editor::putFile(const char *path) {
    ofstream fout(path);

    if(this -> content[this -> content.size() - 1].size() > 0)
        this -> content.push_back(vector<char>());

    for(int line = 0; line < (int)this -> content.size(); line++) {
        for(int column = 0; column < (int)this -> content[line].size(); column++)
            fout << this -> content[line][column];
        fout << '\n';
    }

    fout.close();
}

void Editor::insertChar(char character) {
    if(32 <= character && character <= 126) { // character
        this -> content[this -> cursor.y].insert(this -> content[this -> cursor.y].begin()+this -> cursor.x, character);
        this -> cursor.x += 1;
    }
    else if(character == 13) { // enter
        this -> content.insert(this -> content.begin()+this -> cursor.y+1, vector<char>());
        this -> content[this -> cursor.y+1].insert(this -> content[this -> cursor.y+1].begin(), this -> content[this -> cursor.y].begin()+this -> cursor.x, this -> content[this -> cursor.y].end());
        this -> content[this -> cursor.y].erase(this -> content[this -> cursor.y].begin()+this -> cursor.x, this -> content[this -> cursor.y].end());
        this -> cursor.x = 0;
        this -> cursor.y += 1;
    }
    else if(character == 8) { // backspace
        if(this -> cursor.x == 0) {
            if(this -> cursor.y != 0) {
                this -> cursor.x = this -> content[this -> cursor.y-1].size();
                this -> content[this -> cursor.y-1].insert(this -> content[this -> cursor.y-1].end(), this -> content[this -> cursor.y].begin(), this -> content[this -> cursor.y].end());
                this -> content[this -> cursor.y].clear();
                this -> content.erase(this -> content.begin()+this -> cursor.y);
                this -> cursor.y -= 1;
            }
        }
        else {
            this -> content[this -> cursor.y].erase(this -> content[this -> cursor.y].begin()+this -> cursor.x-1);
            this -> cursor.x -= 1;
        }
    }
    else if(character == 9) { // tab
        this -> content[this -> cursor.y].insert(this -> content[this -> cursor.y].begin()+this -> cursor.x, static_cast<char>(32));
        this -> cursor.x += 1;
        this -> content[this -> cursor.y].insert(this -> content[this -> cursor.y].begin()+this -> cursor.x, static_cast<char>(32));
        this -> cursor.x += 1;
    }
}

void Editor::insertCharPos(char character, Point pos) {
    //TODO:Check
    if(32 <= character && character <= 126) { // character
        this -> content[pos.y].insert(this -> content[pos.y].begin()+pos.x, character);
        if(this -> cursor.y == pos.y && this -> cursor.x > pos.x)
            this -> cursor.x += 1;
    }
    else if(character == 13) { // enter
        this -> content.insert(this -> content.begin()+pos.y+1, vector<char>());
        this -> content[pos.y+1].insert(this -> content[pos.y+1].begin(), this -> content[pos.y].begin()+pos.x, this -> content[pos.y].end());
        this -> content[pos.y].erase(this -> content[pos.y].begin()+pos.x, this -> content[pos.y].end());
        if(this -> cursor.y > pos.y)
            this -> cursor.y += 1;
        else if(this -> cursor.y == pos.y && this -> cursor.x > pos.x) {
            this -> cursor.x = this -> cursor.x - pos.x;
            this -> cursor.y += 1;
        }
    }
    else if(character == 8) { // backspace
        if(pos.x == 0) {
            if(pos.y != 0) {
                pos.x = this -> content[pos.y-1].size();
                this -> content[pos.y-1].insert(this -> content[pos.y-1].end(), this -> content[pos.y].begin(), this -> content[pos.y].end());
                this -> content[pos.y].clear();
                this -> content.erase(this -> content.begin()+pos.y);
                if(this -> cursor.y > pos.y)
                    this -> cursor.y -= 1;
                else if(this -> cursor.y == pos.y) {
                    this -> cursor.x = this -> cursor.x + pos.x;
                    this -> cursor.y -= 1;
                }
            }
        }
        else {
            this -> content[pos.y].erase(this -> content[pos.y].begin()+pos.x-1);
            if(this -> cursor.y == pos.y && this -> cursor.x >= pos.x)
                this -> cursor.x -= 1;
        }
    }
    else if(character == 9) { // tab
        this -> content[pos.y].insert(this -> content[pos.y].begin()+pos.x, static_cast<char>(32));
        if(this -> cursor.y == pos.y && this -> cursor.x >= pos.x)
            this -> cursor.x += 1;
        this -> content[pos.y].insert(this -> content[pos.y].begin()+pos.x, static_cast<char>(32));
        if(this -> cursor.y == pos.y && this -> cursor.x >= pos.x)
            this -> cursor.x += 1;
    }
}

void Editor::moveCursor(int direction) {
    if(direction == 1 && this -> cursor.x > 0)
        this -> cursor.x -= 1;
    if(direction == 2 && this -> cursor.y > 0) {
        this -> cursor.y -= 1;
        if(this -> content.size() != 0 && this -> cursor.x > this -> content[this -> cursor.y].size())
            this -> cursor.x = this -> content[this -> cursor.y].size();
    }
    if(direction == 3 && this -> content.size() != 0 && this -> cursor.x < this -> content[this -> cursor.y].size())
        this -> cursor.x += 1;
    if(direction == 4 && this -> cursor.y < (int)this -> content.size()-1) {
        this -> cursor.y += 1;
        if(this -> content.size() != 0 && this -> cursor.x > this -> content[this -> cursor.y].size())
            this -> cursor.x = this -> content[this -> cursor.y].size();
    }
}

void Editor::clear() {
    for(int i = 0; i < (int)this -> content.size(); i++)
        this -> content[i].clear();
    this -> content.clear();
}

void Editor::draw(sf::RenderWindow *window, sf::Font *font, UIConfig& config) {
    Box box;
    string str;

    int aux = this -> content.size(), p = 0;
    while(aux) {
        p++;
        aux /= 10;
    }

    config.CODEEDIT_MARGIN_WIDTH = p*10+p*1.5;
    config.LIMIT_COLUMN_CODE = (config.CODE_WIDTH-config.CODEEDIT_MARGIN_WIDTH*2)/BLOCK_CODE_WIDTH-1;
    config.LIMIT_LINE_CODE = (config.CODE_HEIGHT-config.CODEEDIT_MARGIN_HEIGHT*2)/BLOCK_CODE_HEIGHT;

    Point codeP = {0, 0};
    if(this -> cursor.x > config.LIMIT_COLUMN_CODE)
        codeP.x = this -> cursor.x-config.LIMIT_COLUMN_CODE;
    if(this -> cursor.y > config.LIMIT_LINE_CODE-1)
        codeP.y = this -> cursor.y-config.LIMIT_LINE_CODE+1;

    int k = 1, nr = 0;
    p = 10, aux = codeP.y+1;
    while(aux) {
        nr++;
        aux /= 10;
    }
    aux = nr;
    while(aux) {
        if(aux%2 == 1)
            aux--, k *= p;
        else
            aux /= 2, p = p*p;
    }


    Color baseColor = Color(255, 255, 255);

    for(int line = 0, aLine = codeP.y; aLine < (int)this -> content.size() && line < (int)this -> content.size() && line < config.LIMIT_LINE_CODE; line++, aLine++) {
        box.x = MARGIN+config.CODEEDIT_MARGIN_WIDTH-(nr-1)*BLOCK_CODE_WIDTH;
        box.y = MARGIN+line*BLOCK_CODE_HEIGHT+config.CODEEDIT_MARGIN_HEIGHT;
        box.length = nr*BLOCK_CODE_WIDTH;
        box.height = BLOCK_CODE_HEIGHT;

        str = intToString(aLine+1);
        window -> draw(createTextForCode(box, str, *font, baseColor));
        if(aLine == k-2)
            k *= 10, nr++;
        for(int column = 0, aColumn = codeP.x; aColumn < (int)this -> content[aLine].size() && column < (int)this -> content[aLine].size() && column < config.LIMIT_COLUMN_CODE; column++, aColumn++) {
            box.x = MARGIN+(column+1)*BLOCK_CODE_WIDTH+config.CODEEDIT_MARGIN_WIDTH;
            box.y = MARGIN+line*BLOCK_CODE_HEIGHT+config.CODEEDIT_MARGIN_HEIGHT;
            box.length = BLOCK_CODE_WIDTH;
            box.height = BLOCK_CODE_HEIGHT;

            str = this -> content[aLine][aColumn];
            window -> draw(createTextForCode(box, str, *font, baseColor));
        }
    }


    // line
    VertexArray line(LineStrip, 2);
    line[0].position = Vector2f(MARGIN+config.CODEEDIT_MARGIN_WIDTH+BLOCK_CODE_WIDTH, MARGIN+config.CODEEDIT_MARGIN_HEIGHT);
    line[1].position = Vector2f(MARGIN+config.CODEEDIT_MARGIN_WIDTH+BLOCK_CODE_WIDTH, MARGIN+config.CODE_HEIGHT-config.CODEEDIT_MARGIN_HEIGHT);
    line[0].color = baseColor;
    line[1].color = baseColor;
    window -> draw(line);

    // cursor
    Point startP, stopP;
    Point aCursor = this -> cursor;
    if(this -> cursor.x > config.LIMIT_COLUMN_CODE)
        aCursor.x = config.LIMIT_COLUMN_CODE;
    if(this -> cursor.y > config.LIMIT_LINE_CODE-1)
        aCursor.y = config.LIMIT_LINE_CODE-1;
    startP.x = MARGIN+(aCursor.x+1)*BLOCK_CODE_WIDTH+config.CODEEDIT_MARGIN_WIDTH;
    startP.y = MARGIN+aCursor.y*BLOCK_CODE_HEIGHT+config.CODEEDIT_MARGIN_HEIGHT;
    stopP.x = startP.x;
    stopP.y = startP.y+BLOCK_CODE_HEIGHT;
    line = VertexArray(LineStrip, 2);
    line[0].position = Vector2f(startP.x, startP.y);
    line[1].position = Vector2f(stopP.x, stopP.y);
    line[0].color = Color(255, 0, 0);
    line[1].color = Color(255, 0, 0);
    window -> draw(line);
}

string Editor::intToString(int value) {
    string str = "";
    if(value == 0)
        str = "0";
    int p = 1, auxValue = value;
    while(auxValue) {
        p *= 10;
        auxValue /= 10;
    }
    p /= 10;
    while(p) {
        str += (char)((value/p)%10+'0');
        p /= 10;
    }
    return str;
}

