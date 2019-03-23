//
//  AbsBinaryDNode.cpp
//  VOMMtest
//
//  Created by Benjamin Smith on 2/17/19.
//

#include <stdio.h>
#include <sstream>
#include <cstring>

/* HEADER
 If you use this code donít forget to reference us :) BibTeX: http://www.cs.technion.ac.il/~rani/el-yaniv_bib.html#BegleiterEY04
 
 This code is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (<a href="http://www.gnu.org/copyleft/gpl.html">GPL</a>) for more details.*/

#include "../include/algs/decomp/AbsBinaryDNode.hpp"

using namespace vmm_algs_decomp;
using namespace std;

BitSet::BitSet(unsigned int size) {
    bits.resize(size, 0);   // set all to false
}
BitSet::~BitSet() {
}

void BitSet::OR(BitSet *other) {
    vector<bool>::iterator it2 = other->bits.begin();
    for (vector<bool>::iterator it = bits.begin(); it != bits.end() && it2 != other->bits.end(); it++, it2++) {
        *it = *it || *it2;
    }
    //            bits = bits | other->bits;
}
void BitSet::AND(BitSet *other) {
    vector<bool>::iterator it2 = other->bits.begin();
    for (vector<bool>::iterator it = bits.begin(); it != bits.end() && it2 != other->bits.end(); it++, it2++) {
        *it = *it && *it2;
    }
    //            bits = bits & other->bits;
}
int BitSet::Cardinality() {
    int card = 0;
    for (vector<bool>::iterator it = bits.begin(); it != bits.end(); it++) {
        if (*it)
            card++;
    }
    return card;
}
bool BitSet::equals(BitSet* other) {
    bool theSame = bits.size() == other->bits.size();
    
    vector<bool>::iterator it = bits.begin(), it2 = other->bits.begin();
    while (theSame && it != bits.end() && it2 != other->bits.end()) {
        theSame = *it == *it2;
        it++;
        it2++;
    }
    return theSame;
    //            return numBits == other->numBits && bits == other->bits; // TODO: should check to see if all the values in each are the same
}
void BitSet::Set(int bit, bool val) {
    if (bit >= 0 && bit < bits.size())
        bits.at(bit) = val;
    // else
    //      error!!! out of bounds
}
bool BitSet::Get(int bit) {
    if (bit >= 0 && bit < bits.size())
        return bits.at(bit);
    else
        return 0;   // error!!!
}
string BitSet::toString() {
    stringstream data;
//    data << "bitset";
    for (vector<bool>::iterator it = bits.begin(); it != bits.end(); it++)
        data << ((*it) ? "1" : "0");
//    data << "|";
    return data.str();
}
void BitSet::fromString(std::string data) {
//    if (data.compare("bitset") == 0) {
        bits.clear();
        data = data.substr(6); // strip off 'bitset'
        for (int i = 0; i < data.size(); i++) {
            bits.push_back(data[i] == '0' ? 0 : 1);
        }
//    }
}
    /**
     * <p>Title: AbsDNode</p>
     * <p>Description: Abstract implementation for the DecompositionNode</p>
     * <p>Copyright: Copyright (c) 2003</p>
     * <p>Company: </p>
     * @author not attributable
     * @version 1.0
     */

AbsBinaryDNode::~AbsBinaryDNode() {
    while (children.size() > 0) {
        delete *(children.begin());
        children.erase(children.begin());
    }
    if (softClasifier)
        delete softClasifier;
    if (descendants)
        delete descendants;
}

AbsBinaryDNode::AbsBinaryDNode(int abSize, AbsBinaryDNode* rightChild, AbsBinaryDNode* leftChild, int softModelDepth) {
    myType = nodeType::AbsBinaryDNode;
    
    descendants = new BitSet(abSize);// all bits are initially false
    children.resize(BRANCHING_FACTOR);
    children[RIGHT] = rightChild;
    children[LEFT] = leftChild;
    
    // -- ASSERT --
    if (rightChild && leftChild) {
        BitSet test(abSize);
        test.OR(rightChild->descendants);
        test.AND(leftChild->descendants);
        
        if (test.Cardinality() != 0) {  // they can't be the same.
            throw "ILL decomp node init: duplicated descendant";
        }
    }
    
    if (rightChild)
        descendants->OR(rightChild->descendants);
    if (leftChild)
        descendants->OR(leftChild->descendants);
    
    softClasifier = new DecompVolfNode();
    softClasifier->init(abSize, VolfNode::DEFAULT_ALPHA_FACTOR);
}

vmm_algs_decomp::BitSet* AbsBinaryDNode::getDescendants() {
    return descendants;
}
//  int hashCode(){
//    return descendants->hashCode();
//  }

bool AbsBinaryDNode::equals(AbsBinaryDNode* obj) {
    try {
        return descendants->equals(obj->descendants);
    }
    catch (exception ex) { //}(ClassCastException cce){
        return false;
    }
}

TreeMap* AbsBinaryDNode::getDescendantsByLevel() {
    TreeMap* levelsMap = new TreeMap();
    buildLevels(this, levelsMap, 0);
    return levelsMap;
}

int AbsBinaryDNode::symbol() {
    return mySym;
}

std::string AbsBinaryDNode::toString() {
    stringstream data;
    
//    data << "absBDNode" << endl;
    data << "m" << mySym << "|";
    
    if (softClasifier)
        data << "s" << softClasifier->toString() << "|";
    
    if (descendants)
        data << "d" << descendants->toString() << "|";
    
    for (vector<AbsBinaryDNode*>::iterator it = children.begin(); it != children.end(); it++) {
        if (*it != NULL)
            data << "c" << *it << " " << (*it)->toString() << "|";
    }
    
//    data << "|";
    return data.str();
}
AbsBinaryDNode* AbsBinaryDNode::fromString(std::string data) {
    stringstream ss(data);
    string onestring;
    
    while (getline(ss, onestring, '\n')) {
        if (onestring.size() >= 5) {
            AbsBinaryDNode* newNode = AbsBinaryDNode::fromString(onestring);

            if (newNode == NULL)
                return NULL;

//            if (_contexts.at(newNode->GetByte()) != NULL)
//                delete _contexts.at(newNode->GetByte());
//
//            _contexts.at(newNode->GetByte()) = newNode;
            return newNode;
        }
    }
    return NULL;
}

void AbsBinaryDNode::buildLevels(AbsBinaryDNode* root, TreeMap* levelsMap, int level){
    if (root->getType() == nodeType::BinDLeaf) { //root instanceof BinDLeaf){   // this is a true leaf, it doesn't have children to follow
        if (levelsMap->find(level) == levelsMap->end()) {
            levelsMap->insert(TreeNode(level, new LeafMap()));
        }
        LeafMap* lm = levelsMap->at(level);
        lm->insert(LeafNode(root->symbol(), root));
        return;
    }
    buildLevels(root->children[LEFT], levelsMap, level+1);
    buildLevels(root->children[RIGHT], levelsMap, level+1);
}
