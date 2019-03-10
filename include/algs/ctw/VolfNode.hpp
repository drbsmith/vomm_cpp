#ifndef _VOLFNODE_HPP
#define _VOLFNODE_HPP

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

class VolfNode {

private:
  //value defined in Volf thesis section 4.3.2
  const static int MAX_COUNT_BEFORE_COUNT_HALVING = 256;

  int abSize;
  int alpha;//as defined in Volf's thesis, for the 0 order estimator.
  vector<int> symCountArr;
  int allCount;
  double beta;

  double alphaInversed;
  double abSizeDivAlpha;

  vector<VolfNode*> children;// mem consuming //

public:
    /**
     * Value defined in Volf's thesis, section 4.3.2
     */
    const static int DEFAULT_ALPHA_FACTOR = 16;
    
    VolfNode() : symCountArr(NULL) {

    }
    
    ~VolfNode() {
        while (children.size() > 0)
            children.erase(children.begin());
    }

  /**
   * Initializes this VolfNode
   *
   * @param alphabetSize size of the samples AB
   * @param alphaFactor as defined in Volf's thesis, for the 0 order estimator.
   */
  void init(int alphabetSize, int alphaFactor) {
        abSize = alphabetSize;
        alpha = alphaFactor;
        alphaInversed = 1.0/alpha;
        abSizeDivAlpha = abSize/(double)alpha;

        symCountArr.resize(abSize, 0);
//        Arrays.fill(symCountArr,0);
        allCount = 0;
        children.resize(abSize);
        beta = 1.0;
  }


  double predict(int symbol, ContextIterator* context) {
      if (symbol < 0)
          return -1;    // error!
      
      vector<double>* pwArr = predict(context);
//    vmm.util.PerdictAssertion.assertThis(pwArr);
      if (pwArr && symbol < pwArr->size()) {
          double ret = pwArr->at(symbol);
          delete pwArr; // clean up
          
          return ret;
      } else {
          return -1;    // error again!
      }
  }

  double learn(int symbol, ContextIterator* context) {
      if (symbol < 0)
          return -1;    // error.
    vector<double>* res = learnAll(symbol, context);
//    vmm.util.PerdictAssertion.assertThis(res);
      if (res && symbol < res->size()) {
          double ret = res->at(symbol);
          delete res;
          return ret;
      } else {
          return -1; // error.
      }
  }

private:
  vector<double>* predict(ContextIterator* context) {
      if (symCountArr.size() != abSize)
          return NULL;  // not initialized properly
      
    if (context->hasNext()) {
      VolfNode* childOnContextPath = getChild(context->nextSymbol());
        if (!childOnContextPath)
            return NULL;    // symbol was out of range.
        
      vector<double>* ethaArr = childOnContextPath->predict(context);
        if (!ethaArr)
            return NULL;    // error condition
      double denominator = 0.0;
      //1. compute b'
      double betaTag = beta / (double)(allCount + abSizeDivAlpha);
      //2. compute intermediate result etha(context, sym)
      for (int sym = 0; sym < abSize; ++sym) {
        ethaArr->at(sym) += betaTag * (symCountArr.at(sym) + alphaInversed);
        denominator += ethaArr->at(sym);
      }
      //3. compute conditional weighted probabilities
      for(int sym=0; sym<abSize; ++sym) {
        ethaArr->at(sym) /= denominator;
      }
      //4., 5. appear in learn method (no updates)
      return ethaArr;
    }
    else { //leaf - 0 order prediction
      vector<double>* peArr = new vector<double>(abSize, 0.0);
      for(int sym=0; sym<abSize; ++sym) {
        peArr->at(sym) = (symCountArr[sym]+alphaInversed) / (allCount + abSizeDivAlpha);
      }
      return peArr;
    }
  }


  vector<double>* learnAll(int newSymbol, ContextIterator* context) {
      if (symCountArr.size() != abSize)
          return NULL;  // not initialized properly
      
    if (context->hasNext()) {

      VolfNode* childOnContextPath = getChild(context->nextSymbol());
        if (childOnContextPath == NULL)
            return NULL;    // error on nextSymbol
      vector<double>* ethaArr = childOnContextPath->learnAll(newSymbol, context);
        if (ethaArr == NULL || ethaArr->size() < newSymbol+1);
            return NULL;    // error
      double childOnContextPw = ethaArr->at(newSymbol);
      double denominator = 0.0;
      //1. compute b'
      double betaTag = beta / (allCount + abSizeDivAlpha);
      //2. compute intemediate results into etha(context, sym)
      for(int sym=0; sym<abSize; ++sym) {
        ethaArr->at(sym) += betaTag * (symCountArr[sym]+alphaInversed);
        denominator += ethaArr->at(sym);
      }
      //3. compute conditional weighted probabilities
      for(int sym=0; sym<abSize; ++sym) {
        ethaArr->at(sym) /= denominator;
      }

      //4. update beta
      //     consts are from volf thesis
      beta = ((beta>1500000)||(beta<(1.0/1500000)))?
          beta/2.0 :
          (betaTag * (symCountArr[newSymbol]+alphaInversed))/childOnContextPw;

      //5. update symbol count
      symCountArr[newSymbol]++;
      allCount++;
      if (symCountArr[newSymbol] > MAX_COUNT_BEFORE_COUNT_HALVING)
        rescaleCounts();

      return ethaArr;
    }
    else { //leaf - 0 order prediction
      vector<double>* peArr = new vector<double>(abSize);
      for(int sym=0; sym<abSize; ++sym) {
        peArr->at(sym) = (symCountArr[sym]+alphaInversed) / (double)(allCount + abSizeDivAlpha);
      }
      symCountArr[newSymbol]++;
      allCount++; /* ver. JULY 2007 (due versions confusion?) */
      if (symCountArr[newSymbol] > MAX_COUNT_BEFORE_COUNT_HALVING)
        rescaleCounts();

      return peArr;
    }
  }

    VolfNode* getChild(int sym) {
        if (sym > abSize-1 || sym > children.size()) // we assume children will always be abSize!!!
            return NULL;
        
        if (children.at(sym) == NULL) {
          children.at(sym) = new VolfNode();
          children.at(sym)->init(abSize, alpha);
        }
        return children.at(sym);
    }

  void rescaleCounts() {
    allCount = 0;
    for(int i=0, rounding=0; i<symCountArr.size(); ++i){
      rounding = symCountArr.at(i)%2;
        symCountArr.at(i) = symCountArr.at(i) >> (1 + rounding);  // TODO: is this the correct precedence, or should the shift happen before the addition?
      allCount += symCountArr.at(i);
    }
  }
};

}
#endif //_VOLFNODE_HPP
