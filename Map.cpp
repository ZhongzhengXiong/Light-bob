//
// Created by XiongZZ on 2017/6/4.
//
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include "Map.h"


using namespace std;

string lightOff[11] = {"○", "①", "②", "③", "④", "⑤", "⑥", "⑦", "⑧", "⑨", "⑩"};
string lightOn[11] = {"●", "❶", "❷", "❸", "❹", "❺", "❻", "❼", "❽", "❾", "❿"};
string directionsStr[4] = {"↑", "→", "↓", "←"};


Map::Map(int size1, int **matrix1, Light *light1, int lightNum1) : size(size1), lightNum(lightNum1) {
    lights = new Light[lightNum];
    matrix = new int *[size];
    for (int i = 0; i < size; i++)
        matrix[i] = new int[size];
    mapNodes = new Node *[size];
    for (int i = 0; i < size; i++) {
        mapNodes[i] = new Node[size];
    }

    for (int i = 0; i < lightNum; i++) {
        lights[i] = {light1[i].x, light1[i].y, light1[i].status};
        mapNodes[light1[i].x][light1[i].y].setHasLight(true);
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = matrix1[i][j];
            mapNodes[i][j].setX(i);
            mapNodes[i][j].setY(j);
            mapNodes[i][j].setVisited(false);
            mapNodes[i][j].setHeight(matrix1[i][j]);
        }
    }
}

int Map::boundCheck(const Robot *robot) {
    int tempy = robot->y + (2 - robot->direction) % 2;
    int tempx = robot->x + (robot->direction - 1) % 2;
    if (tempy > -1 && tempy < size && tempx > -1 && tempx < size)
        return 1;
    else
        return 0;
}

int Map::judgeF(const Robot *robot) {
    int tempy = robot->y + (2 - robot->direction) % 2;
    int tempx = robot->x + (robot->direction - 1) % 2;
    return matrix[tempx][tempy] - robot->height;
}

int Map::turnLight(const Robot *robot) {
    int on = lightNum;
    for (int i = 0; i < lightNum; i++) {
        if (lights[i].status == 0) {
            on--;
            if (robot->x == lights[i].x && robot->y == lights[i].y) {
                lights[i].status = 1;
                on++;
            }
        }
    }
    return (lightNum - on);
}

int Map::isEnded() {
    int end = 1;
    for (int i = 0; i < lightNum; i++) {
        if (lights[i].status == 0) {
            end = 0;
            break;
        }
    }
    return end;
}

void Map::path(Node *root, int direction) {
    queue<Node *> nodes;
    root->setFather(NULL);
    root->setDirection(direction);
    Node *lightNode;
    bool finded = false;
    int lightFinded = 0;
    while (lightFinded != lightNum) {
        nodes.push(root);
        while (!nodes.empty()) {
            Node *tmpNode = nodes.front();
            nodes.pop();
            if (tmpNode->getHasLight() && !tmpNode->getVisited()) {
                lightFinded++;
                finded = true;
                tmpNode->setHasLight(false);
                root = tmpNode;
                root->setVisited(true);
                lightNode = tmpNode;
                break;
            }
            tmpNode->setVisited(true);
            int x = tmpNode->getX();
            int y = tmpNode->getY();
            int height = tmpNode->getHeight();
            //up
            if (x > 0 && (!mapNodes[x - 1][y].getVisited()) && (mapNodes[x - 1][y].getHeight() - height) <= 1) {
                mapNodes[x - 1][y].setDirection(0);
                mapNodes[x - 1][y].setFather(tmpNode);
                nodes.push(&(mapNodes[x - 1][y]));
            }
            //right
            if (y < (size - 1) && (!mapNodes[x][y + 1].getVisited()) &&
                (mapNodes[x][y + 1].getHeight() - height) <= 1) {
                mapNodes[x][y + 1].setDirection(1);
                mapNodes[x][y + 1].setFather(tmpNode);
                nodes.push(&(mapNodes[x][y + 1]));
            }
            //down
            if (x < (size - 1) && (!mapNodes[x + 1][y].getVisited()) &&
                (mapNodes[x + 1][y].getHeight() - height) <= 1) {
                mapNodes[x + 1][y].setDirection(2);
                mapNodes[x + 1][y].setFather(tmpNode);
                nodes.push(&(mapNodes[x + 1][y]));
            }
            //left
            if (y > 0 && (!mapNodes[x][y - 1].getVisited()) && (mapNodes[x][y - 1].getHeight() - height) <= 1) {
                mapNodes[x][y - 1].setDirection(3);
                mapNodes[x][y - 1].setFather(tmpNode);
                nodes.push(&(mapNodes[x][y - 1]));
            }
        }
        //print the path
        if (finded) {
            Node *iNode = lightNode;
            stack<Node *> pathNodes;
            while (iNode != NULL) {
                pathNodes.push(iNode);
                iNode = iNode->getFather();
            }
            int tmpDn = 0;
            int nextDn = 0;
            while (pathNodes.size() > 1) {
                Node *tmp = pathNodes.top();
                pathNodes.pop();
                tmpDn = tmp->getDirection();
                Node *next = pathNodes.top();
                nextDn = next->getDirection();
                if ((((tmpDn + 1) + 4) % 4) == nextDn)
                    cout << "R";
                else if ((((tmpDn - 1) + 4) % 4) == nextDn)
                    cout << "L";
                else if (((tmpDn - nextDn) == 2) || ((tmpDn - nextDn) == -2))
                    cout << "LL";

                if (next->getHeight() > tmp->getHeight())
                    cout << "J";
                else
                    cout << "F";
            }
            cout << "O";
        } else {
            cout << "you can't reach the light position ~.";
            break;
        }

        //init map nodes for next search
        int rdirection = root->getDirection();
        NodesInit();
        root->setDirection(rdirection);
        finded = false;
        queue<Node *> empty;
        swap(nodes, empty);
    }
    cout << endl;
    NodesInit();
    for (int i = 0; i < lightNum; i++) {
        mapNodes[lights[i].x][lights[i].y].setHasLight(true);
    }
}

void Map::NodesInit() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mapNodes[i][j].setVisited(false);
            mapNodes[i][j].setDirection(-1);
            mapNodes[i][j].setFather(NULL);
        }
    }
}

void Map::print(const Robot *robot) {
    string up = "┬";
    string down = "┴";
    string left = "├";
    string right = "┤";
    string rightUp = "┐";
    string leftUp = "┌";
    string leftDown = "└";
    string rightDown = "┘";
    string internal = "┼";
    string hline = "─";
    string vline = "│";

    int rx = robot->x, ry = robot->y, direction = robot->direction;
    for (int i = 0; i < 2 * size + 1; i++) {
        for (int j = 0; j < 2 * size + 1; j++) {
            if (i % 2 == 0) {
                if (i == 0) {
                    if (j == 0)
                        cout << leftUp;
                    else if (j == 2 * size)
                        cout << rightUp << endl;
                    else if (j % 2 == 0)
                        cout << up;
                    else
                        cout << hline;
                } else if (i == 2 * size) {
                    if (j == 0)
                        cout << leftDown;
                    else if (j == 2 * size)
                        cout << rightDown << endl;
                    else if (j % 2 == 0)
                        cout << down;
                    else
                        cout << hline;
                } else {
                    if (j == 0)
                        cout << left;
                    else if (j == 2 * size)
                        cout << right << endl;
                    else if (j % 2 == 0)
                        cout << internal;
                    else
                        cout << hline;
                }
            } else {
                if (j % 2 == 0)
                    cout << vline;
                else {
                    int i0 = i / 2, j0 = j / 2;
                    if (i0 == rx && j0 == ry) {
                        cout << directionsStr[direction];
                        continue;
                    }
                    int isLight = 0;
                    for (int k = 0; k < lightNum; k++) {
                        if (i0 == lights[k].x && j0 == lights[k].y) {
                            if (lights[k].status == 0)
                                cout << lightOff[matrix[i0][j0]];
                            else
                                cout << lightOn[matrix[i0][j0]];
                            isLight = 1;
                            break;
                        }
                    }
                    if (isLight)
                        continue;
                    if (matrix[i0][j0] == 0)
                        cout << "  ";
                    else
                        cout << matrix[i0][j0] << " ";
                }
                if (j == 2 * size)
                    cout << "\n";
            }
        }

    }
}