#ifndef BYTESET_HPP
#define BYTESET_HPP

#include <vector>

namespace vmm_algs_com_colloquial_arithcode {

/*@ron 16/10/03*/
//import java.util.BitSet;

/** A set of bytes.  Supports add operations, containment queries,
 * and may be cleared.
 *
 * @author <a href="http://www.colloquial.com/carp/">Bob Carpenter</a>
 * @version 1.1
 * @since 1.0
 */

/**
 * Changes made by Ron are marked with @ron
 * Aim: support a general K sized alphabet.
 * class should be named SymbolSet..
 */
    using namespace std;
    
class ByteSet {
/*@ron 16/10/03 fresh code with ~old api
   old implementation is commented.
   */

  /*@ron alphabet size*/
private:
    int abSize;
    
public:
  /*@ron */
    vector<bool> bitSet; //BitSet bitSet;

  /*@ron num of sym elements*/
    int symCount;

  /** Construct a byte set.
   */
    ByteSet(int alphabetSize) {
      abSize = alphabetSize;
        for (int i = 0; i < abSize; i++)
            bitSet.push_back(false);
//      bitSet = new BitSet(abSize);
      symCount = 0;
    }

  /** Remove all of the bytes from this set.
   */
  void clear() {
//    bitSet.clear(); // set all bits to false.
      for (vector<bool>::iterator it = bitSet.begin(); it != bitSet.end(); it++)
          *it = false;
      symCount = 0;
  }

  /* Adds a byte, specified as an integer, to this set.
   * @param i Byte to add to this set.
   * @since 1.1
   */
  /*@ron changed i to sym*/
  void add(int sym) {
      while (sym >= bitSet.size())
          bitSet.push_back(false);
      
      bitSet.at(sym) = true; //.set(sym);
      ++symCount;
      
  }

/*@ron DEPRECATED !*/
  /** Add a byte to this set.
   * @param b Byte to add to this set.
   *
  public void add(byte b) {
    throw new UnsupportedOperationException("@ron: Deprecated");
  }
*/

  /** Removes a byte, specified as an integer, from this set.
   * @param i Integer specification of byte to remove from this set.
   * @since 1.1
   */
  /*@ron sym*/
  void remove(int sym) {
      if (sym < bitSet.size())
          bitSet.at(sym) = false;
//      bitSet.clear(sym);
      --symCount;
  }


  /*@ron DEPRECATED !*/
  /** Removes a byte from this set.
   * @param b Byte to remove from this set.
   * @since 1.1
   *
  public void remove(byte b) {  }*/

  /** Adds all the members of specified set to this set. The result is
   * that this set's value is the union of its previous value with the
   * specified set.  The argument set is unchanged.
   * @param that Byte set to add to this set.
   * @since 1.1
   */
  void add(ByteSet* that) {
      for (int i = 0; i < bitSet.size() && i < that->bitSet.size(); i++)
          bitSet.at(i) = bitSet.at(i) | that->bitSet.at(i);
//    bitSet.or(that.bitSet);
    symCount += that->symCount;
  }

  /** Removes all the members of specified set from this set. The result is
   * that this set's value is the complement of its previous value with the
   * specified set.  The argument set is unchanged.
   * @param that Byte set to remove from this set.
   * @since 1.1
   */
  void remove(ByteSet* that) {
      for (int i = 0; i < bitSet.size() && i < that->bitSet.size(); i++) {
          bitSet.at(i) = bitSet.at(i) | that->bitSet.at(i);
          bitSet.at(i) = bitSet.at(i) ^ that->bitSet.at(i);
      }
//    bitSet.or(that.bitSet);
//    bitSet.xor(that.bitSet);
    symCount -= that->symCount;
  }

  /** Removes all the elements of this set that are not in the specified
   * set.  The result is that this set's value is the intersection of its
   * previous value with the specified set.  The argument set is unchagned.
   * @param that Byte set to restrict this set to.
   * @since 1.1
   */
  void restrict(ByteSet* that) {
      for (int i = 0; i < bitSet.size() && i < that->bitSet.size(); i++)
          bitSet.at(i) = bitSet.at(i) & that->bitSet.at(i);
//    bitSet.and(that.bitSet);

    symCount = 0;
    for(int i=0; i<abSize; ++i) {
      if (bitSet.at(i)) {
        ++symCount;
      }
    }
  }

  /** Returns <code>true</code> if byte specified as an integer
   * is a member of this set.  Conversion is done by casting.
   * @param i Integer representation of byte to be tested for membership.
   * @return <code>true</code> if the specified byte is a member of this set.
   * @since 1.0
   */
  /*@ron sym*/
  bool contains(int sym) {
    return bitSet.at(sym);
  }

/*@ron DEPRECATED !*/
  /** Returns <code>true</code> if specified byte is a member
   * of this set.
   * @param b Byte to test for membership in this set.
   * @return <code>true</code> if the specified byte is a member of this set.
   *
  public boolean contains(byte b) {

  }*/

  /** Returns number of elements in this set.
   * @return Number of elements in this set.
   */
  int size() {
    return symCount;
  }

  /** An empty set.  Unsafe, because nothing prevents
   * the addition of elements. So it's kept to the package.
   * The decision to do it this way was to keep ByteSet itself a final
   * class for the sake of efficiency.
   */
//  static final ByteSet EMPTY_SET = new ByteSet(0);


 /*@ron fresh code ends here*/


};
    
}

#endif
