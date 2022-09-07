#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <stdlib.h>

using namespace std;

int maph = 10;
int mapw = 10;
char map[10][10] =

{
    { 2,0,0,1,0,1,0,1,0,0},
    { 1,1,0,1,0,1,0,1,0,0},
    { 0,1,0,1,0,1,0,1,0,0},
    { 0,1,0,1,0,1,0,0,0,0},
    { 0,1,0,1,0,1,1,1,1,0},
    { 0,1,0,1,0,1,0,0,0,0},
    { 0,1,0,1,0,1,0,1,0,1},
    { 0,1,0,0,0,1,0,1,0,0},
    { 0,1,1,1,0,1,0,1,0,0},
    { 0,0,0,1,0,0,0,1,1,9}
};
/*
{
    { 0,0,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
    { 1,1,0,0,0,0,0,0,0,0},
    { 0,1,0,0,0,0,0,0,0,0},
    { 2,1,0,0,0,0,0,0,0,9},
    { 0,1,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0,0,0,0},
};
*/
const int dir_x[4] = { 0, 0, 1, -1 };
const int dir_y[4] = { -1, 1, 0, 0 };


/* Coord: 한 정점의 좌표 */
class Coord {
public:
    Coord(int _x, int _y) : x(_x), y(_y) {}
    int x;
    int y;
};

// 셀 --> 지도의 동일한 크기의 셀[10][10] 이 필요~
// 노드의 비용과 부모의 정보를 담는다.
class Cell {

public:
    Cell()
    {
        parent_x = parent_y = -1;
        visit = false;
    }
    bool visit;
    int parent_x;
    int parent_y;
};

vector<Coord> TrackingQueue;



bool isInMap(int x, int y)
{
    if (x >= 0 && x < mapw && y >=0 && y < maph) return true;
    return false;
}

bool isDestination(int x, int y)
{
    if (map[y][x] == 9) return true;
    return false;
}

bool isWall(int x, int y)
{
    if (map[y][x] == 1) return true;
    return false;
}

vector<vector<Cell>> cell(maph, vector<Cell>(mapw));
queue<Coord> searchList; 

int start_x = 0;
int start_y = 5;
int dest_x = 9;
int dest_y = 5;


void init()
{
    // 출발지점의 비용값은 0, 부모를 출발지로 넣어준다.
    Cell& startcell = cell[start_y][start_x];
    startcell.parent_x = start_x;
    startcell.parent_y = start_y;

    searchList.push(Coord(start_x, start_y));
}

int TrackingPath(int dx, int dy)
{
    stack<Coord> TrackingStack;
    int cx, cy;

    cx = dx;
    cy = dy;

    TrackingStack.push({ cx, cy });

    while (!(cell[cy][cx].parent_x == cx &&
        cell[cy][cx].parent_y == cy)) {
        int temp_x = cell[cy][cx].parent_x;
        int temp_y = cell[cy][cx].parent_y;
        cx = temp_x;
        cy = temp_y;
        TrackingStack.push({ cx, cy });
    }

    while (!TrackingStack.empty()) {
        TrackingQueue.push_back(TrackingStack.top());
        TrackingStack.pop();
    }

    return 0;
}


int bfs_search()
{

    while (!searchList.empty()) {

        Coord current = searchList.front();
        searchList.pop();

        int cx = current.x;
        int cy = current.y;

        // 현지점의 방문처리
        cell[cy][cx].visit = true;

        float nf, ng, nh;

        // 동서남북 방향에 대해서 계산
        for (int i = 0; i < 4; ++i) {
            int nx = cx + dir_x[i];
            int ny = cy + dir_y[i];
                
            // 여기가 지도안인가? 아니면 당연히 쳐다볼필요가 없지...^^;
            if (isInMap(nx, ny)) {

                Cell& nextcell = cell[ny][nx];

                // 도착했나?
                if (isDestination(nx, ny)) {
                    nextcell.parent_x = cx;
                    nextcell.parent_y = cy;

                    // 도착되어 cell의 parent를 역추적해서 경로를 빼내올수 있다.
                    TrackingPath(dest_x,dest_y);

                    return true;
                }
                // closedList 첵크해서 전에 방문했다고 해둔곳 은 또 볼필요없고...
                // 벽이 아닌곳인가?
                else if (!cell[ny][nx].visit && !isWall( nx, ny)) {

                        // 다음셀에 바로 직전 좌표도 넣어주어주면
                        // 목표점에 도달았을때 다시 되돌아갈수 있다.. 이걸 꺼꾸러하면 찾아갈수 있는 경로도 된다.
                        nextcell.parent_x = cx;
                        nextcell.parent_y = cy;

                        searchList.push(Coord(nx,ny));
                }
            }
        }

    }

    return 0;
}



void print_map(int cx, int cy)
{
    for (int y = 0; y < maph; y++)
    {
        for (int x = 0; x < mapw; x++) {
            
            if (x == cx && y == cy)
                cout << "@";
            else
                if (map[y][x] == 0 || map[y][x] == 2) cout << " ";
                else if (map[y][x] == 9) cout << "*";
                else cout << "#";
        }
        cout << endl;
    }
}

void autoset_start_dest_position()
{
    for (int y = 0; y < maph; y++)
    {
        for (int x = 0; x < mapw; x++) {
            if (map[y][x] == 2) {
                start_x = x;
                start_y = y;
            }
            else if (map[y][x] == 9) {
                dest_x = x;
                dest_y = y;

            }
        }
    }
}


int main()
{
    autoset_start_dest_position();
    init();
    bfs_search();

    for (auto v = TrackingQueue.begin(); v < TrackingQueue.end(); v++)
    {
        system("cls");
        print_map(v->x, v->y);
    }
    
    return 0;
}
