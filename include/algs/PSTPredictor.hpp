#ifndef _PSTPREDICTOR_HPP
#define _PSTPREDICTOR_HPP
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

#include "../pred/VMMPredictor.hpp"
#include "pst/PSTArithPredictor.hpp"
#include "pst/PSTNodeInterface.hpp"

#include "math.h"
using namespace vmm_util;
using namespace std;

namespace vmm_algs_pst {

//import vmm.pred.*;
//
//import vmm.algs.pst.*;
//import vmm.util.*;

/**
 * <p><b>Probability Suffix Tree Predictor</b></p>
 * <p>Usage example: <br>
 * pst.init(256, 0.001, 0.0, 0.0001, 1.05, 20);
 * pst.learn("abracadabra");
 * System.out.println("logeval : " + pst.logEval("cadabra"));
 * System.out.println("P(c|abra) : " + pst.predict('c', "abra"));
 * </p>
 *
 * <p>Copyright: Copyright (c) 2004</p>
 *
 * @author <a href="http://www.cs.technion.ac.il/~ronbeg">Ron Begleiter</a>
 * @version 1.0
 */
    class PSTPredictor  : public vmm_pred::VMMPredictor {
    private:
        const double NEGTIVE_INVERSE_LOG_2 = - (1 / log(2.0));

        int abSize;
        double pMin, alpha, gamma, r;
        int vmmOrder;
        
        bool trained;   // flag to escape predicts before being built

        PSTNodeInterface* pst;  // handle to the tree

        PSTArithPredictor* pstPredictor;   // used for predictions, save for CPU efficiency
    public:
        PSTPredictor();
        
        virtual ~PSTPredictor();

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
        void init(int _abSize, double _pMin, double _alpha, double _gamma, double _r, int _vmmOrder);

        void learn(vector<int>* trainingSequence);

        double predict(int symbol, vector<int>* context);

        double logEval(vector<int>* testSequence);

        double logEval(vector<int>* testSequence, vector<int>* initialContext);
        
        double* predictAll(vector<int>* context);
        
        string ModelToString();
        void ModelFromString(string data);
        
        void learn(string s) { VMMPredictor::learn(s); }
        double predict(int symbol, string context) { return VMMPredictor::predict(symbol, context); }
        double logEval(string s) { return VMMPredictor::logEval(s); }
        double logEval(string t, string c) { return VMMPredictor::logEval(t, c); }
        double* predictAll(string context) { return VMMPredictor::predictAll(context); }
    };
}
#endif // _VMMPREDICTOR_HPP
