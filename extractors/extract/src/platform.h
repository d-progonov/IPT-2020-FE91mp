/**
 *  Copyright (c) DDE Lab, SUNY Binghamton, 2007-2009
 *  Written by Jan Kodovsky, e-mail: jan@kodovsky.com
 *
 *  @author Jan Kodovsky
 */

// Purpose of this file: platform specific settings. The only thing
// that needs to be done is to  uncomment  the platform under which
// you are compiling.

#define LINUX
//#define WINDOWS_XP


////////////////////////////////////////////////////////////////////
// Do not change the following unless you know what you are doing //
////////////////////////////////////////////////////////////////////

#ifdef LINUX
  #ifdef DCT_IDCT
    #undef DCT_IDCT
    #include "dct_m.h"
    #include "idct_m.h"
  #endif // DCT_IDCT
#endif //LINUX

#ifdef WINDOWS_XP
  #pragma warning(disable : 4996) /* Disable all deprecation warnings */
  #ifdef DCT_IDCT
    #undef DCT_IDCT
    extern "C" {
      #include "dct_m.h"
      #include "idct_m.h"
    }
  #endif // DCT_IDCT
#endif // WINDOWS_XP
