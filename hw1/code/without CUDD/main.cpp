#pragma once
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <climits>
#include <string>
#include <algorithm>
#include <unordered_map>

#include "parser.h"

using namespace std;

struct Vertex{
    char var;
    Vertex* low;
    Vertex* high;

    Vertex(char v, Vertex* l = nullptr, Vertex* h = nullptr) : var(v), low(l), high(h){}
};

struct Triple {
    char var;
    Vertex* low;
    Vertex* high;

    Triple(char v, Vertex* l = nullptr, Vertex* h = nullptr) : var(v), low(l), high(h) {}

    // Define equality operator for comparing triples
    bool operator==(const Triple& other) const {
        return var == other.var && low == other.low && high == other.high;
    }
};

// Define a hash function for triples
struct TripleHash {
    size_t operator()(const Triple& triple) const {
        size_t hashValue = 17;
        hashValue = hashValue * 31 + triple.var;
        hashValue = hashValue * 31 + reinterpret_cast<size_t>(triple.low);
        hashValue = hashValue * 31 + reinterpret_cast<size_t>(triple.high);
        return hashValue;
    }
};

Vertex* build_BDD(const string &function, const string &var_orders, int index);
Vertex* old_or_new(char &var, Vertex *low, Vertex *high);
Vertex* searchUniqueTable(const Triple& triple);
int count_Nodes(Vertex* vertex);
void function_simplify(string &high_f, string &low_f, char &var);
void insertIntoUniqueTable(const Triple& triple, Vertex* vertex);
void removeFromUniqueTable(const Triple& triple);
void clearUniqueTable();
bool vertex_equal(Vertex *a, Vertex *b);

// Define a hash table to store processed triples
unordered_map<Triple, Vertex*, TripleHash> uniqueTable;
Vertex v0('0'), v1('1');


int main(int argc, char* argv[]){
    //----------------------------variable declaration-------------------------------------
    ifstream input;
    ofstream output;
    string function;
    vector<string> var_orders, products;
    int min_num = INT_MAX;

    //----------------------------initializing---------------------------------------------
    input.open(argv[1]);
    output.open(argv[2]);

    //--------------------------------bdd part---------------------------------------------
    read_data(input, function, var_orders);

    //--------------------------------testing part---------------------------------------------
    for(auto & order : var_orders){
        int i = 0;
        Vertex* bdd = build_BDD(function, order, i);
        //for(auto &m : uniqueTable){
        //    cout << m.first.var << endl;
        //    //printf(" %d %d\n", &m.first.high, &m.first.low);
        //}
        int node_num = uniqueTable.size();
        min_num = (min_num > node_num) ? node_num : min_num;
        uniqueTable.clear();
    }
    //printf("num: %d\n", min_num);

    //-------------------------------------------------------------------------------------

    output << min_num << endl;
    input.close();
    output.close();

    return 0;
}

void function_simplify(string &high_f, string &low_f, char &var){

    vector<string> low_products, high_products;
    char complement = toupper(var);
    replace(low_f.begin(), low_f.end(), var, '0');
    replace(low_f.begin(), low_f.end(), complement, '1');
    replace(high_f.begin(), high_f.end(), var, '1');
    replace(high_f.begin(), high_f.end(), complement, '0');

    parse_products(low_products, low_f);
    parse_products(high_products, high_f);


    low_f.clear();
    high_f.clear();

    for(auto &p : low_products){
        bool zero = false, one = false; 
        if(p.size() == 1 && p[0] == '1'){//if p is a single "1", then low_f is 1
            low_f = '1';
            break;
        }

        for(auto &c : p){
            if(c == '0')
                zero = true;
            if(c == '1')
                one = true;
        }
        if(zero)//discard this product
            continue;
        else if(one){//remove the ones(product is not single 1)
            size_t pos = 0;
            string delimeter = "1";
            while((pos = p.find(delimeter)) != std::string::npos)
                p.erase(pos, delimeter.length());
        }
        if(p != "" && low_f != "")
            low_f = low_f + "+" + p; 
        else if(p != "" && low_f == "")
            low_f = low_f + p;
    }

    for(auto &p : high_products){
        bool zero = false, one = false;
        if(p.size() == 1 && p[0] == '1'){//if p is a single "1", then high_f is 1
            high_f = '1';
            break;
        }

        for(auto &c : p){
            if(c == '0')
                zero = true;
            if(c == '1')
                one = true;
        }
        if(zero)//discard this product
            continue;
        else if(one){//remove the ones(product is not single 1)
            size_t pos = 0;
            string delimeter = "1";
            while((pos = p.find(delimeter)) != std::string::npos)
                p.erase(pos, delimeter.length());
        }
        if(p != "" && high_f != "")
            high_f = high_f + "+" + p; 
        else if(p != "" && high_f == "")
            high_f = high_f + p;
    }
    if(low_f == "")
        low_f = "0";
    if(high_f == "")
        high_f = "0";


}

Vertex* build_BDD(const string &function, const string &order, int index){

    if(index > order.size())
        return nullptr;
    //cout << "s: " << function << endl;
    //cout << "v: " << order[index] << endl;
    //cout << "i: " << index << endl;

    if(function == "0"){
        if(searchUniqueTable(Triple('0')) == nullptr)
            insertIntoUniqueTable(Triple('0'), &v0);
//        printf("zero\n");
        return &v0;
    }
    else if(function == "1"){
        if(searchUniqueTable(Triple('1')) == nullptr)
            insertIntoUniqueTable(Triple('1'), &v1);
//        printf("one\n");
        return &v1;
    }
    else{
        char var = order[index];
        index++;

        string low_f = function, high_f = function;
 
        function_simplify(high_f, low_f, var);
        //cout << high_f << ";" << low_f << endl;

        Vertex* high = build_BDD(high_f, order, index);
        Vertex* low = build_BDD(low_f, order, index);
        //Triple low_t(low->var, (low)?low->low:nullptr, (low)?low->high:nullptr), high_t(high->var, (high)?high->low:nullptr, (high)?high->high:nullptr);
        if(low == high) 
            return low;
        else{
            //insertIntoUniqueTable(Triple(var, low, high), new Vertex(var, low, high));//the key will be different
            return old_or_new(var, low, high);
        }
    }

}

Vertex* old_or_new(char &var, Vertex *low, Vertex *high){
    Vertex* res;
    Triple tmp(var, low, high);
    res = searchUniqueTable(tmp);
    if(res != nullptr){
        return res;
    }
    else{
        res = new Vertex(var, low, high);
        insertIntoUniqueTable(tmp, res);
        return res;
    }   
}

int count_Nodes(Vertex* vertex){
    if (!vertex){
        return 0;
    }

    return 1 + count_Nodes(vertex->low) + count_Nodes(vertex->high);
}

bool vertex_equal(Vertex *a, Vertex *b){
    bool res = true;
    if(a == nullptr && b == nullptr)
        return true;
    if(a->var != b->var)
        return false;
    if(!vertex_equal(a->high, b->high))
        return false;
    if(!vertex_equal(a->low, b->low))
        return false;
    return true;
}

// Function to insert a triple into the unique table
void insertIntoUniqueTable(const Triple& triple, Vertex* vertex){
    uniqueTable[triple] = vertex;
}

// Function to search for a triple in the unique table
Vertex* searchUniqueTable(const Triple& triple){
    auto it = uniqueTable.find(triple);
    if (it != uniqueTable.end()) {
        return it->second;
    }
    return nullptr;
}

// Function to remove a triple from the unique table
void removeFromUniqueTable(const Triple& triple){
    uniqueTable.erase(triple);
}

// Function to clear the unique table
void clearUniqueTable(){
    uniqueTable.clear();
}