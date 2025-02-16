#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

class Coordinate{
public:
    int x, y;
    Coordinate(int x=0, int y=0): x(x), y(y){};

    void print();
    Coordinate left();
    Coordinate right();
    Coordinate up();
    Coordinate down();
};

struct Block{
    Coordinate leftDown, rightUp;
};

class Net{
public:
    std::string name;
    std::vector<Coordinate> routePath;
    bool routed = false;
    int gridUsage = 0;
    Coordinate source, target;

    void condense_route(const std::vector<Coordinate> &path);
    void print();
    void output_to_file(std::ofstream &output);
    int hpwl();
};

int find_direction(const Coordinate &i, const Coordinate &iNext);