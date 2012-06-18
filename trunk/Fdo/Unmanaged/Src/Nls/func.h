#ifndef NLS_FUNC_H_DEFINED
/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#define NLS_FUNC_H_DEFINED     1


#include <stdlib.h>
#include <string.h>
#include <locale.h>

#ifndef NLS_CACHE_LIMIT
#define NLS_CACHE_LIMIT 10
#endif
#define NLS_BUFFSZ      5119

#ifndef _WIN32

#if defined(__cplusplus)
extern "C" {
#endif

#include <Inc/nl_types.h>

extern nl_catd  nls_cat_open    (char *cat_name);
extern char *   nls_lang_info   (char *name);

#if defined(__cplusplus)
}
#endif

#endif

#endif

