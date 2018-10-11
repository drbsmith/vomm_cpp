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

void PPMCPredictor::learn(string trainingSequence) {
    for (int symIndex = 0; symIndex < trainingSequence.length(); ++symIndex) {
        ppmc->use(trainingSequence[symIndex]);
    }
}
void PPMCPredictor::learn(int* trainingSequence, int seqLength) {
    for (int index = 0; index < seqLength; ++index) {
        ppmc->use(trainingSequence[index]);
    }
}

double PPMCPredictor::predict(int symbol, string context) {
    try {
        ppmc->clearContext();
        for (int i = 0; i < context.length(); ++i) {
            ppmc->predict( (int) context[i]); //updates the ppmc context
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

double PPMCPredictor::logEval(string testSequence) {
    try {
        ppmc->clearContext();
        
        double value = 0.0;
        
        for (int i = 0; i < testSequence.length(); ++i) {
            value += log(ppmc->predict( (int) testSequence[i]));
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

double PPMCPredictor::logEval(string testSequence, string initialContext) {
    for (int symIndex = 0; symIndex < initialContext.length(); ++symIndex) {
        ppmc->use(initialContext[(int) symIndex]);
    }
    return logEval(testSequence);
}

string PPMCPredictor::ModelToString() {
    return ppmc->ModelToString();
}

void PPMCPredictor::ModelFromString(string data) {
    ppmc->ModelFromString(data);
}
