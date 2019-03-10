#ifndef LZNODEHASHSTORAGE_HPP
#define LZNODEHASHSTORAGE_HPP
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
 
/*
 * Created on Jul 9, 2003
 */

#include "LZmsNode.hpp"
#include "LZNodeStorage.hpp"
#include <map>
#include <vector>

namespace vmm_algs_lzms {
    
/**
 * @author Moti
 *
 * LZNodeHashStorage uses a hash to store/retrieve LZNodes
 */
class LZNodeHashStorage : public LZNodeStorage {
    private:
    std::map< int, LZmsNode*> storage;

    public:
    ~LZNodeHashStorage();
    
        void put(int key, LZmsNode* node);

        vmm_algs_lzms::LZmsNode* get(int key);

        std::vector<LZmsNode*> * toArray();
    };
} // namespace

#endif // LZNODEHASHSTORAGE_HPP
