#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

class QMcCluskey{
private:
    vector<set<pair<string,set<unsigned int>>>> boolFun;
    vector<string> implicants;
    map<unsigned int, vector<string>> table;
public:
    QMcCluskey(){}
    QMcCluskey(vector<set<string>> vec){
        boolFun = vec;
    }
    void QMAlgo();
    void Petrick_Method();
    void createTable();
    void merge(int round);
    string compare(string c, string n);
    void printAns();
};