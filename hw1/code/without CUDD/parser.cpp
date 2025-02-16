#include "parser.h"

void read_data(ifstream &input, string &function, vector<string> &var_orders){
    input >> function; //read the boolean equation
    function.pop_back();
    string tmp;
    while(input >> tmp){
        tmp.pop_back();
        var_orders.push_back(tmp);
    }
}

void parse_products(vector<string> &products, string function){
    string delimeter = "+";
    string token;
    size_t pos = 0;

    while((pos = function.find(delimeter)) != std::string::npos){
        token = function.substr(0, pos);
        products.push_back(token);
        function.erase(0, pos + delimeter.length());
    }
    token = function.substr(0, pos);
    products.push_back(token);
    function.erase(0, pos + delimeter.length());
}