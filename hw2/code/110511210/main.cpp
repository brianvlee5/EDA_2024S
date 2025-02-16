#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <ctime>
#include <unordered_map>
#include <algorithm>
#include <stdio.h>
#include <limits.h>

#define unlock_limit 20//times that the program can unlock all cells

using namespace std;

int net_num, cell_num;

//class declaration
//---------------------------------------------------------------
class cell;
class net;
class max_record;

class max_record{
public:
    int G;
    int moved;
    vector<cell> partition; 
};

class net{
public:
    int id;
    vector<cell*> c_in_net;
    int a_num, b_num;
    bool a_lock, b_lock;//if there is at least one cell lock in part a/b
    void calculate_ab();
};

class cell{
public:
    int id;
    int part;//which part is it in
    int gain;
    int locked;
};
//---------------------------------------------------------------

//function declaration
//---------------------------------------------------------------
void construct_CellStorage(vector<cell> &cell_storage){ 
    for(int i=1; i<=cell_num; i++){
        cell temp;
        temp.id = i;
        temp.gain = 0;
        temp.locked = 0;
        (i<=cell_num/2) ? temp.part = 0 : temp.part = 1;
        cell_storage.push_back(temp);
    }
}

//constructing cell & net vector
void construct_CellAndNet(vector<vector<int>> &cell_vec, vector<net> &net_vec, ifstream &input, vector<cell> &cell_storage){
    //preconstruct cell vec
    for(int i=0; i<cell_num; i++){
        vector<int> temp;
        cell_vec.push_back(temp);
    }

    //constructing the net vec
    for(int i=0; i<net_num; i++){
        int cell_id;
        net temp_n;
        cell *temp_c;
        string s, token;//for istream -> string
        
        getline(input, s);
        stringstream ss(s);//for string parsing
        temp_n.id = i+1;//setting id of net
        temp_n.a_lock = false;
        temp_n.b_lock = false;
        while(getline(ss, token, ' ')){
            cell_id = stoi(token);
            cell_vec[cell_id-1].push_back(temp_n.id);
            temp_c = &cell_storage[cell_id-1];//setting temp_c to point to cell storage; cell storage's index start from 0
            temp_n.c_in_net.push_back(temp_c);
        }
        net_vec.push_back(temp_n);//update the net vector net_num times
    }
}

//construct bucket A&B
void construct_Bucket(vector<cell> &cell_storage, vector<unordered_map<int, int>> &bucket_A, vector<unordered_map<int, int>> &bucket_B, int &A_max, int &B_max){
    for(int i=0; i<cell_num; i++){
        if(cell_storage[i].part){
            bucket_B[cell_storage[i].gain+net_num].emplace(cell_storage[i].id, 0);
            if(cell_storage[i].gain+net_num > B_max)
                B_max = cell_storage[i].gain+net_num;
        }
            
        else{
            bucket_A[cell_storage[i].gain+net_num].emplace(cell_storage[i].id, 0);
            if(cell_storage[i].gain+net_num > A_max)
                A_max = cell_storage[i].gain+net_num;
        }
           
    }
}

void initialize_Gain(vector<cell> &cell_storage, vector<vector<int>> &cell_vec, vector<net> &net_vec){
    for(int i=0; i<net_num; i++)
        net_vec[i].calculate_ab();
    
    for(int i=0; i<cell_num; i++){ 
        cell_storage[i].gain = 0;
        for(auto &c : cell_vec[i]){
            int F, T, net_n = c-1;
            if(cell_storage[i].part == 0){//cell in A now
                F = net_vec[net_n].a_num, T = net_vec[net_n].b_num;
            }
            else{
                T = net_vec[net_n].a_num, F = net_vec[net_n].b_num;
            }
                
            if(F == 1){
                cell_storage[i].gain++;
            }   
            if(T == 0){
                cell_storage[i].gain--;
            }                
        }    
    }
}

void initialize_Sum(vector<cell> &cell_storage, int &A_sum, int &B_sum){
    A_sum = 0, B_sum = 0;
    for(int i=0; i<cell_num; i++){
        if(cell_storage[i].part == 0)
            A_sum++;
        else 
            B_sum++;
    }
}

void find_Max_Gain(vector<cell> &cell_storage, vector<vector<int>> &cell_vec, vector<net> &net_vec, vector<unordered_map<int, int>> &bucket_A, vector<unordered_map<int, int>> &bucket_B, int &A_sum, int &B_sum, int &max_gain, int &move_index, int &G){
    int max_gain_a=INT_MIN, max_gain_b=INT_MIN;//recording the max gain in a/b
    int move_index_a=0, move_index_b=0;//recording the move index in a/b
    max_gain=0, move_index=0;//recording final max gain & move index
    double balance_A = (double)(A_sum-1)/cell_num, balance_B = (double)(B_sum-1)/cell_num;
    //bool a_balance = balance_A >= 0.45 && balance_A <= 0.55;
    //bool a_balance_small = balance_A >= 0.4 && balance_A <= 0.6;//for input1
    //bool b_balance = balance_B >= 0.45 && balance_B <= 0.55;
    //bool b_balance_small = balance_B >= 0.4 && balance_B <= 0.6;//for input1
    bool a_balance = (A_sum-1) >= 0.45 * (cell_num-1) && (A_sum-1) <= 0.55 * (cell_num-1);
    bool a_balance_small = balance_A >= 0.4 && balance_A <= 0.6;//for input1
    bool b_balance = (B_sum-1) >= 0.45 * (cell_num-1) && (B_sum-1) <= 0.55 * (cell_num-1);
    bool b_balance_small = balance_B >= 0.4 && balance_B <= 0.6;//for input1
    
    //max in bucket a
    for(int i=net_num*2; i>=0; i--){       
        if(!bucket_A[i].empty()){
            for(auto it = bucket_A[i].begin(); it != bucket_A[i].end(); it++){
                if(!it->second){                                 
                    if(a_balance){
                        max_gain_a = i-net_num;
                        move_index_a = it->first;
                        break;
                    }
                }
            }  
        }  
        if(max_gain_a != INT_MIN)
            break;      
    }
    //max in bucket b
    
    for(int i=net_num*2; i>=0; i--){
        if(!bucket_B[i].empty()){
            for(auto it = bucket_B[i].begin(); it != bucket_B[i].end(); it++){
                if(!it->second){ 
                    if(b_balance){                        
                        max_gain_b = i-net_num;
                        move_index_b = it->first;
                        break;
                    }
                }
            }  
        }
        if(max_gain_b != INT_MIN)
            break;        
    }

    

    if(max_gain_a > max_gain_b){
        max_gain = max_gain_a;
        move_index = move_index_a;
        A_sum--, B_sum++;//change total sum of a/b
        if(move_index == 0)//means that cannot move anymore due to balance
            return;
        for(auto &r : cell_vec[move_index-1]){//traverse all nets connected
            int i = r-1;//net index(number-1)
            net_vec[i].a_num--, net_vec[i].b_num++;//change a/b sum of each net 
        }
        bucket_A[max_gain+net_num].erase(move_index);
    }
    else{
        max_gain = max_gain_b;
        move_index = move_index_b;
        A_sum++, B_sum--;
        if(move_index == 0)//means that cannot move anymore due to balance
            return;
        for(auto &r : cell_vec[move_index-1]){//traverse all nets connected
            int i = r-1;//net index(number-1)
            net_vec[i].a_num++, net_vec[i].b_num--;
        }
        bucket_B[max_gain+net_num].erase(move_index);
    }
    cell_storage[move_index-1].part = (cell_storage[move_index-1].part+1)%2;//move to the other group
    cell_storage[move_index-1].locked = 1;
    G += max_gain;

    
}

void find_Max_Gain_new(vector<cell> &cell_storage, vector<vector<int>> &cell_vec, vector<net> &net_vec, vector<unordered_map<int, int>> &bucket_A, vector<unordered_map<int, int>> &bucket_B, int &A_sum, int &B_sum, int &max_gain, int &move_index, int &G, int &A_max, int &B_max){
    int max_gain_a=INT_MIN, max_gain_b=INT_MIN;//recording the max gain in a/b
    int move_index_a=0, move_index_b=0;//recording the move index in a/b
    max_gain=0, move_index=0;//recording final max gain & move index
    double balance_A = (double)(A_sum-1)/cell_num, balance_B = (double)(B_sum-1)/cell_num;
    //bool a_balance = balance_A >= 0.45 && balance_A <= 0.55;
    //bool a_balance_small = balance_A >= 0.4 && balance_A <= 0.6;//for input1
    //bool b_balance = balance_B >= 0.45 && balance_B <= 0.55;
    //bool b_balance_small = balance_B >= 0.4 && balance_B <= 0.6;//for input1
    bool a_balance = (A_sum-1) >= 0.45 * (cell_num-1) && (A_sum-1) <= 0.55 * (cell_num-1);
    bool a_balance_small = balance_A >= 0.4 && balance_A <= 0.6;//for input1
    bool b_balance = (B_sum-1) >= 0.45 * (cell_num-1) && (B_sum-1) <= 0.55 * (cell_num-1);
    bool b_balance_small = balance_B >= 0.4 && balance_B <= 0.6;//for input1
    
    //max in bucket a
    for(int i=A_max; i>=0; i--){       
        if(!bucket_A[i].empty()){
            if(a_balance){
                max_gain_a = i-net_num;
                move_index_a = bucket_A[i].begin()->first;
            }
            //for(auto it = bucket_A[i].begin(); it != bucket_A[i].end(); it++){
            //    printf("%d: %d\n", it->first, it->second);
            //    if(!it->second){                                 
            //        if(a_balance){
            //            max_gain_a = i-net_num;
            //            move_index_a = it->first;
            //            printf("next\n");
            //            break;
            //        }
            //    }
            //}  
        }  
        if(max_gain_a != INT_MIN){
            break;      
        }
    }        

    //max in bucket b   
    for(int i=B_max; i>=0; i--){
        if(!bucket_B[i].empty()){
            if(b_balance){
                max_gain_b = i-net_num;
                move_index_b = bucket_B[i].begin()->first;
            }
            //for(auto it = bucket_B[i].begin(); it != bucket_B[i].end(); it++){
            //    if(!it->second){ 
            //        if(b_balance){                        
            //            max_gain_b = i-net_num;
            //            move_index_b = it->first;
            //            break;
            //        }
            //    }
            //}  
        }
        if(max_gain_b != INT_MIN){
            break;        
        }
    }
    

    if(max_gain_a > max_gain_b){
        max_gain = max_gain_a;
        move_index = move_index_a;
        A_sum--, B_sum++;//change total sum of a/b
        if(move_index == 0)//means that cannot move anymore due to balance
            return;
        for(auto &r : cell_vec[move_index-1]){//traverse all nets connected
            int i = r-1;//net index(number-1)
            net_vec[i].a_num--, net_vec[i].b_num++;//change a/b sum of each net 
        }
        bucket_A[max_gain+net_num].erase(move_index);
    }
    else{
        max_gain = max_gain_b;
        move_index = move_index_b;
        A_sum++, B_sum--;
        if(move_index == 0)//means that cannot move anymore due to balance
            return;
        for(auto &r : cell_vec[move_index-1]){//traverse all nets connected
            int i = r-1;//net index(number-1)
            net_vec[i].a_num++, net_vec[i].b_num--;
        }
        bucket_B[max_gain+net_num].erase(move_index);
    }
    cell_storage[move_index-1].part = (cell_storage[move_index-1].part+1)%2;//move to the other group
    cell_storage[move_index-1].locked = 1;
    G += max_gain;

    
}

void record_Max(max_record &record, vector<cell> &cell_storage, int &move_index, int &G, int &G_max){
    if(move_index == 0)//means that cannot move anymore due to balance
        return;
    record.G = G;
    record.moved = move_index;
    record.partition = cell_storage;
    G_max = G;
}

void update_Gain(vector<cell> &cell_storage, vector<vector<int>> &cell_vec, vector<net> &net_vec, int &move_index, vector<unordered_map<int, int>> &bucket_A, vector<unordered_map<int, int>> &bucket_B){
    if(move_index == 0)//means that cannot move anymore due to balance
        return;
    for(auto &c : cell_vec[move_index-1]){//traverse net connected with moved cell
        int F, T, net_n = c-1, from_p;
        if(cell_storage[move_index-1].part == 0){//cell in A now(moved)
            T = net_vec[net_n].a_num, F = net_vec[net_n].b_num, from_p = 1;
        }
        else{
            F = net_vec[net_n].a_num, T = net_vec[net_n].b_num, from_p = 0;
        }
        F++, T--;
        if(T == 0){
            for(auto &cc : net_vec[net_n].c_in_net){
                if(!cell_storage[cc->id-1].locked){
                    cell_storage[cc->id-1].gain++;
                    if(cell_storage[cc->id-1].part == 0){
                        if(!cell_storage[cc->id-1].locked){
                            bucket_A[cell_storage[cc->id-1].gain-1+net_num].erase(cc->id);
                            bucket_A[cell_storage[cc->id-1].gain+net_num].emplace(cc->id, cell_storage[cc->id-1].locked);
                        }   
                    }
                    else{
                        if(!cell_storage[cc->id-1].locked){
                            bucket_B[cell_storage[cc->id-1].gain-1+net_num].erase(cc->id);
                            bucket_B[cell_storage[cc->id-1].gain+net_num].emplace(cc->id, cell_storage[cc->id-1].locked); 
                        }
                    }
                }
                    
            }
        }    
        else if(T == 1){//minus the only one in to part
            for(auto &cc : net_vec[net_n].c_in_net){
                if(!cell_storage[cc->id-1].locked && cell_storage[cc->id-1].part == !from_p){
                    cell_storage[cc->id-1].gain--;
                    if(cell_storage[cc->id-1].part == 0){
                        if(!cell_storage[cc->id-1].locked){
                            bucket_A[cell_storage[cc->id-1].gain+1+net_num].erase(cc->id);
                            bucket_A[cell_storage[cc->id-1].gain+net_num].emplace(cc->id, cell_storage[cc->id-1].locked);
                        }
                    }
                    else{
                        if(!cell_storage[cc->id-1].locked){
                            bucket_B[cell_storage[cc->id-1].gain+1+net_num].erase(cc->id);
                            bucket_B[cell_storage[cc->id-1].gain+net_num].emplace(cc->id, cell_storage[cc->id-1].locked);
                        } 
                    }
                    break;
                }
            }
        }
        F--, T++;
        if(F == 0){
            for(auto &cc : net_vec[net_n].c_in_net){
                if(!cell_storage[cc->id-1].locked){
                    cell_storage[cc->id-1].gain--;
                    if(cell_storage[cc->id-1].part == 0){
                        if(!cell_storage[cc->id-1].locked){
                            bucket_A[cell_storage[cc->id-1].gain+1+net_num].erase(cc->id);
                            bucket_A[cell_storage[cc->id-1].gain+net_num].emplace(cc->id, cell_storage[cc->id-1].locked);
                        }
                    }
                    else{
                        if(!cell_storage[cc->id-1].locked){
                            bucket_B[cell_storage[cc->id-1].gain+1+net_num].erase(cc->id);
                            bucket_B[cell_storage[cc->id-1].gain+net_num].emplace(cc->id, cell_storage[cc->id-1].locked);
                        } 
                    }
                }
                    
            }
        } 
        else if(F == 1){
            for(auto &cc : net_vec[net_n].c_in_net){
                if(!cell_storage[cc->id-1].locked && cell_storage[cc->id-1].part == from_p){
                    cell_storage[cc->id-1].gain++;
                    if(cell_storage[cc->id-1].part == 0){
                        if(!cell_storage[cc->id-1].locked){
                            bucket_A[cell_storage[cc->id-1].gain-1+net_num].erase(cc->id);
                            bucket_A[cell_storage[cc->id-1].gain+net_num].emplace(cc->id, cell_storage[cc->id-1].locked);
                        }
                    }
                    else{
                        if(!cell_storage[cc->id-1].locked){
                            bucket_B[cell_storage[cc->id-1].gain-1+net_num].erase(cc->id);
                            bucket_B[cell_storage[cc->id-1].gain+net_num].emplace(cc->id, cell_storage[cc->id-1].locked);
                        } 
                    }
                    break;
                }
            }
        }             
    }
}

void update_Gain_new(vector<cell> &cell_storage, vector<vector<int>> &cell_vec, vector<net> &net_vec, int &move_index, vector<unordered_map<int, int>> &bucket_A, vector<unordered_map<int, int>> &bucket_B, int &A_max, int &B_max){
    if(move_index == 0)//means that cannot move anymore due to balance
        return;
    for(auto &c : cell_vec[move_index-1]){//traverse net connected with moved cell
        int F, T, net_n = c-1, from_p;
        if(cell_storage[move_index-1].part == 0){//cell in A now(moved)
            T = net_vec[net_n].a_num, F = net_vec[net_n].b_num, from_p = 1;
        }
        else{
            F = net_vec[net_n].a_num, T = net_vec[net_n].b_num, from_p = 0;
        }
        F++, T--;
        if(T == 0){
            for(auto &cc : net_vec[net_n].c_in_net){
                int cc_id = cc->id;
                if(!cell_storage[cc_id-1].locked){
                    cell_storage[cc_id-1].gain++;
                    int cs_gain = cell_storage[cc_id-1].gain;
                    if(cell_storage[cc_id-1].part == 0){
                        bucket_A[cs_gain-1+net_num].erase(cc_id);
                        bucket_A[cs_gain+net_num].emplace(cc_id, cell_storage[cc_id-1].locked);
                        if(cs_gain+net_num > A_max)
                            A_max = cs_gain+net_num;
                    }
                    else{
                        bucket_B[cs_gain-1+net_num].erase(cc_id);
                        bucket_B[cs_gain+net_num].emplace(cc_id, cell_storage[cc_id-1].locked);
                        if(cs_gain+net_num > B_max)
                            B_max = cs_gain+net_num;
                    }
                }
                    
            }
        }    
        else if(T == 1){//minus the only one in to part
            for(auto &cc : net_vec[net_n].c_in_net){
                int cc_id = cc->id;
                if(!cell_storage[cc_id-1].locked && cell_storage[cc_id-1].part == !from_p){
                    cell_storage[cc_id-1].gain--;
                    int cs_gain = cell_storage[cc_id-1].gain;
                    if(cell_storage[cc_id-1].part == 0){
                        bucket_A[cs_gain+1+net_num].erase(cc_id);
                        bucket_A[cs_gain+net_num].emplace(cc_id, cell_storage[cc_id-1].locked);
                        if(cs_gain+net_num > A_max)
                            A_max = cs_gain+net_num;
                    }
                    else{
                        bucket_B[cs_gain+1+net_num].erase(cc_id);
                        bucket_B[cs_gain+net_num].emplace(cc_id, cell_storage[cc_id-1].locked);
                        if(cs_gain+net_num > B_max)
                            B_max = cs_gain+net_num;
                    }
                    break;
                }
            }
        }
        F--, T++;
        if(F == 0){
            for(auto &cc : net_vec[net_n].c_in_net){
                int cc_id = cc->id;
                if(!cell_storage[cc_id-1].locked){
                    cell_storage[cc_id-1].gain--;
                    int cs_gain = cell_storage[cc_id-1].gain;
                    if(cell_storage[cc_id-1].part == 0){
                        bucket_A[cs_gain+1+net_num].erase(cc_id);
                        bucket_A[cs_gain+net_num].emplace(cc_id, cell_storage[cc_id-1].locked);
                        if(cs_gain+net_num > A_max)
                            A_max = cs_gain+net_num;
                    }
                    else{
                        bucket_B[cs_gain+1+net_num].erase(cc_id);
                        bucket_B[cs_gain+net_num].emplace(cc_id, cell_storage[cc_id-1].locked);
                        if(cs_gain+net_num > B_max)
                            B_max = cs_gain+net_num;
                    }
                }
                    
            }
        } 
        else if(F == 1){
            for(auto &cc : net_vec[net_n].c_in_net){
                int cc_id = cc->id;
                if(!cell_storage[cc_id-1].locked && cell_storage[cc_id-1].part == from_p){
                    cell_storage[cc_id-1].gain++;
                    int cs_gain = cell_storage[cc_id-1].gain;
                    if(cell_storage[cc_id-1].part == 0){
                        bucket_A[cs_gain-1+net_num].erase(cc_id);
                        bucket_A[cs_gain+net_num].emplace(cc_id, cell_storage[cc_id-1].locked);
                        if(cs_gain+net_num > A_max)
                            A_max = cs_gain+net_num;
                    }
                    else{
                        bucket_B[cs_gain-1+net_num].erase(cc_id);
                        bucket_B[cs_gain+net_num].emplace(cc_id, cell_storage[cc_id-1].locked);
                        if(cs_gain+net_num > B_max)
                            B_max = cs_gain+net_num;
                    }
                    break;
                }
            }
        }             
    }
}

void unlock_and_move(vector<cell> &cell_storage, max_record &record, vector<unordered_map<int, int>> &bucket_A, vector<unordered_map<int, int>> &bucket_B, int &G){//move to the max G state
//    for(int i=0; i<cell_num; i++){//first erase all cells in buckets
//        if(cell_storage[i].part)
//            bucket_B[cell_storage[i].gain+net_num].erase(cell_storage[i].id);
//        else
//            bucket_A[cell_storage[i].gain+net_num].erase(cell_storage[i].id);
//    }
    cell_storage = record.partition;
    for(int i=0; i<cell_num; i++){
        cell_storage[i].locked = 0;
    }
    G = record.G;
}

void output_Partition(ofstream &output, max_record &record){
    for(int i=0; i<cell_num; i++){
        output << record.partition[i].part << '\n';
    }
}

void print_Partition(max_record &record, vector<net> &net_vec){
    printf("G: %d\n", record.G);
    for(int i=0; i<cell_num; i++){
        //printf("%d: %d\n", i+1, record.partition[i].part);
    }

    //cut part
    int cut_nets = 0;
    bool cut;
    int comp;
    for(auto &r:net_vec)
    { 
        cut = false;
        comp = record.partition[(r.c_in_net[0]->id)-1].part;
        for(auto &rr:r.c_in_net)
        {
            if(record.partition[rr->id-1].part != comp)
            {
                cut = true;
                break;
            }
        }
        if(cut)
            cut_nets++;
    }
    printf("cuts:%d\n", cut_nets);
}

void print_Gain(vector<cell> &cell_storage){
    for(int i=0; i<cell_num; i++){
        printf("%d(%d): %d\n", i+1, cell_storage[i].part, cell_storage[i].gain);
    }
    printf("\n");
}

void print_Net(vector<net> &net_vec){
    for(int i=0; i<net_num; i++){
        printf("%d: %d %d\n", i+1, net_vec[i].a_num, net_vec[i].b_num);
    }
}

void print_Bucket(vector<unordered_map<int, int>> &bucket){
    for(int i=0; i<2*net_num+1; i++){
        if(!bucket[i].empty()){
            printf("%d:\n", i-net_num);
            for(auto it = bucket[i].begin(); it!=bucket[i].end(); it++){
                    printf("%d(%d) ", it->first, it->second);
            }
            printf("\n");
        }  
    }
}
//---------------------------------------------------------------

int main(int argc, char *argv[]){
    //printf("----start----\n");
    //cout << (double)clock()/CLOCKS_PER_SEC << '\n';
    //printf("-------------\n");
    //variable declaration
    //---------------------------------------------------------------
    ifstream input; 
    ofstream output;

    string dash_n;
    vector<cell> cell_storage;//store the original cell
    vector<vector<int>> cell_vec;//storing which net connected to this cell
    vector<net> net_vec;//storing which cell in this net
    max_record record;//recording the max G and partition
    
    int round=0;//how many times of fm executed
    int unlock_times=0;//how many time unlocked
    int G_max_total = INT_MIN;//recording the max G in total
    int G_max = INT_MIN;//recording the max G during each round
    int G=0;//recording every rounds G(during fm)
    int A_sum, B_sum;//for recording the numbers of node in a/b 
    int A_max=0, B_max=0;//for recording the max gain pointer of a/b
    int max_gain;
    int move_index;
    string out_add = "output.txt";

    clock_t timer, init_time;
    //---------------------------------------------------------------
   

    //input
    //---------------------------------------------------------------
    init_time = clock();
    input.open(argv[1]);
    output.open(out_add);
    input >> net_num >> cell_num;
    getline(input, dash_n);//read in the '\n'

    vector<unordered_map<int, int>> bucket_A(2*net_num+1), bucket_B(2*net_num+1);//for calculating max gain  
    construct_CellStorage(cell_storage);
    construct_CellAndNet(cell_vec, net_vec, input, cell_storage);
    
    //---------------------------------------------------------------
    
    //fm
    //---------------------------------------------------------------
    
    initialize_Gain(cell_storage, cell_vec, net_vec);
//    print_Gain(cell_storage);
    construct_Bucket(cell_storage, bucket_A, bucket_B, A_max, B_max);
    initialize_Sum(cell_storage, A_sum, B_sum);

    while(1){
//        printf("move_index:%d G:%d\n", move_index, G);
//        print_Gain(cell_storage);
        find_Max_Gain_new(cell_storage, cell_vec, net_vec, bucket_A, bucket_B, A_sum, B_sum, max_gain, move_index, G, A_max, B_max);
        round++;
        if(G > G_max)
            record_Max(record, cell_storage, move_index, G, G_max);
        update_Gain_new(cell_storage, cell_vec, net_vec, move_index, bucket_A, bucket_B, A_max, B_max);
        
        //printf("A:\n");
        //print_Bucket(bucket_A);
        //printf("B:\n");
        //print_Bucket(bucket_B); 
        bool all_locked = (round > cell_num);//if all cells are locked
        bool cannot_move = (move_index == 0);
        bool G_neg = (max_gain < 0);
        if(all_locked || cannot_move){
            bool can_unlock = (unlock_times < unlock_limit);//if still able to unlock
            if(can_unlock){
                unlock_times++;
                round = 0;//reset round
                //printf("unlock\n");
                unlock_and_move(cell_storage, record, bucket_A, bucket_B, G);
                initialize_Gain(cell_storage, cell_vec, net_vec);
                initialize_Sum(cell_storage, A_sum, B_sum);
                construct_Bucket(cell_storage, bucket_A, bucket_B, A_max, B_max);
                continue;
            }       
            else
                break;
        }      
        timer = clock();
        if(timer-init_time > 29500000)
            break;
    }
    //---------------------------------------------------------------

    //output
    //---------------------------------------------------------------
    output_Partition(output, record);
    //print_Partition(record, net_vec);
    //---------------------------------------------------------------
    //printf("----end----\n");
    //cout << (double)clock()/CLOCKS_PER_SEC << '\n';
    //printf("-----------\n");
    input.close();
    output.close();
    return 0;
}

void net::calculate_ab(){
    a_num = 0, b_num = 0;
    for(auto &c : this->c_in_net){
        if(c->part == 0)
            a_num++;
        else
            b_num++;
    }
}
