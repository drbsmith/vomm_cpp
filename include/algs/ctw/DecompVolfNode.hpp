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
#include <string>
#include <memory>

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

      vector<shared_ptr<DecompVolfNode> > children;// mem consuming //

    public:
        DecompVolfNode();
        ~DecompVolfNode();
        DecompVolfNode(string data);    // reconstruct from an encoded string
      /**
       * Initializes this VolfNode
       *
       * @param symABSize size of the samples AB
       * @param alphaFactor as defined in Volf's thesis, for the 0 order estimator.
       */
        void init(int symABSize, int alphaFactor);

        double predict(int symbol, int symSetLabel, ContextIterator* context);

        double learn(int symbol, int symSetLabel, ContextIterator* context);

        string toString();
        shared_ptr<DecompVolfNode> fromString(string data);
        void AddChild(shared_ptr<DecompVolfNode> child);
        
    private:
        double* predict(ContextIterator* context);

        double* learnAll(int newSymbol, int symSetLabel, ContextIterator* context);

        shared_ptr<DecompVolfNode> getChild(int sym);

        void rescaleCounts();


    };

}

#endif // _DECOMPVOLFNODE_HPP
