#ifndef _SAMPLEITSAMPLEBRIDGE_HPP
#define _SAMPLEITSAMPLEBRIDGE_HPP

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

#include "Samples.hpp"
#include "SampleIterator.hpp"

namespace vmm_util {

    /**
     * <p>Title: </p>
     * <p>Description: </p>
     * <p>Copyright: Copyright (c) 2003</p>
     * <p>Company: </p>
     * @author not attributable
     * @version 1.0
     */

class SampleIterator2SamplesBridge : public Samples {
    private:

    vector<int> symList; //TIntArrayList symList;

    public:
      SampleIterator2SamplesBridge() {
          //symList = new TIntArrayList();
      }
    SampleIterator2SamplesBridge(SampleIterator* sampleIter) {
        init(sampleIter);
    }

    void init(SampleIterator* sampleIter) {
        while (sampleIter->hasNext()) {
            int sym = sampleIter->next();
            symList.push_back(sym);
        }
    }

//    void init(string SamplesPath){
//        throw "Method init(String SamplesPath) not yet implemented.";
//    }

      /**
       * inits with sample of index i of samples
       */
//    void init(Samples sourceSamples, int i){
//        throw "Method init(String SamplesPath) not yet implemented.";
//    }

    virtual void disposeAll() override {
        symList.clear();
    }

//    string toString(int sampleInd){
//        return "SIter2SBridge";
//    }

      /**
       * discarding sampleIndex
       * @param sampleIndex not relevant
       * @param index within this SampleIterator2SamplesBridge
       * @return sym
       */
      virtual int get(int sampleIndex, int index) override {
          if (index < symList.size())
              return symList.at(index);
          else
              return 0;
      }


      virtual unsigned long size(int sampleIndex = 0) override {
          if (sampleIndex == 0) {
              return symList.size();
          } else
              return -1;
     }


      virtual unsigned long size() override {
          /*Only one sample*/
          return 1;
      }

      virtual unsigned long allLength() override {
          return symList.size();
      }

};
    
}

#endif // _SAMPLEITSAMPLEBRIDGE_HPP
