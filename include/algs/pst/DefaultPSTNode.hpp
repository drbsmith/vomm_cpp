#ifndef _DEFAULTPSTNODE_HPP
#define _DEFAULTPSTNODE_HPP

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

#include <unordered_map>
#include <string>
#include <vector>

#include "PSTNodeInterface.hpp"
//#include "../../util/UnsignedByteConverter.hpp"

using namespace std;

namespace vmm_algs_pst {

//import vmm.util.*;
//
//import java.io.*;
//import java.nio.ByteBuffer;
//import java.util.*;

/*!*/

/**
 * Title:
 * Description:
 *               Updated: get(CharSequence str)
 * Copyright:    Copyright (c) 2003
 * Company:
 * @version 1.2
 */

class DefaultPSTNode : public PSTNodeInterface {

    typedef std::pair<int, DefaultPSTNode*> PSTNode;
    typedef unordered_map<PSTNode::first_type, PSTNode::second_type> PSTChildren;
    
private:
    const static string XML_NODE_ST, XML_NODE_ET, TOSTRING_INIT, XML_EXCEP_AND, XML_EXCEP_LT,XML_ID_CDATA_ST;
    const static string XML_ID_CDATA_ET, XML_ID_ST, XML_ID_ET, XML_PROB_ST, XML_PROB_ET, XML_PROB_CDATA_ST;
    const static string XML_PROB_CDATA_ET, XML_CHILDREN_ST, XML_CHILDREN_ET;

    const static int ABSIZE = 256;  // default if not supplied

    // BDS: Do these properties need to be public? Not very safe...
    vector<int> idStr;
    vector<double> nextSymProbability;/*ZZZ*/
    PSTChildren children;
//    vector<DefaultPSTNode*>* children;/*ZZZ*/
    bool isLeaf;/*ZZZ*/

    int absize;
    
public:
    DefaultPSTNode();

    DefaultPSTNode(int alphabetSize);

    DefaultPSTNode(vector<int>* idStr, vector<double>* nextSymProbability);
    
    ~DefaultPSTNode();

    int getAlphabetSize();
    vector<int>* getIDString();

    double predict(int ch);
    double predict(string charSeq);
    double predict(vector<int>* charSeq);
    double predict(Byte b);
    double predict(vector<Byte> *bytes);
    
    vector<double>* predict();

  /**
   * @returns PSTNodeInterface corresponds to the largest suffix of the string defined
   *          by seq.
   */
  /**
   * @returns PSTNodeInterface corresponds to the largest suffix of the byteSeq
   */
    PSTNodeInterface* get(vector<Byte> *byteSeq);
    PSTNodeInterface* get(ContextIterator* context);
    PSTNodeInterface* get(string sbuff);
    PSTNodeInterface* get(vector<int>* sbuff);
    PSTNodeInterface* get(int symbol);

private:
    PSTNodeInterface* get(vector<Byte> *bytes, int currentByteIndex);

    void insert(int symbol, vector<double>* nextSymProbability);

public:
    /**
     * @return this node sub tree height
     */
    int subTreeHeight();
    
    string toString();
    static DefaultPSTNode* FromString(string data);    // reconstitute from serialized string

    static vector<double>* str2Prob(string data, int absize);
private:
    string prob2Str();

    static string GetOneNode(string data);  // parse one node's worth of data from the string
};

}
#endif // _DEFAULTPSTNODE_HPP
