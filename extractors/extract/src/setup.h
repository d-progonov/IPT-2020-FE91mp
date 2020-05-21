/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2008
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#ifndef JET_SETUP_H
#define JET_SETUP_H

#include <string>

/** Setup is the class containing basic setup of the batch feature extraction: path to the input directory containing JPEG images (or path to the single image), paths to the logfile and to the output file with features, precision of the features. */
class Setup {
 public:
  // Constructors
  /** Construct setup with given values.
   @param inputDir Directory with input JPEG files
   @param inputFile Single input JPEG file
   @param featureFile Output file with features (path)
   @param logFile Logfile (path)
   @param precision Precision of the calculated features
   @param crohow_v Number of pixels to be cropped during calibration in vertical direction
   @param crophow_h Number of pixels to be cropped during calibration in horizontal direction
   @param single The boolean value whether only single JPEG image is at the input
   @param calitype Type of calibration (1=classical merged 274 features,2=cc-merged cartesian)
   */
   Setup(std::string inputDir, std::string inputFile, std::string featureFile, std::string logFile, int precision, int crophow_v, int crophow_h, bool single, int calitype);
  /** Construct default setup. */
  Setup();

  // Set functions
  /** Set the directory with input JPEG files.
      @param inputDir Directory with the input JPEG files
   */
  void setInputDir(std::string inputDir);
  /** Set the single input image path.
  @param inputFile Single input image path
   */
  void setInputFile(std::string inputFile);
  /** Set the output file with features (path).
      @param featureFile Output file with features (path)
   */
  void setFeatureFile(std::string featureFile);
  /** Set the logfile (path).
      @param logFile Logfile (path)
   */
  void setLogFile(std::string logFile);
  /** Set the precision of the calculated features.
      @param precision Precision of the calculated features
   */
  void setPrecision(int precision);
  /** Set the number of pixels to be cropped during calibration in vertical direction.
  @param precision Number of pixels to be cropped during calibration in vertical direction
   */
  void setCrophowV(int crophow_v);
  /** Set the number of pixels to be cropped during calibration in horizontal direction.
  @param precision Number of pixels to be cropped during calibration in horizontal direction
  */
  void setCrophowH(int crophow_h);
  /** Set the boolean value whether only single JPEG image is at the input. Otherwise the whole directory of JPEG images is expected.
  @param single The boolean value whether only single JPEG image is at the input
   */
  void setSingle(bool single);
  /** Set the calibration type. If the value equals to 1, classical 274 merged features are extracted. If it equals 2, cc-merged cartesian calibrated features are extracted.
  @param calitype Calibration type value (1 or 2)
   */
  void Setup::setCalitype(int calitype);

  // Get functions
/** Get the directory with input JPEG files.
  @return the directory with input JPEG files
 */\
  std::string getInputDir();
/** Get the input single image path.
   @return the path of the single input image
 */
   std::string getInputFile();
/** Get the output file with features (path).
  @return the path of the output file with features
 */
  std::string getFeatureFile();
/** Get the logfile (path).
  @return the path of the logfile
 */
  std::string getlogFile();
/** Get the precision of the calculated features.
  @return the desired precision of calculated features
 */
  int getPrecision();
/** Get the number of pixels to be cropped during calibration in vertical direction.
  @return the number of pixels to be cropped during calibration in vertical direction
 */
  int getCrophowV();
/** Get the number of pixels to be cropped during calibration in horizontal direction.
  @return the number of pixels to be cropped during calibration in horizontal direction
 */
  int getCrophowH();
  /** Get the boolean value whether only single JPEG image is at the input. Otherwise the whole directory of JPEG images is expected.
  @return the boolean value whether only single JPEG image is at the input
  */
  bool getSingle();
  /** Get the calibration type. Value=1 for classical 274 merged features, value=2 for cartesian calibrated (cc-merged) features.
  @return the calibration type integer value
   */
  int Setup::getCalitype();

 private:
   std::string inputDir_;     ///< Directory with the input JPEG files.
   std::string inputFile_;    ///< Single input JPEG file.
   std::string featureFile_;  ///< Resulting file with features (path).
   std::string logFile_;      ///< Logfile (path).
   int precision_;            ///< Precision of the calculated features.
   int crophow_v_;            ///< Number of pixels to be cropped during calibration in vertical direction.
   int crophow_h_;            ///< Number of pixels to be cropped during calibration in horizontal direction.
   bool single_;              ///< Is only single JPEG image at the input? Otherwise the whole directory of JPEG images is expected.
   int calitype_;             ///< What calibration type should be applied. Value=1 means classical merged 274 features. Value=2 means cc-merged by cartesian product, 2x274 features.
};

#endif // JET_SETUP_H
