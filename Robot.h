//
// Created by XiongZZ on 2017/6/4.
//

#ifndef LIGHT_BOT_ROBOT_H
#define LIGHT_BOT_ROBOT_H

#include <string>

class Map;


class Robot {
    int x, y;
    int direction;
    int height;
public:
    friend class Map;
    friend class Game;
    friend void store(std::string filename);
    Robot(){};
    Robot(int x1, int y1, int direction1, int height): x(x1), y(y1), direction(direction1), height(height){}
    Robot(const Robot& robot){
        x = robot.x;
        y = robot.y;
        direction = robot.direction;
        height = robot.height;
    }
    void turnLeft();
    void turnRight();
    void jump(int height1);
    void goForward();
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
    void setX(int x1){
        x = x1;
    }
    void setY(int y1){
        y = y1;
    }
    int getHeight(){
        return height;
    }
    void setHeight(int height1){
        height = height1;
    }
    int getDirection(){
        return direction;
    }
};


#endif //LIGHT_BOT_ROBOT_H
