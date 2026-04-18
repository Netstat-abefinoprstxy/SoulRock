#include <stdlib.h>
#include <iostream>
#include <map>
#include <utility>
#include "Map.h"
#include "../Scroll/Scrolls.h"
#include "../Scroll/Finder.h"
#include "../Stone/Stone.h"
#include "../Stone/SpecificStones.h"
using namespace std;

Map::Map() {
    genStone(WHITE);
    genStone(ORANGE);
    genStone(RED);
    genStone(GREEN);
    genStone(BLUE);
    genStone(PURPLE);
    genStone(BLACK);
}

Map::~Map() {
    for (auto stone : stones) {
        delete stone.second;
        stone.second = nullptr;
    }
    for (auto scroll : scrolls) {
        delete scroll.second;
        scroll.second = nullptr;
    }
    for (auto finder : finders) {
        delete finder.second;
        finder.second = nullptr;
    }
}

Checkpoint Map::getItem(pair<int, int> position, bool scrFound) {
    for (auto finder : finders) {
        if (finder.first == position) {
            switch (finder.second->getStnColor()) {
                case WHITE:
                    return OBT_WHTFDR;
                case ORANGE:
                    return OBT_ORGFDR;
                case RED:
                    return OBT_REDFDR;
                case BLACK:
                    return OBT_BLKFDR;
                case GREEN:
                    return OBT_GRNFDR;
                case BLUE:
                    return OBT_BLUFDR;
                case PURPLE:
                    return OBT_PPLFDR;
            }
        }
    }
    
    for (auto stone : stones) {
        if (stone.first == position) {
            switch (stone.second->getActualColor()) {
                case WHITE:
                    if (scrFound) { return OBT_WHTSTN; }
                    return OBT_WHTSCR;
                case ORANGE:
                    if (scrFound) { return OBT_ORGSTN; }
                    return OBT_ORGSCR;
                case RED:
                    if (scrFound) { return OBT_REDSTN; }
                    return OBT_REDSCR;
                case BLACK:
                    if (scrFound) { return OBT_BLKSTN; }
                    return OBT_BLKSCR;
                case GREEN:
                    if (scrFound) { return OBT_GRNSTN; }
                    return OBT_GRNSCR;
                case BLUE:
                    if (scrFound) { return OBT_BLUSTN; }
                    return OBT_BLUSCR;
                case PURPLE:
                    if (scrFound) { return OBT_PPLSTN; }
                    return OBT_PPLSCR;
            }
        }
    }

    return OBT_WHTSTN;
}

bool Map::willPurpleStoneKickBack() {
    for (auto stone : stones) {
        if (stone.second->getColor() == "Purple") {
            return dynamic_cast<PurpleStone*>(stone.second)->willKickBack();
        }
    }

    return false;
}

void Map::removeItem(pair<int, int> position) {
    stones.erase(position);
    finders.erase(position);
}

bool Map::isOccupied(pair<int, int> position) {
    return (   stones.count(position) 
            || finders.count(position)  );
}

void Map::updateBoard() {
    // Everything spaces to begin with!
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            board[i][j] = ' ';
        }
    }
    map<pair<int, int>, Stone*> newStones;
    for (auto stone : stones) {
        Stone* tmpStone = stone.second;
        tmpStone->uniqueFunction();
        newStones.emplace(tmpStone->getPosition(), tmpStone);
    }
    stones.clear();
    for (auto stone : newStones) {
        stones.emplace(stone.first, stone.second);
    }
    // Fill the stone auras and map positions
    for (auto stone : stones) {
        board[stone.first.first][stone.first.second] = '!';
        fillAura(stone.first, stone.second->aura);
    }
    
    for (auto finder : finders) {
        board[finder.first.first][finder.first.second] = '^';
        fillAura(finder.first, 2);
    }
}

void Map::fillAura(pair<int, int> stonePos, int aura) {
    for (int i = stonePos.first - aura; i <= stonePos.first + aura; ++i) {
        for (int j = stonePos.second - aura; j <= stonePos.second + aura; ++j) {
            if (!isOccupied(make_pair(i, j)) && i >= 0 && j >= 0 && i < MAX_SIZE && j < MAX_SIZE) {
                board[i][j] = '#';
            }
        }
    }
}
void Map::genStone(Color color) {
    Stone* stnPtr;
    pair<int, int> position;
    // Get unoccupied position
    do {
        position = getNextRand();
    } while (isOccupied(position));
    // Determine which stone to generate
    switch (color) {
        case WHITE:
            stnPtr = new WhiteStone(position);
            break;
        case ORANGE:
            stnPtr = new OrangeStone(position);
            break;
        case RED:
            stnPtr = new RedStone(position);
            break;
        case GREEN:
            stnPtr = new GreenStone(position);
            break;
        case BLUE:
            stnPtr = new BlueStone(position);
            break;
        case PURPLE:
            stnPtr = new PurpleStone(position);
            break;
        case BLACK:
            stnPtr = new BlackStone(position);
    }
    stones.emplace(position, stnPtr);
    genScroll(stnPtr);
    genFinder(stnPtr);
}

void Map::genScroll(Stone* stnPtr) {
    Scroll* scrollPtr = new Scroll;
    scrolls.emplace(stnPtr, scrollPtr);
}

void Map::genFinder(Stone* stnPtr) {
    pair<int, int> position;
    do {
        position = getNextRand();
    } while (isOccupied(position));
    Finder* finderPtr = new Finder(position, stnPtr);
    finders.emplace(position, finderPtr);
}

pair<int, int> Map::getNextRand() {
    return (make_pair(rand() % 64, rand() % 64));
}

Scroll* Map::getScroll(Color color) {
    for (auto scroll : scrolls) {
        if (scroll.first->getActualColor() == color) {
            return scroll.second;
        }
    }

    return nullptr;
}

Finder* Map::getFinder(Color color) {
    for (auto finder : finders) {
        if (finder.second->getStnColor() == color)
            return finder.second;
    }

    return nullptr;
}
