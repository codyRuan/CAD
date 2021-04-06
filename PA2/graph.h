#include <vector>
#include <list>
#include <fstream>

using namespace std;

struct Node{
    char opt;
    int cost = 0;
    int color = 0; // 0 white 1 black
    int opt_finish_time = -1;
};

class Graph{
private:
    int num_vertex, num_adder, num_muls, num_operator;
    vector<vector<int>> adjList;
    vector<vector<int>> parentList;
    vector<vector<int>> adderList, mulList;
    vector<int> adder_readyNodes, mul_readyNodes, ongoingNodes;
    vector<Node> Nodes;
    list<int> TopoList;
protected:
    void deleteNode(int vertex);
    void checker(int time);
    void sortList(vector<int> &vec);
public:
    Graph():num_vertex(0){}    
    Graph(int N, int a, int m):num_vertex(N), num_operator(N), num_adder(a), num_muls(m){
        adjList.resize(N+1);
        Nodes.resize(N+1);
        parentList.resize(N+1);
    }
    void AddEdgeList(int from, int to);
    void AddNode(int num, int sym);
    void DFS();
    void DFSVisit(int vertex);
    int ML_RCS();
    void writeAns(ofstream &ofile);
};