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
#include <string>
#include <memory> // shared_ptr
// #include <bitset> // would like to use this, but size isn't known at compile time and can't be changed dynamically

#include "../../util/Context.hpp"
#include "DecompositionNode.hpp"
//#include "BinDLeaf.hpp"
#include "../ctw/DecompVolfNode.hpp"
#include "../ctw/VolfNode.hpp"

namespace vmm_algs_decomp {

    using namespace std;
    using namespace vmm_algs_ctw;
    
    typedef pair<const int, shared_ptr<DecompositionNode> > LeafNode;
    typedef map<LeafNode::first_type, LeafNode::second_type> LeafMap;    // leaves in our tree
    typedef pair<const int, shared_ptr<LeafMap> > TreeNode;
    typedef map<TreeNode::first_type, TreeNode::second_type> TreeMap;     // trunks in our tree
    
    class BitSet {
    protected:
        vector<bool> bits; //int *bits;
//        unsigned int numBits;
    public:
        BitSet();
        BitSet(unsigned int size);
        ~BitSet();
        
        void resize(unsigned int size);
        
        void OR(BitSet *other);
        void AND(BitSet *other);
        int Cardinality();
        bool equals(BitSet* other);
        void Set(int bit, bool val);
        bool Get(int bit);
        
        std::string toString();
        void fromString(std::string);
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
        DecompVolfNode softClasifier;
        BitSet descendants;
        vector<shared_ptr<AbsBinaryDNode> > children;
        
        // mySym, used primarily by BinDLeaf
        int mySym;

//        AbsBinaryDNode() {
//            myType = nodeType::AbsBinaryDNode;
//            softClasifier = NULL;
//            descendants = NULL;
//        }

    public:
        AbsBinaryDNode();
        AbsBinaryDNode(int abSize, shared_ptr<AbsBinaryDNode> rightChild, shared_ptr<AbsBinaryDNode> leftChild, int softModelDepth);
        
        virtual ~AbsBinaryDNode();
        
        void Init(int abSize, shared_ptr<AbsBinaryDNode> rightChild, shared_ptr<AbsBinaryDNode> leftChild, int softModelDepth);

        virtual double predict(int symbol, Context* context)=0; //

        BitSet* getDescendants();
//  int hashCode(){
//    return descendants->hashCode();
//  }

        bool equals(AbsBinaryDNode* obj);

        TreeMap* getDescendantsByLevel();
        
        int symbol();
        
        std::string toString();
        static AbsBinaryDNode* fromString(std::string data);
        
        static void DeleteTreeMap(TreeMap* tree);
        
    private:
        TreeMap* buildLevels(shared_ptr<AbsBinaryDNode> root, TreeMap* levelsMap, int level);

    };  // class AbsBinaryDNode
}   // namespace

#endif // _ABSBINARYDNODE_HPP
