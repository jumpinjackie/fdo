/******************************************************************************
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 * 
  *
 ******************************************************************************/

// =============================================================================
// | The file contains the structures and constants used by the Generic Long
// | Transaction Digraph Manager.
// =============================================================================

#ifndef DIGR_H
#define DIGR_H

#ifdef _WIN32
    typedef _int64  FdoInt64;
#else
//DOM-IGNORE-BEGIN
#include <stdint.h>
#ifndef FdoInt64Defined
#define FdoInt64Defined
    typedef int64_t FdoInt64;
#endif
//DOM-IGNORE-END
#endif

/* ------------------------------------------------------------------------- */
/*                      Options for digr_depend() call                       */
/* ------------------------------------------------------------------------- */

#define DIGR_INCLUSIVE  0x0001      /* return versions in sel-list           */
#define DIGR_ONELEVEL   0x0002      /* only follow one level of dependencies */

/* ------------------------------------------------------------------------- */
/*                             Internal options                              */
/* ------------------------------------------------------------------------- */

#define DIGR_LEVEL2     0x4000


/* ------------------------------------------------------------------------- */
/*                                Structures                                 */
/* ------------------------------------------------------------------------- */

struct bucket {

    struct bucket *next;           /* next edge in parent or child list */
    struct vertex *vertex;         /* vertex at other end of edge       */
    long           edge_weight;    /* weight associated with this edge  */

};  //  struct bucket

struct vertex {

    FdoInt64      id;           /* unique vertex id number                 */
    int           value;        /* used for graph traversals               */
    long          min_weight;   /* used for get_dep edge weight algorithm  */
    struct vertex *stk_next;    /* used for connected components algorithm */
    struct bucket *parents;     /* parents of this vertex                  */
    struct bucket *children;    /* children of this vertex                 */

};  //  struct vertex

typedef struct {

    struct bucket *vertlist;
    int           vertcnt;
    int           now;
    int           now_base;
    void          *vtxhash;

} digr_context_def, *digr_context;

#endif
