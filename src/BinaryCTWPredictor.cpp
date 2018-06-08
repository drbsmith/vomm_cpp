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
