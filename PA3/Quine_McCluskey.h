#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

class QMcCluskey{
    friend bool compareSize(const set<string>,const set<string>);
private:
    vector<set<pair<string,set<unsigned int>>>> boolFun;
    vector<pair<string,set<unsigned int>>> implicants;
    map<unsigned int, vector<string>> table;
    map<string, unsigned int> encode_table;
    vector<set<set<string>>> POS;
    vector<string> minLogic;

    void getMinLogic();
    void createTable();
    void merge(int round);
    string compare(string c, string n);
    void make_POS();
    void POS2SOP();
    set<set<string>> getUnion(set<set<string>> a, set<set<string>> b);
    set<string> Union(set<string> a, set<string> b);
public:
    QMcCluskey(){}
    QMcCluskey(vector<set<pair<string,set<unsigned int>>>> vec){
        boolFun = vec;
    }
    void QMAlgo();
    void Petrick_Method();
    vector<string> getMin();
    void printAns();
    void printTable();
    void printPOS();
};