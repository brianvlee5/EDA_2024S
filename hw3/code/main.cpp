#pragma once 
#include <fstream>
#include <stdio.h>

#include "parser.h"
#include "router.h"

int main(int argc, char* argv[]){
    std::ifstream input; 
    std::ofstream output;
    std::vector<Block> blocks;
    std::vector<Net> netPins;
    int colNum, rowNum;

    //----------------------------initializing---------------------------------------------
    input.open(argv[1]);
    output.open(argv[2]);
    parse_input(input, colNum, rowNum, blocks, netPins);
 
    //creating the grid
    std::vector<int> row(colNum, EMPTY);
    std::vector<std::vector<int>> grid(rowNum, row);
    std::vector<int> routeOrder;
    create_grid(grid, colNum, rowNum, blocks, netPins);
    set_route_order(routeOrder, netPins);

    //-------------------------------testbench----------------------------------------------
    //for(int i=0; i<distGrid.size(); i++){
    //    for(int j=0; j<distGrid[i].size(); j++){
    //        printf("%2d ", grid[i][j]);
    //    }
    //    printf("\n");
    //}


    //--------------------------------routing---------------------------------------------
    std::vector<std::vector<int>> distGrid = grid;

    int routedWires = 0;
    while(routedWires < netPins.size()){
        if(do_route(grid, distGrid, netPins, routeOrder[routedWires])){
            routedWires++;
        }
        else{
            routedWires--;
            rip_wire(grid, netPins, routeOrder[routedWires]);
            routeOrder.push_back(routeOrder[routedWires]);
            routeOrder.erase(routeOrder.begin()+routedWires);
        }
        distGrid = grid;
    }

    //--------------------------------output-----------------------------------------------
    output_routing(output, netPins);
    return 0;
}