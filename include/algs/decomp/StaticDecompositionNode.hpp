#ifndef _STATICDECOMPOSITIONNODE_HPP
#define _STATICDECOMPOSITIONNODE_HPP
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
#include "AbsBinaryDNode.hpp"
#include "DecompositionNode.hpp"

namespace vmm_algs_decomp {

    using namespace vmm_util;

    /**
     * <p>Title: </p>
     * <p>Description: </p>
     * <p>Copyright: Copyright (c) 2003</p>
     * <p>Company: </p>
     * @author not attributable
     * @version 1.0
     */

    class StaticDecompositionNode : public AbsBinaryDNode {
public:
        StaticDecompositionNode(int abSize, shared_ptr<AbsBinaryDNode> rightChild, shared_ptr<AbsBinaryDNode> leftChild, int softModelDepth) :
                        AbsBinaryDNode(abSize, rightChild, leftChild, softModelDepth) {
                            myType = DecompositionNode::nodeType::StaticDecompositionNode;
        }
      /**
       * Traing this StaticDecompositionNode upon symbol.
       */
      virtual void train(int symbol, Context* context)=0;
    };

}

#endif // _STATICDECOMPOSITIONNODE_HPP
