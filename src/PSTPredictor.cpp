//
//  PSTPredictor.cpp
//  max-external
//
//  Created by Benjamin Smith on 6/1/18.
//

//#include <iostream>
#include <stdio.h>
#include <sstream>
#include "../include/algs/PSTPredictor.hpp"

#include "../include/algs/pst/PSTBuilder.hpp"
#include "../include/util/SampleIterator2SamplesBridge.hpp"
#include "../include/util/IntSampleIter.hpp"

using namespace vmm_algs_pst;

PSTPredictor::PSTPredictor() : trained(false), pst(NULL), pstPredictor(NULL)  { }

PSTPredictor::~PSTPredictor() {
    if (pst)
        delete pst;
    
    if (pstPredictor)
        delete pstPredictor;
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
    
    // puts it in a limbo state if we've already 'learned'...
}

void PSTPredictor::learn(vector<int>* trainingSequence) {
    PSTBuilder builder(abSize);
    
    SampleIterator2SamplesBridge* samples = new SampleIterator2SamplesBridge(); // the builder will keep this and free it when destroyed
    IntSampleIter ssi(trainingSequence);
    
    samples->init(&ssi);

    if (pst)
        delete pst;
    
    pst = builder.build(samples, pMin, alpha, gamma, r, vmmOrder);
    
    string tree = ((DefaultPSTNode*)pst)->toString();
    
    if (pstPredictor)
        delete pstPredictor;
    
    pstPredictor = new PSTArithPredictor(pst);
    
    trained = true;
}

double PSTPredictor::predict(int symbol, vector<int>* context) {
    if (symbol >= abSize || !trained)
        return -1;
    
    try {
        vector<double>* pArr = NULL; // = new double[abSize];
        
//        if (pstPredictor)
//            delete pstPredictor;
//        pstPredictor = new PSTArithPredictor(pst);
//        PSTArithPredictor pstPredictor(pst);  <-- now retained at the class level
        
        for (int i = 0, sym = -1; i < context->size(); ++i) {
            sym = (int) context->at(i);
            if (sym < abSize) {
                pArr = pstPredictor->predict();
                pstPredictor->increment(sym);
            }
        }
        pArr = pstPredictor->predict();
        if (pArr)
            return pArr->at(symbol);
        else return -1;
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
    if (!trained)
        return NULL;
    
    try {
        vector<double>* pArr = NULL; // = new double[abSize];
        
        //        if (pstPredictor)
        //            delete pstPredictor;
        //        pstPredictor = new PSTArithPredictor(pst);
        //        PSTArithPredictor pstPredictor(pst);  <-- now retained at the class level
        
        for (int i = 0, sym = -1; i < context->size(); ++i) {
            sym = (int) context->at(i);
            if (sym < abSize) {
                pArr = pstPredictor->predict();
                pstPredictor->increment(sym);
            }
        }
        pArr = pstPredictor->predict(); // returns pointer to internal vector, DO NOT DELETE
        
        double* out = new double[pArr->size()];
        for (int j = 0; j < pArr->size(); j++) {
            out[j] = pArr->at(j);
        }
        return out;
    }
    catch (exception npe) { //NullPointerException npe) {
        if (pst == NULL) {
            //throw new VMMNotTrainedException();
        }
        else {
            throw npe;
        }
    }
    return NULL;
}

double PSTPredictor::logEval(vector<int>* testSequence) {
    return logEval(testSequence, NULL);
}

double PSTPredictor::logEval(vector<int>* testSequence, vector<int>* initialContext) {
    try {
        if (!trained)
            return NULL;
        
        vector<double>* pArr = NULL; // = new double[abSize];
        double eval = 0.0;
        
//        if (pstPredictor)
//            delete pstPredictor;
//        pstPredictor = new PSTArithPredictor(pst);
 //       PSTArithPredictor pstPredictor(pst); // <-- now retained at class level
        
        if (initialContext != NULL) {
            for (int i = 0, sym = -1; i < initialContext->size(); ++i) {
                sym = (int) initialContext->at(i);
                if (sym < abSize)
                    pstPredictor->increment(sym);
            }
        }
        
        for (int i = 0, sym = -1; i < testSequence->size(); ++i) {
            sym = (int) testSequence->at(i);
            if (sym < abSize) {
                pArr = pstPredictor->predict();
                if (pArr)
                    eval += log(pArr->at(sym));
                pstPredictor->increment(sym);
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

string PSTPredictor::ModelToString() {
    stringstream ss;
    
    ss << "<PST_VMM>" << abSize << " " << pMin << " " << alpha << " " << gamma << " " << r << " " << vmmOrder << "<TREE>";
    
    ss << pst->toString();
    
    ss << "</TREE></PST_VMM>" << endl;
    
    return ss.str();
}

void PSTPredictor::ModelFromString(string data) {
    string::size_type pos = data.find("<PST_VMM>");
    data = data.substr(pos + 9);
    
    try {
        abSize = stoi(data,&pos);
        data = data.substr(pos+1);
        pMin = stod(data,&pos);
        data = data.substr(pos+1);
        alpha = stod(data,&pos);
        data = data.substr(pos+1);
        gamma = stod(data,&pos);
        data = data.substr(pos+1);
        r = stod(data,&pos);
        data = data.substr(pos+1);
        vmmOrder = stoi(data,&pos);
        data = data.substr(pos+1);
    } catch (invalid_argument) {}   // just leave it at default if the data is corrupt
    
    pos = data.find("<TREE>");
    data = data.substr(pos+6);
    
    if (data.size() > 0)
        pst = DefaultPSTNode::FromString(data);
    
    if (pst) {   // should check integrity better, but GIGO is on the user right now
        trained = true;
    
        pstPredictor = new PSTArithPredictor(pst);
    }
}
