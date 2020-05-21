/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2009
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

#ifndef JET_HELP_H
#define JET_HELP_H

#include "setup.h"

/** Parse the command line arguments.
  @param argc Number of command line arguments
  @param argv List of command line arguments
  @param setup Basic structure for storing information parsed from the command line
  @param module What module of JET is under consideration (it influences the way how command line is parsed)
 */
int parseCommandLine(int argc, char* argv[], Setup* setup, int module);

/** Print help for the program to standard output.
  @param module What module of JET is under consideration (it influences the help content)
*/
int printHelp(int module);

#endif // JET_HELP_H
