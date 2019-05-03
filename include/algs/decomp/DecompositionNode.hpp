#ifndef _DECOMPOSITIONNODE_HPP
#define _DECOMPOSITIONNODE_HPP
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

#include <memory>
#include "../../util/Context.hpp"

namespace vmm_algs_decomp {

    /**
     * <p>Title: </p>
     * <p>Description: </p>
     * <p>Copyright: Copyright (c) 2003</p>
     * <p>Company: </p>
     * @author not attributable
     * @version 1.0
     */

    class DecompositionNode : public std::enable_shared_from_this<DecompositionNode> {
    public:
        enum nodeType {BinDLeaf, AbsBinaryDNode, DynamicBinDNode, StaticBinDNode, StaticDecompositionNode, Null};
    protected:
        nodeType myType;
    public:
        nodeType getType() { return myType; }
        
        virtual ~DecompositionNode() {} // myType = nodeType::Null; }
      /**
       * Predicts the probability of symbol according to this DecompositionNode
       * and its descendants.
       */
      //double predict(int symbol);

        virtual double predict(int symbol, vmm_util::Context* context)=0;
    };

}
#endif // _DECOMPOSITIONNODE_HPP
