// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
/************************************************************************
*                                                                       *
* Name                                                                  *
*   types.h                                                             *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/types.h>                                              *
*                                                                       *
* Description                                                           *
*                                                                       *
************************************************************************/

#ifndef INC_TYPES_H
#define INC_TYPES_H

#include <Inc/os.h>

#ifndef VISION_NUTC
#ifdef unix
#ifndef _AIX
#include <rpc/types.h>
#else
/* This is only temporay solution for AIX machines, it will be changed as
   soon as AIX's /usr/include/rpc/types.h upgrades
 */

/*
 *   COMPONENT_NAME: LIBCRPC
 *
 *   FUNCTIONS: mem_alloc
 *      mem_free
 *      
 *
 *   ORIGINS: 24
 *
 *
 * Copyright (c) 1990 by Sun Microsystems, Inc.
 *      @(#)types.h 1.20 88/02/08 SMI      
 */

/*  @(#)types.h 1.6 90/07/19 4.1NFSSRC SMI  */


/*
 * Rpc additions to <sys/types.h>
 */

#ifndef __RPC_TYPES_H
#define __RPC_TYPES_H

#define bool_t  int
#define enum_t  int
#define __dontcare__    -1

#ifndef FALSE
#   define  FALSE   (0)
#endif

#ifndef TRUE
#   define  TRUE    (1)
#endif

#ifndef NULL
#   define NULL 0
#endif

#ifndef _KERNEL
#define mem_alloc(bsize)    malloc(bsize)
#define mem_free(ptr, bsize)    free(ptr)
#else
#include <sys/malloc.h>
#define kmem_alloc(bsize)   xmalloc(bsize, 2, kernel_heap)
#define kmem_free(ptr, bsize)   xmfree(ptr, kernel_heap)
#define mem_alloc(bsize)    xmalloc(bsize, 2, kernel_heap)
#define mem_free(ptr, bsize)    xmfree(ptr, kernel_heap)
#endif

#include <sys/types.h>

#include <sys/time.h>

#endif  /* !__RPC_TYPES_H */
#endif /*_AIX*/

#endif /* unix*/

#endif /* VISION_NUTC */


#ifdef  FULLPROTO

typedef void * types_addr_t;

#else /* FULLPROTO */

typedef char * types_addr_t;

#endif /* FULLPROTO */

#endif /* INC_TYPES_H */
