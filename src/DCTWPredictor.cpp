//
//  DCTWPredictor.cpp
//  max-external
//
//  Created by Benjamin Smith on 6/3/18.
//

#include <stdio.h>
#include <cmath>

#include "../include/algs/DCTWPredictor.hpp"

using namespace vmm_algs_decomp;

const double DCTWPredictor::NEGTIVE_INVERSE_LOG_2 = - (1 / log(2.0));

DCTWPredictor::DCTWPredictor() : dctw(NULL), trainingContext(NULL) {
}

DCTWPredictor::~DCTWPredictor() {
    if (dctw)
        delete dctw;
    if (trainingContext)
        delete trainingContext;
}

void DCTWPredictor::init(int _abSize, int _vmmOrder) {
    abSize = _abSize;
    vmmOrder = _vmmOrder;
    trainingContext = new DefaultContext(vmmOrder);
}

void DCTWPredictor::learn(string trainingSequence) {
    DecompositionTreeBuilder builder(abSize, vmmOrder);
    
    StringSampleIter ssi(trainingSequence);
    dctw = builder.buildStatic((SampleIterator*)(&ssi));
    
//    Context* context = new DefaultContext(vmmOrder);  // now using class variable to preserve across training steps
    
    for (int i=0, symbol=-1; i<trainingSequence.size(); ++i) {
        symbol = (int)trainingSequence[i];
        dctw->train(symbol, trainingContext);   // is dctw holding on to the context?
        trainingContext->add(symbol);
    }
}
void DCTWPredictor::learn(int* trainingSequence, int seqLength) {
    DecompositionTreeBuilder builder(abSize, vmmOrder);
    
    IntSampleIter ssi(trainingSequence, seqLength);
    dctw = builder.buildStatic((SampleIterator*)(&ssi));
    
    //    Context* context = new DefaultContext(vmmOrder);  // now using class variable to preserve across training steps
    
    for (int i=0, symbol=-1; i < seqLength; ++i) {
        symbol = trainingSequence[i];
        dctw->train(symbol, trainingContext);   // is dctw holding on to the context?
        trainingContext->add(symbol);
    }
}

double DCTWPredictor::predict(int symbol, string context) {
    try {
        Context* ctwContext = new DefaultContext(vmmOrder);
        for (int i = 0; i < context.length(); ++i) {
            ctwContext->add( (int) context[i]);
        }
        
        double ret = dctw->predict(symbol, ctwContext);
        delete ctwContext;
        return ret; // does dctw keep ctwContext or do we need to free it?
    }
    catch (exception ex) { //} (NullPointerException npe) {
        if (dctw == NULL) {
            throw "VMMNotTrainedException"; // new VMMNotTrainedException();
        }
        else {
            throw ex; // npe;
        }
    }
    
}
double DCTWPredictor::predict(int symbol, int* context, int contextLength) {
    try {
        Context* ctwContext = new DefaultContext(vmmOrder);
        for (int i = 0; i < contextLength; ++i) {
            ctwContext->add(context[i]);
        }
        
        double ret = dctw->predict(symbol, ctwContext);
        delete ctwContext;
        return ret; // does dctw keep ctwContext or do we need to free it?
    }
    catch (exception ex) { //} (NullPointerException npe) {
        if (dctw == NULL) {
            throw "VMMNotTrainedException"; // new VMMNotTrainedException();
        }
        else {
            throw ex; // npe;
        }
    }
}

double DCTWPredictor::logEval(string testSequence) {
    return logEval(testSequence, "");
}

double DCTWPredictor::logEval(string testSequence, string initialContext) {
    try {
        Context* context = new DefaultContext(vmmOrder);
        for (int i = 0; i < initialContext.size(); ++i) {
            context->add( (int) initialContext[i]);
        }
        
        double eval = 0.0;
        for (int i = 0, sym = 0; i < testSequence.size(); ++i) {
            sym = (int) testSequence[i];
            eval += log(dctw->predict(sym, context));
            context->add(sym);
        }
        
        return eval * NEGTIVE_INVERSE_LOG_2;
    }
    catch (exception ex) { //} (NullPointerException npe) {
        if (dctw == NULL) {
            throw "VMMNotTrainedException"; // new VMMNotTrainedException();
        }
        else {
            throw ex; // npe;
        }
    }
}