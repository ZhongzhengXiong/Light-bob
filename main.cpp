#include <iostream>
#include <fstream>
#include "Map.h"

#include <windows.h>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <stack>

#define RANKSIZE 5
#define DEFAULT_MAPSIZE 8
#define MAX_LIGHTS_NUM 10
#define MAX_HEIGHT 6

typedef long time_t;

using namespace std;



struct RankItem {
    int order;
    string name;
    int time;
};


void rankFile(string filename, string addName, int addTime);

void showRank(string filename);

void randomMap(int level);


void load(string filename);

void store(string filename);

void save();

Map *map;
Robot *robot;
int usedTime;

string helpstr = "Usage : [option] [args...]\n"
        "\n"
        "Where options include:\n"
        "        -go     execute the args gived,F for forward, R for turn right, L for turn left, J for jump, O for turn the light on\n"
        "        -reset  go back to the original status\n"
        "        -level  pick level 1~9\n"
        "        -help   print the help message\n"
        "        -end    end the game\n";

int main() {
    SetConsoleOutputCP(65001);
    randomMap(1);
    load("random.txt");
    time_t begin, end;

    cout << "game begins" << endl;
    map->print(robot);
    cout << "you have used " << usedTime << "s" << endl;
    begin = time(NULL) - usedTime;

    map->path(&(map->getMapNodes()[robot->getX()][robot->getY()]), robot->getDirection());

    rankFile("rank.txt", "xzz", 2);
    showRank("rank.txt");

    while (true) {
        string option;
        cin >> option;
        if (option == "rank") {

        }
        if (option == "h") {
            cout << helpstr;
        }
        if (option == "save") {
            end = time(NULL);
            usedTime = end - begin;
            save();
            map->print(robot);
        }
        if (option == "load") {
            cout << "Do you want to give up this game and begin a new game? (Y/N): ";
            while (cin >> option) {
                if (option == "Y") {
                    cout << "Enter the file name you want to load: ";
                    string filename;
                    cin >> filename;
                    load(filename);
                    begin = time(NULL) - usedTime;
                    break;
                } else if (option == "N") {
                    break;
                } else {
                    cout << "Wrong Input, Please input Y or N: ";
                }
            };
            map->print(robot);
            cout << "you have used " << usedTime << "s" << endl;
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
                rankFile("rank.txt", addName, usedTime);
                break;
            }
        }
        if (option == "End") {
            end = time(NULL);
            usedTime = end - begin;
            save();
            break;
        }
    }
    return 0;
}


void save() {
    string option;
    cout << "Do you want to save the game(Y/N) ?";
    while (cin >> option) {
        if (option == "Y") {
            cout << "Enter the file name: ";
            string filename;
            cin >> filename;
            store(filename);
            break;
        } else if (option == "N") {
            break;
        } else {
            cout << "Wrong Input, Please input Y or N: ";
        }
    };
}


void store(string filename) {
    ofstream output(filename);
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
    output << usedTime;
    output.close();
}


void load(string filename) {
    ifstream in(filename);
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
    in.close();

    robot = new Robot(rx, ry, rd, matrix[rx][ry]);
    map = new Map(size, matrix, light, lightNum);
    usedTime = time;

    delete (light);
    for (int i = 0; i < size; i++)
        delete (matrix[i]);
}

void rankFile(string filename, string addName, int addTime) {
    int addOrder = -1;
    RankItem rankItem[RANKSIZE + 1];
    ifstream in(filename);
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

    ofstream out(filename);
    for (int i = 0; i < RANKSIZE; i++) {
        if (rankItem[i].order == -1)
            break;
        out << rankItem[i].order << "   " << rankItem[i].name << "   " << rankItem[i].time << endl;
    }
    out.close();
}


void showRank(string filename) {
    ifstream in(filename);
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


void randomMap(int level) {
    ofstream out("random.txt");
    if (level == 1) {
        int size = DEFAULT_MAPSIZE;
        int randomMap[size][size];
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                randomMap[i][j] = 0;
            }
        }
        srand((unsigned) time(NULL));
        int lightNum = (rand() % (MAX_LIGHTS_NUM)) + 1;
        int wallNum = lightNum;
        vector<int> positions;
        for (int i = 0; i < (size * size); i++)
            positions.push_back(i);
        random_shuffle(positions.begin(), positions.end());
        vector<int>::iterator iter = positions.begin();
        int lightPositions[lightNum];
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
        int direction = (rand() % 4);
        int rx = *iter / size;
        int ry = *iter % size;
        out << size << endl;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                out << randomMap[i][j] << " ";
            }
            out << endl;
        }
        out << rx << " " << ry << " " << direction << endl;
        out << lightNum << endl;
        for (int i = 0; i < lightNum; i++) {
            int x = lightPositions[i] / size;
            int y = lightPositions[i] % size;
            out << x << " " << y << endl;
        }

    }
    if (level == 2) {
        int size = DEFAULT_MAPSIZE;
        int randomMap[size][size];
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                randomMap[i][j] = 0;
            }
        }

        srand((unsigned) time(NULL));
        int x = (rand() % (size - 2)) + 1;
        int y = (rand() % (size - 2)) + 1;
        stack<int> path;
        cout << "position" << endl;

        for(int i = 1; i < (MAX_HEIGHT + 1); i++){
            cout << x << "  " << y << endl;
            randomMap[x][y] = i;
            path.push(x * size + y);
            int tmpx, tmpy;
            do{
                int direction = (rand() % 4);
                tmpy = y + (2 - direction) % 2;
                tmpx = x + (direction - 1) % 2;
            }while(!(tmpx > 0 && tmpx < (size - 1) && tmpy > 0 && tmpy < (size - 1) && randomMap[tmpx][tmpy] == 0) );
            x = tmpx;
            y = tmpy;
        }
        vector<int> wallPositions;
        int pheight = MAX_HEIGHT;
        while(!path.empty()){
            int pp = path.top();
            int px = pp / size;
            int py = pp % size;
            wallPositions.push_back(pp);
            for(int i = 0; i < 4; i++){
                int npx = px + (i - 1) % 2;
                int npy = py + (2 - i) % 2;
                if(npx > 0 && npx < (size - 1) && npy > 0 && npy < (size - 1) && randomMap[npx][npy] == 0 ){
                    randomMap[npx][npy] = pheight - (rand() % 2);
                    wallPositions.push_back(npx * size + npy);
                }
            }
            pheight--;
            path.pop();
        }
        int lightNum = (rand() % (MAX_LIGHTS_NUM / 2)) + (MAX_LIGHTS_NUM / 2);
        random_shuffle(wallPositions.begin(), wallPositions.end());
        vector<int>::iterator iter1 = wallPositions.begin();
        int lightPositions[lightNum];
        for(int i = 0; i < lightNum/2; i++){
            lightPositions[i] = *iter1;
            iter1++;
        }
        vector<int> floors;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(randomMap[i][j] == 0)
                    floors.push_back(i*size + j);
            }
        }
        random_shuffle(floors.begin(), floors.end());
        vector<int>::iterator iter2 = floors.begin();
        for(int i = lightNum/2; i < lightNum; i++){
            lightPositions[i] =  *iter2;
            iter2++;
        }
        int direction = (rand() % 4);
        int rx = *iter2 / size;
        int ry = *iter2 % size;
        out << size << endl;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                out << randomMap[i][j] << " ";
            }
            out << endl;
        }
        out << rx << " " << ry << " " << direction << endl;
        out << lightNum << endl;
        for (int i = 0; i < lightNum; i++) {
            int x = lightPositions[i] / size;
            int y = lightPositions[i] % size;
            out << x << " " << y << endl;
        }
    }
    if (level == 3) {


    }
    out.close();
}

