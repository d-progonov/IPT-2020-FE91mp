/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2008
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#ifndef CFEATUREVECTOR_H_
#define CFEATUREVECTOR_H_

#include <string>

#include "cfeature.h"
#include "macros.h"

/** Class for maintaining the whole featurevector. Here is the list of features involved:
 global histogram [0-10]
 local histogram [11-65]
 dual histogram [66-164]
 variation [165]
 blockiness [166-167]
 cooccurence [168-192]
 markov [193-273]
*/
class cfeaturevector{
public:
  cfeaturevector();  ///< Class constructor, incitialize individual features.
  ~cfeaturevector(); ///< Class destructor.
  cfeature* feature[318];///< Features themselves
  cfeature* markov[324];///< Auxiliary markov fields
  cfeature* cooccurence[50];///< Auxiliary cooccurence fields
  cfeature* variation[2];///< Auxiliary variation fields
  void Clean();///< Clean all the contained data.
  void calibration(); ///< Calibration of all the features.
  vd getCartesianCalibratedFeatures(); ///< Returns cc-merged features [see Calibration Revisited, ACM 2009].
private:
  std::string FeatureGroups[11]; ///< List of names of groups of features
};

#endif // CFEATUREVECTOR_H_
