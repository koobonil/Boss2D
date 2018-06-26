/**********************************************************************
 * File:        conv_net_classifier.h
 * Description: Declaration of Convolutional-NeuralNet Character Classifier
 * Author:    Ahmad Abdulkader
 * Created:   2007
 *
 * (C) Copyright 2008, Google Inc.
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 ** http://www.apache.org/licenses/LICENSE-2.0
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 *
 **********************************************************************/

// The ConvNetCharClassifier inherits from the base classifier class:
// "CharClassifierBase". It implements a Convolutional Neural Net classifier
// instance of the base classifier. It uses the Tesseract Neural Net library
// The Neural Net takes a scaled version of a bitmap and feeds it to a
// Convolutional Neural Net as input and performs a FeedForward. Each output
// of the net corresponds to class_id in the CharSet passed at construction
// time.
// Afterwards, the outputs of the Net are "folded" using the folding set
// (if any)
#ifndef CONV_NET_CLASSIFIER_H
#define CONV_NET_CLASSIFIER_H

#include <string>
#include "char_samp.h"
#include BOSS_TESSERACT_U_char_altlist_h //original-code:"char_altlist.h"
#include BOSS_TESSERACT_U_char_set_h //original-code:"char_set.h"
#include BOSS_TESSERACT_U_feature_base_h //original-code:"feature_base.h"
#include BOSS_TESSERACT_U_classifier_base_h //original-code:"classifier_base.h"
#include BOSS_TESSERACT_U_neural_net_h //original-code:"neural_net.h"
#include BOSS_TESSERACT_U_lang_model_h //original-code:"lang_model.h"
#include "tuning_params.h"

namespace tesseract {

// Folding Ratio is the ratio of the max-activation of members of a folding
// set that is used to compute the min-activation of the rest of the set
static const float kFoldingRatio = 0.75;

class ConvNetCharClassifier : public CharClassifier {
 public:
  ConvNetCharClassifier(CharSet *char_set, TuningParams *params,
      FeatureBase *feat_extract);
  virtual ~ConvNetCharClassifier();
  // The main training function. Given a sample and a class ID the classifier
  // updates its parameters according to its learning algorithm. This function
  // is currently not implemented. TODO(ahmadab): implement end-2-end training
  virtual bool Train(CharSamp *char_samp, int ClassID);
  // A secondary function needed for training. Allows the trainer to set the
  // value of any train-time parameter. This function is currently not
  // implemented. TODO(ahmadab): implement end-2-end training
  virtual bool SetLearnParam(char *var_name, float val);
  // Externally sets the Neural Net used by the classifier. Used for training
  void SetNet(tesseract::NeuralNet *net);

  // Classifies an input charsamp and return a CharAltList object containing
  // the possible candidates and corresponding scores
  virtual CharAltList * Classify(CharSamp *char_samp);
  // Computes the cost of a specific charsamp being a character (versus a
  // non-character: part-of-a-character OR more-than-one-character)
  virtual int CharCost(CharSamp *char_samp);


 private:
  // Neural Net object used for classification
  tesseract::NeuralNet *char_net_;
  // data buffers used to hold Neural Net inputs and outputs
  float *net_input_;
  float *net_output_;

  // Init the classifier provided a data-path and a language string
  virtual bool Init(const string &data_file_path, const string &lang,
                    LangModel *lang_mod);
  // Loads the NeuralNets needed for the classifier
  bool LoadNets(const string &data_file_path, const string &lang);
  // Loads the folding sets provided a data-path and a language string
  virtual bool LoadFoldingSets(const string &data_file_path,
                               const string &lang,
                               LangModel *lang_mod);
  // Folds the output of the NeuralNet using the loaded folding sets
  virtual void Fold();
  // Scales the input char_samp and feeds it to the NeuralNet as input
  bool RunNets(CharSamp *char_samp);
};
}
#endif  // CONV_NET_CLASSIFIER_H
