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

using namespace vmm_algs_ctw;
using namespace vmm_algs_pst;
using namespace vmm_algs_decomp;
using namespace std;

int main(int argc, const char * argv[]) {
    
    cout << "PST: " << endl;
    PSTPredictor pst;
    
    pst.init(256, 0.001, 0.0, 0.0001, 1.05, 20);
    pst.learn("abracadabra");
    
    cout << "logeval : " << pst.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << pst.predict('c', "abra") << endl << endl;
    
    cout << "Binary CTW:" << endl;
    BinaryCTWPredictor ctw;
    ctw.init(6, 8);
     char data[] = {97, 98, 114, 97, 99, 97, 100, 97, 98, 114, 97}; // same as seq = "abracadabra"
     string seq(data);
     
     ctw.learn(seq);
    
    cout << "logeval : " << ctw.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << ctw.predict('c', "abra") << endl << endl;

    
    DCTWPredictor dctw;
    dctw.init(256, 5);
    dctw.learn("abracadabra");
    
    cout << "logeval : " << dctw.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << dctw.predict('c', "abra") << endl;
    
    cout << "P(c|dab) : " << dctw.predict('c', "dab") << endl;
    cout << "P(c|ra) : " << dctw.predict('c', "ra") << endl;
    cout << "P(c|bra) : " << dctw.predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << dctw.predict('c', "abcd") << endl;
    cout << "P(c|r) : " << dctw.predict('c', "r") << endl << endl;
    
    return 0;
}
