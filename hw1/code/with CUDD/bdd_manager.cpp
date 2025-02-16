#include "bdd_manager.h"

void write_dd(DdManager *gbm, DdNode *bdd, char* filename){
    FILE *outfile;
    outfile = fopen(filename, "w");
    DdNode **ddnodearray = (DdNode**)malloc(sizeof(DdNode*));
    ddnodearray[0] = bdd;
    Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile);
    free(ddnodearray);
    fclose(outfile);
}

void example(){
    char filename[30];
    DdManager *gbm;
    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    DdNode *bdd, *var, *tmp_neg, *tmp;
    int i;
    bdd = Cudd_ReadOne(gbm);
    Cudd_Ref(bdd);

    for(i = 4; i>=1; i--){
        var = Cudd_bddIthVar(gbm, i);
        tmp_neg = Cudd_Not(var);
        tmp = Cudd_bddAnd(gbm, tmp_neg, bdd);
        Cudd_Ref(tmp);
        Cudd_RecursiveDeref(gbm, bdd);
        bdd = tmp;
    }
    var = Cudd_bddIthVar(gbm, 0);
    tmp = Cudd_bddAnd(gbm, var, bdd);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(gbm, bdd);
    bdd = tmp;
    int node_num = Cudd_DagSize(bdd);
    printf("size: %d\n", node_num);

    bdd = Cudd_BddToAdd(gbm, bdd);
    sprintf(filename, "./bdd/graph.dot");
    write_dd(gbm, bdd, filename);
    Cudd_Quit(gbm);
}