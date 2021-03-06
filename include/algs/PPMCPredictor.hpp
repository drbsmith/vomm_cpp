//
//  PPMCPredictor.hpp
//  VOMMtest
//
//  Created by Benjamin Smith on 10/2/18.
//

#ifndef PPMCPredictor_hpp
#define PPMCPredictor_hpp

#include <stdio.h>

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
#include "oppm/OnlinePPMModel.hpp"
//#include "oppm/OPPMtestException.hpp"

#include <string>
//import vmm.pred.VMMNotTrainedException;

//import vmm.algs.oppm.*;

namespace vmm_algs_ppmc {
    
    using namespace vmm_pred;
    using namespace vmm_algs_oppm;
    using namespace std;
    /**
     * <p><b>PPMC Predictor</b></p>
     * <p>
     * Usage example:
     *
     * PPMCPredictor ppmc;
     * ppmc.init(256, 5);
     * ppmc.learn("abracadabra");
     * System.out.println("logeval : " + ppmc.logEval("cadabra"));
     * System.out.println("P(c|abra) : " + ppmc.predict('c', "abra"));
     * </p>
     *
     * Using <a href="http://www.colloquial.com/carp/">Bob Carpenter</a> code.
     * <p>Copyright: Copyright (c) 2004</p>
     * @author <a href="http://www.cs.technion.ac.il/~ronbeg">Ron Begleiter</a>
     * @version 1.0
     */
    
    class PPMCPredictor : public VMMPredictor {
        
    private:
        const double NEGTIVE_INVERSE_LOG_2 = - (1 / log(2.0));
        
        OnlinePPMModel* ppmc;
        
    public:
        PPMCPredictor();
        
        ~PPMCPredictor();
        
        void init(int abSize, int vmmOrder);
        
        /** Learn: use an array of integers to train the model
         * @param trainingSequence the array of integers to use for training
         */
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
    };  // class
} // namespace


#endif /* PPMCPredictor_hpp */
