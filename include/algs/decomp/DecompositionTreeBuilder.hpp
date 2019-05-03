#ifndef _DECOMPOSITIONTREEBUILDER_HPP
#define _DECOMPOSITIONTREEBUILDER_HPP
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

#include <map>
//#include <vector>

#include "../../util/SampleIterator.hpp"
#include "AbsBinaryDNode.hpp"
#include "BinDLeaf.hpp"
#include "DecompositionNode.hpp"
#include "DynamicBinDNode.hpp"
#include "StaticBinDNode.hpp"
#include "StaticDecompositionNode.hpp"

namespace vmm_algs_decomp {

    using namespace vmm_util;
    using namespace std;

/**
 * <p>Title: DecompositionTreeBuilder</p>
 * <p>Description: Builds a Decomposition Tree from Samples</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author ron
 * @version 1.0
 */

    class DecompositionTreeBuilder {
    private:
        int absize;
        int softModelDepth;

    public:        
        DecompositionTreeBuilder(int _absize, int _softModelDepth);

  /**
   * Build dynamic
   */
        shared_ptr<DecompositionNode> build(SampleIterator* sample);

  /***/
  /**
 * build static
 * (should never use code replication again..)
 */
        
        shared_ptr<StaticDecompositionNode> buildStatic(SampleIterator* samples);

  /***/

    private:
        shared_ptr<AbsBinaryDNode> buildHuf(TreeMap* countMap);
        
  /*****/
  //      calculateABCount()
//      must return a sorted map. In c++ the map is always ascending by Key.
  /*****/
        TreeMap* calculateAbCount(SampleIterator* sample);

//        void swap(AbsBinaryDNode* n1, AbsBinaryDNode* n2);

};
}   // namespace
#endif //_DECOMPOSITIONTREEBUILDER_HPP
