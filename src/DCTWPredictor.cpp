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
