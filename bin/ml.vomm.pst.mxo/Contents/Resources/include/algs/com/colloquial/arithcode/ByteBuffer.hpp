#ifndef BYTEBUFFER_HPP
#define BYTEBUFFER_HPP

#include <string>
#include <sstream>
#include <vector>

namespace vmm_algs_com_colloquial_arithcode {

/** <P>Stores a queue of bytes in a buffer with a maximum size.  New
 * bytes are added to the tail of the queue, and if the size exceeds
 * the maximum, bytes are removed from the front of the queue.  Used
 * to model a sliding window of a fixed width over a stream of bytes
 * presented a byte at a time.  The bytes in the current window are
 * accessed through an array of bytes, an offset and a length.
 *
 * <P>For instance, with a maximum length of 2, beginning with an
 * empty buffer and adding bytes 1, 2, 3, and 4 in that order leads to
 * queues <code>{1}</code>, <code>{1,2}</code>, <code>{2,3}</code> and
 * <code>{3,4}</code>.
 *
 * @author <a href="http://www.colloquial.com/carp/">Bob Carpenter</a>
 * @version 1.0
 * @since 1.0
 */

/**
 * Changes made by Ron are marked with @ron
 * Aim: support a general K sized alphabet.
 */
class ByteBuffer {
    
private:
    /** Number of contexts that fit in the buffer without shifting.
     */
    const int BUFFER_SIZE_MULTIPLIER = 32;
    /** Array of bytes used to buffer incoming bytes.
     */
    /*@ron 16/10/03 changes: 1)  _bytes-->_symbols , 2) type byte[] --> int[]*/
    std::vector<int> _symbols; // = NULL;
    int _symSize;
    
    /** Maximum number of bytes in queue before adding pushes one off.
     */
    int _maxWidth;
    
    /** Offset of first byte of current context in buffer.
     */
    int _offset = 0;
    
    /** Number of bytes in the context.  Maximum will be given during construction.
     */
    int _length = 0;
public:
    ByteBuffer() {
        // nothing to do here..
    }
    /** Construct a context buffer of given maximum size.
     * @param maxWidth Maximum number of bytes in a context.
     */
    ByteBuffer(int maxWidth) {
        _maxWidth = maxWidth;

        /*@ron 16/10/03 changed: new int[..]*/
        _symSize = BUFFER_SIZE_MULTIPLIER * maxWidth;
//        _symbols = new int[_symSize];
    }
    
    ~ByteBuffer() {
//        if (_symbols)
//            delete [] _symbols;
//        _symbols = NULL;
    }
    
    /** Set a new max width and clear everything
     *
     */
    void SetMaxWidth(int maxWidth) {
//        if (_symbols)
//            delete [] _symbols;
        
        _maxWidth = maxWidth;
        _symSize = BUFFER_SIZE_MULTIPLIER * maxWidth;
        _symbols.clear();
        _length = 0;
        _offset = 0;
    }

    /** Current array of bytes backing this byte buffer.  The returned
     * bytes are not a copy and should not be modified.
     * @return Array of bytes backing this buffer.
     */

    /*@ron 16/10/03 changed: public int[] .. */
//    int* bytes() {
//        return _symbols;
//    }
    
    int GetByte(int index) {
 //       if (index < _length)
            return _symbols.at(index);
//        else
//            return -1;  // error
    }

    /** Current offset of this buffer into the byte array.
     * @return Offset of the buffer into the byte array.
     */
    int offset() {
        return _offset;
    }

    /** Current length of this buffer.
     * @return Length of this buffer.
     */
    int length() {
        return _length;
    }

    /** Add a byte to the end of the context, removing first element if
     * necessary.
     * param b Byte to push onto the tail of the context.
     */
    void buffer(/*@ron*/int sym) {
        if (nextFreeIndex() > maxIndex())
            tampDown();
        _symbols.push_back(sym);
//        _symbols[nextFreeIndex()] = sym;
        if (_length < _maxWidth)
            ++_length;
        else
            ++_offset;
    }

    /** Return a string representation of this context using
     * the current localization to convert bytes to characters.
     * @return String representation of this context.
     */


    std::string toString() {
      /* @old code
	return new String(_symbols,_offset,_length);
       */
      /*@ron 16/10/03 new code*/
        std::stringstream sbuff; // = new StringBuffer(_symbols.length*2);
        for(int i=0; i<_symSize; ++i) {
            sbuff << _symbols.at(i) << ",";
//        sbuff.append(i).append(',');
      }
        return sbuff.str(); //sbuff.toString();
    }


    /** Index in the buffer for next element. May point beyond the
     * maximum index if there is no more space.
     * @return Index for next element.
     */
private:
    int nextFreeIndex() {
        return _offset+_length;
    }

    /** The maximum index in the buffer.
     * param Index of last element in the buffer.
     */
    int maxIndex() {
        return _symSize-1;
    }

    /** Moves bytes in context down to start of buffer.
     */
    void tampDown() {
//        for (int i = 0; i < _offset; i++)
            _symbols.erase(_symbols.begin(), _symbols.begin()+_offset);
//        for (int i = 0; i < _length-1; ++i) {
//            _symbols[i] = _symbols[_offset+i+1];
//        }
        _offset = 0;
    }



//public:
//    static void main(String args[]) {
//
//      ByteBuffer buf = new ByteBuffer(4);
//
//      buf.buffer(1);
//      buf.buffer(2);
//      buf.buffer(3);
//      buf.buffer(4);
//      buf.buffer(5);
//      buf.buffer(6);
//
//
//      int []syms = buf.bytes();
//      System.out.println("offset="+buf.offset()+" length="+buf.length());
//      for(int i=buf.offset(), start=i; i<start+buf.length(); ++i) {
//        System.out.print("|"+syms[i]+"|");
//      }

};

}

#endif
