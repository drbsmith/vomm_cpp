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
    public:
        BinaryCTWPredictor() : ctw(NULL) { }
        
        ~BinaryCTWPredictor() {
            if (ctw)
                delete ctw;
        }

        void init(int abSize, int vmmOrder) {
            ctw = new CTWVolfModel();
            ctw->init(BINARY_AB_SIZE, vmmOrder, KT_ALPHA);
            abSymSizeInBits = (int) ceil( -log(abSize) *
                                              NEGTIVE_INVERSE_LOG_2);
        }

  /**
   * The trainingSequence is translated into a binary sequence
   * e.g.,
   * alphabet = { 1, 2, 3 }
   * trainingSequence = 122313
   * then bin(trainingSequence) = 011010110111
   * @param trainingSequence a sequence over a general alphabet
   */
  void learn(string trainingSequence) {
    for (int i = 0, sym = -1; i < trainingSequence.size(); ++i) {
      sym = (int) trainingSequence[i];
      for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
        ctw->learn( (sym & BIT_MASKS[bit]) >> bit);
      }
    }
  }

  double predict(int symbol, string context) {
    try {
      for (int i = 0, sym = -1; i < context.size(); ++i) {
        sym = (int) context[i];
        for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
          ctw->predict( (sym & BIT_MASKS[bit]) >> bit); //update the ctw context
        }
      }

      double p = 1.0;
      for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
        p *= ctw->predict( (symbol & BIT_MASKS[bit]) >> bit); //update the ctw context
      }
      return p;
    }
      catch (exception ex) { //NullPointerException npe) {
          if (ctw == NULL) {
              throw "VMMNotTrainedException"; //new VMMNotTrainedException();
          }
          else {
              throw ex; //npe;
          }
    }

  }

  double logEval(string testSequence) {
    ctw->clearContext();
    return logEval(testSequence, "");
  }

  double logEval(string testSequence, string initialContext) {
    try {
      double eval = 0.0;

      for (int i = 0, sym = -1; i < initialContext.size(); ++i) {
        sym = (int) initialContext[i];
        for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
          ctw->predict( (sym & BIT_MASKS[bit]) >> bit);
        }
      }

      for (int i = 0, sym = -1; i < testSequence.size(); ++i) {
        sym = (int) testSequence[i];
        for (int bit = abSymSizeInBits - 1; bit >= 0; --bit) {
          eval += log(ctw->predict( (sym & BIT_MASKS[bit]) >> bit));
        }
      }
      return eval * NEGTIVE_INVERSE_LOG_2;
    }
      catch (exception ex) { //}(NullPointerException npe) {
          if (ctw == NULL) {
              throw "VMMNotTrainedException"; //new VMMNotTrainedException();
          }
          else {
              throw ex; //npe;
          }
    }

  }

//   public static void main(String args[]) {
//       BinaryCTWPredictor p = new BinaryCTWPredictor();
//       p.init(6,8);
//       char [] data = {1,2,5,1,3,1,4,1,2,5,1};
//       String seq = new String(data);
//       p.learn(seq);
//       System.out.println(seq);
//       System.out.println(""+p.logEval(seq, seq));
//
//        /*  int sym = 'a';
//          for (int bit = 6; bit >= 0; --bit) {
//              System.out.println(""+ ((sym & BIT_MASKS[bit]) >> bit) );
//          }*/
//  }


    };
}

#endif
