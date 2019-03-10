//
//  PSTPredictor.cpp
//  max-external
//
//  Created by Benjamin Smith on 6/1/18.
//

#include <stdio.h>
#include "../include/algs/PSTPredictor.hpp"

#include "../include/algs/pst/PSTArithPredictor.hpp"
#include "../include/algs/pst/PSTBuilder.hpp"
#include "../include/util/SampleIterator2SamplesBridge.hpp"
#include "../include/util/IntSampleIter.hpp"

using namespace vmm_algs_pst;

PSTPredictor::PSTPredictor() : pst(NULL) { }

PSTPredictor::~PSTPredictor() {
    if (pst)
        delete pst;
}

/**
 * Initializes this PSTPredictor.
 *
 * @param _abSize alphabet size
 * @param _pMin refer to the paper
 * @param _alpha refer to the paper
 * @param _gamma refer to the paper
 * @param _r refer to the paper
 * @param _vmmOrder refer to the paper
 */
void PSTPredictor::init(int _abSize, double _pMin, double _alpha, double _gamma, double _r, int _vmmOrder) {
    abSize = _abSize;
    pMin = _pMin;
    alpha = _alpha;
    gamma = _gamma;
    r = _r;
    vmmOrder = _vmmOrder;
}

void PSTPredictor::learn(vector<int>* trainingSequence) {
    PSTBuilder builder(abSize);
    
    SampleIterator2SamplesBridge* samples = new SampleIterator2SamplesBridge(); // the builder will keep this and free it when destroyed
    IntSampleIter ssi(trainingSequence);
    
    samples->init(&ssi);

    pst = builder.build(samples, pMin, alpha, gamma, r, vmmOrder);
    
    string tree = ((DefaultPSTNode*)pst)->toString();
}

double PSTPredictor::predict(int symbol, vector<int>* context) {
    if (symbol >= abSize)
        return -1;
    
    try {
        vector<double> pArr(abSize); // = new double[abSize];
        
        PSTArithPredictor pstPredictor(pst);
        
        for (int i = 0, sym = -1; i < context->size(); ++i) {
            sym = (int) context->at(i);
            if (sym < abSize) {
                pstPredictor.predict(&pArr);
                pstPredictor.increment(sym);
            }
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
double* PSTPredictor::predictAll(vector<int>* context) {
    double* out = new double[abSize];
    
    
    for (int j = 0; j < abSize; j++) {
        out[j] = predict(j, context);
    }
    return out;
}

double PSTPredictor::logEval(vector<int>* testSequence) {
    return logEval(testSequence, NULL);
}

double PSTPredictor::logEval(vector<int>* testSequence, vector<int>* initialContext) {
    try {
        vector<double> pArr(abSize); // = new double[abSize];
        double eval = 0.0;
        
        PSTArithPredictor pstPredictor(pst);
        
        if (initialContext != NULL) {
            for (int i = 0, sym = -1; i < initialContext->size(); ++i) {
                sym = (int) initialContext->at(i);
                if (sym < abSize)
                    pstPredictor.increment(sym);
            }
        }
        
        for (int i = 0, sym = -1; i < testSequence->size(); ++i) {
            sym = (int) testSequence->at(i);
            if (sym < abSize) {
                pstPredictor.predict(&pArr);
                eval += log(pArr.at(sym));
                pstPredictor.increment(sym);
            }
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
