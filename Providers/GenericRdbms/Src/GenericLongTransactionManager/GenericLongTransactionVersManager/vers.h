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
// The file contains the structures and constants used by the Long Transaction
// Version Manager.
// =============================================================================

#ifndef _VERS_H_
#define _VERS_H_

#ifdef _WIN32
    typedef _int64  FdoInt64;
#else
//DOM-IGNORE-BEGIN
#include <stdint.h>
#ifndef FdoInt64Defined
#define FdoInt64Defined
    typedef int64_t FdoInt64;
//DOM-IGNORE-END
#endif
#endif

#include "../GenericLongTransactionDiGrManager/digr.h"
#include "lhash.h"


/* ------------------------------------------------------------------------- */
/*                             Options and Flags                             */
/* ------------------------------------------------------------------------- */

// Long transacrion context option flags.

#define VERS_QRY_DEL        0x0001  /* include deleted versions             */
#define VERS_QRY_ADD        0x0002  /* include sentinel versions            */
#define VERS_QRY_SEL        0x0004  /* always include sel-set versions      */
#define VERS_QRY_PERM       0x0008  /* include version 0 data               */
#define VERS_QRY_NVO        0x0010  /* override All-Versions with DataView  */
#define VERS_QRY_NOIDX      0x0020  /* disable RDBMS index use for version  */
                                    /* qualification                        */
#define VERS_SELPERM        0x0040  /* version 0 was explicitly added to    */
                                    /* selection-set                        */
#define VERS_QRY_ALL        0x0080  /* All-Versions query mode              */
#define VERS_QRY_LAST       0x0100  /* Recent-Versions query mode           */
#define VERS_QRY_MB         0x0200  /* generate multi-branch queries        */
#define VERS_QRY_INSERT_CLS 0x0400  /* generate special insert clause; this */
                                    /* is only valid for multibranch data-  */
                                    /* bases                                */
#define VERS_QRY_ALL_MB     0x0800  /* query all branches (scenarios)       */


// Version types.

#define VER_TYPE_SCENARIO   'S'
#define VER_TYPE_INTERNAL   'I'
#define VER_TYPE_SINGLE     ' '     /* actually NULL in the database        */
#define VER_TYPE_BASE       'B'


// Additional options returned by vers_ctx_chk(); must be distinct from the
// above flags

#define VERS_CHK_ACTV   0x2000      /* active_version changed               */
#define VERS_CHK_DEP    0x4000      /* deplist changed                      */
#define VERS_CHK_GEN    0x8000      /* generation number changed            */


// The version context flags that can be set or queried by the user

#define VERS_USER_WOPTS (VERS_QRY_PERM|VERS_QRY_DEL|VERS_QRY_ADD|\
                         VERS_QRY_SEL|VERS_QRY_ALL|VERS_QRY_LAST|\
                         VERS_QRY_MB|VERS_QRY_ALL_MB)
#define VERS_USER_OPTS  (VERS_USER_WOPTS|VERS_QRY_NVO)


// The version context flags the require a call to vers_deplist_upd when changed
// to update the selection dependency list.

#define VERS_UPD_OPTS   (VERS_QRY_NVO)


// Options for querying the long transaction context long transaction lists.

#define VERS_SELSET_USER    0x0001  /* user's selection set                 */
#define VERS_SELSET_SYS     0x0002  /* system selection set (0, active)     */
#define VERS_SELSET_DEP     0x0004  /* dependancy list for current sel-set  */


// Options for querying the dependents of a long transaction.

#define VERS_DEP_QRYACT     0x0001  /* dependents query is active (internal) */
#define VERS_DEP_CHILD      0x0002  /* get children instead of parents */


// Additional options for retrieving the parent/child dependents of a long
// transaction.

#define VERS_DEP_INCL_V0    0x0004  /* include v0 in parent dependecies */
#define VERS_DEP_EXCL       0x0008  /* exclude version from dependencies */


#define VERS_ACTIVE_NULL (-1L)  /* empty/null active version value */

#define VERS_VN_NULL    (0L)    /* NULL version_next value */


// N-Versions override options.

#define VERS_NVO_NONE   (-1L)   /* clear N-Versions override */
#define VERS_NVO_QRY    (-2L)   /* don't change N-Versions override */


/* ------------------------------------------------------------------------- */
/*                                Structures                                 */
/* ------------------------------------------------------------------------- */

typedef struct  vers_version {

    FdoInt64 version;
    char     type;
    long     scenario;

} vers_version_def;

typedef struct _vers_context {

    int                     flags;

    /* active version as member of selset (usually same as active_version) */
    FdoInt64                qry_act_version;

    /* The current selection set */
    int                     nselset;
    int                     tselset;
    FdoInt64                *selset;

    /* auxilary version, used for N-Version override */
    FdoInt64                aux_version;

    /* dependency generation number at last deplist_upd */
    long                    qry_generation;

    /* The current new generation numbers list at last deplist_upd */
    int                     nqrygenlist;
    int                     tqrygenlist;
    long                    *qrygenlist;

    /* The current selection set dependancy list */
    int                     ndeplist;
    int                     tdeplist;
    FdoInt64                *deplist;

    /* version dependency cache context */
    digr_context            digr_ctx;

    /* version used cache context */
    vers_lhash_table_def    *vers_used;

    /* active version list retrieval parameters */
    int                     get_flags;
    int                     get_idx;

    /* active version dependents retrieval parameters */
    int                     getdep_flags;
    int                     getdep_idx;
    int                     ndepvers;
    int                     tdepvers;
    FdoInt64                *depvers;

    /* current active lock_id for vers_chk_new_vers() */
    FdoInt64                act_lock_id;

    /* database active version */
    FdoInt64                active_version;

    /* dependency generation number at last cache load */
    long                    generation;

    /* The current new generation numbers list (since last cache load) */
    int                     ngenlist;
    int                     tgenlist;
    long                    *genlist;

    /* direct version update flag */
    int                     upd_mode;

    /* generation number retrieval parameter */
    int                     getgen_idx;

    /* multi-branch database flag. Maybe is should be bitmap flag ? */
    int                     db_is_mb;

    /* fields for versionlist. */
    vers_version_def        *versions;
    int                     nversions;
    int                     tversions;
	void                    *adb;

} vers_context_def, *vers_context;

typedef void *vers_ctx_state;

#endif

