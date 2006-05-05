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
// The file contains the structures and constants used by the linear hash func-
// tions
// =============================================================================


#ifndef VERS_LHASH_H
#define VERS_LHASH_H


/* ------------------------------------------------------------------------- */
/*                               Definitions                                 */
/* ------------------------------------------------------------------------- */

// Types of keys. Variable is presumed to be a string.

#define VERS_LHASH_FIXED    0
#define VERS_LHASH_VARIABLE 1


/* ------------------------------------------------------------------------- */
/*                                Structures                                 */
/* ------------------------------------------------------------------------- */

typedef struct vers_lhash_element_s vers_lhash_element_def;


struct vers_lhash_element_s {

    void                   *key_p;          /* Entry's key                  */
    void                   *value_p;        /* Value of entry               */
    vers_lhash_element_def *next_p;         /* Next entry in list           */

};

typedef struct {

    char                   *name_p;         /* Name for debugging purposes  */
    int                    size;            /* Size of hash table           */
    int                    num_elements;    /* Number of entries in table   */
    int                    key_type;
    int                    size_of_key;     /* Length of key, 0 if varying. */
    int                    base_lhash;
    int                    split_lhash;
    int                    upper;
    int                    lower;
    int                    bucket;
    unsigned               (*hash_fn)();
    vers_lhash_element_def **table_p;       /* Hash table itself.           */

} vers_lhash_table_def;

#endif
