#pragma once
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <climits>

#include "cudd-3.0.0/cudd/cudd.h"
#include "parser.h"
#include "bdd_manager.h"

using namespace std;

void set_order(string &order, DdManager* &gbm, vector<DdNode*> &alphabets, vector<int> &var_used); 
void set_bdd(DdManager* &gbm, vector<string> &products, vector<DdNode*> &alphabets, DdNode *&bdd);

int main(int argc, char *argv[]){
    //----------------------------variable declaration-------------------------------------
    ifstream input;
    ofstream output;
    string function;
    char filename[30];
    vector<int> var_used;
    vector<string> var_orders, products;
    vector<DdNode*> alphabets(26);
    DdManager *gbm;
    DdNode *bdd;
    int min_node_num = INT_MAX, node_num;

    //----------------------------initializing---------------------------------------------
    input.open(argv[1]);
    output.open(argv[2]);
    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    bdd = Cudd_ReadZero(gbm);
    Cudd_Ref(bdd);

    //--------------------------------bdd part---------------------------------------------
    
    read_data(input, function, var_orders);
    parse_products(products, function);
    for(auto &order : var_orders){
        set_order(order, gbm, alphabets, var_used);
        set_bdd(gbm, products, alphabets, bdd);
        node_num = Cudd_DagSize(bdd);
        if(node_num < min_node_num)
            min_node_num = node_num+1;
        for(int i=0; i<var_used.size(); i++){
            Cudd_RecursiveDeref(gbm, alphabets[var_used[i]]);
        } 
        var_used.clear();
    }
    //cout << "min: " << min_node_num << endl;
    output << min_node_num;
    //bdd = Cudd_BddToAdd(gbm, bdd);
    //sprintf(filename, "./bdd/graph.dot");
    //write_dd(gbm, bdd, filename);


    //--------------------------------ending-----------------------------------------------
    input.close();
    output.close();
    Cudd_Quit(gbm);
    
    return 0;
}


void set_order(string &order, DdManager* &gbm, vector<DdNode*> &alphabets, vector<int> &var_used){
    order.pop_back();//pop out the "."  
    for(int i=0; i<order.size(); i++){
        int index = order[i] - 'a';
        var_used.push_back(index);
        alphabets[index] = Cudd_bddIthVar(gbm, i);//order the alphabets 
    }
} 

void set_bdd(DdManager* &gbm, vector<string> &products, vector<DdNode*> &alphabets, DdNode *&bdd){
    //--------------------------------products---------------------------------------------
    int p_num = products.size(), s_size;
    for(int i=0; i<p_num; i++){
        DdNode *tmp_p, *tmp, *tmp2;
        int alphabet_index;
        tmp_p = Cudd_ReadOne(gbm);
        Cudd_Ref(tmp_p);
        s_size = products[i].size();
        for(int j=0; j<s_size; j++){
            char a_now = products[i][j];//alphabet now
            bool upper = false;//flag for upper case
            if(a_now >= 'a' && a_now <= 'z')
                alphabet_index = products[i][j] - 'a';
            else{
                alphabet_index = products[i][j] - 'A';
                upper = true;
            }
            DdNode *tmp_alph = Cudd_Not(alphabets[alphabet_index]);//the alphabet we are on now
            tmp = Cudd_bddAnd(gbm, (upper)?tmp_alph:alphabets[alphabet_index], tmp_p);
            Cudd_Ref(tmp);
            Cudd_RecursiveDeref(gbm, tmp_p);
            tmp_p = tmp;
        }
        if(i == 0)
            bdd = tmp_p;
        else{
            tmp = Cudd_bddOr(gbm, tmp_p, bdd);
            Cudd_Ref(tmp);
            Cudd_RecursiveDeref(gbm, bdd);
            bdd = tmp;
        }
    }
}