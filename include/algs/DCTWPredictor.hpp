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

    public:
        DCTWPredictor() : dctw(NULL) {  }
        
        ~DCTWPredictor() {
            if (dctw)
                delete dctw;
        }
        
        void init(int _abSize, int _vmmOrder) {
            abSize = _abSize;
            vmmOrder = _vmmOrder;
        }

        void learn(string trainingSequence) {
            DecompositionTreeBuilder builder(abSize, vmmOrder);

            StringSampleIter ssi(trainingSequence);
            dctw = builder.buildStatic((SampleIterator*)(&ssi));

            Context* context = new DefaultContext(vmmOrder);

            for (int i=0, symbol=-1; i<trainingSequence.size(); ++i) {
                symbol = (int)trainingSequence[i];
                dctw->train(symbol, context);   // is dctw holding on to the context?
                context->add(symbol);
            }
        }

        double predict(int symbol, string context) {
            try {
              Context* ctwContext = new DefaultContext(vmmOrder);
              for (int i = 0; i < context.length(); ++i) {
                ctwContext->add( (int) context[i]);
              }

              return dctw->predict(symbol, ctwContext); // does dctw keep ctwContext or do we need to free it?
            }
            catch (exception ex) { //} (NullPointerException npe) {
              if (dctw == NULL) {
                  throw "VMMNotTrainedException"; // new VMMNotTrainedException();
              }
              else {
                  throw ex; // npe;
              }
            }

        }

        double logEval(string testSequence) {
            return logEval(testSequence, "");
        }

        double logEval(string testSequence, string initialContext) {
            try {
              Context* context = new DefaultContext(vmmOrder);
              for (int i = 0; i < initialContext.size(); ++i) {
                context->add( (int) initialContext[i]);
              }

              double eval = 0.0;
              for (int i = 0, sym = 0; i < testSequence.size(); ++i) {
                sym = (int) testSequence[i];
                eval += log(dctw->predict(sym, context));
                context->add(sym);
              }

              return eval * NEGTIVE_INVERSE_LOG_2;
            }
            catch (exception ex) { //} (NullPointerException npe) {
                if (dctw == NULL) {
                    throw "VMMNotTrainedException"; // new VMMNotTrainedException();
                }
                else {
                    throw ex; // npe;
                }
            }
        }

    };
}
    
#endif // _DCTWPREDICTOR_HPP
