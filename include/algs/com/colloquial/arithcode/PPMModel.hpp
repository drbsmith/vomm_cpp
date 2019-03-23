#ifndef PPMMODEL_HPP
#define PPMMODEL_HPP

#include "ArithCodeModel.hpp"
#include "ByteBuffer.hpp"
#include "ByteSet.hpp"
#include "ExcludingAdaptiveUnigramModel.hpp"
#include "PPMNode.hpp"

#include <vector>
#include <sstream>

namespace vmm_algs_com_colloquial_arithcode {

/** Provides a cumulative, adaptive byte model implementing
 * prediction by partial matching up to a specified maximum context size.
 * Uses Method C for estimation.
 *
 * Constants that control behavior include the maximum total count before
 * rescaling, and the minimum count to retain after rescaling (an escape
 * is always maintained with a count of at least <code>1</code>).
 * <P>
 * For more details, see <a href="../../../tutorial.html">The Arithemtic Coding Tutorial</a>.
 *
 * @author <a href="http://www.colloquial.com/carp/">Bob Carpenter</a>
 * @version 1.1
 * @since 1.0
 */

/**
 * Changes made by Ron are marked with @ron
 * Aim: support a general K sized alphabet.
 * class should be named SymbolSet..
 * All changes made in 16/10/03 otherwise date is stated
 */
class PPMModel : public ArithCodeModel {

  /*@ron*/
private:
    /** Minimum context length to look down sequence of nodes.
     * Shorter contexts use backoff model.
     */
    const int MIN_CONTEXT_LENGTH = 1;
    
    const int BYTE_ABSIZE = 256;
    
    /** Count of bytes coded to use in pruning.
     */
    // private int _byteCount; // implied = 0; uncomment for pruning
    
protected:
    int abSize;
    
    /** Model to use for short contexts. */
    ExcludingAdaptiveUnigramModel* _backoffModel = NULL;
    
    /** Nodes at depth 1 in the model. All order 0 nodes are included in the unigram
     */
    vector<PPMNode*> _contexts;
    
    /** Maximum context length to search in trie.  Maximum count will
     * be for maximum context length plus one.
     */
    int _maxContextLength;
    
    /** Root of the trie structure of counts.  Dummy byte as symbol.
     */
//private:
//    PPMNode* _rootNode = new PPMNode(-1);
    
    /** Current context length.
     */
protected:
    int _contextLength; // implied = 0;
    
    /** Current context node.
     */
    PPMNode* _contextNode = NULL;
    
    /** Bytes buffered for use as context.
     */
    ByteBuffer* _buffer;
    
    /** Storage for the excluded bytes
     */
    ByteSet* _excludedBytes = NULL;

    /** Construct a new model with the specified maximum length of
     * context to use for prediction.
     * @param maxContextLength Maximum length of context to use for prediction.
     */
public:
    PPMModel(int maxContextLength);

    /**@ron added absize  */
    PPMModel(int maxContextLength, int alphabetSize);
    
    ~PPMModel();

    int GetAlphaSize();

    // specified in ArithCodeModel
    bool escaped(int symbol);

    // specified in ArithCodeModel
    void exclude(int i);

    // specified in ArithCodeModel
    void interval(int symbol, int* result);

    // specified in ArithCodeModel
    int pointToSymbol(int count);

    // specified in ArithCodeModel
    int totalCount();

    // specified in ArithCodeModel
    void increment(int i);

    /** Exclude all of the bytes in the specified byte set.
     * @param bytesToExclude Set of bytes to exclude from outcome.
     * @since 1.1
     */
    void exclude(ByteSet* bytesToExclude);

    /** Returns interval for byte specified as an integer in 0 to 255 range.
     * @param i Integer specification of byte in 0 to 255 range.
     * @param result Array specifying cumulative probability for byte i.
     */
private:
    void intervalByte(int i, int* result);

    /** Returns interval for escape in current context.
     * @param result Array for specifying cumulative probability for escape symbol in current context.
     */
protected:
    void intervalEscape(int* result);

    // code used for pruning is edited out and marked as follows
    //PRUNE private void prune() {
    //PRUNE   for (int i = 0; i < 256; ++i) if (_contexts[i] != null) _contexts[i] = _contexts[i].prune();
    //PRUNE }

    /** Adds counts for given byte to model in current context and then updates the current context.
     * Rescales counts if necessary.  Called by both encoding and deocding.
     * @param b Byte to add to model.
     *
    private void increment(byte b) {

	_buffer->buffer(b);

	int firstByte = _buffer->bytes()[_buffer->offset()];
	if (_contexts[Converter.byteToInteger(firstByte)] == null)
	    _contexts[Converter.byteToInteger(firstByte)] = new PPMNode(firstByte);
	if (_buffer->length() > 1)
	    _contexts[Converter.byteToInteger(firstByte)].increment(_buffer->bytes(),
								    _buffer->offset()+1,
								    _buffer->length()-1);
	// _backoffModel.increment(Converter.byteToInteger(b));  //  updates backoff model; best to exclude it by .1 b/B!
	_contextLength = Math.min(_maxContextLength,_buffer->length());
	getContextNodeBinarySearch();
	_excludedBytes.clear();
	//PRUNE if (++_byteCount == PRUNE_INTERVAL) { _byteCount = 0; prune(); } // pruning
    } @ron */

    /** Use binary search to set the context node up to the currently
     * specified context length.  May set it to <code>null</code> if
     * not found.
     */
    void getContextNodeBinarySearch();

    /* un-used variant lookung up context lengths by starting with shortest and
     * continuing to increase until found.
    private void getContextNodeShortToLong() {
	int maxContextLength = _contextLength;
	_contextNode = null;
	_contextLength = MIN_CONTEXT_LENGTH-1;
	for (int contextLength = MIN_CONTEXT_LENGTH; contextLength <= maxContextLength; ++contextLength) {
	    PPMNode node = lookupNode(contextLength);
	    if (node == null || node.isChildless(_excludedBytes)) {
		continue; // return;  lose around .01 b/B total (not even average) with return, but 25% slower
	    }
	    _contextNode = node;
	    _contextLength = contextLength;
	    if (node.isDeterministic(_excludedBytes)) return;
	}
    }
    */

    /** Starting at the longest context, count down in length to set
     * a valid context or give up.  This version finds the shortest deterministic
     * context <= in length to the current context length, but if there is
     * no deterministic context, returns longest context length that exists
     * that is <= in length to the current context.
     * Could also implement this in short to long order
     */
    void getContextNodeLongToShort() ;

    /** Returns node from the current byte buffer of
     * the specified context length, or null if there isn't one.
     * @param contextLength Number of bytes of context used.
     * @return Node found at that context.
     */
private:
    PPMNode* lookupNode(int contextLength);

    /** Looks up a node from the given bytes, offset and length starting
     * from the specified node.
     * @param node Node from which to search.
     * @param bytes Sequence of bytes to search.
     * @param offset Offset into sequence of bytes of the first byte.
     * @param length Number of bytes to look up.
     * @return Node found for the given bytes.
     */
    /*@ron */
    static PPMNode* lookup(PPMNode* node, ByteBuffer* bytes /*int* bytes*/, int offset, int length);

    /** Period between prunings in number of bytes.
     */
    //PRUNE private static final int PRUNE_INTERVAL = 250000; // loses about 10% compression rate, saves lots of space

public:
    string ModelToString();
    /* ModelFromString
     *
     * @param data the encoded string used to reconstruct part of the model trei
     */
    void ModelFromString(string data);
    
    };
}

#endif

