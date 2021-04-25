#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "Quine_McCluskey.h"

using namespace std;
/******
 * 2021/4/24 codyRuan
    EDA PA3 -logic minimization-
    using Quine-McCluskey and Petrick_Method.
*******/
void read_file(ifstream &ifile, ofstream &ofile);
void spiltstr(vector<string> &vec, string s, string splitsep);
void convertToBoolean(vector<set<string>> &boolvec, vector<string> &charvec);
int main(int argc, char *argv[]){
    ofstream ofile(argv[2], ios::out);
    ifstream ifile(argv[1], ios::in);
    read_file(ifile, ofile);
    return 0;
}

void read_file(ifstream &ifile, ofstream &ofile){
    string inOrder, outOrder, tempS, inputFunc;
    char tempC;
    getline(ifile, inOrder);
    getline(ifile, outOrder);
    vector<string> charFunc;
    vector<set<string>> booleanFunc((inOrder.size()-10)/2+1);
    ifile >> tempS >> tempC >> inputFunc;
    spiltstr(charFunc, inputFunc, "+"); // spilt the input by +
    convertToBoolean(booleanFunc, charFunc);
    QMcCluskey QM(booleanFunc);
    QM.QMAlgo();
    // QM.printAns();
}

void spiltstr(vector<string> &vec, string s, string splitsep){
    int current = 0, next = 0;
    while(next != -1){
        next = s.find_first_of(splitsep, current);
        if(next != current){
            string tmp = s.substr(current, next-current);
            if(!tmp.empty()){
                vec.push_back(tmp);
            }
        }
        current = next + 1;
    }
}

void convertToBoolean(vector<set<string>> &boolvec, vector<string> &charvec){
    string tmpstr;
    vector<string> tmpvec;
    int numOfone = 0;
    for(string s : charvec){
        tmpstr = s.substr(1,s.size()-2);
        // cout << tmpstr << endl;
        spiltstr(tmpvec, tmpstr, "*");
        tmpstr.clear();
        numOfone = 0;
        for(string t : tmpvec){
            if(t.size() == 1){  // a
                tmpstr += '1';
                numOfone ++;
            }
            else                // !a
                tmpstr += '0';
        }
        tmpvec.clear();
        boolvec.at(numOfone).insert(tmpstr);
        // cout << tmpstr << endl;
    }
}