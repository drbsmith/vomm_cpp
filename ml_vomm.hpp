//
//  ml_hmm.hpp
//  ml.fcm
//
//  Created by Ben Smith, 2017.
//
//

#ifndef ml_hmm_hpp
#define ml_hmm_hpp

#include "ext.h"
#include <stdio.h>
#include <vector>
#include <stdexcept>

#include "include/algs/PSTPredictor.hpp"
#include "include/algs/BinaryCTWPredictor.hpp"
#include "include/algs/DCTWPredictor.hpp"

//#undef nil
//#undef Nil

using namespace std;
using namespace vmm_pred;

class mlVOMM {
private:
    
    // handle to our parent max object
    void* maxObject;
    
    VMMPredictor* myPST;
public:
    mlVOMM(void* parentObject);
    
    ~mlVOMM();
    
    double logEval(char* test);
    double predict(int state, char* test);
};

#endif /* ml_hmm_hpp */
