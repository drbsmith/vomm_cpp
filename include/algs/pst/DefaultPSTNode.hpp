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

private:
    const static string XML_NODE_ST, XML_NODE_ET, TOSTRING_INIT, XML_EXCEP_AND, XML_EXCEP_LT,XML_ID_CDATA_ST;
    const static string XML_ID_CDATA_ET, XML_ID_ST, XML_ID_ET, XML_PROB_ST, XML_PROB_ET, XML_PROB_CDATA_ST;
    const static string XML_PROB_CDATA_ET, XML_CHILDREN_ST, XML_CHILDREN_ET;

    const static int ABSIZE = 256;
public:
    // BDS: Do these properties need to be public? Not very safe...
    string idStr;/*ZZZ*/
    vector<double> *nextSymProbability;/*ZZZ*/
    vector<DefaultPSTNode*>* children;/*ZZZ*/
    bool isLeaf;/*ZZZ*/

    int absize;

    DefaultPSTNode();

    DefaultPSTNode(int alphabetSize);

    DefaultPSTNode(string idStr, vector<double>* nextSymProbability);
    
    virtual ~DefaultPSTNode();

    int getAlphabetSize();
    string getString();

    double predict(int ch);
    double predict(string charSeq);
    double predict(Byte b);
    double predict(vector<Byte> *bytes);
    
    void predict(vector<double> *pArr);

  /**
   * @returns PSTNodeInterface corresponds to the largest suffix of the string defined
   *          by seq.
   */
//  PSTNodeInterface* get(string seq){
//    StringBuffer sbuff = new StringBuffer(seq.toString());
//    return get(sbuff);
//  }

  /**
   * @returns PSTNodeInterface corresponds to the largest suffix of the byteSeq
   */
    PSTNodeInterface* get(vector<Byte> *byteSeq);
    PSTNodeInterface* get(ContextIterator* context);
    PSTNodeInterface* get(string sbuff);
    PSTNodeInterface* get(char symbol);

private:
    PSTNodeInterface* get(vector<Byte> *bytes, int currentByteIndex);

    void insert(char symbol, vector<double>* nextSymProbability);

public:
    /**
     * @return this node sub tree height
     */
    int subTreeHeight();
    
//   PSTNodeInterface load(File source){
//    try{
//      FileInputStream fin = new FileInputStream(source);
//      ObjectInputStream in = new ObjectInputStream(fin);
//      readObject(in);
//      in.close();
//    }catch(Exception e){
//      e.printStackTrace();
//    }
//    return this;
//  }
//
//  void save(File dest){
//    try{
//      FileOutputStream fout = new FileOutputStream(dest);
//      ObjectOutputStream out = new ObjectOutputStream(fout);
//      writeObject(out);
//      out.close();
//    }catch(Exception e){
//      e.printStackTrace();
//    }
//  }

    string toString();

private:
    string prob2Str();


//  private void writeObject(java.io.ObjectOutputStream out) throws IOException{
//    out.writeObject(idStr);
//    out.writeInt(nextSymProbability.length);
//    for(int i=0; i<nextSymProbability.length; ++i)
//      out.writeDouble(nextSymProbability[i]);
//    out.writeInt(children.length);
//    Vector v = new Vector(children.length);
//    Vector vInd = new Vector(children.length);
//    for(int i=0; i<children.length; ++i){
//      if(children[i]!=null){ v.add(children[i]); vInd.add(new Integer(i)); }
//    }
//    out.writeInt(v.size());
//    for(int i=0, test=v.size(); i<test; ++i){
//      out.writeInt(((Integer)vInd.elementAt(i)).intValue());
//      out.writeObject(v.elementAt(i));
//    }
//
//    out.writeObject(v);
//  }
//  private void readObject(java.io.ObjectInputStream in)
//     throws IOException, ClassNotFoundException{
//   idStr = (String)in.readObject();
//   int nsPLen = in.readInt();
//   nextSymProbability = new double[nsPLen];
//   for(int i=0; i<nsPLen; ++i)
//     nextSymProbability[i] = in.readDouble();
//   int childLen = in.readInt();
//   children = new DefaultPSTNode[childLen];
//   int elmSize = in.readInt();
//   for(int i=0,ind=0; i<elmSize; ++i){
//     ind = in.readInt();
//     children[ind] = (DefaultPSTNode)in.readObject();
//   }
// }

};

}
#endif // _DEFAULTPSTNODE_HPP
