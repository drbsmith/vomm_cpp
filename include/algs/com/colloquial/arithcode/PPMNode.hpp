#ifndef PPMNODE_HPP
#define PPMNODE_HPP

#include "ByteBuffer.hpp"
#include "ByteSet.hpp"

#include <string>

namespace vmm_algs_com_colloquial_arithcode {
    
/** <P>A node in a depth-bounded suffix tree that represents counts of
 * sequences of bytes.  Nodes in the trie are accessed through
 * sequences of bytes in the same way as the {@link java.util.Map#get}
 * method of {@link java.util.Map}. Sequences of bytes are added as
 * descendants of the current node if necessary and their counts are
 * incremented and rescaled if necessary through an increment
 * operation that works in the same way as {@link java.util.Map#put} in
 * {@link java.util.Map}.

 * <P>The entire trie is accessible through the root node.  A byte is
 * stored with each node, and a (scaled) count of the times the the
 * sequence of bytes leading to this node in the trie were seen.  Each
 * node contains a linked list of child nodes, the first of which is
 * provided as a member element.  Each node also represents a member
 * of the linked list of siblings, and the next sibling is provided as
 * a member element.
 *
 * <P>The nodes provide the cumulative probability estimates for the
 * model through the <code>total</code>, <code>interval</code> and
 * <code>intervalEscape</code> methods.
 *
 * @author <a href="http://www.colloquial.com/carp/">Bob Carpenter</a>
 * @version 1.1
 * @see PPMModel
 * @since 1.0
*/

/**
 * Changes made by Ron are marked with @ron
 * Aim: support a general K sized alphabet.
 * class should be named SymbolSet..
 * All changes made in 16/10/03 otherwise date is stated
 */


class PPMNode {
    
private:
    
    /** The byte for this node.
     */
    int _byte;
    
    /** The scaled count for this node.
     */
    short _count = 1;
    
    /** The scaled number of outcomes used to calculate escape likelihoods.
     */
    short _numberOfOutcomes; // implied = 0;
    
    /** The first child of this node.
     */
    PPMNode* _firstChild = NULL; // implied = null;
    
    /** The next sibling of this node.
     */
    PPMNode* _nextSibling; // = NULL; // implied = null;
    
    // Link back up to our parent, or NULL if we don't have a parent.
    PPMNode* _parent = NULL;

    /** Minimum count for a node to survive pruning.
     */
    // private static final int MIN_PRUNE_COUNT = 2;
    
    /** Minimum count for which to retain a node during rescaling. Surprisingly insensitive.
     */
    static const int MIN_COUNT = 128;
    
    /** Maximum count for daughter node before rescaling all daughters.  Max value is
     * 8K; higher values cause overflow in the arithmetic coder.  Higher values compress
     * better, lower values are generally faster up to the point they cause thrashing.
     * 8K is about .01 b/B more compressed, and about 25% slower vs. 4K.
     * 2K is about .01 b/B less compressed, and roughly same speed as 4K.
     */
    static const int MAX_INDIVIDUAL_COUNT = 8*1024;
    
public:
    /** Construct a node with the specified byte and next sibling.
     * @param b Byte represented by node.
     * @param nextSibling The next daughter node in the list of daughters.
     */
    PPMNode(/*@ron byte*/int b, PPMNode* nextSibling);

    /** Construct a node with the specified byte.
     * @param b Byte represented by node.
     */
    PPMNode(/*@ron byte*/int b);
    
    /** Construct a node from the encoded string
     * @param data the encoded node specification.
     */
    PPMNode(std::string data);
    
    PPMNode* GetFirstChild();
    void SetFirstChild(PPMNode* child);
    int GetByte();
    PPMNode* GetNextSibling();
    void SetNextSibling(PPMNode* sibling);
    PPMNode* GetParent();
    void SetParent(PPMNode* parent);
    int GetOutcomes();
    int GetCount();

    /** Returns <code>true</code> if the number of children for
     * this node is <code>1</code>.
     * @return <code>true</code> if the scaled number of outcomes for this node is <code>1</code>.
     */
    bool isDeterministic();

    /** Returns <code>true</code> if this node has no children, not counting
     * specified exclusions.
     * @param excludedBytes Bytes to exclude as children
     * @return  <code>true</code> if this node has no children, not counting
     */
    bool isChildless(ByteSet* excludedBytes);

    /** Total count for this node, not including those bytes in the specified set.
     * @param excludedBytes Set of bytes to exclude from counts.
     * @return Total count for this node.
     */
    int totalCount(ByteSet* excludedBytes);


    /** Calculates the interval for the specified byte from this node and writes
     * it into the specified array.
     * @param i Byte whose interval is calcuated.
     * @param excludedBytes Set of bytes to exclude from counts.
     * @param result Array in which to write the range for the specified byte.
     */
    void interval(int i, ByteSet* excludedBytes, int* result);

    /** Calculates the interval for the specified byte from this node and writes
     * it into the specified array.
     * @param b Byte whose interval is calcuated.
     * @param excludedBytes Set of bytes to exclude from counts.
     * @param result Array in which to write the range for the specified byte.
     *
    private void interval(byte b, ByteSet excludedBytes, int[] result) {
	result[0] = 0;
	for (PPMNode dtrNode = _firstChild; dtrNode != null; dtrNode = dtrNode._nextSibling) {
	    if (excludedBytes.contains(dtrNode._byte)) continue;
	    if (dtrNode._byte == b) {
		result[1] = result[0] + dtrNode._count;
		result[2] = result[1] + _numberOfOutcomes;
		for (dtrNode = dtrNode._nextSibling; dtrNode != null; dtrNode = dtrNode._nextSibling)
		    if (!excludedBytes.contains(dtrNode._byte))
			result[2] += dtrNode._count;
		return;
	    }
	    result[0] += dtrNode._count;
	}
    } @ron */

    /** The interval for the escape count, less the set of excluded bytes.
     * @param excludedBytes Set of bytes to exclude from counts.
     * @param result Array into which to write the range for the specified bytes.
     */
    void intervalEscape(ByteSet* excludedBytes, int* result);

    /** Increment the counts for this node for the string specified in
     * the buffer.
     * @param buffer Buffer of bytes from which to read event to increment.
     */
    void increment(ByteBuffer* buffer);


    /** Returns <code>true</code> if this node has a child with the specified byte, specified
     * as an integer.
     * @param i Byte coded as integer to check.
     * @return <code>true</code> if there is a child node with the specified byte.
     */
    bool hasDaughter(int i);

    /** Returns <code>true</code> if this node has a child with the specified byte.
     * @param b Byte to check.
     * @return </code>true</code> if there is a child node with the specified byte.
     *
    private boolean hasDaughter(byte b) {
	for (PPMNode dtrNode = _firstChild; dtrNode != null; dtrNode = dtrNode._nextSibling)
	    if (dtrNode._byte == b) return true;
	return false;
    }@ron */

    /** Retrieves the symbol for which the midCount is between its low and high
     * counts (inclusive on low, exclusive on high).
     * @param midCount Count for which to find symbol.
     * @param excludedBytes Set of bytes to exclude from counts.
     * @return Symbol with specified count.
     */
    int pointToSymbol(int midCount, ByteSet* excludedBytes);

    /** Extends this node with the given sequence of bytes, specified
     * by an array, offset and length.
     * @param bytes Byte array providing bytes to extend.
     * @param offset Index of first byte in array.
     * @param length Number of bytes to extend.
     */
    void complete(/*int*/ ByteBuffer* bytes, int offset, int length);

    /** Increment the count of all of the nodes along the sequence of
     * bytes determined by the specified array, beginning at the specified
     * offset and continuing for the specified length number of bytes.
     * @param bytes Array from which to read bytes.
     * @param offset Index of first byte to read from array.
     * @param length Total number of bytes to read from array.
     */
    void increment(/*int* */ ByteBuffer* bytes, int offset, int length);

    /** Prunes this node and its children, returning <code>null</code> if
     * the node's count is too low and pruning all children with counts
     * too low.
     * @return This node if it is above the minimum number of counts.
     */
    /*
    private PPMNode prune() {
	if (_count < MIN_PRUNE_COUNT) return (PPMNode) null;
	if (_firstChild != null) _firstChild = _firstChild.pruneSiblings();
	return this;
    }
    */

    /** Prunes the siblings of this node, returning the next sibling
     * or <code>null</code> if there aren't any.
     * @return Linked list of siblings above the pruning threshold.
     */
    /*
    private PPMNode pruneSiblings() {
	if (_count < MIN_PRUNE_COUNT) {
	    if (_nextSibling == null) return null;
	    return _nextSibling.pruneSiblings();
	}
	if (_firstChild != null) _firstChild = _firstChild.pruneSiblings();
	if (_nextSibling == null) return this;
	_nextSibling = _nextSibling.pruneSiblings();
	return this;
    }
    */

private:
    /** Rescale all of the counts of the children of this node.
     * Divides by 2, rounding up, but eliminates all nodes that
     * fall below count threshold.  Total number of outcomes is also
     * rescaled, but it will never fall below <code>1</code> to allow
     * possiblity for escapes.
     */
    void rescale();

    /** Rescale the counts on this node and the siblings of this node.  Divides by 2, rounding
     * up, so no count every drops below 1.  Returns rescaled node, which may not
     * be original sibling or may be <code>null</code> if siblings scale below
     */
    PPMNode* rescaleSiblings();
    
    // Utility function to count the number of occurrences of a given token in a string
    static int countChar(string s, const char token);
    
public:
    /** ToString
     * encode the tree in a string format.
     * @return a string containing an encoded version of the node and its children/siblings
     */
    std::string ToString();
    
    /** FromString
     * reconstruct a tree from an encoded string
     * @param data the string to decode
     */
    static PPMNode* FromString(std::string data);
};
    
}


#endif //PPMNODE_HPP
