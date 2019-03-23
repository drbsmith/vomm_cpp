#ifndef _PSTARITHPREDICTOR_HPP
#define _PSTARITHPREDICTOR_HPP

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

#include "../../util/Context.hpp"
#include "../../util/DefaultContext.hpp"
#include "PSTNodeInterface.hpp"
#include "DefaultPSTNode.hpp"

namespace vmm_algs_pst {

//import vmm.util.*;
    using namespace vmm_util;

/**
 * <p>Title: PSTArithPredictor</p>
 * <p>Description: PST Implementation of the ArithPredictor</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: Technion</p>
 * @author Ron Begleiter
 * @version 1.0
 */

class PSTArithPredictor {
private:
  PSTNodeInterface* pst;
  Context* context;

public:
    PSTArithPredictor(PSTNodeInterface* pstRoot) : pst(pstRoot), context(NULL) {
      if (pst != NULL)
          context = new DefaultContext(pst->subTreeHeight());
  }

    ~PSTArithPredictor() {
        delete context;
    }

  /**
   * @see arith.ArithPredictor
   */
  vector<double>* predict(){
      if (pst == NULL || context == NULL)
          return NULL;
      
    PSTNodeInterface* contextNode = pst->get(context->getIterator());
      
    return contextNode->predict();   // fills prediction with the probabilities from contextNode
  }

  /**
   * @see arith.ArithPredictor
   */
  void increment(int symbol){
      if (context != NULL)
          context->add(symbol);
  }

  /**
   * @see arith.ArithPredictor
   */
  int alphabetSize(){
      if (pst != NULL)
          return pst->getAlphabetSize();
      else
          return -1;
  }

};

}

#endif // _PSTARITHPREDICTOR_HPP
