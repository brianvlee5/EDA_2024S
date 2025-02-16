#pragma once
#include <vector>
#include <queue>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <utility>

#include "coordinate.h"

#define WIRE -3
#define PIN -2
#define BLOCKED -1
#define EMPTY 0

void create_grid(std::vector<std::vector<int>> &grid, const int &colNum, const int &rowNum, const std::vector<Block> &blocks, const std::vector<Net> &netPins);

void set_route_order(std::vector<int> &routeOrder, std::vector<Net> netPins);

bool check_overlap();

bool do_route(std::vector<std::vector<int>> &grid, std::vector<std::vector<int>> &distGrid, std::vector<Net> &netPins, int order);

void rip_wire(std::vector<std::vector<int>> &grid, std::vector<Net> &netPins, int order);

bool hpwl_compair(Net a, Net b);
bool pair_compair(std::pair<int, int> a, std::pair<int, int> b);

bool operator==(const Coordinate &a, const Coordinate &b);