//
//  PSTBuilder.cpp
//  max-external
//
//  Created by Benjamin Smith on 6/1/18.
//

#include <stdio.h>
#include <algorithm>
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
//    if (pstRoot)  // this gets passed out to the build caller, they will free it.
//        delete pstRoot;
    if (seenAlphabet)
        delete seenAlphabet;
    if (samples)
        delete samples;
    
    for (int i = 0; i < suffStrNextSymProb.size(); i++)
        if (suffStrNextSymProb.at(i) != NULL) {
            delete suffStrNextSymProb.at(i);
        }
    for (int i = 0; i < queryStrs.size(); i++)
        if (queryStrs.at(i) != NULL)
            delete queryStrs.at(i);
}

PSTNodeInterface* PSTBuilder::build(Samples* _samples, double pMin, double alpha,
                        double nextSymProbMin, double addedValThreshold,
                        int strMaxLength){
    samples = _samples;
    init(pMin, nextSymProbMin);
    
    //start building the PST
    vector<int>* str;
    vector<double> strNSymProb; // = new vector<double>();
    vector<double>* suffStrNSymProb = NULL;
    
    while (queryStrs.size() > 0) {
        str = *(queryStrs.begin());
        suffStrNSymProb = *(suffStrNextSymProb.begin());

        initHitCounts(str); // 0 out the strCharHits and charStrHits vectors
        
        computeNextSymProb(&strNSymProb);

        if (isConditionB(&strNSymProb, suffStrNSymProb, alpha, nextSymProbMin, addedValThreshold)) {
            addToTree(str, &strNSymProb, nextSymProbMin);
        }
        if (str->size() < strMaxLength)
            updateQueryStrs(str, &strNSymProb, pMin);
        
        delete *queryStrs.begin();
        delete *suffStrNextSymProb.begin();
        
        queryStrs.erase(queryStrs.begin()); //0);
        suffStrNextSymProb.erase(suffStrNextSymProb.begin()); //.remove(0);
    }
    
    return pstRoot;
}

/** @ron 1/03
 * Golan's PST version.
 *
 */
PSTNodeInterface* PSTBuilder::buildGolanPST(Samples* _samples, int nMin, int minNumHits,
                                double nextSymProbMin, double r, int strMaxLength){
    samples = _samples;
    initGolan(nextSymProbMin);
    
    //start building the PST
    vector<int>* str;
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
        if (str->size() < strMaxLength)
            updateQueryStrsGolan(str, &strNSymProb, nMin);
        
        delete *queryStrs.begin();
        delete *suffStrNextSymProb.begin();
        
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
    vector<int>* str;
    for (int i = 0; i < alphabetSize; ++i) {
        prob->at(i) = (double)strCharHits[i]/(double)allLength;
        if (prob->at(i) > pMin) {
            str = new vector<int>;
            str->push_back(i);
            queryStrs.push_back(str); //(char)i);
//            suffStrNextSymProb.push_back(prob); // TODO: something funny here. The same vector gets added over and over, and gets changed in the loop. Would it make a copy in Java, so it's adding versions of the array? Or else why is the same thing added over and over?
        }
    }
    for (int i = 0; i < queryStrs.size(); i++) {
        vector<double> *p = new vector<double>;
        p->assign(prob->begin(), prob->end());
        suffStrNextSymProb.push_back(p);
    }
    
    vector<double> rootsProb; // = new vector<double>; //(alphabetSize);
    //    System.arraycopy(prob,0,rootsProb,0,alphabetSize);
    rootsProb.insert(rootsProb.end(), prob->begin(), prob->end());
    delete prob;    // has been copied for pstRoot to use.
    pstRoot = createPSTRoot(smooth(&rootsProb, nextSymProbMin)); // create holds on to rootsProb
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
    vector<int>* str;
    for(int i=0; i<alphabetSize; ++i){
        prob->at(i) = ((double)strCharHits[i])/allLength;
        str = new vector<int>();
        str->push_back(i);
        queryStrs.push_back(str); //(char)i);
        suffStrNextSymProb.push_back(prob);
    }
    
    vector<double> rootsProb; // = new vector<double>(alphabetSize);
    //    System.arraycopy(prob,0,rootsProb,0,alphabetSize);
    rootsProb.insert(rootsProb.end(), prob->begin(), prob->end());
    pstRoot = createPSTRoot(smooth(&rootsProb, nextSymProbMin));
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

void PSTBuilder::updateQueryStrsGolan(vector<int>* str, vector<double>* nextSymProb, int nMin){
    
    for(int i=0; i<alphabetSize; ++i){
        if( charStrHitsPerSample[i]> nMin){
            vector<int>* newStr = new vector<int>();
            newStr->push_back(i);
            newStr->insert(newStr->end(), str->begin(), str->end());
            queryStrs.push_back(newStr);
            suffStrNextSymProb.push_back(nextSymProb);
        }
    }
}


void PSTBuilder::updateQueryStrs(vector<int>* str, vector<double>* nextSymProb, double pMin){
    double allPossibleMatches = 0;
    
    if (!samples)
        return;
    
    int test = (int)samples->size();
    int chStrLen = (int)str->size() + 1;
    for (int i = 0; i < test; ++i) {
        allPossibleMatches += samples->size(i) - chStrLen + 1;
    }
    for (int i = 0; i < charStrHits.size(); ++i){
        if (charStrHits.at(i) / allPossibleMatches >= pMin) {
            vector<int>* newStr = new vector<int>();
            newStr->push_back(i);
            newStr->insert(newStr->end(), str->begin(), str->end());
            queryStrs.push_back( newStr );
            
            vector<double>* prob = new vector<double>();  // make a copy to store in the vector
            prob->assign(nextSymProb->begin(), nextSymProb->end());
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
    return new DefaultPSTNode(NULL, nextSymProb);
}

void PSTBuilder::addToTree(vector<int>* str, vector<double> *strNSymProb, double nextSymProbMin) {
    vector<double> prob; // = new vector<double>;
    PSTNodeInterface* deepestNode = pstRoot->get(str);
    
    if (!deepestNode)
        return;
    
    if (deepestNode->getIDString()->size() == str->size()-1) {
        prob.assign(strNSymProb->begin(), strNSymProb->end());
        deepestNode->insert(str->at(0), smooth(&prob, nextSymProbMin));
    }
    else{
        vector<int> savedStrChHits, savedChStrHits;
        savedStrChHits.assign(charStrHits.begin(), charStrHits.end());
        savedChStrHits.assign(strCharHits.begin(), strCharHits.end());
        
        for (int i = (int)str->size() - (int)(deepestNode->getIDString()->size()) - 1; i > -1; --i) {
            int sym = str->at(i);
            deepestNode = deepestNode->get(sym);
            
            if (deepestNode == NULL)
                break;  // this should never happen...because children should have been added in order
            vector<int> subStr;
            subStr.assign(str->begin()+i, str->end());
            initHitCounts(&subStr);    // get string from i to the end
            
            computeNextSymProb(&prob);
            smooth(&prob, nextSymProbMin);
            deepestNode->insert(str->at(i), &prob);
        }
        // delete prob; <-- do not delete! the vectors are used by 'deepestNode'!
        
        charStrHits.clear();
        charStrHits.assign(savedChStrHits.begin(), savedChStrHits.end());
        strCharHits.clear();
        strCharHits.assign(savedStrChHits.begin(), savedStrChHits.end());
    }
}

void PSTBuilder::initHitCounts(vector<int>* str){
    if (!samples)
        return; // error!
    //    Arrays.fill(strCharHits,0);
    //    Arrays.fill(charStrHits,0);
    
    std::fill(strCharHits.begin(), strCharHits.end(), 0);
    std::fill(charStrHits.begin(), charStrHits.end(), 0);
//    for (int i = 0; i < strCharHits.size(); i++) {
//        strCharHits[i] = 0;
//        charStrHits[i] = 0;   // these 2 vectors must be the same size, always
//    }
    strHits = 0;
    
    std::fill(charStrHitsPerSample.begin(), charStrHitsPerSample.end(), 0);
//    for (int i = 0; i < charStrHitsPerSample.size(); i++)
//        charStrHitsPerSample[i] = 0;
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
        int loopTest = sampleSize - (int)str->size();
        int j;
//        post("numOfSamples %d sample %d size %d loopTest %d sample %s", numOfSamples, sampleID, sampleSize, loopTest, str.c_str());
        for (int i = 0; i < loopTest; ++i) {
            for (j = 0; j < str->size(); ++j) {
                if (samples->get(0, i+j) != str->at(j)) // get sampleID, not used. just 0
                    break;
            }
            if (j == str->size()) {    /// we got to the end..
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
        return retVal;
    
//    retVal->resize(strCharHits.size()); // make sure we have enough entries, don't need to clear
    
    double strCharAll = 0; // get total hits in our vector
    for (int i=0; i < strCharHits.size(); ++i) {
        strCharAll += strCharHits.at(i);
    }
    
    retVal->clear();
    for (int i = 0; i < strCharHits.size(); ++i) {
        retVal->push_back(strCharHits.at(i)/strCharAll);
    }
    return retVal;
}
