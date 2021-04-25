#include "Quine_McCluskey.h"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

void QMcCluskey::QMAlgo(){
    for(int i = 0; i < boolFun.size()-2; i++){
        merge(i);
        printAns();
    }
}

void QMcCluskey::merge(int round){
    vector<set<string>> tmpvec(boolFun);
    boolFun.clear();
    boolFun.resize(tmpvec.size());
    bool CanMerge = false;
    for(int i = 0; i < tmpvec.size()-1; i++){
        set<string> currentSet, nextSet;
        int idx;
        if(i != tmpvec.size()-round-1){ // 判斷是不是本輪最後一個
            currentSet = tmpvec.at(i);
            nextSet = tmpvec.at(i+1);
            idx = i;
        }
        else{
            currentSet = tmpvec.at(i);
            nextSet = tmpvec.at(i-1);
            idx = i-1;
        }
        for(string currentStr : currentSet){
            CanMerge = false;
            for(string nextStr : nextSet){
                string tmp = compare(currentStr, nextStr);
                if(!tmp.empty()){
                    boolFun.at(idx).insert(tmp);
                    CanMerge = true;
                }
            }
            if(!CanMerge)
                implicants.push_back(currentStr);
        }
    }
}

string QMcCluskey::compare(string c, string n){
    int different = 0, index;
    for(int i = 0; i < c.size(); i++){
         if(c[i] != n[i]){
             different++;
             index = i;
         }
    }
    string Mergestr;
    if(different == 1){
        Mergestr.assign(c);
        Mergestr[index] = '-';
    }
    return Mergestr;
}

void QMcCluskey::printAns(){
    cout << "implicants:" << endl;
    for(string s : implicants){
        cout << s << endl;
    }
    cout << "merged boolean:" << endl;
    for(set<string> ss : boolFun){
        for(string s : ss){
            cout << s << " ";
        }
        cout << endl;
    }
}

void QMcCluskey::Petrick_Method(){
    createTable();
}

void QMcCluskey::createTable(){
    
}