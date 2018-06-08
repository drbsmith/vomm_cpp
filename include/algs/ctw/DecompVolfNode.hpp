#ifndef _DECOMPVOLFNODE_HPP
#define _DECOMPVOLFNODE_HPP
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

#include "../../util/ContextIterator.hpp"

#include <vector>

namespace vmm_algs_ctw {
    
    using namespace vmm_util;
    using namespace std;

/**
 * <p>Title: VolfNode</p>
 * <p>Description: Coding Paul Volf imp.
 * </p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author Ron
 * @version 1.0
 */

    class DecompVolfNode {
      /**
       * Value defined in Volf's thesis, section 4.3.2
       */
    public:
      const static int DEFAULT_ALPHA_FACTOR = 16;

    private:
      //value defined in Volf thesis section 4.3.2
      const static int MAX_COUNT_BEFORE_COUNT_HALVING = 256;

      int abSize;
      int alpha;//as defined in Volf's thesis, for the 0 order estimator.
      int *symCountArr;
      int allCount;
      double beta;

      double alphaInversed;
      double abSizeDivAlpha;

      vector<DecompVolfNode*> children;// mem consuming //

    public:
        DecompVolfNode() : symCountArr(NULL) {

      }
        ~DecompVolfNode() {
            if (symCountArr)
                delete symCountArr;
        }

      /**
       * Initializes this VolfNode
       *
       * @param symABSize size of the samples AB
       * @param alphaFactor as defined in Volf's thesis, for the 0 order estimator.
       */
      void init(int symABSize, int alphaFactor) {
        abSize = symABSize;
        alpha = alphaFactor;

        alphaInversed = 1.0/alpha;
        abSizeDivAlpha = 2/(double)alpha;/*ZZZ*/

        symCountArr = new int[2];/*ZZZ*/
          symCountArr[0] = 0; symCountArr[1] = 0;
//        Arrays.fill(symCountArr,0);
        allCount = 0;
          vector<DecompVolfNode*>::iterator it = children.begin();
          while (it != children.end()) {
              delete *it++;
          }
          children.clear();
        beta = 1.0;
      }

      double predict(int symbol, int symSetLabel, ContextIterator* context) {
        double *pwArr = predict(context);
//        vmm.util.PerdictAssertion.assertThis(pwArr);
          if (pwArr) {
              double ret = pwArr[symSetLabel];
              delete pwArr;
              return ret;
          } else
              return -1;
      }

      double learn(int symbol, int symSetLabel, ContextIterator* context) {
        double *res = learnAll(symbol, symSetLabel, context);
//        vmm.util.PerdictAssertion.assertThis(res);
          if (res) {
              double ret = res[symSetLabel];
              delete res;
              return ret;
          } else
              return -1;
      }

    private:
      double* predict(ContextIterator* context) {
        if (context->hasNext()) {
          DecompVolfNode* childOnContextPath = getChild(context->nextSymbol());
          double *ethaArr = childOnContextPath->predict(context);
          double denominator = 0.0;
          //1. compute b'
          double betaTag = beta / (allCount + abSizeDivAlpha);
          //2. compute intermediate result etha(context, sym)
          for( int sym=0; sym<2; ++sym) {
            ethaArr[sym] += betaTag * (symCountArr[sym]+alphaInversed);/**@todo (2)*/
            denominator += ethaArr[sym];
          }
          //3. compute conditional weighted probabilities
          for (int sym=0; sym<2; ++sym) {
            ethaArr[sym] /= denominator;
          }
          //4., 5. appear in learn method (no updates)
          return ethaArr;
        }
        else { //leaf - 0 order prediction
          double *peArr = new double[2];
          for( int sym=0; sym<2; ++sym) {
            peArr[sym] = (symCountArr[sym]+alphaInversed)/(allCount + abSizeDivAlpha);
          }
          return peArr;
        }
      }



      double* learnAll(int newSymbol, int symSetLabel, ContextIterator* context) {
        if (context->hasNext()) {

          DecompVolfNode* childOnContextPath = getChild(context->nextSymbol());
          double *ethaArr = childOnContextPath->learnAll(newSymbol, symSetLabel, context);
          double childOnContextPw = ethaArr[symSetLabel];
          double denominator = 0.0;
          //1. compute b'
          double betaTag = beta / (allCount + abSizeDivAlpha);
          //2. compute intemediate results into etha(context, sym)
          for(int sym=0; sym<2; ++sym) {
            ethaArr[sym] += betaTag * (symCountArr[sym]+alphaInversed);
            denominator += ethaArr[sym];
          }
          //3. compute conditional weighted probabilities
          for(int sym=0; sym<2; ++sym) {
            ethaArr[sym] /= denominator;
          }

          //4. update beta
          if(beta>1500000) {
            beta /= 2.0;
          }
          else if (beta<(1.0/1500000)){
            beta *= 2.0;
          }
          else {
            beta = (betaTag * (symCountArr[symSetLabel] + alphaInversed)) / childOnContextPw;
          }

          //5. update symbol count
          symCountArr[symSetLabel]++;
          allCount++;
          if (symCountArr[symSetLabel]>MAX_COUNT_BEFORE_COUNT_HALVING)
            rescaleCounts();

          return ethaArr;
        } else { //leaf - 0 order prediction
            double *peArr = new double[2];
            for (int sym=0; sym<2; ++sym) {
                peArr[sym] = (symCountArr[sym]+alphaInversed)/(allCount + abSizeDivAlpha);
            }
            symCountArr[symSetLabel]++;
            allCount++;
            if (symCountArr[symSetLabel]>MAX_COUNT_BEFORE_COUNT_HALVING)
                rescaleCounts();

            return peArr;
        }
      }

    private:
      DecompVolfNode* getChild(int sym) {
        if (children.size() < abSize) {
          children.resize(abSize, NULL);
        }
        if (children.at(sym) == NULL) {
          children.at(sym) = new DecompVolfNode();
          children.at(sym)->init(abSize, alpha);
        }
        return children.at(sym);
      }

      void rescaleCounts() {
          if (!symCountArr)
              return;
          
          allCount = 0;
          for(int i=0, rounding=0; i < 2; i++){
              rounding = symCountArr[i] % 2;
              symCountArr[i] = symCountArr[i] >> (1 + rounding);    
              allCount += symCountArr[i];
          }
      }


    };

}

#endif // _DECOMPVOLFNODE_HPP
