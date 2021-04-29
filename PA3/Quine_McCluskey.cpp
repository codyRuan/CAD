#include "Quine_McCluskey.h"
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

bool compareSize(const set<string> a,const set<string> b){
    return (a.size() < b.size());
}

void QMcCluskey::QMAlgo(){
    for(int i = 0; i < boolFun.size()-2; i++){
        merge(i);
        // cout << "round" << i+1 << endl;
        // printAns();
    }
}

void QMcCluskey::merge(int round){
    vector<set<pair<string,set<unsigned int>>>> tmpvec(boolFun);
    boolFun.clear();
    boolFun.resize(tmpvec.size());
    bool CanMerge = false;
    for(int i = 0; i < tmpvec.size()-1; i++){
        set<pair<string,set<unsigned int>>> currentSet, nextSet;
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
        for(pair<string,set<unsigned int>> currentPair : currentSet){
            CanMerge = false;
            for(pair<string,set<unsigned int>> nextPair : nextSet){
                set<unsigned int> tmp_m = currentPair.second;
                string tmp = compare(currentPair.first, nextPair.first);
                if(!tmp.empty()){
                    for(unsigned int i : nextPair.second)
                        tmp_m.insert(i);
                    boolFun.at(idx).insert(make_pair(tmp,tmp_m));
                    CanMerge = true;
                }
            }
            if(!CanMerge)
                implicants.push_back(make_pair(currentPair.first, currentPair.second));
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

void QMcCluskey::Petrick_Method(){
    createTable();
    make_POS();
    POS2SOP();
    getMinLogic();
}

void QMcCluskey::createTable(){
    for(pair<string, set<unsigned int>> s : implicants){
        for(unsigned int ui : s.second){
            if(table.find(ui) != table.end()){
                table.at(ui).push_back(s.first);
            }
            else{
                vector<string> vec;
                vec.push_back(s.first);
                table.insert(make_pair(ui, vec));
            }
        }
    }
}

void QMcCluskey::make_POS(){
    for(auto const& x : table){
        set<set<string>> orSet;
        for(string s : x.second){
            set<string> andSet;
            andSet.insert(s);
            orSet.insert(andSet);
        }
        POS.push_back(orSet);
    }
}

void QMcCluskey::POS2SOP(){
    while(true){
        vector<set<set<string>>> tmp(POS);
        POS.clear();
        for(int i = 0; i < tmp.size()-1; i += 2){
            if(tmp.size() % 2 == 1){
                POS.push_back(tmp.at(tmp.size()-1));
            }
            POS.push_back(getUnion(tmp.at(i), tmp.at(i+1)));
        }
        if(POS.size() == 1)
            return;
    }
}

set<set<string>> QMcCluskey::getUnion(set<set<string>> a, set<set<string>> b){
    set<set<string>> tmp;
    for(set<string> setA : a){
        for(set<string> setB : b){
            tmp.insert(Union(setA, setB));
        }
    }
    return tmp;
}

set<string> QMcCluskey::Union(set<string> a, set<string> b){
    set<string> result = a;
    result.insert(b.begin(), b.end());
    return result;
}

void QMcCluskey::getMinLogic(){
    set<set<string>> SOP = POS.at(0);
    vector<set<string>> v(SOP.begin(), SOP.end());
    sort(v.begin(), v.end(), compareSize);
    for(string s : v.at(0)){
        minLogic.push_back(s);
    }
}

void QMcCluskey::printAns(){
    cout << "implicants:" << endl;
    for(pair<string,set<unsigned int>> s : implicants){
        cout << s.first << " ";
        for(unsigned int i : s.second)
            cout << i << " ";
        cout << endl;
    }
    cout << "merged boolean:" << endl;
    for(set<pair<string,set<unsigned int>>> ss : boolFun){
        for(pair<string,set<unsigned int>> s : ss){
            cout << s.first << " ";
            for(unsigned int i : s.second)
                cout << i << " ";
        }
        cout << endl;
    }
}

void QMcCluskey::printTable(){
    for(auto const& x : table){
        cout << x.first << " ";
        for(string s : x.second)
            cout << s << " ";
        cout << endl;
    }
}

void QMcCluskey::printPOS(){
    for(set<set<string>> ss : POS){
        cout << '(';
        for(set<string> s : ss){
            for(string str : s){
                cout << str << "*";
            }
            cout << "+";
        }
        cout << ')';
    }
}

vector<string> QMcCluskey::getMin(){
    return minLogic;
}