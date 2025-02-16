#include "parser.h"


void parse_input(std::ifstream &input, int &colNum, int &rowNum, std::vector<Block> &blocks, std::vector<Net> &netPins){
    std::string rowStr, colStr, dump, blockNumStr, netNumStr;
    int blockNum, netNum, forCoord[4];
    Block tmpBlk;
    Net tmpNet;

    input >> dump >> rowStr;
    input >> dump >> colStr;
    rowNum = std::stoi(rowStr);
    colNum = std::stoi(colStr);
    input >> dump >> blockNumStr;
    blockNum = std::stoi(blockNumStr);
    for(int i=0; i<blockNum; i++){
        Coordinate leftDown, rightUp;
        for(int j=0; j<4; j++){
            input >> dump;
            forCoord[j] = std::stoi(dump);//getting the coordinates
        }
        leftDown.x = forCoord[0], rightUp.x = forCoord[1];
        leftDown.y = forCoord[2], rightUp.y = forCoord[3];
        tmpBlk.leftDown = leftDown, tmpBlk.rightUp = rightUp;
        blocks.push_back(tmpBlk);
    }
    input >> dump >> netNumStr;
    netNum = std::stoi(netNumStr);
    for(int i=0; i<netNum; i++){
        Coordinate source, target;
        input >> dump;
        tmpNet.name = dump;
        for(int j=0; j<4; j++){
            input >> dump;
            forCoord[j] = std::stoi(dump);//getting the coordinates
        }
        source.x = forCoord[0], source.y = forCoord[1];
        target.x = forCoord[2], target.y = forCoord[3];
        tmpNet.source = source, tmpNet.target = target;
        netPins.push_back(tmpNet);
    }

}

void output_routing(std::ofstream &output, std::vector<Net> &netPins){
    for(auto &n : netPins)
        n.output_to_file(output);
    
}