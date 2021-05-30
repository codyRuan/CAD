#include <vector>
#include <iostream>
#include <set>
#include "treeNode.h"

using namespace std;

class block{
private:
    vector<string> npe;
    vector<pair<double,double>> possible_length;
    pair<double,double> best_length;
    vector<block> all_blocks;
    vector<treeNode *> Nodes;
    treeNode *root;

public:
    block(){}
    block(vector<string> c, vector<pair<double,double>> len){
        npe = c;
        possible_length = len;
        Nodes.resize((npe.size()+1)/2);
    }

    friend class floorplan; 
    double get_cost();
    pair<double,double> getBestLength();
    vector<treeNode *> getNodes();
    vector<string> getNPE();
    void createTree();
    void traversal(treeNode *current);
    void printAllNodes();
    void constructXY(treeNode *current);
    void printAllBlocks();
    void printPossibleLength();
};