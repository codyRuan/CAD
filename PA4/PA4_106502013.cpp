#include <iostream>
#include <fstream>
#include <vector>
#include "floorplan.h"

using namespace std;

void read_file(ifstream &ifile, floorplan &fp);
void write_file(ofstream &ofile, floorplan &fp);

int main(int argc, char *argv[]){
    ofstream ofile(argv[2], ios::out);
    ifstream ifile(argv[1], ios::in);
    floorplan fp;
    read_file(ifile, fp);
    fp.printRec();
    fp.SA();
    fp.printBestArea();
    fp.printBestNPE();
    fp.getBestAns();
    write_file(ofile, fp);
}

void read_file(ifstream &ifile, floorplan &fp){
    int numOfRec;
    int idx, area;
    vector<int> recs;
    ifile >>numOfRec;
    for(int i = 0; i < numOfRec; i++){
        ifile >> idx >> area;
        fp.addRec(area);
    }
    ifile.close();
}

void write_file(ofstream &ofile, floorplan &fp){
    block best_block = fp.getBestBlock();
    ofile << best_block.getBestLength().first << " " << best_block.getBestLength().second << " " << best_block.get_cost() << endl;
    for(treeNode *tn : best_block.getNodes()){
        ofile << tn->getXY().first << " " << tn->getXY().second << " " << tn->getLength().first << " " << tn->getLength().second << endl;
    }
    vector<string> npe = best_block.getNPE();
    for(int i = 0; i < npe.size(); i++){
        ofile << npe.at(i);
        if(i != npe.size()-1)
            ofile << " ";
    }
    ofile.close();
}