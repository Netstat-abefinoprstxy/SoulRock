#ifndef  PLAYER_H
#define  PLAYER_H

#include <map>
#include <utility>
#include "Inventory.h" // For Checkpoint enum
using namespace std;

class Map;
class Player{
    public:
        Player();
        //Mutators
        void addStep() {stepsTaken++;}
        void move(char step);
        void pickUpItem(pair<int,int>, Map*);
        void setCheckpoint(Checkpoint);
        void kickBackPlayer();
        bool hasAllStones() { return inventory.hasOrange(); }
        bool hasOrangeStone() const { return inventory.hasOrange(); }
        bool hasRequiredStones() const { return inventory.hasAllStones(); }
        bool hasRedFinder() const { return inventory.hasRedFinder(); }
        //Accessors
        int getSteps() const {return stepsTaken;}
        pair<int, int> getPosition() const {return position;}
        string displayInventory(int) const;
        int invSize() const { return inventory.itemsSize(); }
    private:
        int stepsTaken;
        map<Checkpoint,bool> checkpoints;
        pair<int,int> position;
        Inventory inventory;
    
};

#endif
