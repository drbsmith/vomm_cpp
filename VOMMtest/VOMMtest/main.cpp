//
//  main.cpp
//  VOMMtest
//
//  Created by Benjamin Smith on 6/7/18.
//  Copyright © 2018 Benjamin Smith. All rights reserved.
//

#include <iostream>

#include "../../include/algs/PSTPredictor.hpp"
#include "../../include/algs/BinaryCTWPredictor.hpp"
#include "../../include/algs/DCTWPredictor.hpp"
#include "../../include/algs/PPMCPredictor.hpp"
#include "../../include/algs/LZmsPredictor.hpp"

using namespace vmm_algs_ctw;
using namespace vmm_algs_pst;
using namespace vmm_algs_decomp;
using namespace vmm_algs_ppmc;
using namespace vmm_algs_lzms;
using namespace std;

int main(int argc, const char * argv[]) {
    
    int data[] = {97, 98, 114, 97, 99, 97, 100, 97, 98, 114, 97}; // same as seq = "abracadabra"
    vector<int> dataVec;
    for (int i = 0; i < 11; i++)
        dataVec.push_back(data[i]);
    
    cout << "PST: " << endl;
    PSTPredictor pst;
    
    pst.init(256, 0.001, 0.0, 0.0001, 1.05, 20);
//    pst.learn("abracadabra");
    
    cout << "logeval : " << pst.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << pst.predict('c', "abra") << endl << endl;
    
    cout << "Binary CTW:" << endl;
    BinaryCTWPredictor ctw;
    ctw.init(6, 8);
//     char data[] = {97, 98, 114, 97, 99, 97, 100, 97, 98, 114, 97}; // same as seq = "abracadabra"
//     string seq(data);
    
    ctw.learn("abracadabra");
    
    cout << "logeval : " << ctw.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << ctw.predict('c', "abra") << endl << endl;

    
    DCTWPredictor dctw;
    dctw.init(256, 5);
    dctw.learn(&dataVec); //"abracadabra");
    
    cout << "DCTW:" << endl;
    cout << "logeval : " << dctw.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << dctw.predict('c', "abra") << endl;
    
    cout << "P(c|dab) : " << dctw.predict('c', "dab") << endl;
    cout << "P(c|ra) : " << dctw.predict('c', "ra") << endl;
    cout << "P(c|bra) : " << dctw.predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << dctw.predict('c', "abcd") << endl;
    cout << "P(d|ra) : " << dctw.predict('d', "ra") << endl << endl;
    
    PPMCPredictor ppmc;
    ppmc.init(256, 5);
//    for (int i = 0; i < 4; i++) {
//        ppmc.learn("abracadabra");
//    }
//    for (int i = 0; i < 11; i++) {
//        int j = data[i];
//        ppmc.learn(&j, 1);
//    }
    ppmc.learn(&dataVec);
    
    cout << "PPMC:" << endl;
    cout << "logeval : " << ppmc.logEval("cadabra") << endl;
//    cout << "logeval : " << ppmc.logEval("cadabra") << endl;
//    cout << "logeval : " << ppmc.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << ppmc.predict('c', "abra") << endl;
    cout << "P(c|dab) : " << ppmc.predict('c', "dab") << endl;
    cout << "P(c|ra) : " << ppmc.predict('c', "ra") << endl;
    cout << "P(c|bra) : " << ppmc.predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << ppmc.predict('c', "abcd") << endl;
    cout << "P(d|ra) : " << ppmc.predict('d', "ra") << endl << endl;
//    for (int i = 0; i < 256; i++) {
//        cout << ppmc.predict(i, "abra") << ", ";
//    }
//    double* pred = ppmc.predictAll("abra");
//    for (int i = 0; i < 256; i++) {
//        cout << pred[i] << ", ";
//    }
//    delete [] pred;
    
    string model = ppmc.ModelToString();    // get the model encoded as an ascii string
    
    ppmc.init(256,5);
    ppmc.ModelFromString(model);    // reconstitute the model from the ascii string!
    
    // Could also feed the model in 1 line at a time:
//    ppmc.ModelFromString("n 97|1|3 [100|1|1 [97|1|1 [98|1|1 [114|1|1 [97|1|0]]]] {99|1|1 [97|1|1 [100|1|1 [97|1|1 [98|1|0]]]] {98|5|1 [114|4|1 [97|3|1 [99|2|1 [97|1|0]]]]}}]");
//    ppmc.ModelFromString("n 98|1|1 [114|1|1 [97|1|1 [99|1|1 [97|1|1 [100|1|0]]]]]");
//    ppmc.ModelFromString("n 99|1|1 [97|1|1 [100|1|1 [97|1|1 [98|1|1 [114|1|0]]]]]");
//    ppmc.ModelFromString("n 114|1|1 [97|1|1 [99|1|1 [97|1|1 [100|1|1 [97|1|0]]]]]");
    
    cout << "PPMC (reconstituted):" << endl;
    cout << "logeval : " << ppmc.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << ppmc.predict('c', "abra") << endl;
    cout << "P(c|dab) : " << ppmc.predict('c', "dab") << endl;
    cout << "P(c|ra) : " << ppmc.predict('c', "ra") << endl;
    cout << "P(c|bra) : " << ppmc.predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << ppmc.predict('c', "abcd") << endl;
    cout << "P(d|ra) : " << ppmc.predict('d', "ra") << endl << endl;
    
    cout << ppmc.ModelToString();
    
    cout << "LZms: " << endl;
    LZmsPredictor lzms;
    
    lzms.init(256, 2, 0);
    lzms.learn("abracadabra");
    
    cout << "logeval : " << lzms.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << lzms.predict('c', "abra") << endl;
    cout << "P(c|dab) : " << lzms.predict('c', "dab") << endl;
    cout << "P(c|ra) : " << lzms.predict('c', "ra") << endl;
    cout << "P(c|bra) : " << lzms.predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << lzms.predict('c', "abcd") << endl;
    cout << "P(d|ra) : " << lzms.predict('d', "ra") << endl << endl;
    
    return 0;
}
