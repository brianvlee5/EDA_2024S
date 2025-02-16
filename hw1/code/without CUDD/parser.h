#pragma once

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>

using namespace std;

void read_data(ifstream &input, string &function, vector<string> &var_orders);
void parse_products(vector<string> &products, string function);