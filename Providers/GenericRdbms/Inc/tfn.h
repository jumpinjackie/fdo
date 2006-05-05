/************************************************************************
*                                                                       *
* Name                                                                  *
*   tfn.h - Define true, false and null constants                       *
*                                                                       *
* Synopsis                                                              *
*   #include <inc/tfn.h>                                                *
*                                                                       *
* Description                                                           *
*       This  header  file  defines  the commonly used constants:       *
*       TRUE,  FALSE and NULL.  Other  header  files  may  define       *
*       these constants  for  historical reasons, but this one is       *
*       considered the correct location.                                *
*                                                                       *
* Modification history                                                  *
*   Mnemonic    Release Date        Who     Review                      *
*                                                                       *
*   OLD         old     85aug02     rad                                 *
*       Created                                                         *
*                                                                       *
*   MOD_HIST    i8805   88sep24     rad                                 *
*       Automatic script to convert modification histories to the       *
*       new standard format.                                            *
*                                                                       *
*   OSFPORT     victor  94Sep05     Mike Tardif                         *
*       Ported to OSF1: corrected all definitions of NULL.              *
*                                                                       *
*   PC PORT     eureka  96Feb10     Ramin Sadafi                            *
*       Ported from PC version to Unix                                  *
*                                                                       *
*   Revision Control Information
*           $Id: //Providers/Oracle/inc/tfn.h#1 $
*       $Author: derrick $
*     $DateTime: 2005/03/02 12:59:17 $
*       $Change: 6496 $
************************************************************************/

#ifndef TFN_H
#define TFN_H   1


#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#ifndef NULL
#if defined(OSF1)
#define NULL    0L
#else
#define NULL    0
#endif
#endif

#endif
