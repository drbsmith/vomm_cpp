#ifndef OFFLINEPPMMODEL_HPP
#define OFFLINEPPMMODEL_HPP
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

/////////////////////
// Converted to c++ by Benjamin Smith (BDS), October 2018.
//  While titled 'offline' the sequential training model implemented
//  here works just as well 'online.' Simply feed it more sequences
//  and it will continue to learn and extend the existing model.
/////////////////////


//import vmm.algs.com.colloquial.arithcode.*;
#include "../com/colloquial/arithcode/PPMModel.hpp"

namespace vmm_algs_oppm {

    using namespace vmm_algs_com_colloquial_arithcode;

/**
 * Offline PPMC implementation.
 * Using <a href="http://www.colloquial.com/carp/">Bob Carpenter</a> code.
 *
 * <p>Copyright: Copyright (c) 2004</p>
 *
 * @author <a href="http://www.cs.technion.ac.il/~ronbeg">Ron Begleiter</a>
 * @version 1.0
 */
    class OfflinePPMModel : public PPMModel {

    private:
        int allRes[3];
        bool isFirstPrediction;

    public:
        OfflinePPMModel(int maxCodeLength, int absize);
        
        ~OfflinePPMModel();

        double predict(int symbol);

        void use(int symbol);

  // specified in ArithCodeModel
        void interval(int symbol, int* result);

  /**
   * Clears this OfflinePPMModel's context.
   * As a result the nexts symbol context will be the empty context.
   */
        void clearContext();

  /** Returns interval for byte specified as an integer in 0 to 255 range.
   * @param i Integer specification of byte in 0 to 255 range.
   * @param result Array specifying cumulative probability for byte i.
   */
    private:
        void calcInterval(int i, int* result);
    };
}   // namespace

#endif // OFFLINEPPMMODEL_HPP
