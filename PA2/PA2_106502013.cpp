#include <iostream>
#include <vector>
#include <fstream>
#include "graph.h"

using namespace std;
/******
 * 2021/3/26 codyRuan
    EDA PA2 -resource-constrained scheduler to minimize the latency-
    using topological sort to calculate critical path and then select the operator
    to the adder/multiplier on the critical path first.
*******/
void read_file(ifstream &ifile, ofstream &ofile);

int main(int argc, char *argv[]){
    ofstream ofile(argv[2], ios::out);
    ifstream ifile(argv[1], ios::in);
    read_file(ifile, ofile);

    return 0;
}

void read_file(ifstream &ifile, ofstream &ofile){
    int adder, muls, numOfnodes, numOfedges;
    ifile >> adder >> muls >> numOfnodes >> numOfedges;
    Graph graph(numOfnodes, adder, muls);
    int num;
    char symbol;
    for (int i = 0; i < numOfnodes; i++){ 
        ifile >> num >> symbol;
        graph.AddNode(num, symbol);
    }
    int front, to;
    for (int i = 0; i < numOfedges; i++){
        ifile >> front >> to;
        graph.AddEdgeList(front, to);
    }
    graph.DFS();
    ifile.close();
    graph.writeAns(ofile);
    ofile.close();
    cout << "done!" << endl;
}