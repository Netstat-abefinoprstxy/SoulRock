#include "Inventory.h"
#include <map>
#include <string>
#include <iostream>
using namespace std;

Inventory::Inventory() {
    // Initialize checkpoint map
    itemNames[OBT_WHTSTN] = "White stone";
    itemNames[OBT_ORGSTN] = "Orange stone";
    itemNames[OBT_REDSTN] = "Red stone";
    itemNames[OBT_GRNSTN] = "Green stone";
    itemNames[OBT_BLUSTN] = "Blue stone";
    itemNames[OBT_PPLSTN] = "Purple stone";
    itemNames[OBT_BLKSTN] = "Black stone";
    itemNames[OBT_WHTFDR] = "White stone finder scroll";
    itemNames[OBT_ORGFDR] = "Orange stone finder scroll";
    itemNames[OBT_REDFDR] = "Red stone finder scroll";
    itemNames[OBT_GRNFDR] = "Green stone finder scroll";
    itemNames[OBT_BLUFDR] = "Blue stone finder scroll";
    itemNames[OBT_PPLFDR] = "Purple stone finder scroll";
    itemNames[OBT_BLKFDR] = "Black stone finder scroll";
    itemNames[OBT_WHTSCR] = "White stone riddle scroll";
    itemNames[OBT_ORGSCR] = "Orange stone riddle scroll";
    itemNames[OBT_REDSCR] = "Red stone riddle scroll";
    itemNames[OBT_GRNSCR] = "Green stone riddle scroll";
    itemNames[OBT_BLUSCR] = "Blue stone riddle scroll";
    itemNames[OBT_PPLSCR] = "Purple stone riddle scroll";
    itemNames[OBT_BLKSCR] = "Black stone riddle scroll";
}

void Inventory::setFlag(Checkpoint checkpoint) {
    items[checkpoint] = true;
}

string Inventory::displayItem(int arg) const {
    // Base case: zero items
    if (items.size() == 0) {
        return "";
    }
    int i = 0;
    for (auto item : items) {
        if (i == arg)
            return (itemNames.at(item.first));
        i++;
    }
    
    return "";
}

string Inventory::getName(Checkpoint checkpoint) const {
    return (itemNames.find(checkpoint)->second);
}

bool Inventory::hasAllStones() const {
    return (items.count(OBT_WHTSTN) &&
            items.count(OBT_REDSTN) &&
            items.count(OBT_GRNSTN) &&
            items.count(OBT_BLUSTN) &&
            items.count(OBT_PPLSTN) &&
            items.count(OBT_BLKSTN));
}

bool Inventory::hasRedFinder() const {
    return (items.count(OBT_REDFDR));
}
