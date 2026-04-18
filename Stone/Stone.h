#ifndef STONE_H
#define STONE_H

#include <utility>
#include <string>
using namespace std;

enum Color {WHITE, ORANGE, RED, GREEN, BLUE, PURPLE, BLACK};

class Stone {
    protected:
        pair<int, int> position;
        Color color;
    public:
        Stone(); // Default constructor
        virtual ~Stone() = default;
        
        // Public booleans
        bool obtained;
        bool scrollFound;
        int aura;
        
        string getColor() const; // returns string color of stone
        Color getActualColor() const { return color; }
        // Pure virtual function (Stone will not be unspecific)
        virtual void uniqueFunction() = 0;
        
        void setPosition(int, int);
        pair<int, int> getPosition() const;
        
};

#endif
