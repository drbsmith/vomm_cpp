#ifndef _DCTWPREDICTOR_HPP
#define _DCTWPREDICTOR_HPP
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

#include "decomp/StaticDecompositionNode.hpp"
#include "../pred/VMMPredictor.hpp"
#include "decomp/DecompositionTreeBuilder.hpp"
#include "../util/DefaultContext.hpp"
#include "../util/IntSampleIter.hpp"
#include "../util/StringSampleIter.hpp"

#include <cmath>
#include <string>

namespace vmm_algs_decomp {

    using namespace vmm_pred;
    using namespace vmm_util;
    using namespace std;

//import vmm.algs.decomp.*;

/**
 * <p>Title: Decomposed CTW Predictor </p>
 *
 * DCTWPredictor dctw = new DCTWPredictor();
 * dctw.init(256, 5);
 * dctw.learn("abracadabra");
 * System.out.println("logeval : " + dctw.logEval("cadabra"));
 * System.out.println("P(c|abra) : " + dctw.predict('c', "abra"));
 *
 * <p>Copyright: Copyright (c) 2004</p>
 * @author <a href="http://www.cs.technion.ac.il">Ron Begleiter</a>
 * @version 1.0
 */

    class DCTWPredictor : public VMMPredictor {
    private:
        const static double NEGTIVE_INVERSE_LOG_2;

        StaticDecompositionNode* dctw;

        int abSize;
        int vmmOrder;
        
        Context* trainingContext;

    public:
        DCTWPredictor();
        
        ~DCTWPredictor();
        
        /** init
         * @param _abSize alphabet size.
         * @param _vmmOrder the maximum order for the model.
         */
        void init(int _abSize, int _vmmOrder);
        
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
    
#endif // _DCTWPREDICTOR_HPP
