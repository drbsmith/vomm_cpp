#ifndef _BINARYCTWPREDICTOR_HPP
#define _BINARYCTWPREDICTOR_HPP

/* HEADER
 If you use this code don’t forget to reference us :) BibTeX: http://www.cs.technion.ac.il/~rani/el-yaniv_bib.html#BegleiterEY04

 This code is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (<a href="http://www.gnu.org/copyleft/gpl.html">GPL</a>) for more details.*/

#include <string>
#include <cmath>

#include "ctw/CTWVolfModel.hpp"
#include "../pred/VMMPredictor.hpp"

namespace vmm_algs_ctw {

    using namespace vmm_pred;
    using namespace std;

/**
 * <p><b> Binary CTW Predictor </b></p>
 *
 * Binary CTW Predictor
 * Projects the sequences on the binary alphabet
 * AB = { 1, 2, 3 }
 * X = 122313
 * then bin(X) = 011010110111
 *
 * Then predicts over the binary sequence using the classical
 * CTW predictor.
 *
 * Example usage:
 *
 * DCTWPredictor dctw = new DCTWPredictor();
 * dctw.init(256, 5);
 * dctw.learn("abracadabra");
 * System.out.println("logeval : " + dctw.logEval("cadabra"));
 * System.out.println("P(c|abra) : " + dctw.predict('c', "abra"));
 *
 * <p>Copyright: Copyright (c) 2004</p>
 * @author Ron Begleiter
 * @version 1.0
 */

    class BinaryCTWPredictor : public VMMPredictor {
    private:
        const static double NEGTIVE_INVERSE_LOG_2;

        const static int KT_ALPHA; // defines the ctw zero-order evaluator;
                // KT_ALPHA=2 <==> using the KT-estimator.

        const static int BINARY_AB_SIZE;
#ifndef BYTE_SIZE   // usually OS defined already
#define BINARY_CTW_BYTE_SIZE
        const static int BYTE_SIZE;
#endif
        const static int BIT_MASKS[]; // should be enough, otherwise exception

        CTWVolfModel* ctw;

        int abSymSizeInBits;
        int abSize;
    public:
        BinaryCTWPredictor();
        
        ~BinaryCTWPredictor();

        void init(int _abSize, int vmmOrder);

  /**
   * The trainingSequence is translated into a binary sequence
   * e.g.,
   * alphabet = { 1, 2, 3 }
   * trainingSequence = 122313
   * then bin(trainingSequence) = 011010110111
   * @param trainingSequence a sequence over a general alphabet
   */
        void learn(vector<int>* trainingSequence);
        
        double predict(int symbol, vector<int>* context);
        
        double logEval(vector<int>* testSequence);
        
        double logEval(vector<int>* testSequence, vector<int>* initialContext);
        
        double* predictAll(vector<int>* context);
        
        string ModelToString() { return ""; };
        void ModelFromString(string data) { };
        
        void learn(string s) { VMMPredictor::learn(s); }
        double predict(int symbol, string context) { return VMMPredictor::predict(symbol, context); }
        double logEval(string s) { return VMMPredictor::logEval(s); }
        double logEval(string t, string c) { return VMMPredictor::logEval(t, c); }
        double* predictAll(string context) { return VMMPredictor::predictAll(context); }
    };
}

#endif
