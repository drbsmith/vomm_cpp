#include "PPMNode.hpp"
#include "ArithCodeModel.hpp"

#include <sstream>
#include <vector>

using namespace vmm_algs_com_colloquial_arithcode;

using namespace std;

/** Construct a node with the specified byte and next sibling.
 * @param b Byte represented by node.
 * @param nextSibling The next daughter node in the list of daughters.
 */
PPMNode::PPMNode(/*@ron byte*/int b, PPMNode* nextSibling) : _byte(b), _nextSibling(nextSibling) {
//        _byte = b;
//        _nextSibling = nextSibling;
}

/** Construct a node with the specified byte.
 * @param b Byte represented by node.
 */
PPMNode::PPMNode(/*@ron byte*/int b) : _byte(b), _nextSibling(NULL) {
//        _byte = b;
//        _nextSibling = NULL;
}

PPMNode::PPMNode(string data) : _firstChild(NULL), _nextSibling(NULL) {
    if (data.size() >= 5) {
        try {
            string::size_type first = data.find('|');
            string element = data.substr(0,first);
            data.erase(0,first+1); // erase the first part
            _byte = stoi(element, nullptr, 10);
            
            first = data.find('|');
            element = data.substr(0,first);
            data.erase(0,first+1); // erase the second part
            _count = stoi(element, nullptr, 10);
            
            // last number goes to the end
            _numberOfOutcomes = stoi(data, nullptr, 10);
        } catch (exception ex) {
            ; // garbage in, garbage out!
        }
    }
}

PPMNode* PPMNode::GetFirstChild() {
    return _firstChild;
}
void PPMNode::SetFirstChild(PPMNode* child) {
    _firstChild = child;
}
int PPMNode::GetByte() {
    return _byte;
}
PPMNode* PPMNode::GetNextSibling() {
    return _nextSibling;
}
void PPMNode::SetNextSibling(PPMNode* sibling) {
    _nextSibling = sibling;
}
int PPMNode::GetOutcomes() {
    return _numberOfOutcomes;
}
PPMNode* PPMNode::GetParent() {
    return _parent;
}
void PPMNode::SetParent(PPMNode* parent) {
    _parent = parent;
}
int PPMNode::GetCount() {
    return _count;
}

/** Returns <code>true</code> if the number of children for
 * this node is <code>1</code>.
 * @param excludedBytes Bytes that have been seen in escaped context that should not be considered children.
 * @return <code>true</code> if the scaled number of outcomes for this node is <code>1</code>.
 */
bool PPMNode::isDeterministic() { //ByteSet* excludedBytes) {
    return (_firstChild->_nextSibling == NULL); // already checked _firstChild != null by calling isChildless first
/* doing it right is about 10-12% slower and less than .01 b/B better
int numOutcomes = 0;
for (PPMNode node = _firstChild; node != null; node = node._nextSibling)
    if (!excludedBytes.contains(node._byte) && ++numOutcomes > 1) return false;
return numOutcomes == 1;
*/
}

/** Returns <code>true</code> if this node has no children, not counting
 * specified exclusions.
 * @param excludedBytes Bytes to exclude as children
 * @return  <code>true</code> if this node has no children, not counting
 */
bool PPMNode::isChildless(ByteSet* excludedBytes) {
// return _firstChild == null; // not much faster and compresses less due to added escapes
    for (PPMNode* node = _firstChild; node != NULL; node = node->_nextSibling) {
        if (!excludedBytes->contains(node->_byte))
            return false;
    }
    return true;
}

/** Total count for this node, not including those bytes in the specified set.
 * @param excludedBytes Set of bytes to exclude from counts.
 * @return Total count for this node.
 */
int PPMNode::totalCount(ByteSet* excludedBytes) {
    int count = _numberOfOutcomes;
    for (PPMNode* child = _firstChild; child != NULL; child = child->_nextSibling)
        if (!excludedBytes->contains(child->_byte))
            count += child->_count;
    return count;
}


/** Calculates the interval for the specified byte from this node and writes
 * it into the specified array.
 * @param i Byte whose interval is calcuated.
 * @param excludedBytes Set of bytes to exclude from counts.
 * @param result Array in which to write the range for the specified byte.
 */
void PPMNode::interval(int i, ByteSet* excludedBytes, int* result) {
/*@ron old code interval(Converter.integerToByte(i),excludedBytes,result);*/
    result[0] = 0;
    for (PPMNode* dtrNode = _firstChild; dtrNode != NULL; dtrNode = dtrNode->_nextSibling) {
        if (excludedBytes->contains(dtrNode->_byte))
            continue;
        if (dtrNode->_byte == i) {
            result[1] = result[0] + dtrNode->_count;
            result[2] = result[1] + _numberOfOutcomes;
            for (dtrNode = dtrNode->_nextSibling; dtrNode != NULL; dtrNode = dtrNode->_nextSibling)
                if (!excludedBytes->contains(dtrNode->_byte))
                    result[2] += dtrNode->_count;
            return;
        }
        result[0] += dtrNode->_count;
    }

}

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
void PPMNode::intervalEscape(ByteSet* excludedBytes, int* result) {
    result[1] = totalCount(excludedBytes);
    result[2] = result[1];
    result[0] = result[1]-_numberOfOutcomes;
}

/** Increment the counts for this node for the string specified in
 * the buffer.
 * @param buffer Buffer of bytes from which to read event to increment.
 */
void PPMNode::increment(ByteBuffer* buffer) {
    if (buffer->length() > 0)
        increment(buffer,buffer->offset(),buffer->length());
}


/** Returns <code>true</code> if this node has a child with the specified byte, specified
 * as an integer.
 * @param i Byte coded as integer to check.
 * @return <code>true</code> if there is a child node with the specified byte.
 */
bool PPMNode::hasDaughter(int i) {
/*@ron old code	return hasDaughter(Converter.integerToByte(i));*/
  /*@ron new code*/
  for (PPMNode* dtrNode = _firstChild; dtrNode != NULL; dtrNode = dtrNode->_nextSibling)
        if (dtrNode->_byte == i)
            return true;
    return false;
}

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
int PPMNode::pointToSymbol(int midCount, ByteSet* excludedBytes) {
    int highCount = 0;
    for (PPMNode* child = _firstChild; child != NULL; child = child->GetNextSibling()) {
        if (excludedBytes->contains(child->_byte)) continue;
        highCount += child->_count;
        if (highCount > midCount)
            return /*Converter.byteToInteger(*/child->_byte/*)*/;
    }
    return ArithCodeModel::ESCAPE;
}

/** Extends this node with the given sequence of bytes, specified
 * by an array, offset and length.
 * @param bytes Byte array providing bytes to extend.
 * @param offset Index of first byte in array.
 * @param length Number of bytes to extend.
 */
void PPMNode::complete(/*int*/ ByteBuffer* bytes, int offset, int length) {
    PPMNode* node = this;
    while (length > 0) {
        ++(node->_numberOfOutcomes);
        node->_firstChild = new PPMNode(bytes->GetByte(offset));
        node->_firstChild->SetParent(node); // set linkage back up
        node = node->_firstChild;
        ++offset;
        --length;
    }
}

/** Increment the count of all of the nodes along the sequence of
 * bytes determined by the specified array, beginning at the specified
 * offset and continuing for the specified length number of bytes.
 * @param bytes Array from which to read bytes.
 * @param offset Index of first byte to read from array.
 * @param length Total number of bytes to read from array.
 */
void PPMNode::increment(/*int* */ ByteBuffer* bytes, int offset, int length) {
    if (_firstChild == NULL) {
        ++_numberOfOutcomes;
        _firstChild = new PPMNode(bytes->GetByte(offset));
        _firstChild->SetParent(this);
        if (length > 1)
            _firstChild->complete(bytes,offset+1,length-1);
        return;
    }
    PPMNode* previousChild = NULL;             // move to front
    for (PPMNode* child = _firstChild; true; child = child->_nextSibling) {
        if (child->_byte == bytes->GetByte(offset)) {
            if (length > 1)
                child->increment(bytes, offset+1, length-1);
            if (previousChild != NULL) {   // move to front
                previousChild->_nextSibling = child->_nextSibling;  // is this the only place where _nextSibling is set?
                child->_nextSibling = _firstChild;
                _firstChild = child;
            }
            if (++(child->_count) > MAX_INDIVIDUAL_COUNT)
                rescale();
            return;
        }
        if (child->_nextSibling == NULL) {
            ++_numberOfOutcomes;
            _firstChild = new PPMNode(bytes->GetByte(offset),_firstChild); // start in front
            _firstChild->SetParent(child);
            if (length > 1)
                _firstChild->complete(bytes,offset+1,length-1); // start in front
            return;
        }
        previousChild = child;                                // move to front
    }
}

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

/** Rescale all of the counts of the children of this node.
 * Divides by 2, rounding up, but eliminates all nodes that
 * fall below count threshold.  Total number of outcomes is also
 * rescaled, but it will never fall below <code>1</code> to allow
 * possiblity for escapes.
 */
void PPMNode::rescale() {
    _numberOfOutcomes = (short)((_numberOfOutcomes + 1)/2);
    _firstChild = _firstChild->rescaleSiblings();
}

/** Rescale the counts on this node and the siblings of this node.  Divides by 2, rounding
 * up, so no count every drops below 1.  Returns rescaled node, which may not
 * be original sibling or may be <code>null</code> if siblings scale below
 */
PPMNode* PPMNode::rescaleSiblings() {
    _count >>= 1;  // cheap divide by 2
    if (_nextSibling == NULL)
        return (_count < MIN_COUNT) ? NULL : this;
    if (_count < MIN_COUNT)
        return _nextSibling->rescaleSiblings();
    _nextSibling = _nextSibling->rescaleSiblings();
    return this;
}
int PPMNode::countChar(string s, const char token) {
    int count = 0;
    for (int i = 0; i < s.size(); i++)
        count += (s.at(i) == token ? 1 : 0);
    return count;
}

std::string PPMNode::ToString() {
    stringstream ss;
    
    ss << (_byte) << "|" << _count << "|" << _numberOfOutcomes;
    if (_firstChild != NULL)
        ss << " [" << _firstChild->ToString() << "]";
    if (_nextSibling != NULL)
        ss << " {" << _nextSibling->ToString() << "}";
    
    return ss.str();
}

PPMNode* PPMNode::FromString(std::string data) {
    if (data.size() < 8)
        return nullptr;
    
    PPMNode* ret = nullptr;
    
    if (data[0] == 'n') {   // it's a node string
        std::vector<std::string> result;
        std::istringstream iss(data);
        for(std::string s; iss >> s; )
            result.push_back(s);
        
        PPMNode *active = NULL;
        for (vector<string>::iterator it = result.begin(); it != result.end(); it++) {
            if ((*it).size() >= 5) { // minimum size for a node specification
                if ((*it)[0] == '[') { // it's a child
                    (*it).erase((*it).begin());
                    PPMNode* child = new PPMNode(*it);
                    active->SetFirstChild(child);
                    child->SetParent(active);
                    
                    active = child;
                } else if ((*it)[0] == '{') { // it's a sibling
                    (*it).erase((*it).begin());
                    PPMNode* sibling = new PPMNode(*it);
                    active->SetNextSibling(sibling);
                    sibling->SetParent(active);
                    
                    active = sibling;
                } else {    // it's the first node
                    ret = new PPMNode(*it);   // create the root
                    active = ret;
                }
                
                int stepUp = countChar(*it, ']');   // if we got to the end then work our way back up the tree
                stepUp += countChar(*it, '}');
                while (stepUp-- > 0 && active != NULL)
                    active = active->GetParent();
            }
        }
    }
    
    return ret;
}
