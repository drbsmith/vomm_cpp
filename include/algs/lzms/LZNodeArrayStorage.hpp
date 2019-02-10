#pragma once
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

#include "LZNodeStorage.hpp"

namespace vmm_algs_lzms {

/**
 * @author Moti
 *
 * LZNodeArrayStorage uses an array to store the entries
 */
    class LZNodeArrayStorage : LZNodeStorage {

	final LZNode* storage;

    public:
        LZNodeArrayStorage(int alphabetSize) {
            storage = new LZNode[alphabetSize];
        }

	/* (non-Javadoc)
	 * @see prediction.LZNodeStorage#put(int, prediction.LZNode)
	 */
        void put(int key, LZNode node) {
            storage[key] = node;
        }

	/* (non-Javadoc)
	 * @see prediction.LZNodeStorage#get(int)
	 */
        LZNode* get(int key) {
            return storage[key];
        }

	/* (non-Javadoc)
	 * @see prediction.LZNodeStorage#toArray()
	 */
        std::vector<LZNode*>* toArray() {
            int size = 0;
            for (int i = 0; i < storage.length; i++)
                if (storage[i] != null)
                    size++;
            LZNode[] result = new LZNode[size];
            for (int i = 0, j = 0; i < storage.length; i++)
                if (storage[i] != null) {
                    result[j] = storage[i];
                    j++;
                }
            return result;
        }
};
}
