#pragma once

#include <stdio.h>
#include "iostream"
#include "cudd-3.0.0/cudd/cudd.h"

using namespace std;

void example();
void write_dd(DdManager *gbm, DdNode *bdd, char* filename);