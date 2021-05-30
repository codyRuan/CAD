#include <iostream>
#include <vector>

using namespace std;

class treeNode{
private:
    treeNode *leftChild;
    treeNode *rightChild;
    treeNode *parent;
    vector<string> npe;
    pair<double,double> length;
    string data;
    pair<double,double> XY;
public:
    treeNode():leftChild(0), rightChild(0), parent(0), data(""){};
    treeNode(vector<string> v, string str):leftChild(0), rightChild(0), parent(0), npe(v), data(str){};

    pair<double,double> getXY();
    pair<double,double> getLength();
    friend class block;
};