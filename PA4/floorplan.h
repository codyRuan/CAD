#include"block.h"
#include <vector>
#include <iostream>

using namespace std;

class floorplan{

private:
    vector<vector<pair<double,double>>> all_blocks;
    block best_block;
    vector<block> all_possible_blocks;

    void init();
    double rounding(double num, int idx);
    block getCost(vector<string> e, block previous);
    vector<string> selectMove(vector<string> e);
    string checkType(vector<string> &e, int idx);
    int num_of_operator(vector<string> &e, int idx);
    bool filter(vector<pair<double,double>> v, pair<double,double> p);
    vector<pair<double,double>> get_possible_length(vector<pair<double,double>> l1, vector<pair<double,double>> l2, string direction);
public:
    floorplan(){}
    void addRec(int area);
    void SA();
    void getBestAns();
    void printBestArea();
    block getBestBlock();
    void printNPE(vector<string> e);
    void printBestNPE();
    void printRec();
};