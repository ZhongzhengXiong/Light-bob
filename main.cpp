#include <iostream>
#include <fstream>
#include "Map.h"

using namespace std;

int main() {

    ifstream in("1.txt");
    int size;
    in >> size;
    int **matrix;
    matrix = new int*[size];
    for(int i = 0; i < size; i++){
        matrix[i] = new int[size];
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            in >> matrix[i][j];
        }
    }
    int rx, ry, rd;
    in >> rx;
    in >> ry;
    in >> rd;
    int lightNum;
    in >> lightNum;
    Light* light;
    light = new Light[lightNum];
    for(int i = 0; i < lightNum; i++){
        int lx, ly;
        in >> lx;
        in >> ly;
        light[i] = {lx, ly, 0};
    }
    in.close();

    Map map(size, matrix, light, lightNum);
    Robot robot(rx, ry, rd, 0);

    delete(light);
    for(int i = 0; i < size; i++)
        delete(matrix[i]);

    cout << "game begins" << endl;
    map.print(robot);
    while(true){
        string option;
        cin >> option;
        if(option == "go"){
            string cmnd;
            getline(cin, cmnd);
            for(int i = 0; i < cmnd.length(); i++){
                char ch = cmnd[i];
                if(ch == 'L')
                    robot.turnLeft();
                if(ch == 'R')
                    robot.turnRight();
                if(ch == 'F' &&  map.boundCheck(robot) && map.judgeF(robot) < 1 ){
                    robot.jump(map.judgeF(robot));
                }
                if(ch == 'J' &&  map.boundCheck(robot) && map.judgeF(robot) == 1)
                    robot.jump(map.judgeF(robot));
                if(ch == 'O')
                    map.turnLight(robot);
            }
            map.print(robot);
            if(map.isEnded()){
                cout << "you win" << endl;
                break;
            }
        }
        if(option == "End")
            break;
    }

    return 0;
}