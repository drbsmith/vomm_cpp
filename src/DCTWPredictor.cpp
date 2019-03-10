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

//void DCTWPredictor::learn(string trainingSequence) {
//    DecompositionTreeBuilder builder(abSize, vmmOrder);
//
//    StringSampleIter ssi(trainingSequence);
//    dctw = builder.buildStatic((SampleIterator*)(&ssi));
//
////    Context* context = new DefaultContext(vmmOrder);  // now using class variable to preserve across training steps
//
//    for (int i=0, symbol=-1; i<trainingSequence.size(); ++i) {
//        symbol = (int)trainingSequence[i];
//        dctw->train(symbol, trainingContext);   // is dctw holding on to the context?
//        trainingContext->add(symbol);
//    }
//}
void DCTWPredictor::learn(vector<int>* trainingSequence) {
    DecompositionTreeBuilder builder(abSize, vmmOrder);
    
    // perform sanity check on the training data. Throw away anything beyond the abSize
    for (vector<int>::iterator it = trainingSequence->begin(); it != trainingSequence->end(); )
        if (*it >= abSize)
            it = trainingSequence->erase(it);
        else
            it++;
    
    IntSampleIter ssi(trainingSequence);
    dctw = builder.buildStatic((SampleIterator*)(&ssi));
    
    //    Context* context = new DefaultContext(vmmOrder);  // now using class variable to preserve across training steps
    
    for (int i=0, symbol=-1; i < trainingSequence->size(); ++i) {
        symbol = trainingSequence->at(i);
        dctw->train(symbol, trainingContext);   // is dctw holding on to the context?
        trainingContext->add(symbol);
    }
}

//double DCTWPredictor::predict(int symbol, string context) {
//    try {
//        Context* ctwContext = new DefaultContext(vmmOrder);
//        for (int i = 0; i < context.length(); ++i) {
//            ctwContext->add( (int) context[i]);
//        }
//
//        double ret = dctw->predict(symbol, ctwContext);
//        delete ctwContext;
//        return ret; // does dctw keep ctwContext or do we need to free it?
//    }
//    catch (exception ex) { //} (NullPointerException npe) {
//        if (dctw == NULL) {
//            throw "VMMNotTrainedException"; // new VMMNotTrainedException();
//        }
//        else {
//            throw ex; // npe;
//        }
//    }
//
//}
double DCTWPredictor::predict(int symbol, vector<int>* context) {
    if (symbol >= abSize)
        return -1;
    
    try {
        Context* ctwContext = new DefaultContext(vmmOrder);
        for (int i = 0; i < context->size(); ++i) {
            if ((int)context->at(i) <= abSize)
                ctwContext->add(context->at(i));
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
double* DCTWPredictor::predictAll(vector<int>* context) {
    double* out = new double[abSize];
    
    Context* ctwContext = new DefaultContext(vmmOrder);
    for (int i = 0; i < context->size(); ++i) {
        if ((int)context->at(i) <= abSize)
            ctwContext->add(context->at(i));
    }
    
    for (int j = 0; j < abSize; j++) {
        out[j] = dctw->predict(j, ctwContext);
    }
    return out;
}

double DCTWPredictor::logEval(vector<int>* testSequence) {
    return logEval(testSequence, NULL);
}

double DCTWPredictor::logEval(vector<int>* testSequence, vector<int>* initialContext) {
    try {
        Context* context = new DefaultContext(vmmOrder);
        if (initialContext != NULL) {
            for (int i = 0; i < initialContext->size(); ++i) {
                if ((int)initialContext->at(i) <= abSize)
                    context->add( (int) initialContext->at(i));
            }
        }
        
        double eval = 0.0;
        for (int i = 0, sym = 0; i < testSequence->size(); ++i) {
            if ((int)testSequence->at(i) <= abSize) {
                sym = (int) testSequence->at(i);
                eval += log(dctw->predict(sym, context));
                context->add(sym);
            }
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

string DCTWPredictor::ModelToString() {
    return dctw->toString();
}
void DCTWPredictor::ModelFromString(string data) {
    dctw->fromString(data);
}
