//
//  main.cpp
//  VOMMtest
//
//  Created by Benjamin Smith on 6/7/18.
//  Copyright © 2018 Benjamin Smith. All rights reserved.
//

// defines to test each of the vomm algorithms
#define TEST_PST
//#define TEST_BIN
#define TEST_DCTW
#define TEST_PPMC
//#define TEST_LZMS

#define TEST_SERIALIZATION

#define ALPHA_SIZE 256
//#define TRAINING_SIZE 5000
//#define USE_MIDI

#include <iostream>
#include <vector>

#include "/Users/smith/Box Sync/Code/midifile/include/MidiFile.h"

#include "../../include/algs/PSTPredictor.hpp"
#include "../../include/algs/BinaryCTWPredictor.hpp"
#include "../../include/algs/DCTWPredictor.hpp"
#include "../../include/algs/PPMCPredictor.hpp"
#include "../../include/algs/LZmsPredictor.hpp"

// using namespaces for each of the different algorithms
using namespace vmm_algs_ctw;
using namespace vmm_algs_pst;
using namespace vmm_algs_decomp;
using namespace vmm_algs_ppmc;
using namespace vmm_algs_lzms;
using namespace std;

int main(int argc, const char * argv[]) {
    vector<int> dataVec;
    double* prob = NULL;
    
#ifdef USE_MIDI
    MidiFile midifile("/Users/smith/Developer/vomm_cpp/01Prelude.mid");
    midifile.absoluteTime();
    midifile.joinTracks();
    
    for (int i=0; i<midifile.getNumEvents(0); i++) {
        int command = midifile.getEvent(0, i).data[0] & 0xf0;
        if (command == 0x90 && midifile.getEvent(0, i).data[2] != 0) {
            // store note-on velocity and time
            int pitch = midifile.getEvent(0, i).data[1];
            dataVec.push_back(pitch);
        }
    }
    cout << "midi notes:" << dataVec.size() << endl;
#else
#ifdef TRAINING_SIZE
    int r = 64;
    for (int i = 0; i < TRAINING_SIZE; i++) {
        r += (rand() % 25) - 12;
        r = (r < 0 ? ALPHA_SIZE-1 : r);
        dataVec.push_back(r % ALPHA_SIZE);
    }
#else
    int data[] = {97, 98, 114, 97, 99, 97, 100, 97, 98, 114, 97}; // same as seq = "abracadabra"
    string test = "The quick brown fox jumped over the lazy dog."; //"abracadabra"; 
    for (int i = 0; i < test.size(); i++)
        dataVec.push_back(test[i]);
#endif
#endif
    
#ifdef TEST_PST
    cout << "PST: " << endl;
    PSTPredictor* pst = NULL;
    for (int i = 0; i < 10; i++) {
        if (pst != NULL)
            delete pst;
        pst = new PSTPredictor();;
        
        pst->init(ALPHA_SIZE, 0.001, 0.0, 0.0001, 1.05, 20);
        pst->learn(&dataVec);
    }
    
    cout << "logeval : " << pst->logEval("cadabra") << endl;
    cout << "P(c|abra) : " << pst->predict('c', "abra") << endl;
    cout << "P(r|dab) : " << pst->predict('r', "dab") << endl;
    cout << "P(c|ra) : " << pst->predict('c', "ra") << endl;
    cout << "P(c|bra) : " << pst->predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << pst->predict('c', "abcd") << endl;
    cout << "P(d|ra) : " << pst->predict('d', "ra") << endl << endl;
    
    for (int i = 0; i < 1000; i++) {
        if (prob)
            delete prob;
        prob = pst->predictAll("abrac");
    }
    cout << "P(all) : " << prob[99] << endl << endl;
    delete prob; prob = NULL;
    
#ifdef TEST_SERIALIZATION
    string pstModel = pst->ModelToString();
    pstModel = pstModel.substr(0, pstModel.size()-50);
//    cout << pstModel << endl;
    
    for (int i = 0; i < 100; i++) {
        if (pst)
            delete pst;
        pst = new PSTPredictor();
        pst->ModelFromString(pstModel);
    }
    
//    pstModel = pst.ModelToString();
//    cout << pstModel << endl;
    
    cout << "Reconstituted from serialized string:" << endl;
    cout << "logeval : " << pst->logEval("cadabra") << endl;
    cout << "P(c|abra) : " << pst->predict('c', "abra") << endl << endl;
    
#endif
    
    delete pst;
#endif // PST
#ifdef TEST_BIN
    cout << "Binary CTW:" << endl;
    BinaryCTWPredictor ctw;
    ctw.init(ALPHA_SIZE, 5);
//     char data[] = {97, 98, 114, 97, 99, 97, 100, 97, 98, 114, 97}; // same as seq = "abracadabra"
//     string seq(data);
    
    ctw.learn(&dataVec);
    
    cout << "logeval : " << ctw.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << ctw.predict('c', "abra") << endl << endl;
    
//    double* prob;
//    for (int i = 0; i < 100; i++) {
//        prob = ctw.predictAll("abrac");
//        cout << prob[0] << endl;
//    }
//    ctw.learn(&dataVec);
//    for (int i = 0; i < 100; i++) {
//        prob = ctw.predictAll("abrac");
//        cout << prob[0] << endl;
//    }
#endif
#ifdef TEST_DCTW
    cout << "DCTW:" << endl;
    DCTWPredictor* dctw = NULL;
    for (int i = 0; i < 100; i++) {
        if (dctw)
            delete dctw;
        dctw = new DCTWPredictor();
        
        dctw->init(ALPHA_SIZE, 5);
        dctw->learn(&dataVec); //"abracadabra");
    }
//    dctw->learn("the quick brown fox jumped over the lazy dog.");
    
    cout << "logeval : " << dctw->logEval("cadabra") << endl;
    cout << "P(c|abra) : " << dctw->predict('c', "abra") << endl;
    
    cout << "P(c|dab) : " << dctw->predict('c', "dab") << endl;
    cout << "P(c|ra) : " << dctw->predict('c', "ra") << endl;
    cout << "P(c|bra) : " << dctw->predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << dctw->predict('c', "abcd") << endl;
    cout << "P(d|ra) : " << dctw->predict('d', "ra") << endl << endl;
    
//    cout << "P(z|the la) : " << dctw->predict('z', "the la") << endl;
//    cout << "P(y|the la) : " << dctw->predict('y', "the la") << endl;
//    cout << "P( |the la) : " << dctw->predict(' ', "the la") << endl;
    for (int i = 0; i < 100; i++) {
        if (prob)
            delete prob;
        prob = dctw->predictAll("abrac");
    }
    if (prob) {
        cout << "P(all) : " << prob[99] << endl << endl;
        delete prob; prob = NULL;
    }
    
    delete dctw;
//    cout << dctw->ModelToString() << endl; // this is not finished, for serializing and deserializing. It's a complex tree with lots of nodes!
#endif
#ifdef TEST_PPMC
    cout << "PPMC:" << endl;
    PPMCPredictor* ppmc = NULL;
    for (int i = 0; i < 100; i++) {
        if (ppmc)
            delete ppmc;
        ppmc = new PPMCPredictor();
        ppmc->init(ALPHA_SIZE, 5);
        
        ppmc->learn(&dataVec);
    }
    
    cout << "logeval : " << ppmc->logEval("cadabra") << endl;
//    cout << "logeval : " << ppmc.logEval("cadabra") << endl;
//    cout << "logeval : " << ppmc.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << ppmc->predict('c', "abra") << endl;
    cout << "P(c|dab) : " << ppmc->predict('c', "dab") << endl;
    cout << "P(c|ra) : " << ppmc->predict('c', "ra") << endl;
    cout << "P(c|bra) : " << ppmc->predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << ppmc->predict('c', "abcd") << endl;
    cout << "P(d|ra) : " << ppmc->predict('d', "ra") << endl << endl;
    
    for (int i = 0; i < 100; i++) {
        if (prob)
            delete prob;
        prob = ppmc->predictAll("abrac");
    }
    cout << "P(all) : " << prob[96] << endl;
    delete prob; prob = NULL;
    
#ifdef TEST_SERIALIZATION
    string model = ppmc->ModelToString();    // get the model encoded as an ascii string
    
    for (int i = 0; i < 100; i++) {
        if (ppmc)
            delete ppmc;
        ppmc = new PPMCPredictor();
        
        ppmc->init(ALPHA_SIZE,5);
        ppmc->ModelFromString(model);    // reconstitute the model from the ascii string!
    }
    
    // Could also feed the model in 1 line at a time:
//    ppmc.ModelFromString("n 97|1|3 [100|1|1 [97|1|1 [98|1|1 [114|1|1 [97|1|0]]]] {99|1|1 [97|1|1 [100|1|1 [97|1|1 [98|1|0]]]] {98|5|1 [114|4|1 [97|3|1 [99|2|1 [97|1|0]]]]}}]");
//    ppmc.ModelFromString("n 98|1|1 [114|1|1 [97|1|1 [99|1|1 [97|1|1 [100|1|0]]]]]");
//    ppmc.ModelFromString("n 99|1|1 [97|1|1 [100|1|1 [97|1|1 [98|1|1 [114|1|0]]]]]");
//    ppmc.ModelFromString("n 114|1|1 [97|1|1 [99|1|1 [97|1|1 [100|1|1 [97|1|0]]]]]");
    
    cout << "PPMC (reconstituted):" << endl;
    cout << "logeval : " << ppmc->logEval("cadabra") << endl;
    cout << "P(c|abra) : " << ppmc->predict('c', "abra") << endl;
    cout << "P(c|dab) : " << ppmc->predict('c', "dab") << endl;
    cout << "P(c|ra) : " << ppmc->predict('c', "ra") << endl;
    cout << "P(c|bra) : " << ppmc->predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << ppmc->predict('c', "abcd") << endl;
    cout << "P(d|ra) : " << ppmc->predict('d', "ra") << endl << endl;//
#endif // TEST_SERIALIZATION
    delete ppmc;
#endif // PPMC
#ifdef TEST_LZMS
    cout << "LZms: " << endl;
    LZmsPredictor lzms;
    
    lzms.init(ALPHA_SIZE, 4, 0);    // LZMS takes the ALPHA_SIZE for initialization, but by difinition the dictionary will expand if new characters/tokens are encountered. It's not a hard limit, like with the others.
    lzms.learn(&dataVec);
    
    cout << "logeval : " << lzms.logEval("cadabra") << endl;
    cout << "P(c|abra) : " << lzms.predict('c', "abra") << endl;
    cout << "P(c|dab) : " << lzms.predict('c', "dab") << endl;
    cout << "P(c|ra) : " << lzms.predict('c', "ra") << endl;
    cout << "P(c|bra) : " << lzms.predict('c', "bra") << endl;
    cout << "P(c|abcd) : " << lzms.predict('c', "abcd") << endl;
    cout << "P(d|ra) : " << lzms.predict('d', "ra") << endl << endl;
    
//    double* pred = lzms.predictAll("abra");
//    for (int i = 0; i < 256; i++) {
//        cout << pred[i] << ", ";
//    }
//    delete [] pred;
#endif
    return 0;
}
