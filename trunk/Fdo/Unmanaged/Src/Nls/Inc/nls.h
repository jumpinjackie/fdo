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

#ifndef NLS_H_DEFINED
/************************************************************************
*                                                                       *
* Name                                                                  *
*   nls.h -- Commonly used NLS defines.                                 *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/nls.h>.                                               *
*                                                                       *
* Description                                                           *
*       This  header file defines commonly used NLS constants:          *
*                                                                       *
************************************************************************/
#define NLS_H_DEFINED     1

#include <Inc/os.h>
#include <Inc/string.h>
#include <Inc/stdlib.h>

#ifndef MS_WINDOWS
#include <stdarg.h>
#endif

#ifdef VISION_NUTC_INC
#include <langinfo.h>
#else
#ifndef _MSC_VER
#include <Inc/langinfo.h>
#endif
#endif

#include <locale.h>

#include <Inc/msg.h>

/*
 * Macros for function prototypes
 */
#ifndef PROTO
#if defined(FULLPROTO) || defined(c_plusplus) || defined(__cplusplus)
#define PROTO(x)    x
#else
#define PROTO(x)    ()
#endif
#endif  /* PROTO */


#ifndef NLS_CACHE_LIMIT
#   define NLS_CACHE_LIMIT 10
#endif
#define NLS_MESGSZ      MSGBUFSIZE
#define NLS_BUFFSZ      5119

/*
 * Macros for replies
 */
#define NLS_YES_REPLY       0x00000001
#define NLS_NO_REPLY        0x00000002
#define NLS_END_REPLY       0x00000004
#define NLS_CANCEL_REPLY    0x00000008
#define NLS_EXIT_REPLY      0x00000010
#define NLS_SHOW_REPLY      0x00000020
#define NLS_HELP_REPLY      0x00000040
#define NLS_NONE_REPLY      0x00000080
#define NLS_ADD_REPLY       0x00000100
#define NLS_ON_REPLY        0x00000200
#define NLS_OFF_REPLY       0x00000400
#define NLS_LIST_REPLY      0x00000800
#define NLS_OUTPUT_REPLY    0x00001000
#define NLS_CHANGE_REPLY    0x00002000
#define NLS_DELETE_REPLY    0x00004000
#define NLS_EDIT_REPLY      0x00008000
#define NLS_UGH_REPLY       0x00010000
#define NLS_NEW_REPLY       0x00020000
#define NLS_APPEND_REPLY    0x00040000

#define NLS_MAX_REPLY       0x0007ffff  /* limited to 32 bits */


#if defined(__cplusplus)
extern "C" {
#endif

#if defined(VISION_NUTC)
extern char * nls_msg_getv      PROTO ((char *msg_string,
                                        char *cat_name,
                                        int  set_num,
                                        int  msg_num,
                                        char *default_msg,
                                         va_list argv));
#endif
extern int      nls_locale_set  PROTO ((void));
#if defined(MS_WINDOWS)
extern void     nls_msg_issue   PROTO ((char severity,
                                        char *cat_name,
                                        int  set_num,
                                        DWORD msg_num,
                                        char *default_msg,
                                        ...));
extern char *   nls_msg_get_A       PROTO ((char *msg_string,
                                        char *cat_name,
                                        int  set_num,
                                        DWORD msg_num,
                                        char *default_msg,
                                        ...));
extern wchar_t *    nls_msg_get_W2       PROTO ((wchar_t *msg_string,
                                        char *cat_name,
                                        int  set_num,
                                        DWORD msg_num,
                                        char *default_msg,
                                        va_list arguments));
extern char *   nls_ftoa     	PROTO ((double number,
                                        int width,
					int precision));
#if defined(_UNICODE) || defined(UNICODE)
#define nls_msg_get nls_msg_get_W2
#else
#define nls_msg_get nls_msg_get_A
#endif

#else

extern void     nls_msg_issue   PROTO ((char severity,
                                        char *cat_name,
                                        int  set_num,
                                        int  msg_num,
                                        char *default_msg,
                                        ...));
extern char *   nls_msg_get     PROTO ((char *msg_string,
                                        char *cat_name,
                                        int  set_num,
                                        int  msg_num,
                                        char *default_msg,
                                        ...));

extern wchar_t *    nls_msg_get_W2      PROTO ((wchar_t *msg_string,
                                        char *cat_name,
                                        int  set_num,
                                        int msg_num,
                                        char *default_msg,
                                        va_list arguments));

extern char *   nls_ftoa     	PROTO ((double number,
                                        int width,
					int precision));

#endif

extern int      nls_match_reply PROTO ((char *reply_string,
                                        int answer_type));

extern char *   nls_lang_info   PROTO ((char *name ));

#if defined(__cplusplus)
}
#endif

#endif

