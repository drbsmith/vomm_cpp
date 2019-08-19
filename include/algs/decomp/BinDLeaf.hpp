#ifndef _BINDLEAF_HPP
#define _BINDLEAF_HPP
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
#include <sstream>

#include "AbsBinaryDNode.hpp"
#include "../../util/Context.hpp"

namespace vmm_algs_decomp {

/**
 * <p>Title: DLeaf </p>
 * <p>Description: Decomposition Tree Leaf </p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */

class BinDLeaf : public AbsBinaryDNode {
public:
    BinDLeaf() {
        myType = DecompositionNode::nodeType::BinDLeaf;
    }
    BinDLeaf(int symbol, int absize, int softModelSize) : AbsBinaryDNode(absize, nullptr, nullptr, softModelSize) {
        myType = DecompositionNode::nodeType::BinDLeaf;
        // super(); <--- happens automatically in c++
        Init(symbol, absize, softModelSize);
    }
    
    void Init(int symbol, int absize, int softModelSize) {
        AbsBinaryDNode::Init(absize, nullptr, nullptr, softModelSize);
        
        descendants.resize(absize); // = new BitSet(absize);
        mySym = symbol;
        descendants.Set(mySym, true);  //  <-- also happens automat
        if (mySym > 250)
            mySym = mySym;
    }

    double predict(int sym, vmm_util::Context* context){
        if (sym == mySym) {
          return 1.0;
        } else {
            std::stringstream ss;
            ss << "BinDLeaf: Never Should Happen! with sym="  << sym << " this.sym=" << mySym;
            throw ss.str();
        }
    }

//  public void train(int symbol){
//    //NoOp
//  }

  bool equals(BinDLeaf* obj){
      return obj->symbol() == mySym; // ((obj instanceof BinDLeaf)&&(((BinDLeaf)obj).sym==this.sym));
  }

  int hashCode(){
    return mySym;
  }

    std::string toString(){
        std::stringstream ss;
        ss << mySym;
        return ss.str();
    }

};

} // namespace
#endif // _BINDLEAF_HPP
