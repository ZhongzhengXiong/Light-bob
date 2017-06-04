//
// Created by XiongZZ on 2017/6/4.
//

#ifndef LIGHT_BOT_ROBOT_H
#define LIGHT_BOT_ROBOT_H



class Robot {
    int x, y;
    int direction;
    int height;
public:
    friend class Map;
    Robot(int x1, int y1, int direction1, int height): x(x1), y(y1), direction(direction1), height(height){}
    void turnLeft();
    void turnRight();
    void jump(int height1);
    void goForward();

};


#endif //LIGHT_BOT_ROBOT_H
