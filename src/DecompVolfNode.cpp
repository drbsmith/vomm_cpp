//
//  DecompVolfNode.cpp
//  VOMMtest
//
//  Created by Benjamin Smith on 2/17/19.
//

#include <stdio.h>
#include <sstream>

#include "../include/algs/ctw/DecompVolfNode.hpp"

using namespace vmm_algs_ctw;

DecompVolfNode::DecompVolfNode() : symCountArr(NULL) {
    
}
DecompVolfNode::~DecompVolfNode() {
    if (symCountArr)
        delete symCountArr;
    
    vector<DecompVolfNode*>::iterator it = children.begin();
    while (it != children.end()) {
        delete *it++;
    }
    children.clear();
}

/**
 * Initializes this VolfNode
 *
 * @param symABSize size of the samples AB
 * @param alphaFactor as defined in Volf's thesis, for the 0 order estimator.
 */
void DecompVolfNode::init(int symABSize, int alphaFactor) {
    abSize = symABSize;
    alpha = alphaFactor;
    
    alphaInversed = 1.0/(double)alpha;
    abSizeDivAlpha = 2.0/(double)alpha;
    
    symCountArr = new int[2];/*ZZZ*/
    symCountArr[0] = 0; symCountArr[1] = 0;
    //        Arrays.fill(symCountArr,0);
    allCount = 0;
    vector<DecompVolfNode*>::iterator it = children.begin();
    while (it != children.end()) {
        delete *it++;
    }
    children.clear();
    beta = 1.0;
}

double DecompVolfNode::predict(int symbol, int symSetLabel, ContextIterator* context) {
    double *pwArr = predict(context);
    //        vmm.util.PerdictAssertion.assertThis(pwArr);
    if (pwArr) {
        double ret = pwArr[symSetLabel];
        delete[] pwArr;
        return ret;
    } else
        return -1;
}

double DecompVolfNode::learn(int symbol, int symSetLabel, ContextIterator* context) {
    double *res = learnAll(symbol, symSetLabel, context);
    //        vmm.util.PerdictAssertion.assertThis(res);
    if (res) {
        double ret = res[symSetLabel];
        delete[] res;
        return ret;
    } else
        return -1;
}

string DecompVolfNode::toString() {
    stringstream data;
    
    data << "d";
    data << abSize << "/";
    data << alpha << "/";
    data << symCountArr[0] << "/";
    data << symCountArr[1] << "/";
    data << allCount << "/";
    data << beta << "/";
    data << alphaInversed << "/";
    data << abSizeDivAlpha << "/";
    
    for (vector<DecompVolfNode*>::iterator it = children.begin(); it != children.end(); it++) {
        if (*it != NULL)
            data << endl << "[" << *it << " " << (*it)->toString() << "]";
    }
    
    return data.str();
}

DecompVolfNode* DecompVolfNode::fromString(std::string data) {
    if (data.size() < 8)
        return NULL;
    
    DecompVolfNode* ret = NULL;
    
    if (data[0] == 'd') {   // it's a node string
        std::vector<std::string> result;
        std::istringstream iss(data);
        for(std::string s; iss >> s; )
            result.push_back(s);
        
//        DecompVolfNode *active = NULL;
        for (vector<string>::iterator it = result.begin(); it != result.end(); it++) {
            if ((*it).size() >= 5) { // minimum size for a node specification
                if ((*it)[0] == '[') { // it's a child
                    (*it).erase((*it).begin());
                    DecompVolfNode* child = new DecompVolfNode(*it);
                    if (ret)    // should always be set already.
                        ret->AddChild(child);
                    else
                        delete child;
                    
//                    active = child;
                }  else {    // it's the first node
                    ret = new DecompVolfNode(*it);   // create the root
//                    active = ret;
                }
            }
        }
    }
    
    return ret;
}

DecompVolfNode::DecompVolfNode(string data) {
    if (data.size() >= 5) {
        try {
            string::size_type first = data.find('/');
            string element = data.substr(0,first);
            data.erase(0,first+1); // erase the first part
            abSize = std::stoi(element, NULL, 10);
            
           first = data.find('/');
            element = data.substr(0,first);
            data.erase(0,first+1); // erase the first part
            alpha = std::stoi(element, NULL, 10);
            
            symCountArr = new int[2];
            
            first = data.find('/');
            element = data.substr(0,first);
            data.erase(0,first+1); // erase the first part
            symCountArr[0] = std::stoi(element, NULL, 10);
            
            first = data.find('/');
            element = data.substr(0,first);
            data.erase(0,first+1); // erase the first part
            symCountArr[1] = std::stoi(element, NULL, 10);
            
            first = data.find('/');
            element = data.substr(0,first);
            data.erase(0,first+1); // erase the first part
            allCount = std::stoi(element, NULL, 10);
            
            string::size_type sz;
            first = data.find('/');
            element = data.substr(0,first);
            data.erase(0,first+1); // erase the first part
            alphaInversed = std::stod(element, &sz);
            
            // last number goes to the end
            abSizeDivAlpha = std::stod(data, &sz);
        } catch (exception ex) {
            ; // garbage in, garbage out!
        }
    }
}

void DecompVolfNode::AddChild(DecompVolfNode* child) {
    if (child != NULL)
        children.push_back(child);
}

double* DecompVolfNode::predict(ContextIterator* context) {
    if (context->hasNext()) {
        DecompVolfNode* childOnContextPath = getChild(context->nextSymbol());
        double *ethaArr = childOnContextPath->predict(context);
        double denominator = 0.0;
        //1. compute b'
        double betaTag = beta / (allCount + abSizeDivAlpha);
        //2. compute intermediate result etha(context, sym)
        for( int sym=0; sym<2; ++sym) {
            ethaArr[sym] += betaTag * (symCountArr[sym]+alphaInversed);/**@todo (2)*/
            denominator += ethaArr[sym];
        }
        //3. compute conditional weighted probabilities
        for (int sym=0; sym<2; ++sym) {
            ethaArr[sym] /= denominator;
        }
        //4., 5. appear in learn method (no updates)
        return ethaArr;
    }
    else { //leaf - 0 order prediction
        double *peArr = new double[2];
        for( int sym=0; sym<2; ++sym) {
            peArr[sym] = (symCountArr[sym]+alphaInversed)/(allCount + abSizeDivAlpha);
        }
        return peArr;
    }
}



double* DecompVolfNode::learnAll(int newSymbol, int symSetLabel, ContextIterator* context) {
    if (context->hasNext()) {
        
        DecompVolfNode* childOnContextPath = getChild(context->nextSymbol());
        double *ethaArr = childOnContextPath->learnAll(newSymbol, symSetLabel, context);
        double childOnContextPw = ethaArr[symSetLabel];
        double denominator = 0.0;
        //1. compute b'
        double betaTag = beta / (allCount + abSizeDivAlpha);
        //2. compute intemediate results into etha(context, sym)
        for(int sym=0; sym<2; ++sym) {
            ethaArr[sym] += betaTag * (symCountArr[sym]+alphaInversed);
            denominator += ethaArr[sym];
        }
        //3. compute conditional weighted probabilities
        for(int sym=0; sym<2; ++sym) {
            ethaArr[sym] /= denominator;
        }
        
        //4. update beta
        if(beta>1500000) {
            beta /= 2.0;
        }
        else if (beta<(1.0/1500000)){
            beta *= 2.0;
        }
        else {
            beta = (betaTag * (symCountArr[symSetLabel] + alphaInversed)) / childOnContextPw;
        }
        
        //5. update symbol count
        symCountArr[symSetLabel]++;
        allCount++;
        if (symCountArr[symSetLabel]>MAX_COUNT_BEFORE_COUNT_HALVING)
            rescaleCounts();
        
        return ethaArr;
    } else { //leaf - 0 order prediction
        double *peArr = new double[2];
        for (int sym=0; sym<2; ++sym) {
            peArr[sym] = (symCountArr[sym]+alphaInversed)/(allCount + abSizeDivAlpha);
        }
        symCountArr[symSetLabel]++;
        allCount++;
        if (symCountArr[symSetLabel]>MAX_COUNT_BEFORE_COUNT_HALVING)
            rescaleCounts();
        
        return peArr;
    }
}

DecompVolfNode* DecompVolfNode::getChild(int sym) {
    if (children.size() < abSize) {
        children.resize(abSize, NULL);
    }
    if (children.at(sym) == NULL) {
        children.at(sym) = new DecompVolfNode();
        children.at(sym)->init(abSize, alpha);
    }
    return children.at(sym);
}

void DecompVolfNode::rescaleCounts() {
    if (!symCountArr)
        return;
    
    allCount = 0;
    for(int i=0, rounding=0; i < 2; i++){
        rounding = symCountArr[i] % 2;
        symCountArr[i] = symCountArr[i] >> (1 + rounding);
        allCount += symCountArr[i];
    }
    }
