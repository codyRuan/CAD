#include "graph.h"
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

void Graph::AddEdgeList(int front, int to){
    adjList.at(front).push_back(to);
    parentList.at(to).push_back(front);
}

void Graph::AddNode(int num, int sym){
    Nodes.at(num).opt = sym;
    if(sym == 'o' || sym == 'i') num_operator--;
}

void Graph::DFS(){ // 主要目的為計算每點的cost
    for (int i = 1; i <= num_vertex; i++){
        if(Nodes.at(i).color == 0){
            DFSVisit(i);
        }
    }
}

void Graph::DFSVisit(int vertex){
    int max_cost = 0;
    for(int n : adjList.at(vertex)){
        if(Nodes.at(n).color == 0){
            DFSVisit(n);
            if(Nodes.at(n).cost > max_cost)
                max_cost = Nodes.at(n).cost;
        }
        else{
            if(Nodes.at(n).cost > max_cost)
                max_cost = Nodes.at(n).cost;
        }
    }
    Nodes.at(vertex).color = 1; // set node to black
    max_cost = Nodes.at(vertex).opt == '+' ? max_cost+=1 : max_cost+=3; // add cost itself
    if(Nodes.at(vertex).opt == 'o' || Nodes.at(vertex).opt == 'i') return;
    TopoList.push_front(vertex);
    Nodes.at(vertex).cost = max_cost;
}

int Graph::ML_RCS(){
    int latency = 0;
    for(int i = 1; i < Nodes.size(); i++){ // 先把input/output拔掉
        if(Nodes.at(i).opt == 'i'){
            deleteNode(i);
            Nodes.at(i).color = 2;
        }
        else if(Nodes.at(i).opt == 'o')
            Nodes.at(i).color = 2;
    }
    while(num_operator != 0){
        checker(latency);
        if(num_operator == 0) break;
        vector<int> tmp1(0), tmp2(0);
        for(int n : ongoingNodes){
            tmp2.push_back(n);
        }
        for(int i = 0; i < num_adder; i++){   // adder
            if(!adder_readyNodes.empty()){
                deleteNode(adder_readyNodes.back());
                tmp1.push_back(adder_readyNodes.back());
                adder_readyNodes.pop_back();
                num_operator --;
            }
        }
        for(int i = 0; i < num_muls-ongoingNodes.size(); i++){  // multiplier
            if(!mul_readyNodes.empty()){
                Nodes.at(mul_readyNodes.back()).color = 2;
                tmp2.push_back(mul_readyNodes.back());
                ongoingNodes.push_back(mul_readyNodes.back());
                Nodes.at(mul_readyNodes.back()).opt_finish_time = (latency+3);
                mul_readyNodes.pop_back();
            }
        }
        
        adderList.push_back(tmp1);
        mulList.push_back(tmp2);
        tmp1.clear();
        tmp2.clear();
        latency++;
    }
    return latency;
}

void Graph::deleteNode(int vertex){
    for(int v : adjList.at(vertex)){
        parentList.at(v).erase(find(parentList.at(v).begin(), parentList.at(v).end(), vertex));
    }
    Nodes.at(vertex).color = 2;
}

void Graph::checker(int time){ // 檢查是否有已完成的乘法和是否有可工作的運算子
    for(int i = 1; i < parentList.size(); i++){
        if(parentList.at(i).empty()){ // check if exist free operator
            if(Nodes.at(i).opt == '+' && Nodes.at(i).color != 2)
                adder_readyNodes.push_back(i);
            else if(Nodes.at(i).opt == '*' && Nodes.at(i).color != 2)
                mul_readyNodes.push_back(i);
        }
        if(Nodes.at(i).opt_finish_time == time){ // check mul
            ongoingNodes.erase(find(ongoingNodes.begin(), ongoingNodes.end(), i));
            deleteNode(i);
            num_operator --;
        }
    }
    sort(adder_readyNodes.begin(), adder_readyNodes.end()); // delete duplicate data
    adder_readyNodes.erase(unique(adder_readyNodes.begin(), adder_readyNodes.end()), adder_readyNodes.end());
    sort(mul_readyNodes.begin(), mul_readyNodes.end());
    mul_readyNodes.erase( unique(mul_readyNodes.begin(), mul_readyNodes.end()), mul_readyNodes.end());

    sortList(adder_readyNodes); // 由cost小到大排序
    sortList(mul_readyNodes);
}

void Graph::sortList(vector<int> &vec){ // 由cost小到大排序
    for(int i = 0; i < vec.size(); i++){
        for(int j = 0; j < vec.size()-1; j++){
            int tmp;
            if(Nodes.at(vec.at(j)).cost > Nodes.at(vec.at(j+1)).cost){
                tmp = vec.at(j);
                vec.at(j) = vec.at(j+1);
                vec.at(j+1) = tmp;
            }
        }
    }
}

void Graph::writeAns(ofstream &ofile){
    ofile << ML_RCS() << '\n';
    for(int i = 0; i < adderList.size(); i++){
        for(int j = 0; j < mulList.at(i).size(); j++){
            ofile << mulList.at(i).at(j);
            if(!adderList.at(i).empty() || j != mulList.at(i).size()-1)
                ofile << " ";
        }
        for(int j = 0; j < adderList.at(i).size(); j++){
            ofile << adderList.at(i).at(j);
            if(j != adderList.at(i).size()-1)
                ofile << " ";
        }
        if(i != adderList.size()-1)
            ofile << '\n';
    }
}
