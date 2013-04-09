

//define this if linking to FdoSpatial utility is OK
#define SLT_USE_FDO_SPATIAL

#ifdef SLT_USE_FDO_SPATIAL
    #include "FdoSpatial.h"
#endif

#ifndef _MSC_VER
  #define __restrict __restrict__
  #define __forceinline __inline__
  #define __fastcall 
#endif
