//
//  LZmsPredictor.cpp
//  VOMMtest
//
//  Created by Benjamin Smith on 11/12/18.
//

#include <stdio.h>
#include "../include/algs/LZmsPredictor.hpp"

using namespace vmm_algs_lzms;
using namespace std;

LZmsPredictor::LZmsPredictor() : lzms(NULL) {
}

LZmsPredictor::~LZmsPredictor() {
    if (lzms)
        delete lzms;
}

void LZmsPredictor::init(int _abSize, int _mParam, int _sParam) {
    abSize = _abSize;
    mParam = _mParam;
    sParam = _sParam;
    
    if (lzms)
        delete lzms;
    
    lzms = new LZmsTree(mParam, sParam, abSize);
}

void LZmsPredictor::learn(std::vector<int>* trainingSequence) {
    lzms->learnSequence(trainingSequence);
}

double LZmsPredictor::predict(int symbol, vector<int>* context) {
    //try {
    
    // using the following relation: P(a | X) = P(Xa)/P(X)
    //            string seqWithSym; // = new StringBuffer();
    //              seqWithSym.append(context).append((char)symbol);
    //              string seqNoSym = new StringBuffer();
    //              seqNoSym.append(context);
    vector<int> seqWithSym;
    seqWithSym.insert(seqWithSym.begin(), context->begin(), context->end());
    seqWithSym.push_back(symbol);
    
    return pow( 2.0, - (logEval(&seqWithSym) - logEval(context)));    //seqNoSym
    //}
    //            catch (NullPointerException npe) {
    //              if (lzms == null) {
    //                throw new VMMNotTrainedException();
    //              }
    //              else {
    //                throw npe;
    //              }
    //            }
    
}
double* LZmsPredictor::predictAll(vector<int>* context) {
    double* out = new double[abSize];
    
    vector<int> seqWithSym;
    seqWithSym.insert(seqWithSym.begin(), context->begin(), context->end());
    seqWithSym.push_back(0);    // init final slot
    
    double logOfContext = logEval(context);
    for (int j = 0; j < abSize; j++) {
        seqWithSym.at(context->size()) = j; // replace last slot with token j
        out[j] = pow( 2.0, - (logEval(&seqWithSym) - logOfContext));
    }
    return out;
}

double LZmsPredictor::logEval(vector<int>* testSequence) {
    //            try {
    //                vector<int> symList; // = new TIntArrayList();
    //                for (int i = 0; i < testSequence.length(); ++i) {
    //                    symList.push_back( (int) testSequence[i]);
    //                }
    if (lzms)
        return lzms->calcLogLikelihood(testSequence); //.toNativeArray());
    else
        return -1;  // error, not trained!
    
    //            }
    //            catch (std::exception npe) { //NullPointerException npe) {
    //              if (lzms == null) {
    //                throw new VMMNotTrainedException();
    //              }
    //              else {
    //                throw npe;
    //              }
    //            }
    
}

double LZmsPredictor::logEval(vector<int>* testSequence, vector<int>* initialContext) {
    logEval(initialContext);
    return logEval(testSequence);
}
