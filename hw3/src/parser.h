#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <vector>

#include "coordinate.h"


void parse_input(std::ifstream &input, int &colNum, int &rowNum, std::vector<Block> &blocks, std::vector<Net> &netPins);
void output_routing(std::ofstream &output, std::vector<Net> &netPins);
