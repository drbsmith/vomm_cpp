#ifndef _STRINGSAMPLEITER_HPP
#define _STRINGSAMPLEITER_HPP
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
#include "SampleIterator.hpp"

using namespace std;

namespace vmm_util {

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */
    class StringSampleIter : public SampleIterator {
    private:
        const string NULL_NAME = "*NONAME*";
        string data;
        int dataInd;

        string name;

    public:
        StringSampleIter(string _data) : data(_data) {
            dataInd = 0;
            name = NULL_NAME;
        }

        bool hasNext() {
            return dataInd < data.size();
        }

        int next() {
            if (hasNext())
                return (int)data[dataInd++];
            else
                return -1;  // error
//                throw "No Such Element";
        }

        void restart () {
            dataInd=0;
        }


        size_t size() {
            return data.size();
        }

        void setName(string _name) {
            name = _name;
        }
        
        string getName() {
            return name;
        }
        
        string getData() {
            return data;
        }

    };

}
#endif // _STRINGSAMPLEITER_HPP
