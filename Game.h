//
// Created by XiongZZ on 2017/6/15.
//

#ifndef LIGHT_BOT_GAME_H
#define LIGHT_BOT_GAME_H

#define RANKSIZE 5
#define DEFAULT_MAPSIZE 8
#define MAX_LIGHTS_NUM 10
#define MAX_HEIGHT 6

#include "Map.h"

struct RankItem {
    int order;
    std::string name;
    int time;
};

class Game {
    Map *map;
    Robot *robot;
    std::string filename;
    int pattern;
    int usedTime;

    void store(std::string filename);

public:
    Game(std::string filename1, int pattern1);

    ~Game() {
        delete map;
        delete robot;
    }

    void control();

    void load();

    void save();

    void rankFile(std::string addName, int addTime);

    void showRank();

    void clearRank();

    void randomMap();
};


#endif //LIGHT_BOT_GAME_H
