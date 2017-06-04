//
// Created by XiongZZ on 2017/6/4.
//

#ifndef LIGHT_BOT_MAP_H
#define LIGHT_BOT_MAP_H

#include "Robot.h"

struct Light {
    int x, y;
    int status;
};

class Map {
    const int size;
    int **matrix;
    const int lightNum;
    Light* lights;
public:
    friend class Robot;
    Map(int size1, int **matrix1, Light *light1, int lightNum1);
    ~Map(){
        delete(lights);
        for(int i = 0; i < lightNum; i++){
            delete(matrix[i]);
        }
    }

    void print(const Robot& robot);

    int isEnded();

    int boundCheck(const Robot &robot);

    int judgeF(const Robot &robot);

    int turnLight(const Robot& robot);

};


#endif //LIGHT_BOT_MAP_H
