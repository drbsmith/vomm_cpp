#include "../include/algs/pst/DefaultPSTNode.hpp"

#include <sstream>

using namespace vmm_algs_pst;

const string DefaultPSTNode::XML_NODE_ST = "<node>";
const string DefaultPSTNode::XML_NODE_ET = "</node>";
const string DefaultPSTNode::TOSTRING_INIT = "\n ";
const string DefaultPSTNode::XML_EXCEP_AND = "&";
const string DefaultPSTNode::XML_EXCEP_LT = "<";
const string DefaultPSTNode::XML_ID_CDATA_ST = "<id><![CDATA[";
const string DefaultPSTNode::XML_ID_CDATA_ET = "]]></id>";
const string DefaultPSTNode::XML_ID_ST = "<id>'";
const string DefaultPSTNode::XML_ID_ET = "'</id>";
const string DefaultPSTNode::XML_PROB_ST = "<probability>";
const string DefaultPSTNode::XML_PROB_ET = "</probability>";
const string DefaultPSTNode::XML_PROB_CDATA_ST = "<probability><![CDATA[";
const string DefaultPSTNode::XML_PROB_CDATA_ET = "]]></probability>";
const string DefaultPSTNode::XML_CHILDREN_ST ="<children>";
const string DefaultPSTNode::XML_CHILDREN_ET ="</children>";

DefaultPSTNode::DefaultPSTNode() {
    idStr = "";
    absize = ABSIZE;
    nextSymProbability = new vector<double>(ABSIZE);
    children  = new vector<DefaultPSTNode*>(ABSIZE);/*ZZZ*/
    isLeaf = true;
}

DefaultPSTNode::DefaultPSTNode(int alphabetSize) {
    idStr = "";
    absize = alphabetSize;
    nextSymProbability = new vector<double>(absize);
    children  = new vector<DefaultPSTNode*>(absize);
    isLeaf = true;
}
DefaultPSTNode::DefaultPSTNode(string _idStr, vector<double>* _nextSymProbability) {
    idStr = _idStr;
    nextSymProbability = _nextSymProbability;
    if (nextSymProbability)
        absize = (int)nextSymProbability->size();
    else
        absize = 0;
    children = new vector<DefaultPSTNode*>(absize);
}

DefaultPSTNode::~DefaultPSTNode() {
    if (nextSymProbability)
        delete nextSymProbability;
    if (children) {
        for (int i = 0; i < children->size(); i++)
            if (children->at(i))
                delete children->at(i);
        delete children;
    }
}

int DefaultPSTNode::getAlphabetSize(){
    return absize;
}

string DefaultPSTNode::getString(){
    return idStr;
}

double DefaultPSTNode::predict(int ch) {
    if (ch < nextSymProbability->size())
        return nextSymProbability->at(ch);
    else
        return -1;  // is this ok? Or should it be MAX_INT?
}

double DefaultPSTNode::predict(string charSeq){
    double prediction = 1;
    PSTNodeInterface* node;
    for (int i=0, test = (int)charSeq.size(); i<test; ++i){
        node = get(charSeq.substr(0,i)); // get(charSeq.subSequence(0,i));
        prediction *= node->predict((int)charSeq[i]);
    }
    return prediction;
}

double DefaultPSTNode::predict(Byte b){
    return predict((int)b);
}
void DefaultPSTNode::predict(vector<double> *pArr){
//    if (pArr->size() != absize) {
//        throw "ILL pArr size in predict";
//    }
    if (!pArr || !nextSymProbability)
        return;
    
    pArr->clear();
    pArr->insert(pArr->end(), nextSymProbability->begin(), nextSymProbability->end());
//    System.arraycopy(nextSymProbability,0,pArr,0,absize);
}

double DefaultPSTNode::predict(vector<Byte> *bytes){
    double prediction = 1;
    PSTNodeInterface* node;
//    System.out.println("-- prediction --");
    double p = 0;
    for(int i=0, test = (int)bytes->size(); i<test; ++i){
        node = get(bytes, i-1);
        int sym = (int)bytes->at(i);
        p = node->predict(sym);
        
        prediction *= p;
//        System.out.println(i+") "+p+" "+node.getString());
    }
//    System.out.println("--");
    return prediction;
}

/**
 * @returns PSTNodeInterface corresponds to the largest suffix of the byteSeq
 */
PSTNodeInterface* DefaultPSTNode::get(vector<Byte> *byteSeq){
    return get(byteSeq, (int)byteSeq->size()-1);
}

PSTNodeInterface* DefaultPSTNode::get(ContextIterator* context){
    if (context->hasNext()) {
        int symbol = context->nextSymbol();
        return (children->at(symbol) != NULL) ? children->at(symbol)->get(context) : this;
    }
    else{
        return (PSTNodeInterface*)this;
    }
}

PSTNodeInterface* DefaultPSTNode::get(string sbuff){
    int strLen = (int)sbuff.size();
    if (strLen == 0) {
        return (PSTNodeInterface*)this;
    }
    else{
        int nextSymIndex = sbuff[strLen-1];
        //            sbuff.setLength(strLen-1);    // this would take one character off the end?
        if (children->at(nextSymIndex) != NULL) {
            PSTNodeInterface* ret = children->at(nextSymIndex)->get(sbuff);
            //                sbuff.setLength(strLen);  // and then all cases will set it back to full size? why?
            return ret;
        } else {// therefore this corresponds to the largest suffix.
            //                sbuff.setLength(strLen);
            return this;
        }
    }
}

PSTNodeInterface* DefaultPSTNode::get(char symbol) {
    return (PSTNodeInterface*)children->at((short)symbol);
}

PSTNodeInterface* DefaultPSTNode::get(vector<Byte> *bytes, int currentByteIndex) {
    if (currentByteIndex == -1) {
        return this;
    } else {
        int nextSymIndex = (int)bytes->at(currentByteIndex);
        if (children->at(nextSymIndex) != NULL){
            PSTNodeInterface* ret = children->at(nextSymIndex)->get(bytes, currentByteIndex-1);
            return ret;
        } else {// therefore this corresponds to the largest suffix.
            return this;
        }
    }
}
void DefaultPSTNode::insert(char symbol, vector<double>* nextSymProbability){
    if (isLeaf) {
        isLeaf = false;
        if (children) {
            for (int i = 0; i < children->size(); i++)
                if (children->at(i))
                    delete children->at(i);
            delete children;
        }
        children = new vector<DefaultPSTNode*>(absize);
    }
    DefaultPSTNode* newNode = new DefaultPSTNode(symbol+idStr,nextSymProbability);
    children->at(symbol) = newNode;
}
/**
 * @return this node sub tree height
 */
int DefaultPSTNode::subTreeHeight() {
    int height = 0;
    if (isLeaf) {
        return 0;
    } else {
        for (int i = 0; i < children->size(); ++i) {
            if (children->at(i) != NULL) {
                int childH = children->at(i)->subTreeHeight();
                height = (height > childH ? height : childH);
            }
        }
        return height + 1;
    }
}
string DefaultPSTNode::toString() {
    stringstream ss;

//     toString.append(XML_NODE_ST);
//     if(idStr.indexOf(XML_EXCEP_AND)>=0||idStr.indexOf(XML_EXCEP_LT)>=0){
//        toString.append(XML_ID_CDATA_ST).append(idStr).append(XML_ID_CDATA_ET);
//     }
//     else{
//        toString.append(XML_ID_ST).append(idStr).append(XML_ID_ET);
//     }
    ss << idStr << " ";
    ss << prob2Str();
//     toString.append(XML_PROB_CDATA_ST).append(prob2Str()).append(XML_PROB_CDATA_ET);
//     toString.append(XML_CHILDREN_ST);

    for (int i=0; i < children->size(); ++i){
        if (children->at(i) != NULL){
            ss << " "; //            toString.append(tabs);
            ss << children->at(i)->toString(); //toString.append(children[i]);
        }
    }
//    toString.append(XML_CHILDREN_ET);
//    toString.append(XML_NODE_ET);
    return ss.str();
}
string DefaultPSTNode::prob2Str() {
    stringstream sbuff;
    sbuff << "[";
    for (int i = 0; i < nextSymProbability->size(); i++){
      if (nextSymProbability->at(i) > 0.001 ) {
          sbuff << (char)i << "=" << nextSymProbability->at(i) << ", "; // this will have weird effects if i is a special character (13 for example)
//        sbuff.append((char)i).append('=').append(nextSymProbability[i]).append(',');
      }
    }
    sbuff << "]";
    
    return sbuff.str();
}
