#ifndef _STATICBINDNODE_HPP
#define _STATICBINDNODE_HPP
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
#include "StaticDecompositionNode.hpp"

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

    class StaticBinDNode : public StaticDecompositionNode {
    public:
          StaticBinDNode(int abSize, AbsBinaryDNode* rightChild, AbsBinaryDNode* leftChild, int softModelDepth) :
                    StaticDecompositionNode(abSize, rightChild, leftChild, softModelDepth) {
                myType = nodeType::StaticBinDNode;
          }

          virtual double predict(int symbol, Context* context){
            int direction = (children[RIGHT]->getDescendants()->Get(symbol)) ?
                RIGHT :
                LEFT;
              
            double prediction = softClasifier->predict(symbol, direction, context->getIterator());
            prediction *= children[direction]->predict(symbol, context);
            return prediction;
          }

         virtual void train(int symbol, Context* context) {
           int direction = (children[RIGHT]->getDescendants()->Get(symbol)) ?
                RIGHT :
                LEFT;

            try {
                softClasifier->learn(symbol, direction, context->getIterator());
                if (children[direction] && children[direction]->getType() == nodeType::StaticBinDNode) {
                    ( (StaticBinDNode*) children[direction])->train(symbol, context);
                }
            }
            catch(std::exception e) {
              // a leaf. <--- this is terrible code, the if statement above should prevent this ever happening
            }
         }
    };

}
#endif // _STATICBINDNODE_HPP
