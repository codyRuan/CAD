#include "block.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

double block::get_cost(){
    return best_length.first * best_length.second;
}

pair<double,double> block::getBestLength(){
    return best_length;
}

vector<treeNode *> block::getNodes(){
    return Nodes;
}

vector<string> block::getNPE(){
    return npe;
}

void block::createTree(){
    vector<treeNode *> stack;
    for(string s : npe){
        if(s != "v" && s != "h"){
            vector<string> npe;
            npe.push_back(s);
            treeNode *node = new treeNode(npe, s);
            stack.push_back(node);
        }
        else{
            vector<string> npe;
            treeNode *n1 = stack.back();
            stack.pop_back();
            treeNode *n2 = stack.back();
            stack.pop_back();
            npe.insert(npe.end(),n2->npe.begin(), n2->npe.end());
            npe.insert(npe.end(),n1->npe.begin(), n1->npe.end());
            npe.push_back(s);
            treeNode *p = new treeNode(npe, s);
            p -> leftChild = n2;
            p -> rightChild = n1;
            n1 -> parent = p;
            n2 -> parent =p;
            stack.push_back(p);
        }
    }
    root = stack.back();
    root -> length = best_length;
    root -> XY = make_pair(0,0);
    cout << "create done!" << endl;
    cout << root->length.first << "," << root->length.second << endl;
    traversal(root);
    constructXY(root);
}

bool AreDoubleSame(double dFirstVal, double dSecondVal)
{
    return std::fabs(dFirstVal - dSecondVal) < 1E-3;
}

void block::traversal(treeNode *current){
    if(current->leftChild == NULL && current->rightChild == NULL){
        auto idx = stoi(current->data);
        Nodes.at(idx) = current;
        return;
    }
    treeNode *left = current -> leftChild, *right = current -> rightChild;
    vector<pair<double,double>> length_left, length_right;
    for(block b : all_blocks){
        if(b.npe == left -> npe)
            length_left = b.possible_length;
        if(b.npe == right -> npe)
            length_right = b.possible_length;
    }
    if(right->data == "4"){
        cout << "fuck" << length_right.size() << endl;
    }
    pair<double,double> firstPair = make_pair(-1,-1), secondPair= make_pair(-1,-1), parentPair = current -> length;
    if(current -> data == "h"){
        for(pair<double,double> Pair : length_left){ // 長邊在左邊
            if(AreDoubleSame(Pair.first, parentPair.first))
                firstPair = Pair;
        }
        if(firstPair.first != -1 && firstPair.second != -1){ 
            for(pair<double,double> Pair : length_right){
                if(AreDoubleSame(Pair.second ,(parentPair.second - firstPair.second)))
                    secondPair = Pair;
            }
        }
        else{ // 長邊在右邊
            for(pair<double,double> Pair : length_right){
                if(AreDoubleSame(Pair.first, parentPair.first))
                    secondPair = Pair;
            }
            for(pair<double,double> Pair : length_left){
                if(AreDoubleSame(Pair.second ,(parentPair.second - secondPair.second)))
                    firstPair = Pair;
            }
        }
        left -> length = firstPair;
        right -> length = secondPair;
    }
    else{
        for(pair<double,double> Pair : length_left){
            if(AreDoubleSame(Pair.second, parentPair.second))
                firstPair = Pair;
        }
        if(firstPair.first != -1 && firstPair.second != -1){
            for(pair<double,double> Pair : length_right){
                if(AreDoubleSame(Pair.first, (parentPair.first - firstPair.first)))
                    secondPair = Pair;
            }
        }
        else{
            for(pair<double,double> Pair : length_right){
                if(AreDoubleSame(Pair.second, parentPair.second))
                    secondPair = Pair;
            }
            for(pair<double,double> Pair : length_left){
                if(AreDoubleSame(Pair.first, (parentPair.first - secondPair.first)))
                    firstPair = Pair;
            }
        }
        left -> length = firstPair;
        right -> length = secondPair;
    }

    traversal(left);
    traversal(right);
}

void block::constructXY(treeNode *current){
    if(current->leftChild == NULL && current->rightChild == NULL) return;
    treeNode *left = current->leftChild, *right = current->rightChild;
    if(current->data == "h"){
        left->XY = current->XY;
        right->XY = make_pair(left->XY.first, left->XY.second+left->length.second);
    }
    else{
        left->XY = current->XY;
        right->XY = make_pair(left->XY.first+left->length.first, left->XY.second);
    }
    constructXY(left);
    constructXY(right);
}

void block::printAllNodes(){
    for(treeNode *tn : Nodes){
        cout << tn->data << " width:" << tn->length.first << ", height:" << tn->length.second << ", position: ("
        << tn->XY.first << "," << tn->XY.second << ")" << endl;
    }
}

void block::printAllBlocks(){
    for(block b : all_blocks){
        for(string s : b.npe)
            cout << s;
        cout << ":";
        b.printPossibleLength();
    }
}

void block::printPossibleLength(){
    for(pair<double,double> p : possible_length){
        cout << "(" << p.first << ", " << p.second << ")";
    }
    cout << endl;
}