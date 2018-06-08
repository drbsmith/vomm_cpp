#ifndef CONTEXT_HPP
#define CONTEXT_HPP
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

#include "ContextIterator.hpp"

namespace vmm_util {

    /**
     * <p>Title: Context</p>
     * <p>Description: Multi-Alphabet Fixed Length Context
     * Each symbol identity is an integer (f: N-->AB).
     * </p>
     * <p>Copyright: Copyright (c) 2003</p>
     * <p>Company: </p>
     * @author Ron Begleiter
     * @version 1.0
     */

    class Context {
    public:
        virtual ~Context() {}
      /**
       * Adds symbol to this Context
       */
      virtual void add(int symbol)=0;
      /**
       * @returns a <code>ContextIterator</code> over this Context
       */
      virtual ContextIterator* getIterator()=0;
    };

}

#endif
