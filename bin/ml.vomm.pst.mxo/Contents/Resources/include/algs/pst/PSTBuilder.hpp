#ifndef _PSTBUILDER_HPP
#define _PSTBUILDER_HPP
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

#include <sstream>

#include "../../util/Samples.hpp"
#include "PSTNodeInterface.hpp"
#include "DefaultPSTNode.hpp"

namespace vmm_algs_pst {

/**
 * <p>Title: Probabilistic Suffix Tree</p>
 * <p>Description: Initializing the PST algorithm</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: Foretell</p>
 * @author Ron Begleiter
 * @version 1.0
 *
 * Notes:
 * 1)
 */

class PSTBuilder {

private:
    const int ALPHABET_RANGE = 256;//ascii // default if not supplied
//    const int S_INITIAL_SIZE = 1024;//ascii
    const int UNSIGNED_BYTE_MASK = 0xFF;

    int alphabetSize;
    bool *seenAlphabet;
    int strHits;
    vector<int> strCharHits, charStrHits;

    vector<int> charStrHitsPerSample;/*@ron 1/03 for Golan's*/

    Samples* samples;

  //seperated to save object creation.
    vector<vector<int>* > queryStrs;
//    vector<string> queryStrs;
    vector<vector<double>*> suffStrNextSymProb; // queryStrs && suffStrNext.. have to be the same size, always! Should really use a pair in a single vector
    
    PSTNodeInterface* pstRoot;

public:
    PSTBuilder();

    PSTBuilder(int abSize);

    ~PSTBuilder();

  PSTNodeInterface* build(Samples* _samples, double pMin, double alpha,
                                double nextSymProbMin, double addedValThreshold,
                          int strMaxLength);

  /** @ron 1/03
   * Golan's PST version.
   *
   * @param _samples .
   * @param nMin .
   * @param minNumHits .
   * @param nextSymProbMin .
   * @param r .
   * @param strMaxLength .
   * @return .
   */
  PSTNodeInterface* buildGolanPST(Samples* _samples, int nMin, int minNumHits,
                                double nextSymProbMin, double r,
                                  int strMaxLength);

private:
    void init(double pMin, double nextSymProbMin);

    void initGolan(double nextSymProbMin);

    bool isConditionB(vector<double> *StrNSymProb, vector<double> *suffStrNSymProb,
                                double alpha, double nextSymProbMin,
                                double addedValThreshold);

    bool isConditionBGolan(vector<double> *StrNSymProb, vector<double> *suffStrNSymProb, int numHits, double r);

    void updateQueryStrsGolan(vector<int>* str, vector<double>* nextSymProb, int nMin);

    void updateQueryStrs(vector<int>* str, vector<double>* nextSymProb, double pMin);

    vector<double>* smooth(vector<double> *prob, double nsMinP);

    PSTNodeInterface* createPSTRoot(vector<double>* nextSymProb);

    void addToTree(vector<int>* str, vector<double> *strNSymProb, double nextSymProbMin);

    void initHitCounts(vector<int>* str);

    vector<double> *computeNextSymProb(vector<double> *retVal);

public:
  /*--- MAIN ---*/
  static void main(string* args) {

//    string samplesDir    = "./checkSamples";
//    string pstXMLFile    = "./log/t4.xml";
//    string pstArcFile    = "./pst/s.arc";
//    string toPredictFile = "./corpora/paper5.txt";
//
//    PSTBuilder PSTBuilder1 = new PSTBuilder();
//    System.out.println("START> "+Calendar.getInstance().getTime());
//    Samples samples = new Samples();
//    samples.init(samplesDir);
//    PSTNodeInterface pst = null;
//    pst = PSTBuilder1.build(samples,/*pMin*/0.001,/*alpha*/0.01,
//                           /*nextSymMin*/0.001,/*info threshold*/1.05,/*maxLn*/12);
//    System.out.println("\nENDS> "+Calendar.getInstance().getTime());
//    try {
//      FileOutputStream fout = new FileOutputStream(pstXMLFile);
//
//      fout.write( ("<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\n").getBytes("ascii"));
//      fout.write( ("<pst>").getBytes());
//      fout.write(pst.toString().getBytes("ascii"));
//      fout.write( ("</pst>").getBytes());
//      fout.close();
//
//      pst.save(new File(pstArcFile));
//      /*
//      DefaultPSTNode pst = new DefaultPSTNode();
//      pst.load(new File("./files/pst/pst1.arc"));
//      fout = new FileOutputStream("./files/results/t2.xml");
//
//      fout.write( ("<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\n").getBytes("ascii"));
//      fout.write( ("<pst>").getBytes());
//      fout.write(pst2.toString().getBytes("ascii"));
//      fout.write( ("</pst>").getBytes());
//      fout.close();
//      */
//    } catch(Exception e){
//    }
//
//    System.out.println(" -- prediction --");
//    System.out.println("START> "+Calendar.getInstance().getTime());
//    try {
//      FileInputStream fin = new FileInputStream(toPredictFile);
//      FileChannel in = fin.getChannel();
//      ByteBuffer bbuff = ByteBuffer.allocate((int)in.size());
//      in.read(bbuff,0);
//      double prediction = pst.predict(bbuff.array());
//      System.out.println("prediction total size="+" size="+
//                         (-Math.log(prediction)/Math.log(2.0)));
//    }
//    catch (Exception ex) {
//      ex.printStackTrace();
//    }
//    System.out.println("\nENDS> "+Calendar.getInstance().getTime());
  }

};

}
#endif //
