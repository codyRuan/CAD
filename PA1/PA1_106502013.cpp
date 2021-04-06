#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>
#include<cctype>
#include <sstream>

using namespace std;

string blank(int a);
set<int> parserIO(set<string> v);
vector<pair<string, vector<string>>> parserGate(vector<pair<string, string>> v);
vector<string> spiltstr(string s, string splitsep);
void writeToFile(ofstream &outfile, set<int> s, int blank);

int main(int argc, char *argv[])
{
    /**  open files **/
    vector<string> allarg(argv,argv + argc);
    ifstream infile;
    ofstream outfile;
    if(argc > 1){
        infile.open(allarg[1], ios::in);
        outfile.open(allarg[2], ios::out);
    }
    else{
        cout << "please enter file name!" << endl;
        exit(1);
    }
    if (!infile) cout << "input file doesn't exist";
    if (!outfile) cout << "output file doesn't exist";

    vector<int> info;
    set<string> inset, outset;
    set<int> intInset, intOutset, gateset, wireset, ioset;
    vector<pair<string, string>> tmp_gates;
    vector<pair<string, vector<string>>> gates;
    string buffer;

    getline(infile,buffer); // skip first line
    char ch;
    int ipt;
    string str;
    for (int i = 0; i < 4; i++){ // get information(numbers of I/O)
        infile >> ch >> ipt >> str;
        info.push_back(ipt);
    }
    getline(infile,buffer);
    getline(infile,buffer); // skip blank line

    /**  get I/O data  **/
    for (int i = 0; i < info[0]; i++){ // get input data
        getline(infile, buffer);
        inset.insert(buffer);
    }
    getline(infile, buffer); // skip blank line
    for (int i = 0; i < info[1]; i++){ // get output data
        getline(infile, buffer);
        outset.insert(buffer);
    }
    getline(infile, buffer); // skip blank line
    for (int i = 0; i < info[2] + info[3]; i++){ // get gates data
        infile >> str >> ch;
        getline(infile, buffer);
        istringstream (str) >> ipt;
        gateset.insert(ipt);
        tmp_gates.push_back(make_pair(str,buffer));
    }

    /**  get I/O names  **/
    intInset = parserIO(inset);
    intOutset = parserIO(outset);
    gates = parserGate(tmp_gates);
    set_union(intInset.begin(), intInset.end(), intOutset.begin(), intOutset.end(), inserter(ioset, ioset.end()));
    set_difference(gateset.begin(), gateset.end(), ioset.begin(), ioset.end(), inserter(wireset, wireset.end()));

    infile.close();

    outfile << "`timescale 1ns / 1ps" << '\n';
    /**  print module  **/
    string outfilename = allarg[1].substr(0, allarg[1].find('.'));
    outfile << "module " << outfilename << " (";
    writeToFile(outfile, ioset, 9 + outfilename.size());
    outfile << ");" << '\n' << '\n';

    /**  print input  **/
    outfile << "input ";
    writeToFile(outfile, intInset, 6);
    outfile << ";" << '\n' << '\n';

    /**  print output  **/
    outfile << "output ";
    writeToFile(outfile, intOutset, 7);
    outfile << ";" << '\n' << '\n';

    /**  print wire  **/
    outfile << "wire ";
    writeToFile(outfile, wireset, 5);
    outfile << ";" << '\n' << '\n';

    /**  print gate  **/
    int tmp_cnt = 0, cnt = 0;
    for(pair<string,vector<string>> p : gates){
        cnt ++;
        string tmp(p.first);
        transform(p.first.begin(), p.first.end(), tmp.begin(), ::tolower);
        if (tmp == "buff") tmp = "buf";
        outfile << tmp << " " << p.first << p.second.size()-1 << '_' << cnt << " (";
        tmp_cnt = 0;
        for(string s : p.second){
            tmp_cnt ++;
            outfile << 'N' << s;
            if(tmp_cnt != p.second.size()) outfile << ", ";
        }
        outfile << ';' << '\n';
    }
    outfile << '\n' << "endmodule" << '\n';
    cout << "done" << endl;
    outfile.close();

    return 0;
}

string blank(int a){
    string tmp;
    for (int i = 0; i < a; i++){
        tmp += ' ';
    }
    return tmp;
}

set<int> parserIO(set<string> s){
    set<int> result;
    int i;
    for (string str : s){
        istringstream (str.substr(str.find('(')+1,str.find(')')-str.find('(')-1)) >> i;
        result.insert(i);
    }
    return result;
}

vector<pair<string, vector<string>>> parserGate(vector<pair<string, string>> v){
    vector<pair<string,vector<string>>> result;
    vector<string> ipts,tmp;
    string gate;
    for (pair<string, string> p : v){
        ipts.push_back(p.first);
        gate = p.second.substr(1, p.second.find('(')-1);
        tmp = spiltstr(p.second.substr(p.second.find('(')+1), ",");
        ipts.insert(ipts.end(), tmp.begin(), tmp.end());
        result.push_back(make_pair(gate, ipts));
        ipts.clear();
        tmp.clear();
    }
    return result;
}

vector<string> spiltstr(string s, string splitsep){
    vector<string> result;
    int current = 0, next = 0;
    while(next != -1){
        next = s.find_first_of(splitsep, current);
        if(next != current){
            string tmp = s.substr(current, next-current);
            if(!tmp.empty()){
                result.push_back(tmp);
            }
        }
        current = next + 2;
    }
    return result;
}

void writeToFile(ofstream &outfile, set<int> Set, int b){
    int cnt = 0;
    for(int s : Set){
        cnt++;
        outfile << 'N' << s;
        if(cnt != Set.size()) outfile << ',';
        if(cnt % 10 == 0 && cnt != Set.size()) outfile << '\n' << blank(b);
    }
}