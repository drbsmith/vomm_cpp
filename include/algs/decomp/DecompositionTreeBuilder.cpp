
#include "DecompositionTreeBuilder.hpp"

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
DecompositionNode* DecompositionTreeBuilder::build(SampleIterator* sample) {
    TreeMap* countMap = calculateAbCount(sample);

    AbsBinaryDNode* root = buildHuf(countMap);
    delete countMap;

    TreeMap* levelsMap = root->getDescendantsByLevel();
    delete root;

    int level, sym1, sym2, superSym;
    LeafMap* oneLevel;
    AbsBinaryDNode *node1, *node2, *superNode;

    while ((levelsMap->size() > 1) ||
           ((levelsMap->size() == 1) &&
            (*levelsMap->begin()).second->size() > 1)) {

        TreeMap::iterator it = levelsMap->end(); --it;
        level = (*it).first;
        oneLevel = levelsMap->at(level);

        LeafMap::iterator it2 = oneLevel->end(); --it2;
        sym1 = (*it2).first; // (Integer)oneLevel.lastKey();
        node1 = (AbsBinaryDNode*)oneLevel->at(sym1); //.get(sym1);
        oneLevel->erase(sym1); //oneLevel.remove(sym1);
        it2 = oneLevel->end(); --it2;
        sym2 = (*it2).first; // (Integer)oneLevel.lastKey();
        node2 = (AbsBinaryDNode*)oneLevel->at(sym2);
        oneLevel->erase(sym2); //oneLevel.remove(sym2);
        if (oneLevel->size() == 0) {
            levelsMap->erase(level); //levelsMap.remove(level);
        }

        superNode = new DynamicBinDNode(absize, node1, node2, softModelDepth);

        if (node2->getType() == DecompositionNode::nodeType::BinDLeaf) { //} instanceof BinDLeaf){
            superSym = sym2;
        } else if (node1->getType() == DecompositionNode::nodeType::BinDLeaf) { //node1 instanceof BinDLeaf){
            superSym = sym1;
        } else {
            superSym = sym2;
        }

        level = level-1; // new Integer(level.intValue()-1);
        if (levelsMap->find(level) == levelsMap->end())
            levelsMap->insert(TreeNode(level, new LeafMap()));
        oneLevel = levelsMap->at(level);
        oneLevel->insert(LeafNode(superSym, superNode));
    }

    TreeMap::iterator it = levelsMap->end(); it--;
    oneLevel = (*it).second; // levelsMap->at(levelsMap.lastKey());

    LeafMap::iterator it2 = oneLevel->end(); it--;
    return (DecompositionNode*)(*it2).second; //oneLevel.get(oneLevel.lastKey());
}



/***/
/**
* build static
* (should never use code replication again..)
*/

StaticDecompositionNode* DecompositionTreeBuilder::buildStatic(SampleIterator* samples) {
    TreeMap* countMap = calculateAbCount(samples);

    AbsBinaryDNode* root = buildHuf(countMap);
    delete countMap;

    TreeMap* levelsMap = root->getDescendantsByLevel();
//        delete root;  // all of the tree is based on root. Deleting here erases the TreeMap. It appears that all nodes are then folded into the return node-tree

    int level, sym1, sym2, superSym;
    LeafMap* oneLevel;
    AbsBinaryDNode *node1, *node2, *superNode;

    while (levelsMap->size() > 1 ||
           ((levelsMap->size() == 1) &&
            levelsMap->begin()->second->size() > 1)) {
            
            TreeMap::iterator it = levelsMap->end(); --it;
            level = (*it).first;
            oneLevel = levelsMap->at(level);

            LeafMap::iterator it2 = oneLevel->end(); --it2;
            sym1 = it2->first; // (Integer)oneLevel.lastKey();
            node1 = (AbsBinaryDNode*)oneLevel->at(sym1); //.get(sym1);
            oneLevel->erase(sym1); //oneLevel.remove(sym1);
            it2 = oneLevel->end(); --it2;
            sym2 = it2->first; // (Integer)oneLevel.lastKey();
            node2 = (AbsBinaryDNode*)oneLevel->at(sym2);
            oneLevel->erase(sym2); //oneLevel.remove(sym2);
            if (oneLevel->size() == 0) {
                levelsMap->erase(level); //levelsMap.remove(level);
            }

            superNode = new StaticBinDNode(absize, node1, node2, softModelDepth);

            if (node2->getType() == DecompositionNode::nodeType::BinDLeaf) { //} instanceof BinDLeaf){
                superSym = sym2;
            } else if (node1->getType() == DecompositionNode::nodeType::BinDLeaf) { //} instanceof BinDLeaf){
                superSym = sym1;
            } else {
                superSym = sym2;
            }

            level = level-1; // new Integer(level.intValue()-1);
            if (levelsMap->find(level) == levelsMap->end())
                levelsMap->insert(TreeNode(level, new LeafMap()));
            oneLevel = levelsMap->at(level);
            oneLevel->insert(LeafNode(superSym, superNode));
        }
    
        TreeMap::iterator it = levelsMap->end();
        it--;
        oneLevel = it->second; // levelsMap->at(levelsMap.lastKey());

        LeafMap::iterator it2 = oneLevel->end();
        it2--;
        return (StaticDecompositionNode*)it2->second; //oneLevel.get(oneLevel.lastKey());
    }

/***/

    AbsBinaryDNode* DecompositionTreeBuilder::buildHuf(TreeMap* countMap) {
        LeafMap *lowCount = NULL;
        LeafMap *superCountVal = NULL;
        AbsBinaryDNode *node1 = NULL, *node2 = NULL;
        int count1, count2;
        DynamicBinDNode *dnode = NULL;
        int superSym, superCount;

        while (countMap->size() > 1 ||
              (countMap->begin()->second->size() > 1 &&
//                  ((((TreeMap)countMap.get(countMap.firstKey())).size()>1)&&
               countMap->size() == 1) ) {
            count1 = countMap->begin()->first; //(Integer)countMap.firstKey();
            lowCount = countMap->begin()->second; //countMap.get(countMap.firstKey());//lowest key @see SortedMap

            LeafMap::iterator it = lowCount->end(); it--;
            superSym = it->first; // (Integer)lowCount.lastKey();
            node1 = (AbsBinaryDNode*)it->second; // (AbsBinaryDNode)lowCount.get(lowCount.lastKey());
            lowCount->erase(superSym); //            lowCount.remove(lowCount.lastKey());

            if (lowCount->size() == 0) {
                countMap->erase(countMap->begin()); //countMap.remove(countMap.firstKey());
                lowCount = countMap->begin()->second; //lowCount = (TreeMap)countMap.get(countMap.firstKey());
            }
            count2 = countMap->begin()->first; //(Integer)countMap.firstKey();

            it = lowCount->end(); it--;
            node2 = (AbsBinaryDNode*)it->second; // (AbsBinaryDNode)lowCount.get(lowCount.lastKey());

            if (superSym < it->first) { //superSym.compareTo(lowCount.lastKey()) < 0) {
                superSym = it->first; // (Integer)lowCount.lastKey();
                swap(node2, node1);
            }

            lowCount->erase(it); //lowCount.remove(lowCount.lastKey());
            if (lowCount->size() == 0) {
                countMap->erase(countMap->begin()); //countMap.remove(countMap.firstKey());
            }

            dnode = new DynamicBinDNode(absize, node1, node2, softModelDepth);

            superCount = count1 + count2; //new Integer(count1.intValue()+count2.intValue());
            superCountVal = (countMap->find(superCount) != countMap->end()) ?
              countMap->at(superCount) : new LeafMap();

            superCountVal->insert(LeafNode(superSym, dnode));
            countMap->insert(TreeNode(superCount, superCountVal));
        }
        superCountVal = countMap->begin()->second; //(countMap.firstKey());
        
        return (AbsBinaryDNode*)superCountVal->begin()->second; //superCountVal.get(superCountVal.firstKey());
    }

/**
 *      calculateABCount()
 *      must return a sorted map. In c++ the map is always ascending by Key.
 */
TreeMap* DecompositionTreeBuilder::calculateAbCount(SampleIterator* sample){
    int *countArr = new int[absize];
    for (int i = 0; i < absize; i++)
        countArr[i] = 0;
    
    while (sample->hasNext()) {
        countArr[sample->next()]++;
    }

    TreeMap* countMap = new TreeMap();

    int sym, count;
    for( int i = 0; i < absize; i++){
        sym = i;
        count = countArr[i];
        if (countMap->find(count) == countMap->end()) {
            countMap->insert(TreeNode(count, new LeafMap()));
        }
        LeafMap* lm = countMap->at(count);
        lm->insert(LeafNode(sym, new BinDLeaf(sym, absize, softModelDepth)));
//            ((TreeMap)countMap.get(count)).put(sym, new BinDLeaf(sym.intValue(), absize));
    }
    
    delete[] countArr;

    return countMap;
}


void DecompositionTreeBuilder::swap(AbsBinaryDNode* n1, AbsBinaryDNode* n2) {
    AbsBinaryDNode* tmp = n1;
    n1 = n2;
    n2 = tmp;
}
