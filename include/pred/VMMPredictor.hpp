#ifndef _VMMPREDICTOR_HPP
#define _VMMPREDICTOR_HPP

/* HEADER
If you use this code don’t forget to reference us :) BibTeX: http://www.cs.technion.ac.il/~rani/el-yaniv_bib.html#BegleiterEY04

This code is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License (<a href="http://www.gnu.org/copyleft/gpl.html">GPL</a>) for more details.*/

#include <string>
#include <vector>

namespace vmm_pred {

/**
 * <p>Title: On prediction using VMM</p>
 * <p>Description: Variable Markov Model Code</p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: Technion</p>
 * @author Ron Begleiter
 * @version 1.0
 */

class VMMPredictor {
public:
    ~VMMPredictor() {}
  /**
   * This VMMPredictor use trainingSequence and consturcts its model.
   * @param trainingSequence a sequence
   */
    virtual void learn(std::string trainingSequence) {
        std::vector<int> seq;
        for (int i = 0; i < trainingSequence.size(); i++) {
            seq.push_back((int)trainingSequence.at(i));
        }
        learn(&seq);
    }
    virtual void learn(std::vector<int>* trainingSequence)=0;

  /**
   * Predicts the next symbol according to some context.
   *
   * @param symbol next symbol
   * @param context a context sequence
   * @return the likelihood prediction
   * @throws VMMNotTrainedException when predicting without training
   */
    double predict(int symbol, std::string context) {
        std::vector<int> seq;
        for (int i = 0; i < context.size(); i++) {
            seq.push_back((int)context.at(i));
        }
        return predict(symbol, &seq);
    }
    virtual double predict(int symbol, std::vector<int>* context)=0;

  /**
   * Predicts the log-likelihood of the testSequence, according
   * to this VMMPredictor model (consturcted upon training sequence) and empty
   * initial context.
   *
   * @param testSequence a sequence
   * @return the -log( Probability (testSequence | trainingSequence) )
   * @throws VMMNotTrainedException when predicting without training
   */
    double logEval(std::string testSequence) {
        std::vector<int> seq;
        for (int i = 0; i < testSequence.size(); i++) {
            seq.push_back((int)testSequence.at(i));
        }
        return logEval(&seq);
    }
    virtual double logEval(std::vector<int>* testSequence)=0;

  /**
   * Predicts the log-likelihood of the testSequence, according
   * to this VMMPredictor model (consturcted upon training sequence) and
   * initialContext.
   *
   * @param testSequence CharSequence to be predicted
   * @param initialContext CharSequence the initial context (length == VMM's order)
   * @return double logEval
   * @throws VMMNotTrainedException when predicting without training
   */
    double logEval(std::string testSequence, std::string initialContext) {
        
        std::vector<int> tseq, icon;
        for (int i = 0; i < testSequence.size(); i++) {
            tseq.push_back((int)testSequence.at(i));
        }
        for (int i = 0; i < initialContext.size(); i++) {
            icon.push_back((int)initialContext.at(i));
        }
        return logEval(&tseq, &icon);
    }
    virtual double logEval(std::vector<int>* testSequence, std::vector<int>* initialContext)=0;

    double* predictAll(std::string context) {
        std::vector<int> seq;
        for (int i = 0; i < context.size(); i++) {
            seq.push_back((int)context.at(i));
        }
        return predictAll(&seq);
    };
    virtual double* predictAll(std::vector<int>* context) =0;
    
    virtual std::string ModelToString() =0;
    virtual void ModelFromString(std::string data) =0;
};
    
} // namespace vmm_pred

#endif //_VMMPREDICTOR_HPP
