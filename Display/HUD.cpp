#include <iostream>
#include <iomanip>
#include <sstream>
#include "Map.h"
#include "../Player/Player.h"
#include "HUD.h"
using namespace std;

namespace {
void clearScreen() {
    cout << "\x1B[2J\x1B[H";
}
}

extern const int MAX_SIZE;

HUD::HUD() {
    // Nothing
}

void HUD::updateBoard() {
    map.updateBoard();
    cout << endl; //NEEDS FIXING
}

void HUD::print(pair<int, int> position) {
    clearScreen();
    cout << render(position);
}

string HUD::render(pair<int, int> position) {
    ostringstream out;

    // Each line with map and inventory items
    int inventoryItem = 0;
    (map.board)[position.first][position.second] = '.';
    out << left << setw(61) << setfill('-') << "" << endl;
    out << setw(30) << setfill(' ') << "|" << "| "
        << setw(28) << "Inventory:" << "|"
        << endl;
    for (int i = position.first - 5; i <= position.first + 5; ++i) {
        if (i >= 0 && i < 64) {
            out << "|   ";
            for (int j = position.second - 5; j <= position.second + 5; ++j) {
                if (j >= 0 && j < 64) {
                    out << "|" << (map.board)[i][j];
                    if (j == (position.second + 5) || j == 63)
                        out << "|";
                } else
                    out << "  ";

            }
        } else
            out << setw(27) << "|";
        out << "   |";
        if (inventoryItem < 9) {
            out << " - " << setw(26) << player.displayInventory(inventoryItem);
            inventoryItem++;
        } else if (inventoryItem == 9) {
            out << setw(29) << " 'E' for full inventory";
            inventoryItem++;
        } else if (inventoryItem == 10) {
            out << setw(29) << setfill('-') << "";
            out << setfill(' ');
            inventoryItem++;
        } else {
            out << setw(29) << "";
        }
        out << "|" << endl;
    }
    out << setw(30) << "|"
        << "| Steps taken: " << setw(15) << player.getSteps() << "|" << endl;
    out << setw(61) << setfill('-') << "" << endl;
    (map.board)[position.first][position.second] = ' ';

    return out.str();
}

bool HUD::isOccupied(pair<int, int> position) {
    return map.isOccupied(position);
}

void HUD::pickUpItem(pair<int, int> position) {
    player.pickUpItem(position, &map);
}
