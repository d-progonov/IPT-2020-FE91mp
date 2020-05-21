/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2009
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#include "clparser.h"
#include "setup.h"
#include "texception.h"
#include "macros.h"

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include <iostream>
#include <sstream>

using namespace std;

namespace bfs = boost::filesystem; 

/** Parse the command line arguments.
  @param argc Number of command line arguments
  @param argv List of command line arguments
  @param setup Basic structure for storing information parsed from the command line
  @param module What module of JET is under consideration (it influences the way how command line is parsed)
 */
int parseCommandLine(int argc, char* argv[], Setup* setup, int module){
  switch(module){
  case MODULE_1__BATCH_FEATURE_EXTRACTION:
    if (argc==1){
      printHelp(module);
    } else {
      string token;
      for (int i=1;i<argc;i++){
        token=argv[i];
        if ((token=="-h" || token=="--help")&&(argc==2)){
          // print help
          printHelp(module);
        } else if (token=="-p" || token=="--precision"){
          // set precision of the features
          if (++i>=argc){
            throw TException("precision not specified");
          }
          stringstream buffer(argv[i]);
          int precision_int=-1;
          if( (buffer >> precision_int).fail() || precision_int<0){
            throw TException("precision specified incorrectly");
          }
          setup->setPrecision(precision_int);
        } else if (token=="-c" || token=="--crop"){
          // set precision of the features
          if (++i>=argc-1){
            throw TException("cropping not specified");
          }
          // set vertical cropping
          stringstream buffer(argv[i++]);
          int crophow_v_int=-1;
          if( (buffer >> crophow_v_int).fail() || crophow_v_int<0){
            throw TException("cropping specified incorrectly");
          }
          setup->setCrophowV(crophow_v_int);
          //set horizontal cropping
          stringstream buffer2(argv[i]);
          int crophow_h_int=-1;
          if( (buffer2 >> crophow_h_int).fail() || crophow_h_int<0){
            throw TException("cropping specified incorrectly");
          }
          setup->setCrophowH(crophow_h_int);
        } else if (token=="-o" || token=="--output"){
          // set the output feature file
          if (++i>=argc){
            throw TException("output feature file not specified");
          }
          token=argv[i];
          setup->setFeatureFile(token);
        } else if (token=="-t" || token=="--calitype"){
          // set the calibration type
          if (++i>=argc){
            throw TException("calibration type not specified");
          }
          token=argv[i];
          if (token=="difference") setup->setCalitype(1);
          else if (token=="cartesian") setup->setCalitype(2);
          else throw TException("unknown calibration type");
        } else if (token=="-s" || token=="--single"){
          // only single JPEG file stays at the input
          if (++i>=argc){
            throw TException("single JPEG image not specified");
          }
          token=argv[i];
          setup->setInputFile(token);
          setup->setSingle(true);
        } else {
          if ((i!=argc-1 && setup->getSingle()==false)||(i!=argc && setup->getSingle()==true)){
            throw TException("command line parameters are invalid");
          }
          if (!bfs::exists(token)){
            throw TException("given directory does not exist");
          }
          setup->setInputDir(token);
        }
      } // for

      if (setup->getSingle()==true){
        // single JPEG image feature extraction
        if (setup->getFeatureFile()==""){
          throw TException("output feature file needs to be specified for single image feature extraction");
        }
        // check whether the given input file exists
        if (!bfs::exists(setup->getInputFile())){
          throw TException("input file doesn't exist");
        }
        // check whether the given input file is JPEG image
        string check=setup->getInputFile();
        if (check.length()<5){
          throw TException("input file is not JPEG image");
        }
        if ((check.compare(check.length()-4,4,".jpg",0,4)!=0) && (check.compare(check.length()-4,4,".JPG",0,4)!=0)){
          throw TException("input file is not JPEG image");
        }
      } else {
        // batch extraction from all images in the given directory
        if (setup->getInputDir()==""){
          throw TException("no input directory specified");
        }
      // check whether there is "/" sign at the end of the path (due to old boost versions)
        string dirCheck=setup->getInputDir();
        if (dirCheck.substr(dirCheck.length()-1,1)=="/"){
        // remove the "/" sign at the end of the path
          setup->setInputDir(dirCheck.substr(0,dirCheck.length()-1));
        }
        if (setup->getFeatureFile()==""){
        // set the default feature file
          if (setup->getCalitype()==1){
	          setup->setFeatureFile(setup->getInputDir() + "/data/merged.fea");
          }
          if (setup->getCalitype()==2){
                 setup->setFeatureFile(setup->getInputDir() + "/data/cc-merged.fea");
          }
        }
        bfs::create_directory(setup->getInputDir() + "/data");
      } // if (setup->getSingle()==true)
    } // if (argc==1)
    break;
  default:
    throw TException("JET module not recognized");
    break;
  }
  return 0;
};

/** Print help for the program to standard output.
  @param module What module of JET is under consideration (it influences the help content)
 */
int printHelp(int module){
  switch(module){
    case MODULE_1__BATCH_FEATURE_EXTRACTION:
      cout << "\n"
      << "NAME\n\n"
      << "    Jpeg Essential Tool (JET), version 2.2\n"
      << "      - MODULE 1 - Feature extractor\n\n"
      << "AUTHOR\n\n"
      << "    Jan Kodovsky, SUNY Binghamton, 2007-2009\n"
      << "    jan@kodovsky.com\n\n"
      << "DISCLAIMER\n\n"
      << "    Copyright (c) DDE Lab, SUNY Binghamton\n"
      << "    Please report bugs or any comments to jan@kodovsky.com\n\n"
      << "SYNOPSIS\n\n"
      << "    extract [options] DIR\n\n"
      << "DESCRIPTION\n\n"
      << "    This program calculates  274 merged extended DCT and  Markov features [1] from\n"
      << "    all  the JPEG images  in the  given directory DIR.  Features are stored in the\n"
      << "    row by row manner, at the end of each row  the name of the processed  image is\n"
      << "    printed.  Option  calitype  allows to extract cartesian calibrated (cc-merged)\n"
      << "    features introduced in [2].\n\n"
      << "OPTIONS\n\n"
      << "    -h\n"
      << "    --help\n"
      << "        Print this help.\n\n"
      << "    -p NUM\n"
      << "    --precision NUM\n"
      << "        Set the precision of output features to NUM decimal  points. Default value\n"
      << "        of precision is 8.\n\n"
      << "    -o FILENAME\n"
      << "    --output FILENAME\n"
      << "        Save  the  calculated  features  in  the given  FILENAME (path).  If  this\n"
      << "        option  is not specified,  resulting features  will be  saved  in the file\n"
      << "        './data/merged.fea'  created  in the same  directory where  the input JPEG\n"
      << "        images are located.\n\n"
      << "    -c NUM NUM\n"
      << "    --crop NUM NUM\n"
      << "        Set by what number of pixels will be  images  cropped  during calibration.\n"
      << "        First  and  second  parameter  corresponds to the  number  of pixels to be\n"
      << "        cropped in the vertical  and  horizontal direction, respectively.  Default\n"
      << "        cropping is by 4 pixels in both directions.\n\n"
      << "    -s FILENAME\n"
      << "    --single FILENAME\n"
      << "        Use  this option if you  want to calculate  the featurevector  from single\n"
      << "        JPEG image only, specified by FILENAME (path). If you use this option, you\n"
      << "        need to  specify the output feature vector directly,  using option -o.  No\n"
      << "        DIR argument is expected under this scenario.\n\n"
      << "    -t TYPE\n"
      << "    --calitype TYPE\n"
      << "        Use  this option to specify  the  way  of  calibration.  TYPE='difference'\n"
      << "        results in a classical  274 merged  features as introduced  in [1]  and is\n"
      << "        default.  Option TYPE='cartesian' results in cartesian calibrated features\n"
      << "        as introduced in [2].\n\n"
      << "DEPENDENCIES\n\n"
      << "    - boost library (filesystem, system) - www.boost.org\n"
      << "    - jpeglib library - www.ijg.org\n\n"
      << "REFERENCES\n\n"
      << "    [1] T. Pevny and J. Fridrich, Merging Markov and DCT features for multi-class\n"
      << "        JPEG steganalysis. In E.J. Delp and P.W. Wong, editors, Proceedings SPIE,\n"
      << "        Electronic Imaging, Security, Steganography, and Watermarking of Multime-\n"
      << "        dia Contents IX, volume 6505, pages 3 1 - 3 14, San Jose, CA,  January 29\n"
      << "        - February 1, 2007.\n"
      << "    [2] J. Kodovsky  and  J. Fridrich,  Calibration  revisited.  In  J. Dittmann,\n"
      << "        S. Craver, and J. Fridrich, editors, Proceedings of the 11th ACM Multime-\n"
      << "        dia & Security Workshop, Princeton, NJ, September 7–8, 2009.\n";
    throw TException("");
    break;
  default:
    throw TException("JET module not recognized");
    break;
  }
  return 0;
}
