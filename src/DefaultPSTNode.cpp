#include "../include/algs/pst/DefaultPSTNode.hpp"

#include <sstream>
//#include <iostream>

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
    absize = ABSIZE;
    nextSymProbability = new vector<double>(ABSIZE);
    children = new PSTChildren(); // new vector<DefaultPSTNode*>(ABSIZE);/*ZZZ*/
    isLeaf = true;
}

DefaultPSTNode::DefaultPSTNode(int alphabetSize) {
    absize = alphabetSize;
    nextSymProbability = new vector<double>(absize);
    children = new PSTChildren(); //new vector<DefaultPSTNode*>(absize);
    isLeaf = true;
}
DefaultPSTNode::DefaultPSTNode(vector<int>* _idStr, vector<double>* _nextSymProbability) {
    if (_idStr)
        idStr.assign(_idStr->begin(), _idStr->end());
    nextSymProbability = _nextSymProbability;
    if (nextSymProbability)
        absize = (int)nextSymProbability->size();
    else
        absize = 0;
    children = new PSTChildren(); //new vector<DefaultPSTNode*>(absize);
    isLeaf = true;
}

DefaultPSTNode::~DefaultPSTNode() {
    if (nextSymProbability)
        delete nextSymProbability;
    if (children) {
        for (auto const& x: *children) { //int i = 0; i < children->size(); i++)
            //if (children->at(i))
            delete x.second; //children->at(i)->second;
        }
        delete children;
    }
}

int DefaultPSTNode::getAlphabetSize(){
    return absize;
}

vector<int>* DefaultPSTNode::getIDString(){
    return &idStr;
}

double DefaultPSTNode::predict(int ch) {
    if (ch < nextSymProbability->size())
        return nextSymProbability->at(ch);
    else
        return -1;  // is this ok? Or should it be MAX_INT?
}

double DefaultPSTNode::predict(string charSeq){
    vector<int> buffer;
    for (string::size_type i = 0; i < charSeq.size(); i++)
        buffer.push_back((int)charSeq[i]);
    
    return predict(&buffer);
}
double DefaultPSTNode::predict(vector<int>* seq){
    double prediction = 1;
    PSTNodeInterface* node;
    for (int i=0, test = (int)seq->size(); i<test; ++i){
        vector<int> substr;
        substr.assign(seq->begin(), seq->begin()+i);
        node = get(&substr); //charSeq.substr(0,i)); // get(charSeq.subSequence(0,i));
        prediction *= node->predict(seq->at(i)); //(int)charSeq[i]);
    }
    return prediction;
}

double DefaultPSTNode::predict(Byte b){
    return predict((int)b);
}
vector<double>* DefaultPSTNode::predict(){
//    if (pArr->size() != absize) {
//        throw "ILL pArr size in predict";
//    }
    if (!nextSymProbability)
        return NULL;
    
    return nextSymProbability; // new vector<double>(*nextSymProbability);
//    pArr->clear();
//    pArr->assign(nextSymProbability->begin(), nextSymProbability->end());
//    pArr->insert(pArr->end(), nextSymProbability->begin(), nextSymProbability->end());
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
    if (!context || !children)
        return NULL;
    
    if (context->hasNext()) {
        int symbol = context->nextSymbol();
        PSTChildren::iterator node = children->find(symbol);
        if (node != children->end())
            return node->second->get(context);
        else
            return this;
//        return (children->at(symbol) != NULL) ? children->at(symbol)->get(context) : this;
    }
    else{
        return (PSTNodeInterface*)this;
    }
}

PSTNodeInterface* DefaultPSTNode::get(string sbuff){
    vector<int> buffer;
    for (string::size_type i = 0; i < sbuff.size(); i++)
        buffer.push_back((int)sbuff[i]);
    
    return get(&buffer);
//    int strLen = (int)sbuff.size();
//    if (strLen == 0) {
//        return (PSTNodeInterface*)this;
//    }
//    else{
//        int nextSymIndex = sbuff[strLen-1];
//        //            sbuff.setLength(strLen-1);    // this would take one character off the end?
//        PSTChildren::iterator node = children->find(nextSymIndex);
//        if (node != children->end()) { //children->at(nextSymIndex) != NULL) {
//            PSTNodeInterface* ret = node->second->get(sbuff); // children->at(nextSymIndex)->get(sbuff);
//            //                sbuff.setLength(strLen);  // and then all cases will set it back to full size? why?
//            return ret;
//        } else {// therefore this corresponds to the largest suffix.
//            //                sbuff.setLength(strLen);
//            return this;
//        }
//    }
}
PSTNodeInterface* DefaultPSTNode::get(vector<int>* sbuff){
    int strLen = (int)sbuff->size();
    if (strLen == 0) {
        return (PSTNodeInterface*)this;
    }
    else{
        int nextSymIndex = sbuff->at(strLen-1);
        //            sbuff.setLength(strLen-1);    // this would take one character off the end?
        PSTChildren::iterator node = children->find(nextSymIndex);
        if (node != children->end()) { //children->at(nextSymIndex) != NULL) {
            PSTNodeInterface* ret = node->second->get(sbuff); // children->at(nextSymIndex)->get(sbuff);
            //                sbuff.setLength(strLen);  // and then all cases will set it back to full size? why?
            return ret;
        } else {// therefore this corresponds to the largest suffix.
            //                sbuff.setLength(strLen);
            return this;
        }
    }
}

PSTNodeInterface* DefaultPSTNode::get(int symbol) {
    PSTChildren::iterator node = children->find(symbol);
    if (node != children->end())
        return node->second;
    else
        return NULL;
//    return (PSTNodeInterface*)children->at((short)symbol);
}

PSTNodeInterface* DefaultPSTNode::get(vector<Byte> *bytes, int currentByteIndex) {
    if (currentByteIndex == -1) {
        return this;
    } else {
        int nextSymIndex = (int)bytes->at(currentByteIndex);
        PSTChildren::iterator node = children->find(nextSymIndex);
        if (node != children->end()) { //children->at(nextSymIndex) != NULL){
            PSTNodeInterface* ret = node->second->get(bytes, currentByteIndex-1); // children->at(nextSymIndex)->get(bytes, currentByteIndex-1);
            return ret;
        } else {// therefore this corresponds to the largest suffix.
            return this;
        }
    }
}
void DefaultPSTNode::insert(int symbol, vector<double>* nextSymProbability){
    if (isLeaf) {
        isLeaf = false;
        if (children) { // should never be NULL, don't really need this check
            for (auto& x: *children) { //int i = 0; i < children->size(); i++)
                //if (children->at(i)) {
                delete x.second; // children->at(i).second;
//                    delete children->at(i);
            }
//            delete children;
            children->clear();
        }
//        children = new vector<DefaultPSTNode*>(absize);
    }
    vector<int> newIDStr;
    newIDStr.push_back(symbol);
    newIDStr.insert(newIDStr.end(), idStr.begin(), idStr.end());
    DefaultPSTNode* newNode = new DefaultPSTNode(&newIDStr, nextSymProbability);
    if (children != NULL)   // should never be NULL, but for safety
        children->insert(PSTNode(symbol, newNode));
    else
        delete newNode;
//    children->at(symbol) = newNode;
}
/**
 * @return this node sub tree height
 */
int DefaultPSTNode::subTreeHeight() {
    if (isLeaf) {
        return 0;
    } else {
        int height = 0;
        for (auto& x: *children) { //int i = 0; i < children->size(); ++i) {
            //if (children->at(i) != NULL) {
            int childH = x.second->subTreeHeight(); // children->at(i)->subTreeHeight();
            height = (height > childH ? height : childH);
            //}
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
    ss << XML_NODE_ST;
    ss << XML_ID_ST;
    for (int i = 0; i < idStr.size(); i++)
        ss << (int)idStr[i] << " ";
    ss << XML_ID_ET;
    ss << (isLeaf ? 1 : 0) << " " << absize << " ";
    ss << prob2Str();
//     toString.append(XML_PROB_CDATA_ST).append(prob2Str()).append(XML_PROB_CDATA_ET);
//     toString.append(XML_CHILDREN_ST);

    ss << XML_CHILDREN_ST;
    for (auto& x: *children) { //int i=0; i < children->size(); ++i){
        if (x.second != NULL) { //children->at(i) != NULL){
            ss << endl; //            toString.append(tabs);
            ss << x.second->toString(); // children->at(i)->toString(); //toString.append(children[i]);
        }
    }
    ss << XML_CHILDREN_ET;
//    toString.append(XML_CHILDREN_ET);
//    toString.append(XML_NODE_ET);
    ss << XML_NODE_ET;
    return ss.str();
}
DefaultPSTNode* DefaultPSTNode::FromString(string data) {    // reconstitute from serialized string
    if (data.size() < 2)
        return NULL;
    
    // replace all endl with spaces
    string::size_type index = 0;
    while (true) {
        /* Locate the substring to replace. */
        index = data.find("\n", index);
        if (index == std::string::npos) break;
        
        /* Make the replacement. */
        data.replace(index, 1, "");
        
        /* Advance index forward so the next iteration doesn't pick it up as well. */
        index += 1;
    }
    
    DefaultPSTNode* newNode = NULL;
    string::size_type sz;
    sz = data.find(XML_NODE_ST);
    data = data.substr(sz + XML_NODE_ST.size());
    
    index = data.find(XML_ID_ST) + XML_ID_ST.size();
    sz = data.find(XML_ID_ET);
    string idstr = data.substr(index, sz - index);
    data = data.substr(sz + XML_ID_ET.size());    // strip off id string
    
    // convert 'id' to characters
    index = 0;
    vector<int> id;
    try {
        while (idstr.size() > 0) {
            char i = stoi(idstr,&index);
            id.push_back(i);
            idstr.erase(0,index+1); // will be followed by a space, cut it off
        }
    } catch (invalid_argument) {}
    
    bool leaf = (stoi(data,&sz) == 1 ? true : false);
    data = data.substr(sz+1);
    int absize = stoi(data, &sz);
    data = data.substr(sz+1);
    
    
    string::size_type probEnd = data.find(XML_PROB_ET);
    vector<double>* prob = NULL;
    if (probEnd > 0) {
        prob = DefaultPSTNode::str2Prob(data.substr(0,probEnd), absize);
    }
    
    newNode = new DefaultPSTNode(&id, prob);
    newNode->isLeaf = leaf;
    
    data = data.substr(probEnd+XML_PROB_ET.size());  // take off probabilities and close brace
    
    // unpack children
    data = data.substr(XML_CHILDREN_ST.size()); // strip off open child tag
    if (data.find(XML_CHILDREN_ET) > 0) {
        while (data.find(XML_NODE_ST) < data.size()) {
            string child = GetOneNode(data); // pull off one child
            
            if (child == "")
                break;  // no child = corrupt data
            
            data = data.substr(child.size());        // strip off child string
            DefaultPSTNode* childNode = DefaultPSTNode::FromString(child);
            if (childNode != NULL) {
//                DefaultPSTNode* temp = newNode->children->at(childNode->idStr[0]);
//                if (newNode->children->find((int)childNode->idStr[0]) != newNode->children->end())
                // erase should do nothing if it can't find the key in question, so above check should be unneccessary
                newNode->children->erase((int)childNode->idStr[0]);
                newNode->children->insert(PSTNode((int)childNode->idStr[0], childNode)); //at(childNode->idStr[0]) = childNode;
//                delete temp;
            }
        }
    }
    
    return newNode;
}
string DefaultPSTNode::prob2Str() {
    stringstream sbuff;
    sbuff << XML_PROB_ST;
    for (int i = 0; i < nextSymProbability->size(); i++){
      if (nextSymProbability->at(i) > 0.0001 ) {
          sbuff << i << "=" << nextSymProbability->at(i) << " ";
//        sbuff.append((char)i).append('=').append(nextSymProbability[i]).append(',');
      }
    }
    sbuff << XML_PROB_ET;
    
    return sbuff.str();
}
vector<double>* DefaultPSTNode::str2Prob(string data, int absize) {
    if (data.size() < 5)
        return NULL;
    
    if (absize < 1)
        return NULL;
    
    vector<double>* ret = new vector<double>(absize);
    for (int i = 0; i < absize; i++)
        ret->at(i) = 0.0001;
    
    if (data.find(XML_PROB_ST) < data.size()) {   // it's a probability string
        data = data.substr(data.find(XML_PROB_ST)+XML_PROB_ST.size());  // remove brace
        std::vector<std::string> result;
        std::istringstream iss(data);
        for(std::string s; iss >> s; )
            result.push_back(s);    // split string into substrings with " " delimiter
        
        std::string::size_type sz;
        
        try {
            for (vector<string>::iterator it = result.begin(); it != result.end(); it++) {
                string s = (*it);
                if (s.size() >= 3) { // minimum size
                    int alpha = stoi(s, &sz);  // could throw errors if string is garbage
                    s = s.substr(sz+1);
                    double prob = stod(s, &sz);
                    
                    if (alpha >= 0 && alpha < absize)
                        ret->at(alpha) = prob;
                }
            }
        } catch (invalid_argument) {
        } catch (out_of_range) {
        }
    }
    return ret;
}

string DefaultPSTNode::GetOneNode(string data) {  // parse one node's worth of data from the string
    string::size_type start = data.find(XML_NODE_ST); // should be right at the start
    if (start > 5)    // sanity check
        return "";
    string::size_type index = XML_NODE_ST.size();
    
    int nested = 1; // how many levels in we are
    
    string::size_type close = 0, open = 0;
    while (nested > 0 && index <= data.size()) {
        close = data.find(XML_NODE_ET, index);  // find next instance of open/close tags
        open = data.find(XML_NODE_ST, index);
        
        if (open < close) {   // there's another open node tag before the next close
            nested++;
            index = open + XML_NODE_ST.size();
        } else {            // the close comes first, take nested down a level
            nested--;
            index = close + XML_NODE_ET.size();
        }
    }
    
    if (close > data.size())    // data is corrupt, this Node string is incomplete
        return "";
    
    // now index should contain the terminating character of the node
    return data.substr(start, index);
}
