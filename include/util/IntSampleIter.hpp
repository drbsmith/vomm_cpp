#ifndef _INTSAMPLEITER_HPP
#define _INTSAMPLEITER_HPP
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
#include <vector>
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
    class IntSampleIter : public SampleIterator {
    private:
        const string NULL_NAME = "*NONAME*";
        vector<int> data;
        int dataInd;

        string name;

    public:
        IntSampleIter(int* _data, int dataSize) {
            for (int i = 0; i < dataSize; i++)
                data.push_back(_data[i]);
            
            dataInd = 0;
            name = NULL_NAME;
        }
        IntSampleIter(vector<int>* _data) {
            data.insert(data.begin(), _data->begin(), _data->end());
            
            dataInd = 0;
            name = NULL_NAME;
        }
        ~IntSampleIter() {
            data.clear();
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


        long size() {
            return data.size();
        }

        void setName(string _name) {
            name = _name;
        }
        
        string getName() {
            return name;
        }
        
        vector<int>* getData() {
            return &data;
        }

    };

}
#endif // _INTSAMPLEITER_HPP
