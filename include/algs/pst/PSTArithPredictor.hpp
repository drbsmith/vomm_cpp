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
  PSTArithPredictor(PSTNodeInterface* pstRoot) {
    pst = pstRoot;
    context = new DefaultContext(pst->subTreeHeight());
  }

    ~PSTArithPredictor() {
        delete context;
    }

  /**
   * @see arith.ArithPredictor
   */
  void predict(vector<double>* prediction){
    PSTNodeInterface* contextNode = pst->get(context->getIterator());
      
    contextNode->predict(prediction);   // fills prediction with the probabilities from contextNode
  }

  /**
   * @see arith.ArithPredictor
   */
  void increment(int symbol){
    context->add(symbol);
  }

  /**
   * @see arith.ArithPredictor
   */
  int alphabetSize(){
    return pst->getAlphabetSize();
  }

};

}

#endif // _PSTARITHPREDICTOR_HPP
