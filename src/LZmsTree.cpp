//
//  LZmsTree.cpp
//  VOMMtest
//
//  Created by Benjamin Smith on 11/11/18.
//

#include <stdio.h>
#include "../include/algs/lzms/LZmsTree.hpp"

using namespace vmm_algs_lzms;
using namespace std;

const double LZmsTree::negativeInverseOfLg = -(1/log(2.0));

LZmsTree::LZmsTree(int _minContext, int _numShifts, int _alphabetSize) : minContext(_minContext), numShifts(_numShifts) {
    root = new LZNode(_alphabetSize);
    negativeInverseOfLogAlphabetSize = -1 / log(_alphabetSize);
    
    likelihoodsSet = false;
}

LZmsTree::~LZmsTree() {
    delete root;
}

void LZmsTree::learnSequence(std::vector<int>* sequence) {
    for (int i = 0; i <= numShifts; i++)
        learnOffsetSequence(sequence, i);
}

void LZmsTree::learnOffsetSequence(vector<int>* sequence, int offset) {
    likelihoodsSet = false; // will have to update likelihoods before rating sequences
    LZNode* currentNode = root;
    LZNode* child; //, *temp;
    
    // good code apparently. Added on 10/30/02
    for (int i = offset; offset <= i && i < sequence->size(); i++) {
        child = currentNode->getChild(sequence->at(i));
        if (child != NULL) {
            currentNode = child;
            continue;
        }
        
        // child == NULL
        if (minContext <= currentNode->getDepth())
            // just have to add on a single character
            //child =
            new LZNode(currentNode, sequence->at(i), true);
        else {
            // have to add on the rest of the window + a single character
            int length = fmin(minContext - currentNode->getDepth() + 1, sequence->size() - i);
            //child =
            LZNode::createBranch(currentNode, sequence, i, length);
            i += length;
        }
        
        currentNode = root;
        i -= minContext;
    }
    
    if (currentNode != root)
        currentNode->markEnd();
}

/**
 Calculates the probability of a string
 @return A double between 0 and 1
 */
double LZmsTree::getProbablityOfSequence(std::vector<int>* sequence) {
    if (!likelihoodsSet) {
        root->setLikelihoods();
        likelihoodsSet = true;
    }
    
    double result = 0; // Will hold the probability. Initialized to log(1).
    LZNode* currentNode = root;
    LZNode *child; //, *temp = NULL;
    
    for (int i = 0; i < sequence->size(); i++) {
        if (currentNode == NULL) {
            // currentNode can't be found in the previous position. Backtrack (upto) min spaces...
            int mark = i;
            i = (i < minContext) ? 0 : i - minContext;
            
            currentNode = root; // reset to the root
            // and trace downwards
            for (int j = i; j < mark && currentNode != NULL; j++)
                currentNode = currentNode->getChild(sequence->at(j));
            
            i = mark; // continue from where we started.
        }
        
        if (currentNode == NULL) // the trace was broken. no similar path exists! <-- used to be before i = mark, but this makes more sense
            currentNode = root; // currentNode will have to be root.
        
        // get the child.
        if (currentNode)    // unless root is invalid...
            child = currentNode->getChild(sequence->at(i));
        
        if (child != NULL)
            result += child->getLogLikelihoodFromParent();
        else
            result += currentNode->getLogLikelihoodToVirtualChild();
        
        currentNode = child;
    }
    
    return result;
}

double LZmsTree::rateSequence(vector<int>* sequence) {
    if (sequence == NULL || sequence->size() == 0)
        return 1;
    /**@ron        return (getProbablityOfSequence(sequence) * negativeInverseOfLogAlphabetSize) / sequence.length;*/
    return (getProbablityOfSequence(sequence) * negativeInverseOfLg) / (double)sequence->size();
}

/**
 * Calculates the loglikelihood of sequence
 * @param sequence of symbols
 * @return -lg (P (sequence | training) )
 * @author Ron B.
 */
double LZmsTree::calcLogLikelihood(vector<int>* sequence) {
    double ret = getProbablityOfSequence(sequence);
    ret *= negativeInverseOfLg;
    return ret;
}
