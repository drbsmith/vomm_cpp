#ifndef LZMSNODE_HPP
#define LZMSNODE_HPP

namespace vmm_algs_lzms {

    class LZmsNode {
    public:
        virtual ~LZmsNode() {}
        
        virtual int GetAlphabetSize() =0;

        // TODO: it wants to return the final leaf, and discard the container. Will they get cleaned up ok?
//        static LZNode* createBranch(LZNode* parent, std::vector<int>* symbols, int off, int len) =0;

        virtual LZmsNode* getChild(int symbol) =0;

        virtual void markEnd() =0;

        // when one creates a node it has alphabetSize leaves. For every leaf expanded it gains alphabetSize - 1 leaves.
        // if the counter had started at 0 we would have a total of: alphabetSize + counter*(alphabetSize - 1) leaves =
        // = alphabetSize - 1 + 1 + counter*(alphabetSize - 1) leaves = (counter + 1)*(alphabetSize - 1) + 1 leaves.
        // since we started the counter from 1 and not from 0, we get the following equation: counter * normalizedABSize + 1
        // aditionally there are endCounter leaves for paths that ended mid-Tree.
        // however, endCounter started from 1, and so contains the +1 from before.
        virtual void setLikelihoods() =0;

        virtual int getAlphabetSize() =0;

        virtual int getDepth() =0;

        virtual double getLogLikelihoodFromParent() =0;

        virtual double getLogLikelihoodToVirtualChild() =0;

        virtual LZmsNode* getParent() =0;

    private:
        virtual void increment(int value) =0;

        virtual void setNumLeaves() =0;

    };

}
#endif
