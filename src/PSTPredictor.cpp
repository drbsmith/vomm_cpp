//
//  PSTPredictor.cpp
//  max-external
//
//  Created by Benjamin Smith on 6/1/18.
//

#include <stdio.h>
#include "../include/algs/PSTPredictor.hpp"

using namespace vmm_algs_pst;

PSTPredictor::PSTPredictor() : pst(NULL) { }

PSTPredictor::~PSTPredictor() {
    if (pst)
        delete pst;
}

/**
 * Initializes this PSTPredictor.
 *
 * @param abSize alphabet size
 * @param pMin refer to the paper
 * @param alpha refer to the paper
 * @param gamma refer to the paper
 * @param r refer to the paper
 * @param vmmOrder refer to the paper
 */
void PSTPredictor::init(int _abSize, double _pMin, double _alpha, double _gamma, double _r, int _vmmOrder) {
    abSize = _abSize;
    pMin = _pMin;
    alpha = _alpha;
    gamma = _gamma;
    r = _r;
    vmmOrder = _vmmOrder;
}

void PSTPredictor::learn(string trainingSequence) {
    PSTBuilder builder(abSize);
    
    SampleIterator2SamplesBridge* samples = new SampleIterator2SamplesBridge(); // the builder will keep this and free it when destroyed
    StringSampleIter ssi(trainingSequence);
    
    samples->init(&ssi);

    pst = builder.build(samples, pMin, alpha, gamma, r, vmmOrder);
    
    string tree = ((DefaultPSTNode*)pst)->toString();
}

double PSTPredictor::predict(int symbol, string context) {
    try {
        vector<double> pArr(abSize); // = new double[abSize];
        
        PSTArithPredictor pstPredictor(pst);
        
        for (int i = 0, sym = -1; i < context.size(); ++i) {
            sym = (int) context[i];
            pstPredictor.predict(&pArr);
            pstPredictor.increment(sym);
        }
        pstPredictor.predict(&pArr);
        return pArr.at(symbol);
    }
    catch (exception npe) { //NullPointerException npe) {
        if (pst == NULL) {
            //throw new VMMNotTrainedException();
        }
        else {
            throw npe;
        }
    }
    return -1;
}

double PSTPredictor::logEval(string testSequence) {
    try {
        vector<double> pArr(abSize); // = new double[abSize];
        double eval = 0.0;
        
        PSTArithPredictor pstPredictor(pst);
        
        for (int i = 0, sym = -1; i < testSequence.size(); ++i) {
            sym = (int) testSequence[i];
            pstPredictor.predict(&pArr);
            eval += log(pArr.at(sym));
            pstPredictor.increment(sym);
        }
        
        return eval * NEGTIVE_INVERSE_LOG_2;
    }
    catch (exception npe) { //} (NullPointerException npe) {
        if (pst == NULL) {
            //throw new VMMNotTrainedException();
        }
        else {
            throw npe;
        }
    }
    
    return -1;
}

double PSTPredictor::logEval(string testSequence, string initialContext) {
    try {
        vector<double> pArr(abSize); // = new double[abSize];
        double eval = 0.0;
        
        PSTArithPredictor pstPredictor(pst);
        
        for (int i = 0, sym = -1; i < initialContext.length(); ++i) {
            sym = (int) initialContext[i];
            pstPredictor.increment(sym);
        }
        
        for (int i = 0, sym = -1; i < testSequence.length(); ++i) {
            sym = (int) testSequence[i];
            pstPredictor.predict(&pArr);
            eval += log(pArr.at(sym));
            pstPredictor.increment(sym);
        }
        
        return eval * NEGTIVE_INVERSE_LOG_2;
    }
    catch (exception npe) { // (NullPointerException npe) {
        if (pst == NULL) {
            //throw new VMMNotTrainedException();
        }
        else {
            throw npe;
        }
    }
    
    return -1;
}
