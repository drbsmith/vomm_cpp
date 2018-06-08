#ifndef _ABSBINARYDNODE_HPP
#define _ABSBINARYDNODE_HPP
/* HEADER
If you use this code don’t forget to reference us :) BibTeX: http://www.cs.technion.ac.il/~rani/el-yaniv_bib.html#BegleiterEY04 

This code is free software; you can redistribute it and/or 
modify it under the terms of the GNU General Public License 
as published by the Free Software Foundation; either version 2 
of the License, or (at your option) any later version. 

This code is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
GNU General Public License (<a href="http://www.gnu.org/copyleft/gpl.html">GPL</a>) for more details.*/ 

#include <vector>
#include <map>
#include <cmath>
// #include <bitset> // would like to use this, but size isn't known at compile time and can't be changed dynamically

#include "../../util/Context.hpp"
#include "DecompositionNode.hpp"
//#include "BinDLeaf.hpp"
#include "../ctw/DecompVolfNode.hpp"
#include "../ctw/VolfNode.hpp"

namespace vmm_algs_decomp {

    using namespace std;
    using namespace vmm_algs_ctw;
    
    typedef pair<const int, DecompositionNode*> LeafNode;
    typedef map<LeafNode::first_type, LeafNode::second_type> LeafMap;    // leaves in our tree
    typedef pair<const int, LeafMap*> TreeNode;
    typedef map<TreeNode::first_type, TreeNode::second_type> TreeMap;     // trunks in our tree
    
    
    class BitSet {
    protected:
        vector<bool> bits; //int *bits;
//        unsigned int numBits;
    public:
        BitSet(unsigned int size) {
            bits.resize(size, 0);   // set all to false
        }
        ~BitSet() {
        }
        
        void OR(BitSet *other) {
            vector<bool>::iterator it2 = other->bits.begin();
            for (vector<bool>::iterator it = bits.begin(); it != bits.end() && it2 != other->bits.end(); it++, it2++) {
                *it = *it || *it2;
            }
//            bits = bits | other->bits;
        }
        void AND(BitSet *other) {
            vector<bool>::iterator it2 = other->bits.begin();
            for (vector<bool>::iterator it = bits.begin(); it != bits.end() && it2 != other->bits.end(); it++, it2++) {
                *it = *it && *it2;
            }
//            bits = bits & other->bits;
        }
        int Cardinality() {
            int card = 0;
            for (vector<bool>::iterator it = bits.begin(); it != bits.end(); it++) {
                if (*it)
                    card++;
            }
            return card;
        }
        bool equals(BitSet* other) {
            bool theSame = bits.size() == other->bits.size();
            
            vector<bool>::iterator it = bits.begin(), it2 = other->bits.begin();
            while (theSame && it != bits.end() && it2 != other->bits.end()) {
                theSame = *it == *it2;
                it++;
                it2++;
            }
            return theSame;
//            return numBits == other->numBits && bits == other->bits; // TODO: should check to see if all the values in each are the same
        }
        void Set(int bit, bool val) {
            if (bit >= 0 && bit < bits.size())
                bits.at(bit) = val;
            // else
            //      error!!! out of bounds
        }
        bool Get(int bit) {
            if (bit >= 0 && bit < bits.size())
                return bits.at(bit);
            else
                return 0;   // error!!!
        }
    };
/**
 * <p>Title: AbsDNode</p>
 * <p>Description: Abstract implementation for the DecompositionNode</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */
    class AbsBinaryDNode : public DecompositionNode {
    public:
        const static int RIGHT = 0;
        const static int LEFT  = 1;

  //private static final int SOFTCLASSIFIER_DEPTH = 16;
        const static int BRANCHING_FACTOR = 2;
    protected:
        DecompVolfNode *softClasifier;
        BitSet* descendants;
        vector<AbsBinaryDNode*> children;
        
        // mySym, used primarily by BinDLeaf
        int mySym;

//        AbsBinaryDNode() {
//            myType = nodeType::AbsBinaryDNode;
//            softClasifier = NULL;
//            descendants = NULL;
//        }

    public:
        virtual ~AbsBinaryDNode() {
            while (children.size() > 0) {
                delete *(children.begin());
                children.erase(children.begin());
            }
            if (softClasifier)
                delete softClasifier;
            if (descendants)
                delete descendants;
        }
        
        AbsBinaryDNode(int abSize, AbsBinaryDNode* rightChild, AbsBinaryDNode* leftChild, int softModelDepth) {
            myType = nodeType::AbsBinaryDNode;
            
            descendants = new BitSet(abSize);// all bits are initially false
            children.resize(BRANCHING_FACTOR);
            children[RIGHT] = rightChild;
            children[LEFT] = leftChild;

            // -- ASSERT --
            if (rightChild && leftChild) {
                BitSet test(abSize);
                test.OR(rightChild->descendants);
                test.AND(leftChild->descendants);
                
                if (test.Cardinality() != 0) {  // they can't be the same.
                    throw "ILL decomp node init: duplicated descendant";
                }
            }

            if (rightChild)
                descendants->OR(rightChild->descendants);
            if (leftChild)
                descendants->OR(leftChild->descendants);
            
            softClasifier = new DecompVolfNode();
            softClasifier->init(abSize, VolfNode::DEFAULT_ALPHA_FACTOR);
        }

        virtual double predict(int symbol, Context* context)=0; // 

        BitSet* getDescendants() {
            return descendants;
        }
//  int hashCode(){
//    return descendants->hashCode();
//  }

  bool equals(AbsBinaryDNode* obj) {
    try {
      return descendants->equals(obj->descendants);
    }
      catch (exception ex) { //}(ClassCastException cce){
          return false;
      }
  }

    TreeMap* getDescendantsByLevel() {
        TreeMap* levelsMap = new TreeMap();
        buildLevels(this, levelsMap, 0);
        return levelsMap;
    }
        
        int symbol() {
            return mySym;
        }
        
    private:
        void buildLevels(AbsBinaryDNode* root, TreeMap* levelsMap, int level){
            if (root->getType() == nodeType::BinDLeaf) { //root instanceof BinDLeaf){   // this is a true leaf, it doesn't have children to follow
                if (levelsMap->find(level) == levelsMap->end()) {
                    levelsMap->insert(TreeNode(level, new LeafMap()));
                }
                LeafMap* lm = levelsMap->at(level);
                lm->insert(LeafNode(root->symbol(), root));
                return;
            }
            buildLevels(root->children[LEFT], levelsMap, level+1);
            buildLevels(root->children[RIGHT], levelsMap, level+1);
        }

    };  // class AbsBinaryDNode
}   // namespace

#endif // _ABSBINARYDNODE_HPP
