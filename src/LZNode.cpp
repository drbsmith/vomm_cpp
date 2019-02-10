//
//  LZNode.cpp
//  max-external
//
//  Created by Benjamin Smith on 11/11/18.
//

#include <stdio.h>

#include "../include/algs/lzms/LZNode.hpp"
#include "../include/algs/lzms/LZNodeStorageFactory.hpp"

using namespace vmm_algs_lzms;
using namespace std;

void LZNode::initializeFromParent(LZNode* _parent) {
    parent = _parent;
    depth = (parent == NULL ? 0 : parent->depth + 1);
    
    counter = endCounter = 1; // only one leaf in the root's subtree (itself)
}

LZNode::LZNode() { }

LZNode::LZNode (int _alphabetSize) : alphabetSize(_alphabetSize) {
    children = LZNodeStorageFactory::create(alphabetSize);
    
    initializeFromParent(NULL);
}
/**
 Constructs a single node
 @param parent The parent that created this node
 */
LZNode::LZNode(LZNode* parent, int symbol, bool _increment) {
    parent->children->put(symbol, this); // add this into parent's children.
    
    alphabetSize = parent->GetAlphabetSize();
    children = LZNodeStorageFactory::create(alphabetSize);
    
    initializeFromParent(parent);
    
    if (_increment)
        increment(1);
}
// TODO: there are two constructors with the arguments flipped, only dif is one calls "increment(1)", this is CONFUSING!!!!
//    LZNode(int symbol, LZNode* parent) {
//        parent->children->put(symbol, this); // add this into parent's children.
//
//        alphabetSize = parent->GetAlphabetSize();
//        children = LZNodeStorageFactory::create(alphabetSize);
//
//        initializeFromParent(parent);
//    }

LZNode::~LZNode() {
    if (children != NULL) {
        delete children;
    }
    if (parent != NULL)
        delete parent;
}

int LZNode::GetAlphabetSize() {
    return alphabetSize;
}

// TODO: it wants to return the final leaf, and discard the container. Will they get cleaned up ok?
LZNode* LZNode::createBranch(LZNode* parent, vector<int>* symbols, int off, int len) {
    vector<LZNode*> descendents; // = new vector<LZNode*>(); //[len + 1];
    
    descendents.push_back(parent);
    for (int i = 1, j = off; i < len + 1; i++, j++) {
        LZNode* p = descendents[i-1];
        descendents.push_back(new LZNode(p, symbols->at(j), false));
    }
    for (int i = 1, addOn = len - 1; addOn >= 0; i++, addOn--)
        descendents[i]->counter += addOn;
    
    parent->increment(len);
    
    return descendents[len]; // return the leaf at the end of the branch
}

LZNode* LZNode::getChild(int symbol) {
    return (LZNode*)(children->get(symbol));
}

void LZNode::markEnd() {
    for (LZNode* iter = this; iter != NULL; iter = iter->parent)
        iter->endCounter++;
}

// when one creates a node it has alphabetSize leaves. For every leaf expanded it gains alphabetSize - 1 leaves.
// if the counter had started at 0 we would have a total of: alphabetSize + counter*(alphabetSize - 1) leaves =
// = alphabetSize - 1 + 1 + counter*(alphabetSize - 1) leaves = (counter + 1)*(alphabetSize - 1) + 1 leaves.
// since we started the counter from 1 and not from 0, we get the following equation: counter * normalizedABSize + 1
// aditionally there are endCounter leaves for paths that ended mid-Tree.
// however, endCounter started from 1, and so contains the +1 from before.
void LZNode::setLikelihoods() {
    if (parent == NULL) {
        setNumLeaves();
        logLikelihoodFromParent = 0;
    }
    
    vector<LZNode* > * childNodes = (vector<LZNode*>*)(children->toArray());
    
    if (childNodes->size() > 0) {
        double inverseNumLeaves = 1.0 / (double)numLeaves;
        for (int i = 0; i < childNodes->size(); i++) {
            childNodes->at(i)->setNumLeaves();
            childNodes->at(i)->logLikelihoodFromParent = log(childNodes->at(i)->numLeaves * inverseNumLeaves);
        }
    }
    
    logLikelihoodToVirtualChild = -log(numLeaves);
    
    for (int i = 0; i < childNodes->size(); i++)
        childNodes->at(i)->setLikelihoods();
}

int LZNode::getAlphabetSize() {
    return alphabetSize;
}

int LZNode::getDepth() {
    return depth;
}

double LZNode::getLogLikelihoodFromParent() {
    return logLikelihoodFromParent;
}

double LZNode::getLogLikelihoodToVirtualChild() {
    return logLikelihoodToVirtualChild;
}

LZNode* LZNode::getParent() {
    return parent;
}

void LZNode::increment(int value) {
    for (LZNode* iter = parent; iter != NULL; iter = iter->parent)
        iter->counter += value;
}

void LZNode::setNumLeaves() {
    numLeaves = counter * (alphabetSize - 1) + endCounter;
}
