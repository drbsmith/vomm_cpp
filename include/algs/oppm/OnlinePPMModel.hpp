#ifndef ONLINEPPMMODEL_HPP
#define ONLINEPPMMODEL_HPP

/////////////////////
// Converted to c++ by Benjamin Smith (BDS), October 2018.
//  Modified the the sequential training model implemented
//  here to work 'online.' Simply feed it more sequences
//  and it will continue to learn and extend the existing model.
/////////////////////


//import vmm.algs.com.colloquial.arithcode.*;
#include "../com/colloquial/arithcode/PPMModel.hpp"

namespace vmm_algs_oppm {

    using namespace vmm_algs_com_colloquial_arithcode;

/**
 * Online PPMC implementation.
 * Based on <a href="http://www.colloquial.com/carp/">Bob Carpenter</a>'s offline code.
 *
 * updated to use 2 different buffers and contexts to retain a training state.
 *
 */
    class OnlinePPMModel : public PPMModel {

    private:
        int allRes[3];
        bool isFirstPrediction;
        
//        int perdictionContextLength;
        ByteBuffer *predictionBuffer, *learningBuffer;  // buffers are swapped when training and predicting, to preserve the context and sequence
        int learningContextLength;

    public:
        OnlinePPMModel(int maxCodeLength, int absize);
        
        ~OnlinePPMModel();

        double predict(int symbol);

        void use(int symbol);

  // specified in ArithCodeModel
        void interval(int symbol, int* result);

  /**
   * Clears this OfflinePPMModel's context.
   * As a result the nexts symbol context will be the empty context.
   * @param predict switches between learningBuffer (false) and predictionBuffer (true)
   */
        void clearContext(bool predict = false);

  /** Returns interval for byte specified as an integer in 0 to 255 range.
   * @param i Integer specification of byte in 0 to 255 range.
   * @param result Array specifying cumulative probability for byte i.
   */
    private:
        void calcInterval(int i, int* result);
    };
}   // namespace

#endif // ONLINEPPMMODEL_HPP
