/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2008
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#ifndef IPP_DATA_TYPES_
#define IPP_DATA_TYPES_

  typedef unsigned char   Ipp8u;
  typedef unsigned short  Ipp16u;
  typedef unsigned int    Ipp32u;
  typedef signed char     Ipp8s;
  typedef signed short    Ipp16s;
  typedef signed int      Ipp32s;
  typedef float           Ipp32f;

  typedef struct {
      int width;
      int height;
  } IppiSize;

#endif // IPP_DATA_TYPES_

#ifndef CFEATURE_H_
#define CFEATURE_H_

/** Class representing one individual feature and its properties */
class cfeature{

public:
  Ipp32s count[2];     ///< Counts for the original and cropped image feature
  Ipp32s factor[2];    ///< Normalization factors for the original and cropped image
  double value;        ///< Final value of the feature: orig/factor1-crop/factor2
/**
 * Construct one feature, belonging to the group index and with the given description.
 *
 * @param index Index of the group of features this one belongs to
 * @param desc Detailed description of this feature
 */
 cfeature(int index,char* description);
/**
 * Class destructor.
 */
 ~cfeature();
/**
 * Clean the data contained in this feature.
 */
 void clean();
/**
 * Final calibration of the feature.
 */
 void calibration();

private:
  int group;           ///< ID of the group of features this one belongs to
  char* description;   ///< Detailed description of this feature
};

#endif // CFEATURE_H_
