#include "floorplan.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <set>
#include<climits> 

using namespace std;

void floorplan::addRec(int area){
    srand(clock());
    int rand_num, top = sqrt(area*2), down = ceil(sqrt(area/2));
    vector<pair<double,double>> legalLength;
    for(int x = down; x < top; x++){
        if(area % x == 0){
            legalLength.push_back(make_pair(double(x), double(area/x)));
        }
    }
    if(legalLength.empty()){
        for(int x = down; x < top; x++){
            if(legalLength.size() >= 4)
                break;
            if( double(x * (area/x)) / double(area) > 0.99 && double(x * (area/x)) / double(area) < 1.01){
                legalLength.push_back(make_pair(x, area/x));
            }
        }
    }
    while(legalLength.empty()){
        int width = rand() % (down-top) + down;
        if(double(width)*rounding(double(area)/double(width), 2) / double(area) > 0.99 
            && double(width)*rounding(double(area)/double(width), 2) / double(area) < 1.01){
            legalLength.push_back(make_pair(width, rounding(double(area)/double(width), 2)));
            legalLength.push_back(make_pair(rounding(double(area)/double(width), 2), width));
        }
            
    }
    all_blocks.push_back(legalLength);
}

double floorplan::rounding(double num, int idx){
    if(idx >= 0)
    {
        int multiplier;
        multiplier = pow(10, idx);
        num = (int)(num * multiplier + 0.5) / (multiplier * 1.0);
    }
    return num;
}

void floorplan::SA(){
    double start = clock(), end;
    init();
    cout << "init done" << endl;
    int move_times = 0, uphill = 0;
    int total_moves = 5*(best_block.npe.size()+1)/2; // 倍數的operand數
    int reject = 0;
    int T = 100, eps = 5;
    vector<string> E = best_block.npe;
    block E_block = best_block;
    /** initialize **/
    do{
        move_times = 0, uphill = 0, reject = 0;
        while(uphill < total_moves && move_times < 2*total_moves){
            end = clock();
            if((end-start)/ CLOCKS_PER_SEC > 3600){
                cout << "time out!" << endl;
                return;
            }
            vector<string> tmpE = selectMove(E);
            move_times ++;
            block tmpE_block = getCost(tmpE, E_block);
            double delta_cost = tmpE_block.get_cost() - E_block.get_cost();
            double ran = (double) rand() / (RAND_MAX + 1.0);
            
            if(delta_cost <= 0 || ran < exp(-delta_cost/T)){
                if(delta_cost > 0) uphill ++;
                E = tmpE;
                E_block = tmpE_block;
                if(tmpE_block.get_cost() < best_block.get_cost())
                    best_block = tmpE_block;
            }
            else
                reject ++;
        }
        T = 0.85 * T;
    } while ( reject/move_times < 0.95 && T > eps);
    best_block.printAllBlocks();
}

void floorplan::init(){
    vector<string> tmp_npe;
    tmp_npe.push_back("0");
    tmp_npe.push_back("1");
    tmp_npe.push_back("v");
    for(int i = 2; i < all_blocks.size(); i++){
        tmp_npe.push_back(to_string(i));
        if(i % 2 == 0)
            tmp_npe.push_back("v");
        else
            tmp_npe.push_back("h");
    }
    block b;
    best_block = getCost(tmp_npe, b);
}

vector<string> floorplan::selectMove(vector<string> e){
    int num_operand = (e.size()+1)/2, num_operator = num_operand - 1;
    vector<string> result(e);
    srand(clock());
    int m = rand() % 3 + 1;
    switch (m)
    {
    case 1: // swap two operand
    {  
        // cout << "enter 1......" << endl;
        int op1 = rand() % num_operand;
        int op2 = rand() % num_operand;
        while(op2 == op1){
            op2 = rand() % num_operand;
        }
        int idx1 = -1, idx2 = -1;
        for(int i = 0; i < result.size(); i++){
            if(checkType(result, i) == "operand")
                idx1 ++;
            if(idx1 == op1){
                idx1 = i;
                break;
            }
        }
        for(int i = 0; i < result.size(); i++){
            if(checkType(result, i) == "operand")
                idx2 ++;
            if(idx2 == op2){
                idx2 = i;
                break;
            }
        }
        swap(result.at(idx1), result.at(idx2));
        // printNPE(result);
        // cout << "1 done" << endl;
        break;
    }
    case 2: // complement operator chain
    {
        // cout << "enter 2......" << endl;
        int op1 = rand() % num_operator;
        int idx1 = -1, idx2 = 0;
        for(int i = 0; i < result.size(); i++){
            if(checkType(result, i) == "operator")
                idx1 ++;
            if(idx1 == op1){
                idx1 = i;
                break;
            }
        }
        int tmp = idx1;
        for(int i = tmp-1; i >= 0; i--){
            if(checkType(result, i) == "operator"){
                idx1--;
            }
            else
                break;
        }
        for(int i = idx1+1; i < result.size(); i++){
            if(checkType(result, i) == "operand"){
                idx2 = i;
                break;
            }
        }
        for(int i = idx1; i < idx2; i++){
            if(result.at(i) == "h")
                result.at(i) = "v";
            else 
                result.at(i) = "h";
        }
        // printNPE(result);
        // cout << "2 done" << endl;
        break;
    }
    case 3: // swap operand and operator
    {
        // cout << "enter 3......" << endl;
        int idx;
        bool fail = false;
        int start_time = clock(), end_time = 0;
        while(true){
            idx = rand() % (e.size()-1);
            if(checkType(result, idx) == "operand" && checkType(result, idx+1) == "operator"){
                if(idx = 0)
                    if(2*num_of_operator(result, idx+1) < idx)
                        break;
                else{
                    if(result.at(idx-1) != result.at(idx+1) && 2*num_of_operator(result, idx+1) < idx)
                        break;
                }
            }
            if(checkType(result, idx) == "operator" && checkType(result, idx+1) == "operand"){
                if(idx >= result.size()-2)
                    break;
                if(result.at(idx) != result.at(idx+2))
                    break;
            }
            end_time = clock();
            if((end_time - start_time)/CLOCKS_PER_SEC > 00.5){
                fail = true;
                break;
            }
        }
        if(!fail)
            swap(result.at(idx), result.at(idx+1));
        // printNPE(result);
        // cout << "3 done" << endl;
        break;
    }
    default:
        break;
    }
    return result;
}

string floorplan::checkType(vector<string> &e, int idx){
    if(e.at(idx) == "h" || e.at(idx) == "v")
        return "operator";
    else
        return "operand";
}

int floorplan::num_of_operator(vector<string> &e, int idx){
    int num = 0;
    for(int i = 0; i <= idx; i++){
        if(checkType(e, i) == "operator")
            num++;
    }
    return num;
}

block floorplan::getCost(vector<string> e, block previous){ // 100% area 0% wirelength
    vector<block> stack;
    vector<block> all_node;
    vector<block> calculated_blocks = previous.all_blocks;
    for(string str : e){
        if(str != "v" && str != "h"){
            auto idx = stoi(str);
            vector<string> com;
            com.push_back(str);
            block rec(com, all_blocks.at(idx));
            stack.push_back(rec);
            all_node.push_back(rec);
        }
        else{
            block r1 = stack.back();
            stack.pop_back();
            block r2 = stack.back();
            stack.pop_back();
            vector<string> compo;
            for(string s : r2.npe)
                compo.push_back(s);
            for(string s: r1.npe)
                compo.push_back(s);
            compo.push_back(str);
            vector<pair<double,double>> posLength;
            bool calculated = false;
            for(block b : calculated_blocks){
                if(b.npe == compo){
                    posLength = b.possible_length;
                    calculated = true;
                    break;
                }
            }
            if(!calculated)
                posLength = get_possible_length(r1.possible_length, r2.possible_length, str);
            block new_rec(compo, posLength);
            stack.push_back(new_rec);
            all_node.push_back(new_rec);
        }
    }
    block last_rec = stack.back();
    double best_area = INT_MAX;
    pair<double,double> length;
    for(pair<double,double> p : last_rec.possible_length){
        double area = p.first * p.second;
        if(area < best_area){
            best_area = area;
            length = p;
        }
    }
    last_rec.best_length = length;
    last_rec.all_blocks = all_node;
    return last_rec;
}

vector<pair<double,double>> floorplan::get_possible_length(vector<pair<double,double>> l1, vector<pair<double,double>> l2, string direction){
    vector<pair<double,double>> result;
    if(direction == "v"){   // vertical
        for(pair<double,double> pair1 : l1){
            for(pair<double,double> pair2 : l2){
                pair<double,double> tmp_pair = make_pair(pair1.first+pair2.first, max(pair1.second, pair2.second));
                if(filter(result, tmp_pair))
                    continue;
                else
                    result.push_back(tmp_pair);
            }
        }
    }
    else{                   // horizontial
        for(pair<double,double> pair1 : l1){
            for(pair<double,double> pair2 : l2){
                pair<double,double> tmp_pair = make_pair(max(pair1.first, pair2.first), pair1.second+pair2.second);
                if(filter(result, tmp_pair))
                    continue;
                else
                    result.push_back(tmp_pair);
            }
        }
    }
    return result;
}

bool floorplan::filter(vector<pair<double,double>> v, pair<double,double> target){
    for(pair<double,double> p : v){
        if(target.first >= p.first && target.second >= p.second)
            return true;
    }
    return false;
}

void floorplan::getBestAns(){
    best_block.createTree();
    best_block.printAllNodes();
}

void floorplan::printRec(){
    for(vector<pair<double,double>> rec : all_blocks){
        for(pair<double,double> length : rec){
            cout << "(" << length.first << "," << length.second << ")" << " ";
        }
        cout << endl;
    }
}

void floorplan::printBestArea(){
    cout << "cost: " << best_block.get_cost() << endl;
}
block floorplan::getBestBlock(){
    return best_block;
}

void floorplan::printNPE(vector<string> e){
    for(string s : e){
        cout << s << " ";
    }
    cout << endl;
}

void floorplan::printBestNPE(){
    printNPE(best_block.npe);
}