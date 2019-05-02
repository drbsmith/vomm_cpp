#ifndef LZMSPREDICTOR_HPP
#define LZMSPREDICTOR_HPP
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
#include "lzms/LZmsTree.hpp"

#include <string>
#include <vector>

using namespace vmm_pred;

namespace vmm_algs_lzms {
/**
 * <b>LZms Predictor</b>
 * Usage example:
 * LZmsPredictor lzms = new LZmsPredictor();
 * lzms.init(256, 2, 0);
 * lzms.learn("abracadabra");
 * System.out.println("logeval : " + lzms.logEval("cadabra"));
 * System.out.println("P(c|abra) : " + lzms.predict('c', "abra"));
 *
 * Using Moti Nisenson's code.
 * <p>Copyright: Copyright (c) 2004</p>
 *
 * @author <a href="http://www.cs.technion.ac.il/~ronbeg">Ron Begleiter</a>
 * @version 1.0
 */

    class LZmsPredictor : public VMMPredictor {

    private:
        vmm_algs_lzms::LZmsTree* lzms;

        int abSize;
        int mParam;
        int sParam;

    public:
        LZmsPredictor();
        
        ~LZmsPredictor();

        void init(int _abSize, int _mParam, int _sParam);

        void learn(std::vector<int>* trainingSequence);
        
        double predict(int symbol,std::vector<int>* context);
        
        double logEval(std::vector<int>* testSequence);
        
        double logEval(std::vector<int>* testSequence, std::vector<int>* initialContext);

        double* predictAll(std::vector<int>* context);
        
        std::string ModelToString() { return ""; };
        void ModelFromString(std::string data) { };
        
        void learn(std::string s) { VMMPredictor::learn(s); }
        double predict(int symbol, std::string context) { return VMMPredictor::predict(symbol, context); }
        double logEval(std::string s) { return VMMPredictor::logEval(s); }
        double logEval(std::string t, std::string c) { return VMMPredictor::logEval(t, c); }
        double* predictAll(std::string context) { return VMMPredictor::predictAll(context); }
    };   // class
    
} // namespace
#endif
