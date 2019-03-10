//
//  BinaryCTWPredictor.cpp
//  max-external
//
//  Created by Benjamin Smith on 6/3/18.
//

#include <stdio.h>
#include "../include/algs/BinaryCTWPredictor.hpp"

using namespace vmm_algs_ctw;

const double BinaryCTWPredictor::NEGTIVE_INVERSE_LOG_2 = - (1 / log(2.0));

const int BinaryCTWPredictor::KT_ALPHA = 2; // defines the ctw zero-order evaluator;
// KT_ALPHA=2 <==> using the KT-estimator.

const int BinaryCTWPredictor::BINARY_AB_SIZE = 2;
#ifdef BINARY_CTW_BYTE_SIZE
const int BinaryCTWPredictor::BYTE_SIZE = 8;
#endif
const int BinaryCTWPredictor::BIT_MASKS[] = {
    1, 2, 4, 8, 16, 32, 64, 128, 256,
    512, 1024,
    (int) pow(2, 11),
    (int) pow(2, 12)}; // should be enough, otherwise exception

BinaryCTWPredictor::BinaryCTWPredictor() : ctw(NULL) { }

BinaryCTWPredictor::~BinaryCTWPredictor() {
    if (ctw)
        delete ctw;
}

void BinaryCTWPredictor::init(int _abSize, int vmmOrder) {
    abSize = _abSize;
    ctw = new CTWVolfModel();
    ctw->init(BINARY_AB_SIZE, vmmOrder, KT_ALPHA);
    abSymSizeInBits = (int) ceil( -log(abSize) *
                                 NEGTIVE_INVERSE_LOG_2);
}

/**
 * The trainingSequence is translated into a binary sequence
 * e.g.,
 * alphabet = { 1, 2, 3 }
 * trainingSequence = 122313
 * then bin(trainingSequence) = 011010110111
 * @param trainingSequence a sequence over a general alphabet
 */
void BinaryCTWPredictor::learn(vector<int>* trainingSequence) {
    for (int i = 0, sym = -1; i < trainingSequence->size(); ++i) {
        sym = (int) trainingSequence->at(i);
        for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
            ctw->learn( (sym & BIT_MASKS[bit]) >> bit);
        }
    }
}

double BinaryCTWPredictor::predict(int symbol, vector<int>* context) {
    try {
        for (int i = 0, sym = -1; i < context->size(); ++i) {
            sym = (int) context->at(i);
            for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
                ctw->predict( (sym & BIT_MASKS[bit]) >> bit); //update the ctw context
            }
        }
        
        double p = 1.0;
        for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
            p *= ctw->predict( (symbol & BIT_MASKS[bit]) >> bit); //update the ctw context
        }
        return p;
    }
    catch (exception ex) { //NullPointerException npe) {
        if (ctw == NULL) {
            throw "VMMNotTrainedException"; //new VMMNotTrainedException();
        }
        else {
            throw ex; //npe;
        }
    }
    
}

double BinaryCTWPredictor::logEval(vector<int>* testSequence) {
    ctw->clearContext();
    return logEval(testSequence, NULL);
}

double BinaryCTWPredictor::logEval(vector<int>* testSequence, vector<int>* initialContext) {
    try {
        double eval = 0.0;
        
        if (initialContext != NULL) {
            for (int i = 0, sym = -1; i < initialContext->size(); ++i) {
                sym = (int) initialContext->at(i);
                for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
                    ctw->predict( (sym & BIT_MASKS[bit]) >> bit);
                }
            }
        }
        
        for (int i = 0, sym = -1; i < testSequence->size(); ++i) {
            sym = (int) testSequence->at(i);
            for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
                eval += log(ctw->predict( (sym & BIT_MASKS[bit]) >> bit));
            }
        }
        return eval * NEGTIVE_INVERSE_LOG_2;
    }
    catch (exception ex) { //}(NullPointerException npe) {
        if (ctw == NULL) {
            throw "VMMNotTrainedException"; //new VMMNotTrainedException();
        }
        else {
            throw ex; //npe;
        }
    }
}

double* BinaryCTWPredictor::predictAll(vector<int>* context) {
    double* out = new double[abSize];
    
//    ctw->clearContext();
//    for (int i = 0, sym = -1; i < context->size(); ++i) {
//        sym = (int) context->at(i);
//        for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
//            ctw->predict( (sym & BIT_MASKS[bit]) >> bit); //update the ctw context
//        }
//    }
    
    for (int j = 0; j < abSize; j++) {
//        double p = 1.0;
//        for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
//            p *= ctw->predict( (j & BIT_MASKS[bit]) >> bit);
//        }
        out[j] = predict(j, context);
//        for (int i = 0; i < context->size(); ++i) {
//            ctw->predict(context->at(i)); //updates the ppmc context
//        }
//        out[j] = ctw->predict(j);
    }
    return out;
}
