#include "coordinate.h"


//----------------------------coordinate------------------------------------------
void Coordinate::print(){
    printf("Coordinate:(%d, %d)\n", x, y);
}


Coordinate Coordinate::left(){
    return Coordinate(x-1, y);
}

Coordinate Coordinate::right(){
    return Coordinate(x+1, y);
}

Coordinate Coordinate::up(){
    return Coordinate(x, y+1);
}

Coordinate Coordinate::down(){
    return Coordinate(x, y-1);
}

//----------------------------net----------------------------------------------

void Net::condense_route(const std::vector<Coordinate> &path){
    int direction, lastDirection = find_direction(source, path.back());
    gridUsage = path.size();
    for(int i=path.size()-1; i>0; i--){
        direction = find_direction(path[i], path[i-1]);
        if(lastDirection != direction)
            routePath.push_back(path[i]);
        lastDirection = direction;
    }
    direction = find_direction(path[0], target);
    if(lastDirection != direction)
        routePath.push_back(path[0]);
    lastDirection = direction;
}

void Net::print(){
    std::cout << name << " " << gridUsage << std::endl;
    printf("begin\n");
    printf("%d %d %d %d\n", source.x, source.y, routePath[0].x, routePath[0].y);
    for(int i=0; i<routePath.size()-1; i++)
        printf("%d %d %d %d\n", routePath[i].x, routePath[i].y, routePath[i+1].x, routePath[i+1].y);
    printf("%d %d %d %d\n", routePath.back().x, routePath.back().y, target.x, target.y);
    printf("end\n");
}


void Net::output_to_file(std::ofstream &output){
    output << name << " " << gridUsage << std::endl;
    output << "begin\n";
    output << source.x << " " << source.y << " " << routePath[0].x << " " << routePath[0].y << "\n";
    for(int i=0; i<routePath.size()-1; i++)
        output << routePath[i].x << " " << routePath[i].y << " " << routePath[i+1].x << " " << routePath[i+1].y << "\n";
    output << routePath.back().x << " " << routePath.back().y << " " << target.x << " " << target.y << "\n";
    output << "end\n";
}

int Net::hpwl(){
    return abs(source.x - target.x) + abs(source.y - target.y);
}



//----------------------------other----------------------------------------------
int find_direction(const Coordinate &i, const Coordinate &iNext){
    //printf("%d %d : %d %d\n", i.x, i.y, iNext.x, iNext.y);
    if(i.x < iNext.x && i.y == iNext.y)
        return RIGHT;
    else if(i.x > iNext.x && i.y == iNext.y)
        return LEFT;
    else if(i.x == iNext.x && i.y < iNext.y)
        return UP;
    else if(i.x == iNext.x && i.y > iNext.y)
        return DOWN;
}