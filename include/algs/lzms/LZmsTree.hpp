#ifndef LZMSTREE_HPP
#define LZMSTREE_HPP

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

#include "../../util/SampleIterator.hpp"
#include "LZNode.hpp"
#include <vector>

namespace vmm_algs_lzms {

/**
@author The AIS DevTeam
@version 0.3
*/

// A single node in the LZTree

/**
   The tree structure defined by the LZ(m,s) algorithm
   @see VarSizedLZ
*/
class LZmsTree {
private:
	LZNode* root;
	int minContext;
	int numShifts;

	double negativeInverseOfLogAlphabetSize;

        /**@ron */
    static const double negativeInverseOfLg;

    bool likelihoodsSet;

	/**
	   Constructs a LZmsTree
	*/
public:
        LZmsTree(int _minContext, int _numShifts, int _alphabetSize);

        void learnSequence(std::vector<int>* sequence);

private:
    void learnOffsetSequence(std::vector<int>* sequence, int offset);

	/**
	   Calculates the probability of a string
	   @return A double between 0 and 1
	*/
    double getProbablityOfSequence(std::vector<int>* sequence);

public:
    double rateSequence(std::vector<int>* sequence) ;

        /**
         * Calculates the loglikelihood of sequence
         * @param sequence of symbols
         * @return -lg (P (sequence | training) )
         * @author Ron B.
         */
    double calcLogLikelihood(std::vector<int>* sequence);
    };
} //namespace

#endif
