#ifndef _SAMPLES_HPP
#define _SAMPLES_HPP

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

using namespace std;

namespace vmm_util {

//import java.io.*;
//import java.nio.*;
//import java.nio.channels.*;

/**
 * <p>Title: Probabilistic Suffix Tree</p>
 * <p>Description: Initializing the PST algorithm</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: Foretell</p>
 *
 * @author Ron Begleiter
 * @version 1.0
 */

class Samples {
private:
    vector<string> samples; //FileChannel samples[];
//    std::string samplesNames;   // was name of files
    string samplesPath;

    // ByteBuffer bbuf;
    int bufSampleIndex;         //sample index

public:
    Samples() {
        //samples = new FileChannel[0];
    }
    virtual ~Samples() {
        samples.clear();
    }

    void init(string _samplesPath){
      samplesPath = _samplesPath;
//      initSamples();
      
//      bbuf = ByteBuffer.allocateDirect(0);
      bufSampleIndex = -1;
    }

  /**
   * inits with sample of index i of samples
   */
    void init(Samples* sourceSamples, int i){
        samples.clear(); // = new FileChannel[1];
//        samplesNames = new string[1];

        samples[0] = sourceSamples->get(i);
//        samplesNames[0] = sourceSamples.samplesNames[i];
//        bbuf = ByteBuffer.allocateDirect(0);
        bufSampleIndex = -1;
    }

  virtual void disposeAll(){
      samples.clear();
      samplesPath = "";
      bufSampleIndex = 0;
//    for (int i=0; i<samples.length; ++i){
//        try{
//            samples[i].close();
//        }
//        catch(exception e){
//            e.printStackTrace();
//        }
//    }
  }

//  string toString(int sampleInd){
//    return samplesNames[sampleInd];
//  }

    // get a specific character (sampleIndex) from a specific string (index)
    virtual int get(int sampleIndex, int index){
        int ret = -1;
        if (sampleIndex >= 0 && sampleIndex < samples.size()){
            bufSampleIndex = sampleIndex;
            string samp = samples.at(sampleIndex);
            if (index < samp.size())
                ret = samp[index];
        }

        return ret;
        //return bbuf.get(index);
    }
    
    virtual string get(int sampleIndex) {
        string ret;
        
        if (sampleIndex >= 0 && sampleIndex < samples.size()) {
            bufSampleIndex = sampleIndex;
            ret = samples.at(sampleIndex);
        }
        return ret;
    }

    virtual size_t size(int sampleIndex) {
		size_t ret = 0;

      if (sampleIndex >= 0 && sampleIndex < samples.size())
          ret = samples.at(sampleIndex).size();

      return ret;
    }

    virtual size_t size() {
        return samples.size();
    }

    virtual size_t allLength() {
		size_t ret = 0;
      for(int i=0; i<samples.size(); ++i){
          ret += samples[i].size();
      }
      return ret;
    }

private:
//  void initSamples(){
//    File f = new File(samplesPath);
//    if(f.isDirectory()){
//        initFromDir(f);
//    }
//    else{
//        initFromFile(f);
//    }
//  }

//  void initFromDir(File dir){
//    File[] files = dir.listFiles();
//
//    samples = new FileChannel[files.length];
//    samplesNames = new string[files.length];
//    FileInputStream fin = null;
//    for (int i=0; i<files.length; ++i){
//        try {
//            samplesNames[i] = files[i].tostring();
//            fin = new FileInputStream(files[i]);
//            samples[i] = fin.getChannel();
//        }
//        catch (IOException ex) {
//            ex.printStackTrace();
//        }
//    }
//  }
//
//    void initFromFile(File sameplsFile){
//        samples = new FileChannel[1];
//        samplesNames = new string[1];
//        FileInputStream fin = null;
//        try{
//              samplesNames[0] = sameplsFile.tostring();
//              fin = new FileInputStream(sameplsFile);
//              samples[0] = fin.getChannel();
//        }
//        catch(IOException ex){
//            ex.printStackTrace();
//        }
//    }


  /** T E S T -- M A I N **
  public static void main(string[] args) {
    Samples samples1 = new Samples();
    samples1.init("./files/gutenberg");
    System.out.println("---- TEST -----");
    for(int size=samples1.size(), i=0; i<size;++i){
      for(int j=0; j<samples1.size(i); ++j){
        System.out.print((char)samples1.get(i,j));
      }
    }
  }*/

};

}

#endif // _SAMPLES_HPP
