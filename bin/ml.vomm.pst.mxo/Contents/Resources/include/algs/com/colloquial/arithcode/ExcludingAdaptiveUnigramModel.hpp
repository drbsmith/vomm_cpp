#ifndef EXCADAUNIMOD_HPP
#define EXCADAUNIMOD_HPP

namespace vmm_algs_com_colloquial_arithcode {

/** Package class for use by the PPMModel.  A fragmentary adaptive
 * unigram model that allows exclusions in converting points to
 * intervals and vice-versa.  One such model will be used for each
 * unigram context.
 *
 * @author <a href="http://www.colloquial.com/carp/">Bob Carpenter</a>
 * @version 1.1
 * @since 1.1
 */
    
#include "ArithCodeModel.hpp"

/**
 * Changes made by Ron are marked with @ron
 * Aim: support a general K sized alphabet.
 * class should be named SymbolSet..
 * All changes made in 16/10/03 otherwise date is stated
 */

class ExcludingAdaptiveUnigramModel {

  /*@ron */
  static const int DEFAULT_ABSIZE = 257;/*ascii + EOF*/
private:
    int abSize;
    int eofIndex;

    /** Construct an excluding adaptive unigram model.
     */
public:
    ExcludingAdaptiveUnigramModel(int alphabetSize) : eofIndex(alphabetSize) {
      /*@ron add on */
      abSize = alphabetSize+1;
      _count = new int[abSize];/*@ron from 257 to abSize*/

        for (int i = 0; i < abSize; i++)
            _count[i] = 1;
//      java.util.Arrays.fill(_count,1); // counts are non-cumulative

        /**/
    }

    ~ExcludingAdaptiveUnigramModel() {
        if (_count)
            delete [] _count;
        _count = 0x00;
    }
    /** Compute the resulting interval to code the specified symbol given
     * the specified excluded bytes.
     * @param symbol Symbol to code.
     * @param result Interval to code the symbol.
     * @param exclusions Bytes to exclude as possible outcomes for interval.
     */
    void interval(int symbol, ByteSet* exclusions, int* result) {
        if (symbol == ArithCodeModel::_EOF) symbol = eofIndex/*@ron EOF_INDEX*/;
        int sum = 0;
        for (int i = 0; i < symbol; ++i)
            if (!exclusions->contains(i))
                sum += _count[i];
        result[0] = sum;
        sum += _count[symbol];
        result[1] = sum;
        for (int i = symbol+1; i < abSize-1; ++i)
            if (!exclusions->contains(i))
                sum += _count[i];
        if (symbol != eofIndex/*@ron EOF_INDEX*/)
            sum += _count[eofIndex/*@ron EOF_INDEX*/];
        result[2] = sum;
        increment(symbol);
    }

    void intervalNoIncrement(int symbol, ByteSet* exclusions, int* result) {
        if (symbol == ArithCodeModel::_EOF)
            symbol = eofIndex /*@ron EOF_INDEX*/;
        int sum = 0;
        for (int i = 0; i < symbol; ++i)
            if (!exclusions->contains(i))
                sum += _count[i];
        result[0] = sum;
        sum += _count[symbol];
        result[1] = sum;
        for (int i = symbol+1; i < abSize-1; ++i)
            if (!exclusions->contains(i))
                sum += _count[i];
        if (symbol != eofIndex/*@ron EOF_INDEX*/)
            sum += _count[eofIndex/*@ron EOF_INDEX*/];
        result[2] = sum;
    }

    /** Return the symbol corresponding to the specified count, given
     * the specified excluded bytes.
     * @param midCount Count of symbol to return.
     * @param exclusions Bytes to exclude from consideration.
     * @return Symbol represented by specified count.
     */
    int pointToSymbol(int midCount, ByteSet* exclusions) {
        int sum = 0;
        for (int mid = 0; ; ++mid) {
            if (mid != eofIndex/*@ron EOF_INDEX*/ && exclusions->contains(mid)) continue;
            sum += _count[mid];
            if (sum > midCount)
                return (mid == eofIndex/*@ron EOF_INDEX*/) ? ArithCodeModel::_EOF : mid;
        }
    }

    /** Total count for interval given specified set of exclusions.
     * @param exclusions Bytes to exclude as outcomes.
     * @return Total count of all non-excluded outcomes.
     */
    int totalCount(ByteSet* exclusions) {
        int total = 0;
        for (int i = 0; i < abSize; ++i)
            if (i == eofIndex/*@ron EOF_INDEX*/ || !exclusions->contains(i)) total += _count[i];
        return total;
    }

    /** Increment the count for the given outcome.
     * @param i Outcome to increment
     */
    void increment(int i) {
        if (++_count[i] > MAX_INDIVIDUAL_COUNT) rescale();
    }

    /** Counts for each outcome. Indices 0 to 255 for the
     * usual counts, 256 for end-of-file, and 257 for total.
     */
private:
    int* _count;

    /** Rescale the counts by dividing all frequencies by 2, but
     * taking a minimum of 1.
     */
    void rescale() {
        for (int i = 0; i < abSize; ++i)
            _count[i] = (_count[i] + 1)/2;
    }

    /** Maximum count before rescaling.
     */
    static const int MAX_INDIVIDUAL_COUNT = 8*1024;

    /** Index in the count array for the end-of-file outcome.
     */
   /*@ron private static final int EOF_INDEX = 256; */



};
}

#endif //EXCADAUNIMOD_HPP
