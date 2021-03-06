
#include "../include/algs/decomp/DecompositionTreeBuilder.hpp"

using namespace vmm_algs_decomp;

/**
* <p>Title: DecompositionTreeBuilder</p>
* <p>Description: Builds a Decomposition Tree from Samples</p>
* <p>Copyright: Copyright (c) 2003</p>
* <p>Company: </p>
* @author ron
* @version 1.0
*/

DecompositionTreeBuilder::DecompositionTreeBuilder(int _absize, int _softModelDepth) {
    absize = _absize;
    softModelDepth = _softModelDepth;
}

/**
* Build dynamic
*/
shared_ptr<DecompositionNode> DecompositionTreeBuilder::build(SampleIterator* sample) {
    TreeMap* countMap = calculateAbCount(sample);

    shared_ptr<AbsBinaryDNode> root = buildHuf(countMap);
    AbsBinaryDNode::DeleteTreeMap(countMap);

    TreeMap* levelsMap = root->getDescendantsByLevel();

    int level, sym1, sym2, superSym;
    shared_ptr<LeafMap> oneLevel;
    shared_ptr<AbsBinaryDNode> node1, node2;
    shared_ptr<DynamicBinDNode> superNode;

    while ((levelsMap->size() > 1) ||
           ((levelsMap->size() == 1) &&
            (*levelsMap->begin()).second->size() > 1)) {

        TreeMap::iterator it = levelsMap->end(); --it;
        level = (*it).first;
        oneLevel = levelsMap->at(level);

        LeafMap::iterator it2 = oneLevel->end(); --it2;
        sym1 = (*it2).first; // (Integer)oneLevel.lastKey();
        node1 = static_pointer_cast<AbsBinaryDNode>(oneLevel->at(sym1)); //.get(sym1);
        oneLevel->erase(sym1); //oneLevel.remove(sym1);
        it2 = oneLevel->end(); --it2;
        sym2 = (*it2).first; // (Integer)oneLevel.lastKey();
        node2 = static_pointer_cast<AbsBinaryDNode>(oneLevel->at(sym2));
        oneLevel->erase(sym2); //oneLevel.remove(sym2);
        if (oneLevel->size() == 0) {
            levelsMap->erase(level); //levelsMap.remove(level);
        }

               superNode = std::make_shared<DynamicBinDNode>();
               superNode->Init(absize, node1, node2, softModelDepth);

        if (node2->getType() == DecompositionNode::nodeType::BinDLeaf) { //} instanceof BinDLeaf){
            superSym = sym2;
        } else if (node1->getType() == DecompositionNode::nodeType::BinDLeaf) { //node1 instanceof BinDLeaf){
            superSym = sym1;
        } else {
            superSym = sym2;
        }

        level = level-1; // new Integer(level.intValue()-1);
        if (levelsMap->find(level) == levelsMap->end())
            levelsMap->insert(TreeNode(level, make_shared<LeafMap>()));
        oneLevel = levelsMap->at(level);
        oneLevel->insert(LeafNode(superSym, superNode));
    }

    TreeMap::iterator it = levelsMap->end(); it--;
    oneLevel = (*it).second; // levelsMap->at(levelsMap.lastKey());

    LeafMap::iterator it2 = oneLevel->end(); it--;
    
    shared_ptr<DecompositionNode> ret = static_pointer_cast<DecompositionNode>((*it2).second);
    
    AbsBinaryDNode::DeleteTreeMap(levelsMap);
//    delete root;
    return ret; //oneLevel.get(oneLevel.lastKey());
}



/***/
/**
* build static
* (should never use code replication again..)
*/

shared_ptr<StaticDecompositionNode> DecompositionTreeBuilder::buildStatic(SampleIterator* samples) {
    TreeMap* countMap = calculateAbCount(samples);

    shared_ptr<AbsBinaryDNode> root = buildHuf(countMap);
    AbsBinaryDNode::DeleteTreeMap(countMap);

    TreeMap* levelsMap = root->getDescendantsByLevel();
//        delete root;  // all of the tree is based on root. Deleting here erases the TreeMap. It appears that all nodes are then folded into the return node-tree

    int level, sym1, sym2, superSym;
    shared_ptr<LeafMap> oneLevel;
    shared_ptr<AbsBinaryDNode> node1, node2;
    shared_ptr<StaticBinDNode> superNode;

    while (levelsMap->size() > 1 ||
           ((levelsMap->size() == 1) &&
            levelsMap->begin()->second->size() > 1)) {
            
        TreeMap::iterator it = levelsMap->end(); --it;
        level = (*it).first;
        oneLevel = levelsMap->at(level);

        LeafMap::iterator it2 = oneLevel->end(); --it2;
        sym1 = it2->first; // (Integer)oneLevel.lastKey();
        node1 = static_pointer_cast<AbsBinaryDNode>(oneLevel->at(sym1)); //.get(sym1);
        oneLevel->erase(sym1); //oneLevel.remove(sym1);
        it2 = oneLevel->end(); --it2;
        sym2 = it2->first; // (Integer)oneLevel.lastKey();
        node2 = static_pointer_cast<AbsBinaryDNode>(oneLevel->at(sym2));
        oneLevel->erase(sym2); //oneLevel.remove(sym2);
        if (oneLevel->size() == 0) {
            levelsMap->erase(level); //levelsMap.remove(level);
        }

               superNode = make_shared<StaticBinDNode>();
               superNode->Init(absize, node1, node2, softModelDepth);

        if (node2->getType() == DecompositionNode::nodeType::BinDLeaf) { //} instanceof BinDLeaf){
            superSym = sym2;
        } else if (node1->getType() == DecompositionNode::nodeType::BinDLeaf) { //} instanceof BinDLeaf){
            superSym = sym1;
        } else {
            superSym = sym2;
        }

        level = level-1; // new Integer(level.intValue()-1);
        if (levelsMap->find(level) == levelsMap->end())
            levelsMap->insert(TreeNode(level, make_shared<LeafMap>()));
        oneLevel = levelsMap->at(level);
        oneLevel->insert(LeafNode(superSym, superNode));
    }

    TreeMap::iterator it = levelsMap->end();
    it--;
    oneLevel = it->second; // levelsMap->at(levelsMap.lastKey());

    LeafMap::iterator it2 = oneLevel->end();
    it2--;
    shared_ptr<StaticDecompositionNode> ret = static_pointer_cast<StaticDecompositionNode>(it2->second);
    
    AbsBinaryDNode::DeleteTreeMap(levelsMap);
    
    return ret; //oneLevel.get(oneLevel.lastKey());
}

/***/

shared_ptr<AbsBinaryDNode> DecompositionTreeBuilder::buildHuf(TreeMap* countMap) {
    shared_ptr<LeafMap> lowCount, superCountVal;
    shared_ptr<AbsBinaryDNode> node1, node2;
    shared_ptr<DynamicBinDNode> dnode;
    int count1, count2;
//    DynamicBinDNode *dnode = NULL;
    int superSym, superCount;

    while (countMap->size() > 1 ||
          (countMap->begin()->second->size() > 1 &&
//                  ((((TreeMap)countMap.get(countMap.firstKey())).size()>1)&&
           countMap->size() == 1) ) {
        count1 = countMap->begin()->first; //(Integer)countMap.firstKey();
        lowCount = countMap->begin()->second; //countMap.get(countMap.firstKey());//lowest key @see SortedMap

        LeafMap::iterator it = lowCount->end(); it--;
        superSym = it->first; // (Integer)lowCount.lastKey();
        node1 = static_pointer_cast<AbsBinaryDNode>(it->second); // (AbsBinaryDNode)lowCount.get(lowCount.lastKey());
        lowCount->erase(superSym); //            lowCount.remove(lowCount.lastKey());

        if (lowCount->size() == 0) {
            countMap->erase(countMap->begin()); //countMap.remove(countMap.firstKey());
            lowCount = countMap->begin()->second; //lowCount = (TreeMap)countMap.get(countMap.firstKey());
        }
        count2 = countMap->begin()->first; //(Integer)countMap.firstKey();

        it = lowCount->end(); it--;
        node2 = static_pointer_cast<AbsBinaryDNode>(it->second); // (AbsBinaryDNode)lowCount.get(lowCount.lastKey());

        if (superSym < it->first) { //superSym.compareTo(lowCount.lastKey()) < 0) {
            superSym = it->first; // (Integer)lowCount.lastKey();
            shared_ptr<AbsBinaryDNode> tmp = node1;
            node1 = node2; node2 = tmp;
//                swap(node2, node1);
        }

        lowCount->erase(it); //lowCount.remove(lowCount.lastKey());
        if (lowCount->size() == 0) {
            countMap->erase(countMap->begin()); //countMap.remove(countMap.firstKey());
        }

              dnode = make_shared<DynamicBinDNode>();
              dnode->Init(absize, node1, node2, softModelDepth);

        superCount = count1 + count2; //new Integer(count1.intValue()+count2.intValue());
        superCountVal = (countMap->find(superCount) != countMap->end()) ?
          countMap->at(superCount) : make_shared<LeafMap>();

        superCountVal->insert(LeafNode(superSym, dnode));
        countMap->insert(TreeNode(superCount, superCountVal));
    }
    superCountVal = countMap->begin()->second; //(countMap.firstKey());
    
    return static_pointer_cast<AbsBinaryDNode>(superCountVal->begin()->second); //superCountVal.get(superCountVal.firstKey());
}

/**
 *      calculateABCount()
 *      must return a sorted map. In c++ the map is always ascending by Key.
 */
TreeMap* DecompositionTreeBuilder::calculateAbCount(SampleIterator* sample){
    vector<int> countArr(absize, 0);
//    int *countArr = new int[absize];
//    for (int i = 0; i < absize; i++)
//        countArr[i] = 0;
    
    while (sample->hasNext()) {
        countArr[sample->next()]++;
    }

    TreeMap* countMap = new TreeMap();

    int sym, count;
    for( int i = 0; i < absize; i++){
        sym = i;
        count = countArr[i];
        if (countMap->find(count) == countMap->end()) {
            countMap->insert(TreeNode(count, make_shared<LeafMap>()));
        }
        shared_ptr<LeafMap> lm = countMap->at(count);
        auto binleaf = make_shared<BinDLeaf>();
        binleaf->Init(sym, absize, softModelDepth);
        lm->insert(LeafNode(sym, binleaf));
//            ((TreeMap)countMap.get(count)).put(sym, new BinDLeaf(sym.intValue(), absize));
    }
    
//    delete[] countArr;

    return countMap;
}


//void DecompositionTreeBuilder::swap(AbsBinaryDNode* n1, AbsBinaryDNode* n2) {
//    AbsBinaryDNode* tmp = n1;
//    n1 = n2;
//    n2 = tmp;
//}
