//
//  PPMCPredictor.cpp
//  VOMMtest
//
//  Created by Benjamin Smith on 10/2/18.
//

#include "../include/algs/PPMCPredictor.hpp"

using namespace vmm_algs_ppmc;

PPMCPredictor::PPMCPredictor() {
    ppmc = NULL;
}

PPMCPredictor::~PPMCPredictor() {
    if (ppmc)
        delete ppmc;
    ppmc = NULL;
}

/**
 * initializes this PPMPredictor
 * @param abSize alphabet size
 * @param vmmOrder VMM order
 */
void PPMCPredictor::init(int abSize, int vmmOrder) {
    if (ppmc)
        delete ppmc;
    
    ppmc = new OnlinePPMModel(vmmOrder, abSize);
}

//void PPMCPredictor::learn(string trainingSequence) {
//    for (int symIndex = 0; symIndex < trainingSequence.length(); ++symIndex) {
//        ppmc->use(trainingSequence[symIndex]);
//    }
//}
void PPMCPredictor::learn(vector<int>* trainingSequence) {
    for (int index = 0; index < trainingSequence->size(); ++index) {
        ppmc->use(trainingSequence->at(index));
    }
}

double PPMCPredictor::predict(int symbol, vector<int>* context) {
    try {
        ppmc->clearContext(true);
        for (int i = 0; i < context->size(); ++i) {
            ppmc->predict( (int) context->at(i)); //updates the ppmc context
        }
        return ppmc->predict(symbol);
    }
    catch (exception ex) { //NullPointerException npe) {
        if (ppmc == NULL) {
            throw "VMMNotTrainedException";
        }
        else {
            throw "NullPointerException";
        }
    }
}

double* PPMCPredictor::predictAll(vector<int>* context) {
    int abSize = ppmc->GetAlphaSize();
    double* out = new double[abSize];
    
    ppmc->clearContext(true);
    for (int j = 0; j < abSize; j++) {
        for (int i = 0; i < context->size(); ++i) {
            ppmc->predict(context->at(i)); //updates the ppmc context
        }
        out[j] = ppmc->predict(j);
    }
    return out;
}

double PPMCPredictor::logEval(vector<int>* testSequence) {
    try {
        ppmc->clearContext(true);
        
        double value = 0.0;
        
        for (int i = 0; i < testSequence->size(); ++i) {
            value += log(ppmc->predict(testSequence->at(i)));
        }
        return value * NEGTIVE_INVERSE_LOG_2; // the Math.log is in natural base
    }
    catch (exception ex) { //NullPointerException npe) {
        if (ppmc == NULL) {
            throw "VMMNotTrainedException";
        }
        else {
            throw "NullPointerException";
        }
    }
}

double PPMCPredictor::logEval(vector<int>* testSequence, vector<int>* initialContext) {
    for (int symIndex = 0; symIndex < initialContext->size(); ++symIndex) {
        ppmc->use(initialContext->at(symIndex));
    }
    return logEval(testSequence);
}

string PPMCPredictor::ModelToString() {
    return ppmc->ModelToString();
}

void PPMCPredictor::ModelFromString(string data) {
    ppmc->ModelFromString(data);
}
