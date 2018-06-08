#ifndef _DEFAULTCONTEXT_HPP
#define _DEFAULTCONTEXT_HPP

#include <string>
#include "Context.hpp"

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
 
namespace vmm_util {

/**
 * <p>Title: </p>
 * <p>Description: Default Context implementation</p>
 * Note that this implementation is not iteration safe!
 * meaning that only one iteration can be done at a time,
 * and while itterating one should not add a symbol to this context.
 * @see getIterator
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */

class DefaultContext : public Context, public ContextIterator {
private:
//  int ILL_INDEX = -1;

  /*Context collection, a symbol is access by its int id.
     ie ASCII id.
   */
  int* context;
unsigned int context_len;
  int nextAddIndex;

  /*Indicates the size of the population in context[]*/
  int population;

  /*Iteration index*/
  int iterInd;
  int iterCount;

public:
    DefaultContext(int maxlength) : context_len(maxlength) {
        _assert(maxlength > 0, "context length<=0");
        context = new int[maxlength];
        nextAddIndex = 0;
        population = 0;
        iterInd = 0;
        iterCount = 0;
    }

    void add(int symbol){
        context[nextAddIndex] = symbol;
        nextAddIndex = (nextAddIndex+1) % context_len;
        population = isFull() ? population : population+1;
    }

    /*Note: this implemntation is not safe*/
    ContextIterator* getIterator(){
        iterInd = indexBefore(nextAddIndex);
        iterCount = 0;
        return (ContextIterator*)this;
    }

    bool hasNext(){
        return iterCount != population;
    }

    int nextSymbol(){
        int sym = context[iterInd];
        iterInd = indexBefore(iterInd);
        iterCount++;
        return sym;
    }

//    static void main(std::string &args){
//        DefaultContext c(3);
//        c.add(1);c.add(2);c.add(3);c.add(4);c.add(5);
//        ContextIterator* iter = c.getIterator();
//        while(iter->hasNext()){
//            System.out.println(""+(int)iter.nextSymbol());
//        }
//    }

private:
  int indexBefore(int index){
    return (index==0)? context_len-1 : (index-1);
  }

  bool isFull(){
    return (population == context_len);
  }

    void _assert(bool condition, std::string descp) {
        if (false == condition) {
            throw "Assertion Failed: " + descp;
        }
    }

};

}

#endif
