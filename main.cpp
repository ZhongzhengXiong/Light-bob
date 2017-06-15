#include <iostream>
//#include "Map.h"
#include "Game.h"

#include <windows.h>
#include <iomanip>
#include <algorithm>
#include <stack>
#include <sstream>

//#define RANKSIZE 5
//#define DEFAULT_MAPSIZE 8
//#define MAX_LIGHTS_NUM 10
//#define MAX_HEIGHT 6


using namespace std;


int main() {
    SetConsoleOutputCP(65001);
    string beginstr = "Before the game, you can choose the game pattern （Easy, Difficult, Super Difficult, Default, Records);\n"
            "Except the records pattern in which you choose the games you saved before, there are 9 levels in each pattern;\n"
            "You can change the pattern in your game, and you can also choose random map when choosing file (10 means random);"
            "Now choose the pattern and level, enjoy yourself! \n";
    string patternStr = "1. Easy\n"
            "2. Difficult\n"
            "3. Super Difficult\n"
            "4. Default\n"
            "5. Records\n";
    cout << beginstr;
    cout << patternStr;
    cout << "Choose the pattern: ";
    int pattern;
    string filename;
    while (cin >> pattern) {
        if (pattern > 0 && pattern < 5) {
            cout << "Choose the level (1~9, 10 means random map): ";
            int level;
            while (cin >> level) {
                if (level < 0 || level > 10)
                    cout << "beyong the boundaries" << endl;
                else {
                    stringstream ss;
                    ss << level;
                    filename = ss.str() + ".txt";
                    if (level == 10)
                        filename = "random.txt";
                    break;
                }
            }
            break;
        }
        if (pattern == 5) {
            cout << "Input the filename you saved before: ";
            cin >> filename;
            filename += ".txt";
            break;
        }
        cout << "beyong the boundaries" << endl;
    }
    pattern--;
    Game game(filename, pattern);
    game.control();
}

