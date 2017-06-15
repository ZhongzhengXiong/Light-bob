//
// Created by XiongZZ on 2017/6/15.
//

#include "Game.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stack>
#include <sstream>

using namespace ::std;
typedef long time_t;


string directory[5] = {"Easy", "Difficult", "SuperDifficult", "Default", "Records"};
string helpstr = "Usage : [option] [args...]\n"
        "\n"
        "Where options include:\n"
        "        -go     execute the args gived,F for forward, R for turn right, L for turn left, J for jump, O for turn the light on\n"
        "        -reset  go back to the original status\n"
        "        -level  pick level 1~10, 10 means random\n"
        "        -save  save the file\n"
        "        -crank  clear\n"
        "        -srank  show the rank at this difficulty\n"
        "        -pattern  change the game pattern, e for easy, d for difficult, sd for super difficult, de for default, re for records\n"
        "        -help   print the help message\n"
        "        -hint   print hints\n"
        "        -end    end the game\n";


Game::Game(std::string filename1, int pattern1) {
    usedTime = 0;
    filename = filename1;
    pattern = pattern1;
    load();

}

void Game::control() {
    cout << "Game begins" << endl;
    map->print(robot);
    time_t begin, end;
    begin = time(NULL) - usedTime;
    while (true) {
        string option;
        cin >> option;
        if (option == "hint") {
            map->path(&(map->getMapNodes()[robot->getX()][robot->getY()]), robot->getDirection());
        }
        if (option == "crank") {
            clearRank();
        }
        if (option == "srank") {
            showRank();
        }
        if (option == "help") {
            cout << helpstr;
        }
        if (option == "reset") {
            usedTime = 0;
            load();
            map->print(robot);
            begin = time(NULL) - usedTime;
        }
        if (option == "level") {
            int level;
            cin >> level;
            if (level < 0 || level > 10)
                cout << "beyong the boundaries" << endl;
            else {
                stringstream ss;
                ss << level;
                filename = ss.str() + ".txt";
                if (level == 10)
                    filename = "random.txt";
                usedTime = 0;
                load();
                map->print(robot);
                begin = time(NULL) - usedTime;
            }
        }
        if (option == "pattern") {
            string pstr;
            cin >> pstr;
            if (pstr == "e") {
                pattern = 0;
            }
            if (pstr == "d") {
                pattern = 1;
            }
            if (pstr == "sd") {
                pattern = 2;
            }
            if (pstr == "de") {
                pattern = 3;
            }
            if (pstr == "re") {
                pattern = 4;
            }
            cout << "Now it's " << directory[pattern] << " pattern " << endl;
            cout << "Input the filename you want to load: ";
            string filestr;
            cin >> filestr;
            filestr += ".txt";
            filename = filestr;
            usedTime = 0;
            load();
            map->print(robot);
            begin = time(NULL) - usedTime;
        }
        if (option == "save") {
            end = time(NULL);
            usedTime = end - begin;
            save();
            map->print(robot);
        }
        if (option == "go") {
            string cmnd;
            getline(cin, cmnd);
            for (int i = 0; i < cmnd.length(); i++) {
                char ch = cmnd[i];
                if (ch == 'L')
                    robot->turnLeft();
                if (ch == 'R')
                    robot->turnRight();
                if (ch == 'F' && map->boundCheck(robot) && map->judgeF(robot) < 1) {
                    robot->jump(map->judgeF(robot));
                }
                if (ch == 'J' && map->boundCheck(robot) && map->judgeF(robot) == 1)
                    robot->jump(map->judgeF(robot));
                if (ch == 'O')
                    map->turnLight(robot);
            }
            map->print(robot);
            end = time(NULL);
            usedTime = end - begin;
            cout << "you have used " << usedTime << "s" << endl;
            if (map->isEnded()) {
                cout << "you win" << endl;
                cout << "Enter your name:  ";
                string addName;
                cin >> addName;
                rankFile(addName, usedTime);
                break;
            }
        }
        if (option == "end") {
            break;
        }
    }
}

void Game::load() {
    string filepath = directory[pattern] + "\\" + filename;
    if (filename == "random.txt")
        randomMap();
    ifstream in(filepath);
    if (!in) {
        cout << "wrong input, game exit." << endl;
        exit(0);
    }
    int size;
    in >> size;
    int **matrix;
    matrix = new int *[size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            in >> matrix[i][j];
        }
    }
    int rx, ry, rd;
    in >> rx;
    in >> ry;
    in >> rd;
    int lightNum;
    in >> lightNum;
    Light *light;
    light = new Light[lightNum];
    for (int i = 0; i < lightNum; i++) {
        int lx, ly;
        in >> lx;
        in >> ly;
        light[i] = {lx, ly, 0};
    }
    int status;

    for (int i = 0; i < lightNum; i++) {
        if (in >> status) {
            light[i].status = status;
        }
    }
    int time;
    if (!(in >> time))
        time = 0;
    int patt;
    if (!(in >> patt))
        patt = pattern;
    in.close();
    robot = new Robot(rx, ry, rd, matrix[rx][ry]);
    map = new Map(size, matrix, light, lightNum);
    usedTime = time;
    pattern = patt;
    delete (light);
    for (int i = 0; i < size; i++)
        delete (matrix[i]);

}


void Game::save() {
    string option;
    cout << "Do you want to save the game(Y/N) ?";
    while (cin >> option) {
        if (option == "Y") {
            cout << "Enter the file name: ";
            string filename;
            cin >> filename;
            filename += ".txt";
            store(filename);
            break;
        } else if (option == "N") {
            break;
        } else {
            cout << "Wrong Input, Please input Y or N: ";
        }
    };
}


void Game::store(std::string filename) {
    string filepath = "Records\\" + filename;
    ofstream output(filepath);
    output << map->size << endl;
    for (int i = 0; i < map->size; i++) {
        for (int j = 0; j < map->size; j++) {
            output << map->matrix[i][j] << " ";
        }
        output << endl;
    }
    output << robot->x << " " << robot->y << " " << robot->direction << endl;
    output << map->lightNum << endl;
    for (int i = 0; i < map->lightNum; i++) {
        output << map->lights[i].x << " " << map->lights[i].y << endl;
    }
    for (int i = 0; i < map->lightNum; i++) {
        output << map->lights[i].status << endl;
    }
    output << usedTime << endl;
    output << pattern;
    output.close();
}

void Game::rankFile(std::string addName, int addTime) {
    int addOrder = -1;
    RankItem rankItem[RANKSIZE + 1];
    string filepath = directory[pattern] + "\\rank.txt";
    ifstream in(filepath);
    bool isAdded = false;
    string rankString;
    for (int i = 0; i < RANKSIZE + 1; i++) {
        int order, time;
        string name;
        if ((in >> order) && (in >> name) && (in >> time)) {
            if (isAdded) {
                rankItem[i] = {order + 1, name, time};
            } else if (time <= addTime) {
                rankItem[i] = {order, name, time};
            } else {
                isAdded = true;
                if (i != 0 && addTime == rankItem[i - 1].time)
                    addOrder = rankItem[i - 1].order;
                else
                    addOrder = order;
                rankItem[i] = {addOrder, addName, addTime};
                rankItem[i + 1] = {order + 1, name, time};
                i++;
            }
        } else {
            rankItem[i] = {-1, "-1", -1};
        }
    }
    in.close();
    for (int i = 0; i < RANKSIZE; i++) {
        if (rankItem[i].order == -1 && !isAdded) {
            if (i != 0 && addTime == rankItem[i - 1].time)
                rankItem[i] = {rankItem[i - 1].order, addName, addTime};
            else
                rankItem[i] = {i + 1, addName, addTime};
            break;
        }
    }
    ofstream out(filepath);
    for (int i = 0; i < RANKSIZE; i++) {
        if (rankItem[i].order == -1)
            break;
        out << rankItem[i].order << "   " << rankItem[i].name << "   " << rankItem[i].time << endl;
    }
    out.close();
}

void Game::showRank() {
    string filepath = directory[pattern] + "\\rank.txt";
    ifstream in(filepath);
    int order, time;
    string name;
    cout << "************RANK************" << endl;
    while ((in >> order) && (in >> name) && (in >> time)) {
        cout << setw(5) << left << order;
        cout << setw(15) << left << name;
        cout << setw(5) << left << time << endl;
    }
    cout << "****************************" << endl;
    in.close();
}

void Game::clearRank() {
    string filepath = directory[pattern] + "\\rank.txt";
    ofstream out(filepath);
    out.close();
}


void Game::randomMap() {
    string filepath = directory[pattern] + "\\random.txt";
    ofstream out(filepath);
    srand((unsigned) time(NULL));
    int size = DEFAULT_MAPSIZE;
    if (pattern == 2)
        size += 3;
    int max_height = MAX_HEIGHT;
    if (pattern == 2)
        max_height += 2;
    int max_lights_num = MAX_LIGHTS_NUM;
    if (pattern == 2)
        max_lights_num += 2;
    int randomMap[size][size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            randomMap[i][j] = 0;
        }
    }
    int lightNum = (rand() % (max_lights_num)) + 1;
    if (pattern != 1) {
        lightNum = (rand() % (max_lights_num / 2)) + (max_lights_num / 2);
    }
    int lightPositions[lightNum];
    int rdirection = (rand() % 4);
    int rx, ry;
    switch (pattern) {
        case 3:
        case 0: {
            int wallNum = lightNum;
            vector<int> positions;
            for (int i = 0; i < (size * size); i++)
                positions.push_back(i);
            random_shuffle(positions.begin(), positions.end());
            vector<int>::iterator iter = positions.begin();


            for (int i = 0; i < lightNum; i++) {
                lightPositions[i] = *iter;
                iter++;
            }
            for (int i = 0; i < wallNum; i++) {
                int x = *iter / size;
                int y = *iter % size;
                randomMap[x][y] = 1;
                iter++;
            }
            srand((unsigned) time(NULL));
            rx = *iter / size;
            ry = *iter % size;
            break;
        }
        case 1:
        case 2: {
            srand((unsigned) time(NULL));
            //generate an accessible path
            int x = (rand() % (size - 5)) + 2;
            int y = (rand() % (size - 5)) + 2;
            stack<int> path;
            for (int i = 1; i < (max_height + 1); i++) {
                randomMap[x][y] = i;
                path.push(x * size + y);
                if (i == max_height)
                    break;
                int tmpx, tmpy;
                do {
                    int direction = (rand() % 4);
                    tmpy = y + (2 - direction) % 2;
                    tmpx = x + (direction - 1) % 2;
                } while (!(tmpx > 0 && tmpx < (size - 1) && tmpy > 0 && tmpy < (size - 1) &&
                           randomMap[tmpx][tmpy] == 0));
                x = tmpx;
                y = tmpy;
            }

            //build walls along the path
            vector<int> wallPositions;
            int pheight = max_height;
            while (!path.empty()) {
                int pp = path.top();
                int px = pp / size;
                int py = pp % size;
                wallPositions.push_back(pp);
                for (int i = 0; i < 4; i++) {
                    int npx = px + (i - 1) % 2;
                    int npy = py + (2 - i) % 2;
                    if (npx > 0 && npx < (size - 1) && npy > 0 && npy < (size - 1) && randomMap[npx][npy] == 0) {
                        randomMap[npx][npy] = pheight - (rand() % 2);
                        wallPositions.push_back(npx * size + npy);
                    }
                }
                pheight--;
                path.pop();
            }
            //generate light position on walls
            random_shuffle(wallPositions.begin(), wallPositions.end());
            vector<int>::iterator iter1 = wallPositions.begin();
            for (int i = 0; i < lightNum / 2; i++) {
                lightPositions[i] = *iter1;
                iter1++;
            }
            //generate robot position and light position on floors
            vector<int> floors;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (randomMap[i][j] == 0)
                        floors.push_back(i * size + j);
                }
            }
            random_shuffle(floors.begin(), floors.end());
            vector<int>::iterator iter2 = floors.begin();
            for (int i = lightNum / 2; i < lightNum; i++) {
                lightPositions[i] = *iter2;
                iter2++;
            }
            rx = *iter2 / size;
            ry = *iter2 % size;
            break;
        }
    }
    out << size << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            out << randomMap[i][j] << " ";
        }
        out << endl;
    }
    out << rx << " " << ry << " " << rdirection << endl;
    out << lightNum << endl;
    for (int i = 0; i < lightNum; i++) {
        int x = lightPositions[i] / size;
        int y = lightPositions[i] % size;
        out << x << " " << y << endl;
    }
    out.close();
}
