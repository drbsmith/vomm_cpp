#include "../include/algs/oppm/OnlinePPMModel.hpp"

using namespace vmm_algs_oppm;
using namespace vmm_algs_com_colloquial_arithcode;

/**
* Online PPMC implementation.
* Based on <a href="http://www.colloquial.com/carp/">Bob Carpenter</a> offline code.
*
*/
OnlinePPMModel::OnlinePPMModel(int maxCodeLength, int absize) : PPMModel(maxCodeLength, absize) {
    isFirstPrediction = true;
    
    learningBuffer = _buffer;
    predictionBuffer = new ByteBuffer(_maxContextLength + 1);
}

OnlinePPMModel::~OnlinePPMModel() {
    // one of our buffers (==_buffer) will be deleted by base destructor.
    if (predictionBuffer && predictionBuffer != _buffer) {
        delete predictionBuffer;
        predictionBuffer = NULL;
    }
    if (learningBuffer && learningBuffer != _buffer) {
        delete learningBuffer;
        learningBuffer = NULL;
    }
}

double OnlinePPMModel::predict(int symbol) {
    if (symbol >= abSize)
        return -1;
    
    _buffer = predictionBuffer;
    
    if (isFirstPrediction) {
        isFirstPrediction = false;
        clearContext(true);
//        _buffer->SetMaxWidth(_maxContextLength + 1);
//        _contextLength = 0;
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

void OnlinePPMModel::use(int symbol) {
    isFirstPrediction = true;   // reset predictions
    _buffer = learningBuffer;
    _contextLength = learningContextLength;
    
    while (PPMModel::escaped(symbol)) {
        PPMModel::interval(ArithCodeModel::ESCAPE, &allRes[0]); // have already done complete walk to compute escape
    }

    PPMModel::interval(symbol, &allRes[0]);
    
    learningContextLength = _contextLength;
}

// specified in ArithCodeModel
void OnlinePPMModel::interval(int symbol, int* result) {
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
void OnlinePPMModel::clearContext(bool predict) {
    if (predict)
        _buffer = predictionBuffer;
    else
        _buffer = learningBuffer;
    
    _buffer->SetMaxWidth(_maxContextLength+1);//new context buffer
    _contextLength = 0;//empty context length
    _contextNode = NULL;
}

/** Returns interval for byte specified as an integer in 0 to 255 range.
* @param i Integer specification of byte in 0 to 255 range.
* @param result Array specifying cumulative probability for byte i.
*/
void OnlinePPMModel::calcInterval(int i, int* result) {
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
