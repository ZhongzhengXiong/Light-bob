//
// Created by XiongZZ on 2017/6/4.
//
#include "Robot.h"


void Robot::goForward() {
    y = y + (2 - direction) % 2;
    x = x + (direction - 1) % 2;
}

void Robot::turnLeft() {
    direction = ((direction-1) % 4 + 4) % 4;
}

void Robot::turnRight() {
    direction = (direction + 1) % 4;
}

void Robot::jump(int height1) {
    goForward();
    height += height1;
}




