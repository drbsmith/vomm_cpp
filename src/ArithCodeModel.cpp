#include "../include/algs/com/colloquial/arithcode/ArithCodeModel.hpp"

using namespace vmm_algs_com_colloquial_arithcode;

const int ArithCodeModel::_EOF = -1;

/** Symbol denoting an escape, meaning that the outcome
 * symbol has no interval in the current context.  Guaranteed to be negative.
 */
const int ArithCodeModel::ESCAPE = -2;

ArithCodeModel::~ArithCodeModel () {};
