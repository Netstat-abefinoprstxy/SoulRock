/*******************************************************************************
 * CPTR 142 - Project 3 - Soul Rock                                            *
 * Designed by: Trevor Natiuk (Natitr)                                         *
 *              Kaelan Willauer (Willka)                                       *
 *              Noah Griffith (Grifno)                                         *
 *              Jose Hernandez (Hernjo)                                        *
 * Last modified: 15 March 2019                                                *
 * ----------------------------------------------------------------------------*
 *                                                                             *
 * Filename: SoulRock.cpp (main game file)                                     *
 *                                                                             *
 ******************************************************************************/

// Display files
#include "Display/HUD.h"
#include "Display/Map.h"
#include "Display/LeaderBoard.h"
// Player files
#include "Player/Inventory.h"
#include "Player/Player.h"
// Scroll files
#include "Scroll/Finder.h"
#include "Scroll/Scrolls.h"
// Stone files
#include "Stone/Stone.h"
#include "Stone/SpecificStones.h"
// Library files
#include <iomanip>  // For setw and setfill
#include <iostream> // For cin and cout
#include <stdlib.h> // For srand
#include <stdio.h>  // For getchar
#include <limits>
#include <string>   // For std::string
#include <utility>  // For std::pair
using namespace std;

namespace {
char getInputChar() {
    string input;
    getline(cin, input);
    return input.empty() ? '\n' : input[0];
}
}

// Function prototypes
void displayFullInventory(Player*);
bool endGame(pair<int, int>&, HUD&);
void key();

int main() {
    // Welcome message
    cout << left << fixed << setprecision(2) << endl;
    cout << setw(24) << setfill('*') << "" << setfill(' ')   << endl
         << setw(23) << "*  Soul Rock v. 1.0"    << "*" << endl
         << setw(23) << "*  By: Trevor Natiuk"   << "*" << endl
         << setw(23) << "*      Kaelan Willauer" << "*" << endl
         << setw(23) << "*      Noah Griffith"   << "*" << endl
         << setw(23) << "*      Jose Hernandez"  << "*" << endl
         << setw(24) << setfill('*') << "" << endl << endl;

    string name;
    int seed;
    cout << "Enter username: ";
    getline(cin, name);
    if (name.empty()) name = "Anonymous";

    while (true) {
        cout << "Enter seed: ";
        cin >> seed;
        if (!cin) {
            cout << "Number please!" << endl;
            cin.clear();
            cin.ignore(999, '\n');
            continue;
        }
        break;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    srand(seed);

    // Generate game
    HUD mainHUD;
    mainHUD.updateBoard();
    pair<int, int> playerPos = mainHUD.player.getPosition();

    // Main loop
    char choice = '\0';
    while (choice != 'x') {
        mainHUD.print(playerPos);
        if (endGame(playerPos, mainHUD)) {
            cout << "You are at the altar! Enter 'x' to return stones.\n";
        } else if (mainHUD.isOccupied(playerPos)) {
            cout << "There is an item here. Enter 'y' to pick up.\n";
        }
        cout << ">> ";

        choice = getInputChar();
        choice = tolower(choice);
        switch (choice) {
            case 'y':
                if (mainHUD.isOccupied(playerPos)) {
                    mainHUD.pickUpItem(playerPos);
                    mainHUD.updateBoard();
                }
                break;
            case 'e':
                displayFullInventory(mainHUD.getPlayerPtr());
                break;
            case 'x':
                if (!endGame(playerPos, mainHUD)) choice = 'f';
                break;
            default:
                mainHUD.player.move(choice);
                mainHUD.updateBoard();

        }
        playerPos = mainHUD.player.getPosition();
    }
    saveGame(name, mainHUD.player.getSteps(), seed);
    displayLeaderBoard();

    return 0;
}

bool endGame(pair<int, int> &playerPos, HUD &mainHUD) {

    return ((playerPos.first == 0 && playerPos.second == 0) && mainHUD.hasAllStones());
}

void displayFullInventory(Player* player) {
    //CLS HERE
    cout << "Inventory: " << endl;
    for (int i = 0; i < player->invSize(); ++i) {
        cout << "- " << player->displayInventory(i) << endl;
    }
    cout << endl;
    key();
}
