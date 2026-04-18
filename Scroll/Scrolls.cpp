#include "Scrolls.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

Scroll::Scroll() {
    int line = rand() % 22 + 1;
    scrollFound = false;
    string temp;
    ifstream readIn;
    readIn.open ("Scroll/riddles.txt");
    for(int i = 1; i < line; i++){
        getline(readIn, temp, '\n');
    }
    getline(readIn, riddle, ':');
    getline(readIn, answer, '\n');
    readIn.close();
}

bool Scroll::checkAnswer(string userAnswer){
    for (size_t i = 0; i < userAnswer.size(); ++i) {
        userAnswer[i] = tolower(userAnswer[i]);
    }
    return (userAnswer == answer);
}
