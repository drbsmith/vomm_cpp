#include "../include/algs/com/colloquial/arithcode/PPMModel.hpp"

using namespace vmm_algs_com_colloquial_arithcode;

PPMModel::PPMModel(int maxContextLength) {
     _maxContextLength = maxContextLength;
     _buffer = new ByteBuffer(maxContextLength+1);
//         _buffer = new ByteBuffer(maxContextLength+1);

    /*@ron */
    abSize = BYTE_ABSIZE;
    _backoffModel =  new ExcludingAdaptiveUnigramModel(abSize);
     for (int i = 0; i < abSize; i++)
         _contexts.push_back(NULL); //new PPMNode(i, NULL));
//        _contexts = new PPMNode[abSize];
    _excludedBytes = new ByteSet(abSize);

}

/**@ron added absize  */
PPMModel::PPMModel(int maxContextLength, int alphabetSize) {
    _maxContextLength = maxContextLength;
    _buffer = new ByteBuffer(maxContextLength+1);
//        _buffer = new ByteBuffer(maxContextLength+1);

    abSize = alphabetSize;
    _backoffModel =  new ExcludingAdaptiveUnigramModel(abSize);
    for (int i = 0; i < abSize; i++)
        _contexts.push_back(NULL); //new PPMNode(i, NULL));
//        _contexts = new PPMNode[abSize];
    _excludedBytes = new ByteSet(abSize);
}

PPMModel::~PPMModel() {
    for (int i = 0; i < _contexts.size(); i++)
        if (_contexts.at(i) != NULL)
            delete _contexts.at(i);
    _contexts.clear();
    
    delete _excludedBytes;
    delete _backoffModel;
    delete _buffer;
}

int PPMModel::GetAlphaSize() {
    return abSize;
}

// specified in ArithCodeModel
bool PPMModel::escaped(int symbol) {
    return (_contextNode != NULL
            && (symbol == ArithCodeModel::_EOF
                || !_contextNode->hasDaughter(symbol)));
}

// specified in ArithCodeModel
void PPMModel::exclude(int i) {
    _excludedBytes->add(i);
}

// specified in ArithCodeModel
void PPMModel::interval(int symbol, int* result) {
    if (symbol == ArithCodeModel::_EOF)
        _backoffModel->interval(ArithCodeModel::_EOF, _excludedBytes, result);
    else if (symbol == ArithCodeModel::ESCAPE)
        intervalEscape(result);
    else
        intervalByte(symbol,result);
}

// specified in ArithCodeModel
int PPMModel::pointToSymbol(int count) {
    if (_contextNode != NULL)
        return _contextNode->pointToSymbol(count, _excludedBytes);
    else
        return _backoffModel->pointToSymbol(count, _excludedBytes);
}

// specified in ArithCodeModel
int PPMModel::totalCount() {
    if (_contextNode == NULL)
        return _backoffModel->totalCount(_excludedBytes);
    else
        return _contextNode->totalCount(_excludedBytes);
}

// specified in ArithCodeModel
void PPMModel::increment(int i) {
  /* @ron old code
increment(Converter.integerToByte(i));*/
  /*@ron*/
    _buffer->buffer(i);

    int firstByte = _buffer->GetByte(_buffer->offset());
    if (_contexts[firstByte] == NULL)
        _contexts[firstByte] = new PPMNode(firstByte);
    if (_buffer->length() > 1)
        _contexts[firstByte]->increment(_buffer, _buffer->offset()+1, _buffer->length()-1);
    // _backoffModel.increment(Converter.byteToInteger(i)); ->//  updates backoff model; best to exclude it by .1 i/B!
    _contextLength = (_maxContextLength < _buffer->length() ? _maxContextLength : _buffer->length());
    getContextNodeBinarySearch();
    _excludedBytes->clear();
}

/** Exclude all of the bytes in the specified byte set.
 * @param bytesToExclude Set of bytes to exclude from outcome.
 * @since 1.1
 */
void PPMModel::exclude(ByteSet* bytesToExclude) {
    _excludedBytes->add(bytesToExclude);
}

/** Returns interval for byte specified as an integer in 0 to 255 range.
 * @param i Integer specification of byte in 0 to 255 range.
 * @param result Array specifying cumulative probability for byte i.
 */
void PPMModel::intervalByte(int i, int* result) {
    if (_contextNode != NULL)
        _contextNode->interval(i, _excludedBytes, result);
    else
        _backoffModel->interval(i, _excludedBytes, result);
    increment(i);
}

/** Returns interval for escape in current context.
 * @param result Array for specifying cumulative probability for escape symbol in current context.
 */
void PPMModel::intervalEscape(int* result) {
    _contextNode->intervalEscape(_excludedBytes,result);
    if (_contextLength >= MIN_CONTEXT_LENGTH)
        for (PPMNode* child = _contextNode->GetFirstChild(); child != NULL; child = child->GetNextSibling())
            _excludedBytes->add(child->GetByte());
    --_contextLength; // could decrement longer contexts more for a speedup in some cases
    getContextNodeLongToShort();
}

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
void PPMModel::getContextNodeBinarySearch() {
    int low = MIN_CONTEXT_LENGTH;
    int high = _contextLength;
    _contextLength = MIN_CONTEXT_LENGTH-1; // not sure we need this (BDS: sets to 0)
    _contextNode = NULL;
    bool isDeterministic = false;
    while (high >= low) {
        int contextLength = (high + low)/2;
        PPMNode* contextNode = lookupNode(contextLength);
        if (contextNode == NULL || contextNode->isChildless(_excludedBytes)) {
            if (contextLength < high)
                high = contextLength;
            else
                --high;
        } else if (contextNode->isDeterministic()) { //_excludedBytes)) {
            _contextLength = contextLength;
            _contextNode = contextNode;
            isDeterministic = true;
            if (contextLength < high)
                high = contextLength;
            else
                --high;
        } else if (!isDeterministic) {
            _contextLength = contextLength;
            _contextNode = contextNode;
            if (contextLength > low)
                low = contextLength;
            else ++low;
        } else {
            if (contextLength > low)
                low = contextLength;
            else
                ++low;
        }
    }
}

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
void PPMModel::getContextNodeLongToShort() {
    while (_contextLength >= MIN_CONTEXT_LENGTH) {
        _contextNode = lookupNode(_contextLength);
        if (_contextNode == NULL || _contextNode->isChildless(_excludedBytes)) {
            --_contextLength;
            continue;
            
        }
        while (_contextLength > MIN_CONTEXT_LENGTH && _contextNode->isDeterministic()) { //_excludedBytes)) {
        // backoff to shortest deterministic node if context node is deterministic
            PPMNode* backoffNode = lookupNode(_contextLength-1);
            if (backoffNode == NULL || !backoffNode->isDeterministic()) //_excludedBytes))
                return;
            _contextNode = backoffNode;
            --_contextLength;
        }
        return;
    }
    _contextNode = NULL;
}

/** Returns node from the current byte buffer of
 * the specified context length, or null if there isn't one.
 * @param contextLength Number of bytes of context used.
 * @return Node found at that context.
 */
PPMNode* PPMModel::lookupNode(int contextLength) {
    int index = _buffer->offset()+_buffer->length()-contextLength;
//        if (_buffer->length() > index) {
        index = _buffer->GetByte(index);
        if (index >= 0 && index < _contexts.size()) {
            PPMNode* node = _contexts.at(index);
            if (node == NULL)
                return NULL;
            return lookup(node, _buffer, _buffer->offset()+_buffer->length()-contextLength+1, contextLength-1);
        }
//        }
    return NULL;
}

/** Looks up a node from the given bytes, offset and length starting
 * from the specified node.
 * @param node Node from which to search.
 * @param bytes Sequence of bytes to search.
 * @param offset Offset into sequence of bytes of the first byte.
 * @param length Number of bytes to look up.
 * @return Node found for the given bytes.
 */
/*@ron */
PPMNode* PPMModel::lookup(PPMNode* node, ByteBuffer* bytes /*int* bytes*/, int offset, int length) {
    if (length == 0)
        return node;
    for (PPMNode* child = node->GetFirstChild(); length > 0 && child != NULL; ) {
        if (bytes->GetByte(offset) == child->GetByte()) {
            if (length == 1)
                return child;
            //                node = child; // BDS: this wouldn't do anything in Java but changes the pointer in c++. Why was it being done at all?
            child = child->GetFirstChild();
            ++offset;
            --length;
        } else {
            child = child->GetNextSibling();
        }
    }
    return NULL;
}

/** Period between prunings in number of bytes.
 */
//PRUNE private static final int PRUNE_INTERVAL = 250000; // loses about 10% compression rate, saves lots of space

string PPMModel::ModelToString() {
    stringstream ss;
//    ss << "Byte|Count|Number of outcomes. '[' = children, '{' = siblings." << endl;
    for (int i = 0; i < _contexts.size(); i++)
        if (_contexts.at(i) != NULL) {
            ss << "n " << _contexts.at(i)->ToString() << endl;
        }
    ss << endl;
    
    return ss.str();
}
/* ModelFromString
 *
 * @param data the encoded string used to reconstruct part of the model trei
 */
void PPMModel::ModelFromString(string data) {
    stringstream ss(data);
    string onestring;
    
    while (getline(ss, onestring, '\n')) {
        if (onestring.size() >= 5) {
            PPMNode* newNode = PPMNode::FromString(onestring);
            
            if (newNode == NULL)
                return;
            
            if (_contexts.at(newNode->GetByte()) != NULL)
                delete _contexts.at(newNode->GetByte());
            
            _contexts.at(newNode->GetByte()) = newNode;
        }
    }
}
