//
//  PSTBuilder.cpp
//  max-external
//
//  Created by Benjamin Smith on 6/1/18.
//

#include <stdio.h>
#include "../include/algs/pst/PSTBuilder.hpp"

using namespace vmm_algs_pst;

PSTBuilder::PSTBuilder() : seenAlphabet(NULL), samples(NULL), pstRoot(NULL) {
    strHits = 0;
    alphabetSize = ALPHABET_RANGE;
    seenAlphabet = new bool[ALPHABET_RANGE];
    strCharHits.resize(ALPHABET_RANGE, 0);// = new int[ALPHABET_RANGE];
    charStrHits.resize(ALPHABET_RANGE, 0); // = new int[ALPHABET_RANGE];
    charStrHitsPerSample.resize(ALPHABET_RANGE, 0);
}

PSTBuilder::PSTBuilder(int abSize) : seenAlphabet(NULL), samples(NULL), pstRoot(NULL) {
    strHits = 0;
    alphabetSize = abSize;
    seenAlphabet = new bool[alphabetSize];
    //    strCharHits = new int[alphabetSize];
    //    charStrHits = new int[alphabetSize];
    strCharHits.resize(alphabetSize, 0);// = new int[ALPHABET_RANGE];
    charStrHits.resize(alphabetSize, 0); // = new int[ALPHABET_RANGE];
    charStrHitsPerSample.resize(alphabetSize, 0); // = new int[alphabetSize];
}

PSTBuilder::~PSTBuilder() {
    if (samples)
        delete samples;
//    if (pstRoot)  // this gets passed out to the build caller, they will free it.
//        delete pstRoot;
    for (int i = 0; i < suffStrNextSymProb.size(); i++)
        if (suffStrNextSymProb.at(i) != NULL) {
            delete suffStrNextSymProb.at(i);    //TODO: because the same vector is added many times it can't be deleted...something must be wrong there!
        }
    if (seenAlphabet)
        delete seenAlphabet;
}

PSTNodeInterface* PSTBuilder::build(Samples* _samples, double pMin, double alpha,
                        double nextSymProbMin, double addedValThreshold,
                        int strMaxLength){
    samples = _samples;
    init(pMin, nextSymProbMin);
    
    //start building the PST
    string str;
    vector<double> strNSymProb; // = new vector<double>();
    vector<double>* suffStrNSymProb = NULL;
    
    while (queryStrs.size() > 0) {
        str = *(queryStrs.begin());
        suffStrNSymProb = *(suffStrNextSymProb.begin());

        initHitCounts(str);
        
        computeNextSymProb(&strNSymProb);

        if (isConditionB(&strNSymProb, suffStrNSymProb, alpha, nextSymProbMin, addedValThreshold)) {
            addToTree(str, &strNSymProb, nextSymProbMin);
        }
        if (str.length() < strMaxLength)
            updateQueryStrs(str, &strNSymProb, pMin);
        
        queryStrs.erase(queryStrs.begin()); //0);
        suffStrNextSymProb.erase(suffStrNextSymProb.begin()); //.remove(0);
    }
    
    return pstRoot;
}

/** @ron 1/03
 * Golan's PST version.
 *
 * @param samples
 * @param pMin
 * @param alpha
 * @param nextSymProbMin
 * @param addedValThreshold
 * @param strMaxLength
 * @return
 */
PSTNodeInterface* PSTBuilder::buildGolanPST(Samples* _samples, int nMin, int minNumHits,
                                double nextSymProbMin, double r, int strMaxLength){
    samples = _samples;
    initGolan(nextSymProbMin);
    
    //start building the PST
    string str;
    vector<double> strNSymProb;
    vector<double>* suffStrNSymProb;
    
    while (queryStrs.size() > 0) {
        str = *queryStrs.begin();
        suffStrNSymProb = *suffStrNextSymProb.begin();
        
        initHitCounts(str);
        computeNextSymProb(&strNSymProb);
        
        if (isConditionBGolan(&strNSymProb, suffStrNSymProb, minNumHits, r)) {
            addToTree(str, &strNSymProb, nextSymProbMin);
        }
        if (str.length() < strMaxLength)
            updateQueryStrsGolan(str, &strNSymProb, nMin);
        
        queryStrs.erase(queryStrs.begin()); //0);
        suffStrNextSymProb.erase(suffStrNextSymProb.begin()); //.remove(0);
    }
    
    return pstRoot;
}

void PSTBuilder::init(double pMin, double nextSymProbMin){
    if ( nextSymProbMin*alphabetSize>1) {
        throw "ILL - smooth :  gamma*|alphabet|>1";
    }
    //seperated to save object creation.
    //    queryStrs = new ArrayList(S_INITIAL_SIZE);
    //    suffStrNextSymProb = new ArrayList(S_INITIAL_SIZE);
//    queryStrs.resize(S_INITIAL_SIZE);
//    suffStrNextSymProb.resize(S_INITIAL_SIZE);
    
    int numOfSamples = (int)samples->size();
    int allLength = (int)samples->allLength();
    
    if (allLength < 1)
        return;
    
    strCharHits.resize(alphabetSize, 0);    //     Arrays.fill(strCharHits,0);
    
    for(int val = 0,sampleSize=0,sampleID=0; sampleID<numOfSamples; ++sampleID){
        sampleSize = (int)samples->size(sampleID);
        for(int i=0; i<sampleSize;++i){
            val = samples->get(sampleID,i); //&UNSIGNED_BYTE_MASK;
            if (val >= 0 && val < alphabetSize) {
                seenAlphabet[val] = true;
                strCharHits[val]++;
            }
        }
    }
    
    //init seenALPHABET & queryStrs
    vector<double> *prob = new vector<double>(alphabetSize, 0); // double[alphabetSize];
    stringstream ss;
    for (int i = 0; i < alphabetSize; ++i) {
        prob->at(i) = (double)strCharHits[i]/(double)allLength;
        if (prob->at(i) > pMin) {
            ss.str("");
            ss << (char)i;
            queryStrs.push_back(ss.str()); //(char)i);
//            suffStrNextSymProb.push_back(prob); // TODO: something funny here. The same vector gets added over and over, and gets changed in the loop. Would it make a copy in Java, so it's adding versions of the array? Or else why is the same thing added over and over?
        }
    }
    for (int i = 0; i < queryStrs.size(); i++) {
        vector<double> *p = new vector<double>;
        p->insert(p->end(), prob->begin(), prob->end());
        suffStrNextSymProb.push_back(p);
    }
    
    vector<double>* rootsProb = new vector<double>; //(alphabetSize);
    //    System.arraycopy(prob,0,rootsProb,0,alphabetSize);
    rootsProb->insert(rootsProb->end(), prob->begin(), prob->end());
    delete prob;    // has been copied for pstRoot to use.
    rootsProb = smooth(rootsProb, nextSymProbMin);
    pstRoot = createPSTRoot(rootsProb); // create holds on to rootsProb
}

void PSTBuilder::initGolan(double nextSymProbMin){
    if(nextSymProbMin*alphabetSize>1){
        throw "ILL - smooth :  gamma*|alphabet|>1";
    }
    
    //seperated to save object creation.
//    queryStrs.resize(S_INITIAL_SIZE);
//    suffStrNextSymProb.resize(S_INITIAL_SIZE);
    //      queryStrs = new ArrayList(S_INITIAL_SIZE);
    //      suffStrNextSymProb = new ArrayList(S_INITIAL_SIZE);
    
    int numOfSamples = (int)samples->size();
    int allLength = (int)samples->allLength();
    
    //      Arrays.fill(strCharHits,0);
    strCharHits.resize(alphabetSize, 0);
    
    int val = 0, sampleSize = 0;
    for (int sampleID = 0; sampleID < numOfSamples; ++sampleID){
        sampleSize = (int)samples->size(sampleID);    // get size of this sample
        for(int i=0; i<sampleSize;++i){
            val = samples->get(sampleID,i)&UNSIGNED_BYTE_MASK;
            seenAlphabet[val] = true;
            strCharHits[val]++;
        }
    }
    
    //init seenALPHABET & queryStrs
    vector<double> *prob = new vector<double>; //(alphabetSize);
    stringstream ss;
    for(int i=0; i<alphabetSize; ++i){
        prob->at(i) = ((double)strCharHits[i])/allLength;
        ss.str("");
        ss << i;
        queryStrs.push_back(ss.str()); //(char)i);
        suffStrNextSymProb.push_back(prob);
    }
    
    vector<double>* rootsProb = new vector<double>(alphabetSize);
    //    System.arraycopy(prob,0,rootsProb,0,alphabetSize);
    rootsProb->insert(rootsProb->end(), prob->begin(), prob->end());
    pstRoot = createPSTRoot(smooth(rootsProb, nextSymProbMin));
}

// StrNSymProb and suffStrNSymProb MUST be the same size!!!!
bool PSTBuilder::isConditionB(vector<double> *StrNSymProb, vector<double> *suffStrNSymProb,
                     double alpha, double nextSymProbMin, double addedValThreshold) {
    if (!StrNSymProb || !suffStrNSymProb) {
        // ("error in PSTBuilder::isConditionB. vector is null. %d %d", StrNSymProb, suffStrNSymProb);
        return false;
    }
    if (StrNSymProb->size() != suffStrNSymProb->size()) {
        // ("error in PSTBuilder::isConditionB. vector sizes do not match. %d != %d", StrNSymProb->size(), suffStrNSymProb->size());
        return false;
    }
    
    double factor = 0;
    for (int i = 0; i < StrNSymProb->size() /*alphabetSize*/; i++){
        if (StrNSymProb->at(i) >= (1 + alpha) * nextSymProbMin) {
            factor = StrNSymProb->at(i) / suffStrNSymProb->at(i);
            if ((factor >= addedValThreshold) || (factor <= 1 / addedValThreshold)) {
                return true;
            }
        }
    }
    return false;
}

bool PSTBuilder::isConditionBGolan(vector<double> *StrNSymProb, vector<double> *suffStrNSymProb, int numHits, double r){
    double factor = 0;
    for (int i=0; i<alphabetSize; ++i) {
        if (strCharHits[i] > numHits){
            factor = StrNSymProb->at(i) / suffStrNSymProb->at(i);
            if ((factor >= r) || (factor <= 1/r)) {
                return true;
            }
        }
    }
    return false;
}

void PSTBuilder::updateQueryStrsGolan(string str, vector<double>* nextSymProb, int nMin){
    
    for(int i=0; i<alphabetSize; ++i){
        if( charStrHitsPerSample[i]> nMin){
            queryStrs.push_back(((char)i)+str);
            suffStrNextSymProb.push_back(nextSymProb);
        }
    }
}


void PSTBuilder::updateQueryStrs(string str, vector<double>* nextSymProb, double pMin){
    double allPossibleMatches = 0;
    
    if (!samples)
        return;
    
    int test = (int)samples->size();
    int chStrLen = (int)str.size() + 1;
    for (int i = 0; i < test; ++i) {
        allPossibleMatches += samples->size(i) - chStrLen + 1;
    }
    for (int i = 0; i < charStrHits.size(); ++i){
        if (charStrHits.at(i) / allPossibleMatches >= pMin) {
            queryStrs.push_back( ((char)i) + str );
            
            vector<double>* prob = new vector<double>;  // make a copy to store in the vector
            prob->insert(prob->end(), nextSymProb->begin(), nextSymProb->end());
            suffStrNextSymProb.push_back(prob);
        }
    }
}

vector<double>* PSTBuilder::smooth(vector<double> *prob, double nsMinP){
    double factor = 1-prob->size() * nsMinP;
    for(int i=0; i < prob->size(); i++){
        //if(seenAlphabet[i]){
        prob->at(i) = factor * prob->at(i) + nsMinP;
        //}
    }
    return prob;
}

PSTNodeInterface* PSTBuilder::createPSTRoot(vector<double>* nextSymProb){
    return new DefaultPSTNode("", nextSymProb);
}

void PSTBuilder::addToTree(string str, vector<double> *strNSymProb, double nextSymProbMin) {
    PSTNodeInterface* deepestNode = pstRoot->get(str);
    
    if (!deepestNode)
        return;
    
    if (deepestNode->getString().size() == str.size()-1) {
        vector<double>* prob = new vector<double>;
        prob->insert(prob->end(), strNSymProb->begin(), strNSymProb->end());  // make our own copy to store
        deepestNode->insert(str[0], smooth(prob, nextSymProbMin));
    }
    else{
        vector<int> savedStrChHits, savedChStrHits;
        savedStrChHits.insert(savedStrChHits.end(), charStrHits.begin(), charStrHits.end());
        savedChStrHits.insert(savedChStrHits.end(), strCharHits.begin(), strCharHits.end());
        //        int *savedStrChHits = new int[strCharHits.length];
        //        int *savedChStrHits = new int[charStrHits.length];
        //      System.arraycopy(charStrHits,0,savedChStrHits,0,charStrHits.length);
        //      System.arraycopy(strCharHits,0,savedStrChHits,0,strCharHits.length);
        
        for (int i = (int)str.size() - (int)(deepestNode->getString().size()) - 1; i > -1; deepestNode = deepestNode->get(str[i]), --i) {
            initHitCounts(str.substr(i,str.size()));    // get string from i to the end
            vector<double> *prob = new vector<double>();
            prob = computeNextSymProb(prob);
            if (prob)
                deepestNode->insert(str[i], smooth(prob, nextSymProbMin));
        }
        // delete prob; <-- do not delete! the vectors are used by 'deepestNode'!
        
        charStrHits.clear();
        charStrHits.insert(charStrHits.end(), savedChStrHits.begin(), savedChStrHits.end());
        strCharHits.clear();
        strCharHits.insert(strCharHits.end(), savedStrChHits.begin(), savedStrChHits.end());
        //          System.arraycopy(savedChStrHits,0,charStrHits,0,charStrHits.length);    // restoring saved hits
        //          System.arraycopy(savedStrChHits,0,strCharHits,0,strCharHits.length);
    }
}

void PSTBuilder::initHitCounts(string str){
    if (!samples)
        return; // error!
    //    Arrays.fill(strCharHits,0);
    //    Arrays.fill(charStrHits,0);
    for (int i = 0; i < strCharHits.size(); i++) {
        strCharHits[i] = 0;
        charStrHits[i] = 0;   // these 2 vectors must be the same size, always
    }
    strHits = 0;
    
    for (int i = 0; i < charStrHitsPerSample.size(); i++)
        charStrHitsPerSample[i] = 0;
    //    Arrays.fill(charStrHitsPerSample, 0);
    vector<bool> isUpdatePerSample(charStrHitsPerSample.size(), 0);
    //    bool isUpdatePerSample[] = new bool[charStrHitsPerSample.length];
    
    //    byte []strBytes = str.getBytes();
    int numOfSamples = (int)samples->size();
    for (int sampleID=0; sampleID < numOfSamples; sampleID++) {
        for (int i = 0; i < isUpdatePerSample.size(); i++)
            isUpdatePerSample[i] = true;
        //      Arrays.fill(isUpdatePerSample, true); /*@ron 1/03*/
        int sampleSize = (int)samples->size(sampleID);
        int loopTest = sampleSize - (int)str.size();
        int j;
//        post("numOfSamples %d sample %d size %d loopTest %d sample %s", numOfSamples, sampleID, sampleSize, loopTest, str.c_str());
        for (int i = 0; i < loopTest; ++i) {
            for (j = 0; j < str.size(); ++j) {
                if (samples->get(sampleID, i+j) != str[j])
                    break;
            }
            if (j == str.size()) {    /// we got to the end..
                strHits++;
                if (i+j < sampleSize) {
                    int charId = samples->get(sampleID, i+j);
                    if (charId >= 0 && charId < strCharHits.size())
                        strCharHits.at(charId)++; //&UNSIGNED_BYTE_MASK]++;
                }
                if (i > 0) {
                    int charId = samples->get(sampleID, i - 1); // & UNSIGNED_BYTE_MASK;
                    if (charId >= 0) {
                        if (charId < charStrHits.size())
                            charStrHits[charId]++;
                        if (charId < isUpdatePerSample.size() && isUpdatePerSample[charId]) {
                            isUpdatePerSample[charId] = false;
                            if (charId < charStrHitsPerSample.size())
                                charStrHitsPerSample[charId]++;
                        }
                    }
                }
            } // if
        } // for i
    } // for sampleID
} // initHitCounts()

// Assigns retVal, caller must delete it!
vector<double>* PSTBuilder::computeNextSymProb(vector<double>* retVal) {
    if (strCharHits.size() < 1 || !retVal)
        return NULL;
    
    retVal->resize(strCharHits.size()); // make sure we have enough entries, don't need to clear
    
    double strCharAll = 0; // get total hits in our vector
    for (int i=0; i < strCharHits.size(); ++i) {
        strCharAll += strCharHits.at(i);
    }
    for (int i = 0; i < retVal->size(); ++i) {
        retVal->at(i) = (double)(strCharHits[i]/strCharAll);
    }
    return retVal;
}
