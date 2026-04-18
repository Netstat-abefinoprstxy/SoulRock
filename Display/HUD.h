#ifndef HUD_H
#define HUD_H

#include <iostream>
#include <string>
#include <utility>
#include "Map.h"
#include "../Player/Player.h"
using namespace std;

class HUD {
    public:
        HUD();
        void print(pair<int, int>);
        string render(pair<int, int>);
        void updateBoard();
        bool isOccupied(pair<int, int>);
        void pickUpItem(pair<int, int>);
        bool hasAllStones() { return player.hasAllStones(); }
        Map* getMapPtr() { return &map; }
        Player* getPlayerPtr() { return &player; }
        
        Player player;
    private:
        Map map;
};

#endif
