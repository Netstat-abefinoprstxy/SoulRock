#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include "LeaderBoard.h"

using namespace std;

void saveGame(string name, int steps, int seed) {
    ofstream save;
    save.open ("Display/leaderBoard.txt", ios::app);
    save << name << ":" << steps << ":" << seed << endl;
    save.close();
    
}

void displayLeaderBoard() {
    cout << "\e[1;1H\e[2J";
    vector<string> names;
    vector<string> steps;
    vector<string> seeds;
    
    string name;
    string step;
    string seed;
    
    ifstream load;
    load.open ("Display/leaderBoard.txt", ios::app);
    while (getline (load, name, ':')) {
        getline (load, step, ':');
        getline (load, seed, '\n');
        
        names.push_back(name);
        steps.push_back(step);
        seeds.push_back(seed);
    }
    load.close();
    
    cout << right << setw(100) << setfill('-') << "" << endl;
    cout << "|" << setfill(' ') << setw(50) << "Leader Board!" << setw(49) << "|" << endl;
    cout << "|" << "  NAMES" << setw(23) << "STEPS" << setw(25) << "SEED" << setw(44) << "|" << endl;
    if (names.empty()) {
        cout << left << "|  " << setw(93) << "No scores saved yet." << right << setw(5) << "|" << endl;
        cout << setfill('-') << setw(100) << "" << endl;
        return;
    }
    cout << left << "|  " << setw(25) << names.back() << setw(25) << steps.back() << setw(25) << seeds.back() << right << setw(22) << "|" << endl;
    for (size_t i = 0; i + 1 < names.size(); ++i) {
        cout << left << "|  " << setw(25) << names.at(i) << setw(25) << steps.at(i) << setw(25) << seeds.at(i) << right << setw(22) << "|" << endl;
    }
    cout << setfill('-') << setw(100) << "" << endl;
 
}
