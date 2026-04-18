#include <utility>
#include <iostream>
#include <cctype>
#include "Player.h"
#include "Inventory.h"
#include "../Scroll/Scrolls.h"
#include "../Scroll/Finder.h"
#include "../Display/Map.h"
using namespace std;

namespace {
void clearScreen() {
    cout << "\x1B[2J\x1B[H";
}
}

// NON-MEMBER FUNCTION PROTOTYPES
void displayRiddle(Scroll*);
void displayFinder(Finder*);
void key();

Player::Player() {
    stepsTaken = 0;
    position = make_pair(0, 0);
}
void Player::kickBackPlayer() {
    int value = rand() % 4;
    for (int i = 0; i < 3; ++i) {
        switch (value) {
            case 0:
                move('w');
                break;
            case 1:
                move('a');
                break;
            case 2:
                move('s');
                break;
            case 3:
                move('d');
                break;
        }
    }
}

void Player::move(char step) {
    //to move W,S,A,D
    switch(step){
        case 'w':       //To move up
            if (position.first != 0)
                position.first--;
            addStep();
            break;
        case 's':       //To move down
            if (position.first != 63)
                position.first++;
            addStep();
            break;
        case 'a':       //To move left
            if (position.second != 0)
                position.second--;
            addStep();
            break;
        case 'd':       //To move right
            if (position.second != 63)
                position.second++;
            addStep();
            break;
    }
}
void Player::pickUpItem(pair<int,int>position, Map* map){

    if(!(map->isOccupied(position))){
        return;
    }

    Checkpoint flag = map->getItem(position, false);
    switch (flag) {
        case OBT_WHTSCR:
            displayRiddle(map->getScroll(WHITE));
            inventory.setFlag(OBT_WHTSCR);
            flag = map->getItem(position, true);
            break;
        case OBT_BLKSCR:
            displayRiddle(map->getScroll(BLACK));
            inventory.setFlag(OBT_BLKSCR);
            flag = map->getItem(position, true);
            break;
        case OBT_ORGSCR:
            if(!(inventory.hasAllStones())){
                return;
            }
            displayRiddle(map->getScroll(ORANGE));
            inventory.setFlag(OBT_ORGSCR);
            flag = map->getItem(position, true);
            break;
        case OBT_BLUSCR:
            displayRiddle(map->getScroll(BLUE));
            inventory.setFlag(OBT_BLUSCR);
            flag = map->getItem(position, true);
            break;
        case OBT_REDSCR:
            if(!(inventory.hasRedFinder())){
                return;
            }
            displayRiddle(map->getScroll(RED));
            inventory.setFlag(OBT_REDSCR);
            flag = map->getItem(position, true);
            break;
        case OBT_GRNSCR:
            displayRiddle(map->getScroll(GREEN));
            inventory.setFlag(OBT_GRNSCR);
            flag = map->getItem(position, true);
            break;
        case OBT_PPLSCR:
            if(map->willPurpleStoneKickBack()) {
                kickBackPlayer();
                return;
            }
            displayRiddle(map->getScroll(PURPLE));
            inventory.setFlag(OBT_PPLSCR);
            flag = map->getItem(position, true);
            break;
        case OBT_WHTFDR:
            displayFinder(map->getFinder(WHITE));
            break;
        case OBT_ORGFDR:
            displayFinder(map->getFinder(ORANGE));
            break;
        case OBT_REDFDR:
            displayFinder(map->getFinder(RED));
            break;
        case OBT_GRNFDR:
            displayFinder(map->getFinder(GREEN));
            break;
        case OBT_BLKFDR:
            displayFinder(map->getFinder(BLACK));
            break;
        case OBT_BLUFDR:
            displayFinder(map->getFinder(BLUE));
            break;
        case OBT_PPLFDR:
            displayFinder(map->getFinder(PURPLE));
            break;
		default:
			break;
    }
    inventory.setFlag(flag);
    map->removeItem(position);
    return;
}

void Player::setCheckpoint(Checkpoint checkpoint){
    //emplace checkpoint when you pick up a stone
    inventory.setFlag(checkpoint);
}

string Player::displayInventory(int argument) const {
    //read out the inventory
    return (inventory.displayItem(argument));
}

// NON-MEMBER FUNCTIONS

void key() {
    cout << "Press enter to continue...";
    string tmp;
    getline(cin, tmp);
}

void displayRiddle(Scroll* scroll) {
    string choice;
    while (true) {
        clearScreen();
        cout << "Q: " << scroll->showRiddle();
        getline(cin, choice);
        if (scroll->checkAnswer(choice)) {
            cout << "Correct!" << endl;
            key();
            return;
        }
    }
}

void displayFinder(Finder* finder) {
    clearScreen();
    cout << finder->displayHint() << endl << endl;
    key();
}
