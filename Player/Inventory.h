#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <string>
using namespace std;

enum Checkpoint { OBT_WHTSTN, OBT_ORGSTN, OBT_REDSTN, OBT_GRNSTN, OBT_BLUSTN,
                  OBT_PPLSTN, OBT_BLKSTN, OBT_WHTFDR, OBT_ORGFDR, OBT_REDFDR,
                  OBT_GRNFDR, OBT_BLUFDR, OBT_PPLFDR, OBT_BLKFDR, OBT_WHTSCR,
                  OBT_ORGSCR, OBT_REDSCR, OBT_GRNSCR, OBT_BLUSCR, OBT_PPLSCR,
                  OBT_BLKSCR };

class Inventory {
    public:
        Inventory();
        void setFlag(Checkpoint);
        int itemsSize() const { return items.size(); }
        string displayItem(int arg) const;
        string getName(Checkpoint) const;
        bool hasAllStones() const;
        bool hasOrange() const { return items.count(OBT_ORGSTN); }
        bool hasRedFinder() const;
    private:
        map<Checkpoint, bool> items;
        map<Checkpoint, string> itemNames;
};

#endif
