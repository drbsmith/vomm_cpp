#include "../include/algs/oppm/OfflinePPMModel.hpp"

using namespace vmm_algs_oppm;
using namespace vmm_algs_com_colloquial_arithcode;

/**
* Offline PPMC implementation.
* Using <a href="http://www.colloquial.com/carp/">Bob Carpenter</a> code.
*
* <p>Copyright: Copyright (c) 2004</p>
*
* @author <a href="http://www.cs.technion.ac.il/~ronbeg">Ron Begleiter</a>
* @version 1.0
*/
OfflinePPMModel::OfflinePPMModel(int maxCodeLength, int absize) : PPMModel(maxCodeLength, absize) {
    isFirstPrediction = true;
}

OfflinePPMModel::~OfflinePPMModel() {
    
}

double OfflinePPMModel::predict(int symbol) {
    if (isFirstPrediction) {
        isFirstPrediction = false;
        _buffer->SetMaxWidth(_maxContextLength + 1);
        _contextLength = 0;
        _contextNode = NULL;
    }

    double p = 1.0;
    while (PPMModel::escaped(symbol)) {
        interval(ArithCodeModel::ESCAPE, &allRes[0]);
        p *= (allRes[1] - allRes[0]) / (double) allRes[2];
    }
    interval(symbol, &allRes[0]);
    return (p * ( (allRes[1] - allRes[0]) / (double) allRes[2]));
}

void OfflinePPMModel::use(int symbol) {
    isFirstPrediction = true;   // reset predictions
    
    while (PPMModel::escaped(symbol)) {
        PPMModel::interval(ArithCodeModel::ESCAPE, &allRes[0]); // have already done complete walk to compute escape
    }

    PPMModel::interval(symbol, &allRes[0]);
}

// specified in ArithCodeModel
void OfflinePPMModel::interval(int symbol, int* result) {
    if (symbol == ArithCodeModel::_EOF) {
        _backoffModel->intervalNoIncrement(ArithCodeModel::_EOF, _excludedBytes, result);
    }
    else if (symbol == ArithCodeModel::ESCAPE) {
        intervalEscape(result);
    }
    else {
        calcInterval(symbol, result); //will not increment symbol
    }
}

/**
* Clears this OfflinePPMModel's context.
* As a result the nexts symbol context will be the empty context.
*/
void OfflinePPMModel::clearContext() {
    _buffer->SetMaxWidth(_maxContextLength+1);//new context buffer
    _contextLength = 0;//empty context length
}

/** Returns interval for byte specified as an integer in 0 to 255 range.
* @param i Integer specification of byte in 0 to 255 range.
* @param result Array specifying cumulative probability for byte i.
*/
void OfflinePPMModel::calcInterval(int i, int* result) {
    if (_contextNode != NULL) {
        _contextNode->interval(i, _excludedBytes, result);
    }
    else {
        _backoffModel->intervalNoIncrement(i, _excludedBytes, result);
    }
    _buffer->buffer(i);
    _contextLength = (_maxContextLength < _buffer->length() ? _maxContextLength : _buffer->length());
    getContextNodeBinarySearch();
    _excludedBytes->clear();
}
