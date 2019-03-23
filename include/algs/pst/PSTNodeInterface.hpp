#ifndef _PSTNODEINTERFACE_HPP
#define _PSTNODEINTERFACE_HPP
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

#include "../../util/ContextIterator.hpp"
#include <string>
#include <vector>

#ifndef Byte
typedef unsigned char Byte;
#endif

namespace vmm_algs_pst {

    using namespace vmm_util;
    using namespace std;

class PSTNodeInterface {
public:
    virtual ~PSTNodeInterface() {}
    
    virtual vector<int>* getIDString()=0;

    virtual double predict(int ch)=0;
    virtual double predict(string ch)=0;
    virtual double predict(vector<int>* seq)=0;
    virtual double predict(Byte b)=0;
    virtual double predict(vector<Byte>* bytes)=0;

    /**
    * Assert param length is this.getAlphabetSizeI()
    * initializes pArr s.a. pArr[sym] = this node sym prediction
    */
    virtual vector<double>* predict()=0;

    /**
    * @returns PSTNodeInterface corresponds to the largest suffix of the string defined
    *          by seq.
    */
    virtual PSTNodeInterface* get(vector<Byte>* byteSeq)=0;//descended
    virtual PSTNodeInterface* get(string seq)=0;//descended
    virtual PSTNodeInterface* get(vector<int>* seq)=0;//descended
    virtual PSTNodeInterface* get(ContextIterator* contextIter)=0;//descended
    virtual PSTNodeInterface* get(int symbol)=0;//child

    virtual void insert(int symbol, vector<double>* nextSymProbability)=0;

    /**
    * @return this node sub tree height
    */
    virtual int subTreeHeight()=0;

    virtual int getAlphabetSize()=0;
    
    virtual string toString()=0;
private:
    virtual PSTNodeInterface* get(vector<Byte> *bytes, int currentByteIndex)=0;
//       PSTNodeInterface load(File source);
//       void save(File dest);
};

}

#endif // _PSTNODEINTERFACE_HPP
