//
//  ml_hmm.cpp
//  ml.HMM
//
//  Created by Ben Smith 2017.
//
//

#include "ml_vomm.hpp"
#include <sstream>
#include <time.h>


using namespace vmm_algs_pst;
using namespace vmm_algs_ctw;
using namespace vmm_algs_decomp;    // dctw

mlVOMM::mlVOMM(void* parentObject) : maxObject(parentObject)
{
    myPST = new PSTPredictor();

    ((PSTPredictor*)myPST)->init(256, 0.001, 0.0, 0.0001, 1.05, 20);
    myPST->learn("abracadabra");
    
    post("logeval : %f", myPST->logEval("cadabra"));
    post("P(c|abra) : %f", myPST->predict('c', "abra"));
    
/*    myPST = new BinaryCTWPredictor();
    ((BinaryCTWPredictor*)myPST)->init(6, 8);
    char data[] = {97, 98, 114, 97, 99, 97, 100, 97, 98, 114, 97}; // same as seq = "abracadabra"
    string seq(data);
    
    myPST->learn(seq);
    post("%s", seq.c_str());
 */
    
    
//    myPST =  new DCTWPredictor();
//    ((DCTWPredictor*)myPST)->init(256, 5);
//    myPST->learn("abracadabra");
    
//    * System.out.println("logeval : " + dctw.logEval("cadabra"));
//    * System.out.println("P(c|abra) : " + dctw.predict('c', "abra"));
}

mlVOMM::~mlVOMM() {
    if (myPST)
        delete myPST;
}


double mlVOMM::logEval(char* test) {
    string s(test);
    double ret = -1;
    if (myPST) {
        ret =  myPST->logEval(test);
        post("logeval %s : %f", test, ret);
    }
    return ret;
}

double mlVOMM::predict(int state, char* test) {
    string s(test);
    double ret = -1;
    if (myPST) {
        ret = myPST->predict(state, test);
        post("P(%c|%s) : %f", state, test, ret);
    }
    return ret;
}
