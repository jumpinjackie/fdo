// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/************************************************************************
*                                                                       *
*  Name                                                                 *
*       mio.h                                                           *
*                                                                       *
*  Synopsis                                                             *
*                                                                       *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*                                                                       *
************************************************************************/

#ifndef MIO_H_DEFINDED
#define MIO_H_DEFINDED


#if defined(_WIN32)
#if defined(c_plusplus) || defined(__cplusplus)
#define MIO_CFILE_SUPPORTED
#endif
#endif

#include <stdio.h>
#include <Inc/os.h>
#include <Inc/Sys/stat.h>


typedef enum 
{
    MIO_SEEK_BEGIN  = SEEK_SET,
    MIO_SEEK_CUR        = SEEK_CUR,
    MIO_SEEK_END        = SEEK_END
} MIOSeekEnum;

typedef enum 
{
    MIO_INVALID         = 0,
    MIO_STDMIO          = 1,
    MIO_MEMORY_PTR      = 2,
    MIO_MEMORY_HANDLE   = 3,
    MIO_CFILE           = 4,
    MIO_RAW             = 5,
    MIO_MEMORY_FILE     = 6
} MIOTechnology;


typedef void* MIOMemoryFile;


#ifdef MS_WINDOWS
typedef struct _stat  MIO_StatusDef;
#else
typedef struct stat  MIO_StatusDef;
#endif

struct MIO_Methods;

#ifdef FULLPROTO

typedef size_t          (*pfnMIORead)( struct MIO_Methods*, void* pBuffer, size_t Bytes);
typedef size_t          (*pfnMIOWrite)( struct MIO_Methods*, void* pBuffer, size_t Bytes );
typedef int             (*pfnMIOSeek)( struct MIO_Methods*, long offset, MIOSeekEnum origin );
typedef int             (*pfnMIOFlush)( struct MIO_Methods* );
typedef int             (*pfnMIOClose)( struct MIO_Methods* );
typedef int             (*pfnMIOGetStatus)( struct MIO_Methods*, MIO_StatusDef* );
typedef void*           (*pfnMIOGetSource)( struct MIO_Methods* );
typedef MIOTechnology   (*pfnMIOGetTechnology)( struct MIO_Methods* );

typedef long            (*pfnMIOGetPosition)( struct MIO_Methods* );
typedef char*           (*pfnMIOGetString)( struct MIO_Methods*, char* string, int nCount );
typedef void            (*pfnMIOSetFilePath)(  struct MIO_Methods*,  char* pszName );
typedef char*           (*pfnMIOGetFilePath)(  struct MIO_Methods* );
typedef int             (*pfnMIOScanf)(  struct MIO_Methods*, const char* szFormat, ... );
typedef void            (*pfnMIOSetData)(  struct MIO_Methods*,  void *data );
typedef void*           (*pfnMIOGetData)(  struct MIO_Methods* );


#else

typedef size_t          (*pfnMIORead)();
typedef size_t          (*pfnMIOWrite)();
typedef int             (*pfnMIOSeek)();
typedef int             (*pfnMIOFlush)();
typedef int             (*pfnMIOClose)();
typedef int             (*pfnMIOGetStatus)();
typedef void*           (*pfnMIOGetSource)();
typedef MIOTechnology   (*pfnMIOGetTechnology)();

typedef long            (*pfnMIOGetPosition)();
typedef char*           (*pfnMIOGetString)();
typedef void            (*pfnMIOSetFilePath)();
typedef char*           (*pfnMIOGetFilePath)();
typedef int             (*pfnMIOScanf)();
typedef void            (*pfnMIOSetData)();
typedef void*           (*pfnMIOGetData)();

#endif

typedef struct MIO_Methods
{
    pfnMIORead          fnRead;
    pfnMIOWrite         fnWrite;
    pfnMIOSeek          fnSeek;
    pfnMIOGetStatus     fnGetStatus;
    pfnMIOFlush         fnFlush;
    pfnMIOGetTechnology fnGetTechnology;
    pfnMIOGetSource     fnGetSource;
    pfnMIOGetPosition   fnGetPosition;
    pfnMIOGetString     fnGetString;
    pfnMIOSetFilePath   fnSetFilePath;
    pfnMIOGetFilePath   fnGetFilePath;
    pfnMIOScanf         fnScanf;

    pfnMIOClose         fnClose;
    /*pfnMIOOpen        fnOpen; */
    pfnMIOGetData       fnGetData;
    pfnMIOSetData       fnSetData;
} MIO_Methods;


#ifdef C_PLUSPLUS
extern "C"
{
#endif


#ifdef FULLPROTO
MIO_Methods*    MIOConstruct( MIOTechnology Type, void* key );
void            MIODestruct( MIO_Methods* );
MIOMemoryFile   MIOMemoryFileCreate( void );
#else
MIO_Methods*    MIOConstruct();
void            MIODestruct();
MIOMemoryFile   MIOMemoryFileCreate();
#endif



#ifdef C_PLUSPLUS
}
#endif




#endif

