/**
 *  Jpeg Essential Tool (JET), version 2.1
 *
 *  MODULE 1 - Batch extraction of merged extended DCT and Markov features
 *
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2009
 *
 *  Please report bugs and remarks to jan@kodovsky.com.
 *
 *  @author Jan Kodovsky
 */
#include "clparser.h"
#include "setup.h"
#include "texception.h"
#include "cimage.h"

#include "macros.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
  int module = MODULE_1__BATCH_FEATURE_EXTRACTION;
  Setup* setup = new Setup();
  try {
    parseCommandLine(argc,argv,setup,module);
    if (setup->getSingle()==false){
      // batch feature extraction from all the images in the given directory
      cout << "batch feature extraction - merged extended DCT and Markov features\n"
           << "directory with input JPEG images: " << setup->getInputDir() << "\n"
           << "output file with calculated features: " << setup->getFeatureFile() << "\n"
           << "cropping during calibration:" << setup->getCrophowV() << "x" << setup->getCrophowH() << "\n"
           << "precision: " << setup->getPrecision() << "\n"
           << "calibration type: " << (setup->getCalitype()==1?"1 (274 merged)":"2 (548 cc-merged)") << "\n"
           << "---------------------------\n";
      BatchFeatureExtraction(setup->getInputDir(),setup->getFeatureFile(),"matlab","274",setup->getPrecision(),setup->getCrophowV(),setup->getCrophowH(),setup->getCalitype());
      cout << "---------------------------\n"
           << "end of processing.\n";
    } else {
      // feature extraction from single JPEG file
      cout << "feature extraction - merged extended DCT and Markov features\n"
           << "single input JPEG image: " << setup->getInputFile() << "\n"
           << "output file with calculated features: " << setup->getFeatureFile() << "\n"
           << "cropping during calibration:" << setup->getCrophowV() << "x" << setup->getCrophowH() << "\n"
           << "precision: " << setup->getPrecision() << "\n"
           << "calibration type: " << (setup->getCalitype()==1?"1 (274 merged)":"2 (548 cc-merged)") << "\n"
           << "---------------------------\n";
      SingleFeatureExtraction(setup->getInputFile(),setup->getFeatureFile(),"matlab","274",setup->getPrecision(),setup->getCrophowV(),setup->getCrophowH(),setup->getCalitype());
      cout << "---------------------------\n"
           << "end of processing.\n";
    }
  } catch (TException ex) {
    if (ex.what()!=""){
      cout << ex.what() << "\n";
    }
  }
  delete setup;
  return 0;
}
