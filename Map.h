//
// Created by XiongZZ on 2017/6/4.
//

#ifndef LIGHT_BOT_MAP_H
#define LIGHT_BOT_MAP_H

#include <vector>

#include "Robot.h"

struct postion {
    int x, y;
};

struct Light {
    int x, y;
    int status;
};


class Node {
    int x;
    int y;
    bool visited;
    Node *father;
    bool hasLight = false;
    int height;
    int direction;
public:
    Node() {};

    Node(int x1, int y1, bool visited1) : x(x1), y(y1), visited(visited1) {}

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    void setX(int x1) {
        x = x1;
    }

    void setY(int y1) {
        y = y1;
    }

    Node *getFather() {
        return father;
    }

    void setFather(Node *father1) {
        father = father1;
    }

    bool getVisited() {
        return visited;
    }

    void setVisited(bool visited1) {
        visited = visited1;
    }

    bool getHasLight() {
        return hasLight;
    }

    void setHasLight(bool hasLight1) {
        hasLight = hasLight1;
    }

    void setHeight(int height1) {
        height = height1;
    }

    int getHeight() {
        return height;
    }

    int getDirection() {
        return direction;
    }

    void setDirection(int direction1) {
        direction = direction1;
    }
};

class Map {
    int size;
    int **matrix;
    int lightNum;
    Light *lights;
    Node **mapNodes;

    void NodesInit();

public:
    friend void store(std::string filename);

    friend void load(std::string filename);

    friend class Robot;

    friend class Game;

    Map() {};

    Map(int size1, int **matrix1, Light *light1, int lightNum1);

    ~Map() {
        delete (lights);
        for (int i = 0; i < size; i++) {
            delete (matrix[i]);
        }
        for (int i = 0; i < size; i++) {
            delete (mapNodes[i]);
        }
    }

    Map(const Map &map) {
        size = map.size;
        matrix = map.matrix;
        lightNum = map.lightNum;
        lights = map.lights;
    }

    Node **getMapNodes() {
        return mapNodes;
    }

    void path(Node *root, int direction);

    void print(const Robot *robot);

    int isEnded();

    int boundCheck(const Robot *robot);

    int judgeF(const Robot *robot);

    int turnLight(const Robot *robot);


};


#endif //LIGHT_BOT_MAP_H
