#include "router.h"



void create_grid(std::vector<std::vector<int>> &grid, const int &colNum, const int &rowNum, const std::vector<Block> &blocks, const std::vector<Net> &netPins){
    for(auto &b : blocks){
        int up, down, left, right;
        up = b.rightUp.y, down = b.leftDown.y, left = b.leftDown.x, right = b.rightUp.x;
        for(int i=down; i<=up; i++){
            for(int j=left; j<=right; j++){
                grid[i][j] = BLOCKED;
            }
        }
    }

    for(auto &n : netPins){
        int sx, sy, tx, ty;
        sx = n.source.x, sy = n.source.y, tx = n.target.x, ty = n.target.y;
        grid[sy][sx] = PIN, grid[ty][tx] = PIN;
    }

//    for(int i=0; i<grid.size(); i++){
//        for(int j=0; j<grid[i].size(); j++){
//            printf("%2d ", grid[i][j]);
//        }
//        printf("\n");
//    }

}


void set_route_order(std::vector<int> &routeOrder, std::vector<Net> netPins){
    std::vector<std::pair<int, int>> order;
    for(int i=0; i<netPins.size(); i++)
        order.push_back(std::pair<int, int>(i, netPins[i].hpwl()));
    std::sort(order.begin(), order.end(), pair_compair);
    for(auto &o : order)
        routeOrder.push_back(o.first);

}


bool check_overlap(){

}


bool do_route(std::vector<std::vector<int>> &grid, std::vector<std::vector<int>> &distGrid, std::vector<Net> &netPins, int order){
    //printf("net[%d] routing\n", order);
    Net &net = netPins[order];
    int rowNum = distGrid.size(), colNum = distGrid[0].size();
    std::queue<Coordinate> q;
    Coordinate now;
    bool arrived = false;
    q.push(net.source);
    while(!q.empty()){
        now = q.front();
        std::vector<Coordinate> adjacent;
        if(now.y-1 >= 0)
            adjacent.push_back(now.down());
        if(now.y+1 < rowNum)
            adjacent.push_back(now.up());
        if(now.x-1 >= 0)
            adjacent.push_back(now.left());
        if(now.x+1 < colNum)
            adjacent.push_back(now.right());
        for(auto &c : adjacent){
            if(c == net.target){
                //printf("arrived\n");
                arrived = true;
                break;
            }
            else if(distGrid[c.y][c.x] == EMPTY){
                q.push(c);
                if(now == net.source)
                    distGrid[c.y][c.x] = 1;
                else
                    distGrid[c.y][c.x] = distGrid[now.y][now.x]+1;
            }
        }
        if(arrived)
            break;
        q.pop();
    }
    if(!arrived){
        return arrived;
    }
    std::vector<Coordinate> path;
    path.push_back(now);
    grid[now.y][now.x] = WIRE;//put now into wire
    while(distGrid[now.y][now.x] > 1){
        std::vector<Coordinate> adjacent;
        if(now.y-1 >= 0)
            adjacent.push_back(now.down());
        if(now.y+1 < rowNum)
            adjacent.push_back(now.up());
        if(now.x-1 >= 0)
            adjacent.push_back(now.left());
        if(now.x+1 < colNum)
            adjacent.push_back(now.right());
        for(auto &c : adjacent){
            if(distGrid[c.y][c.x] == distGrid[now.y][now.x] - 1){
                now = c;
                path.push_back(c);
                grid[c.y][c.x] = WIRE;
                break;
            }
        }
    }
    netPins[order].condense_route(path);
    //netPins[order].print();
    return arrived;
}

void rip_wire(std::vector<std::vector<int>> &grid, std::vector<Net> &netPins, int order){
    //printf("ripping net[%d]\n", order);
    int direction;
    Net &net = netPins[order];
    Coordinate now = net.source;
    direction = find_direction(now, net.routePath[0]);
    switch(direction){
        case LEFT:
            now = now.left();
            break;
        case RIGHT:
            now = now.right();
            break;
        case UP:
            now = now.up();
            break;
        case DOWN:
            now = now.down();
            break;
        default:
            break;
    }
    while(!(now == net.routePath[0])){
        grid[now.y][now.x] = EMPTY;
        switch(direction){
            case LEFT:
                now = now.left();
                break;
            case RIGHT:
                now = now.right();
                break;
            case UP:
                now = now.up();
                break;
            case DOWN:
                now = now.down();
                break;
            default:
                break;
        }
    }
    now = net.routePath[0];
    for(int i=0; i<net.routePath.size()-1; i++){
        direction = find_direction(net.routePath[i], net.routePath[i+1]);
        while(!(now == net.routePath[i+1])){
            //now.print();
            //net.routePath[i].print();
            //printf("d:%d\n", direction);
            grid[now.y][now.x] = EMPTY;
            switch(direction){
                case LEFT:
                    now = now.left();
                    break;
                case RIGHT:
                    now = now.right();
                    break;
                case UP:
                    now = now.up();
                    break;
                case DOWN:
                    now = now.down();
                    break;
                default:
                    break;
            }
        }
    }
    now = net.routePath.back();
    direction = find_direction(now, net.target);
    while(!(now == net.target)){
        grid[now.y][now.x] = EMPTY;
        switch(direction){
            case LEFT:
                now = now.left();
                break;
            case RIGHT:
                now = now.right();
                break;
            case UP:
                now = now.up();
                break;
            case DOWN:
                now = now.down();
                break;
            default:
                break;
        }
    }
    net.routePath.clear();
}


//compair function for sort has to be placed at same cpp
bool hpwl_compair(Net a, Net b){
    int hpwlA, hpwlB;
    hpwlA = abs(a.source.x - a.target.x) + abs(a.source.y - a.target.y);
    hpwlB = abs(b.source.x - b.target.x) + abs(b.source.y - b.target.y);
    return hpwlA < hpwlB;
}

bool pair_compair(std::pair<int, int> a, std::pair<int, int> b){
    return a.second < b.second;
}


bool operator==(const Coordinate &a, const Coordinate &b){
    return (a.x == b.x) && (a.y == b.y);
}