/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2009
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#include "setup.h"

#include <string>

using namespace std;

// Constructors
/** Construct setup with given values.
 @param inputDir Directory with input JPEG files
 @param inputFile Single input JPEG file
 @param featureFile Resulting file with features (path)
 @param logFile Logfile (path)
 @param precision Precision of the calculated features\
 @param crohow_v Number of pixels to be cropped during calibration in vertical direction
 @param crophow_h Number of pixels to be cropped during calibration in horizontal direction
 @param single The boolean value whether only single JPEG image is at the input
 @param calitype Type of calibration (1=classical merged 274 features,2=cc-merged cartesian)
*/
Setup::Setup(string inputDir, string inputFile, string featureFile, string logFile, int precision, int crophow_v, int crophow_h,bool single, int calitype){
  inputDir_=inputDir;
  inputFile_=inputFile;
  featureFile_=featureFile;
  logFile_=logFile;
  precision_=precision;
  crophow_v_=crophow_v;
  crophow_h_=crophow_h;
  single_=single;
  calitype_=calitype;
}
/** Construct default setup. */
Setup::Setup(){
  inputDir_="";
  inputFile_="";
  featureFile_="";
  logFile_="";
  precision_=8;
  crophow_v_=4;
  crophow_h_=4;
  single_=false;
  calitype_=1;
}

// Set functions
/** Set the directory with input JPEG files.
    @param inputDir Directory with the input JPEG files
*/
void Setup::setInputDir(string inputDir){
  inputDir_=inputDir;
}
/** Set the single input image path.
    @param inputFile Single input image path
*/
void Setup::setInputFile(string inputFile){
  inputFile_=inputFile;
}
/** Set the output file with features (path).
    @param featureFile Output file with features (path)
*/
void Setup::setFeatureFile(string featureFile){
  featureFile_=featureFile;
}
/** Set the logfile (path).
    @param logFile Logfile (path)
*/
void Setup::setLogFile(string logFile){
  logFile_=logFile;
}
/** Set the precision of the calculated features.
    @param precision Precision of the calculated features
*/
void Setup::setPrecision(int precision){
  precision_=precision;
}
  /** Set the number of pixels to be cropped during calibration in vertical direction.
  @param precision Number of pixels to be cropped during calibration in vertical direction
   */
  void Setup::setCrophowV(int crophow_v){
    crophow_v_=crophow_v;
  }
  /** Set the number of pixels to be cropped during calibration in horizontal direction.
  @param precision Number of pixels to be cropped during calibration in horizontal direction
   */
  void Setup::setCrophowH(int crophow_h){
    crophow_h_=crophow_h;
  }
  /** Set the boolean value whether only single JPEG image is at the input. Otherwise the whole directory of JPEG images is expected.
  @param single The boolean value whether only single JPEG image is at the input
   */
  void Setup::setSingle(bool single){
    single_=single;
  }
  /** Set the calibration type. If the value equals to 1, classical 274 merged features are extracted. If it equals 2, cc-merged cartesian calibrated features are extracted.
  @param calitype Calibration type value (1 or 2)
   */
  void Setup::setCalitype(int calitype){
    calitype_=calitype;
  }


// Get functions
/** Get the directory with input JPEG files.
    @return the directory with input JPEG files
 */
string Setup::getInputDir(){
  return inputDir_;
}
/** Get the input single image path.
   @return the path of the single input image
 */
string Setup::getInputFile(){
  return inputFile_;
}
/** Get the output file with features (path).
    @return the path of the output file with features
*/
string Setup::getFeatureFile(){
  return featureFile_;
}
/** Get the logfile (path).
    @return the path of the logfile
 */
string Setup::getlogFile(){
  return logFile_;
}
/** Get the precision of the calculated features.
    @return the desired precision of calculated features
*/
int Setup::getPrecision(){
  return precision_;
}
/** Get the number of pixels to be cropped during calibration in vertical direction.
  @return the number of pixels to be cropped during calibration in vertical direction
 */
  int Setup::getCrophowV(){
    return crophow_v_;
  }
/** Get the number of pixels to be cropped during calibration in horizontal direction.
  @return the number of pixels to be cropped during calibration in horizontal direction
 */
  int Setup::getCrophowH(){
    return crophow_h_;
  }
  /** Get the boolean value whether only single JPEG image is at the input. Otherwise the whole directory of JPEG images is expected.
  @return the boolean value whether only single JPEG image is at the input
   */
  bool Setup::getSingle(){
    return single_;
  }
  /** Get the calibration type. Value=1 for classical 274 merged features, value=2 for cartesian calibrated (cc-merged) features.
  @return the calibration type integer value
   */
  int Setup::getCalitype(){
    return calitype_;
  }
