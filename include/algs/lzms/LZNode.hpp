#ifndef LZNODE_HPP
#define LZNODE_HPP
/* HEADER
If you use this code don’t forget to reference us :) BibTeX: http://www.cs.technion.ac.il/~rani/el-yaniv_bib.html#BegleiterEY04 

This code is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
as published by the Free Software Foundation; either version 2 
of the License, or (at your option) any later version. 

This code is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
GNU General Public License (<a href="http://www.gnu.org/copyleft/gpl.html">GPL</a>) for more details.*/ 
 
/*
 * Created on Jul 8, 2003
 */
#include "LZmsNode.hpp"
#include "LZNodeStorage.hpp"
#include <vector>

namespace vmm_algs_lzms {

/**
 * LZNode is a generic node for an LZ-Tree, of size alphabet-size
 */
    class LZNode : LZmsNode {

private:
	LZNodeStorage* children = NULL;
	int alphabetSize;

	LZNode* parent = NULL; // can transfer trees
	int depth;

	//	Counters are used to CALCULATE how many leaves are in the node's subtree (but ISN"T necessarily the number of leaves)
	int counter;
	int endCounter;
	int numLeaves;

	double logLikelihoodFromParent;
	double logLikelihoodToVirtualChild;

    void initializeFromParent(LZNode* _parent);

public:
    LZNode();
    
    LZNode (int _alphabetSize);
    /**
     Constructs a single node
     @param parent The parent that created this node
     */
    LZNode(LZNode* parent, int symbol, bool _increment);
    // TODO: there are two constructors with the arguments flipped, only dif is one calls "increment(1)", this is CONFUSING!!!!
//    LZNode(int symbol, LZNode* parent) {
//        parent->children->put(symbol, this); // add this into parent's children.
//
//        alphabetSize = parent->GetAlphabetSize();
//        children = LZNodeStorageFactory::create(alphabetSize);
//
//        initializeFromParent(parent);
//    }
    
    ~LZNode();
    
    int GetAlphabetSize();

    // TODO: it wants to return the final leaf, and discard the container. Will they get cleaned up ok?
    static LZNode* createBranch(LZNode* parent, std::vector<int>* symbols, int off, int len);

    LZNode* getChild(int symbol);

    void markEnd();

	// when one creates a node it has alphabetSize leaves. For every leaf expanded it gains alphabetSize - 1 leaves.
	// if the counter had started at 0 we would have a total of: alphabetSize + counter*(alphabetSize - 1) leaves =
	// = alphabetSize - 1 + 1 + counter*(alphabetSize - 1) leaves = (counter + 1)*(alphabetSize - 1) + 1 leaves.
	// since we started the counter from 1 and not from 0, we get the following equation: counter * normalizedABSize + 1
	// aditionally there are endCounter leaves for paths that ended mid-Tree.
	// however, endCounter started from 1, and so contains the +1 from before.
    void setLikelihoods();

    int getAlphabetSize();

    int getDepth();

    double getLogLikelihoodFromParent();

    double getLogLikelihoodToVirtualChild();

    LZNode* getParent();

private:
    void increment(int value);
    
    void setNumLeaves();
};

} // namespace
#endif
