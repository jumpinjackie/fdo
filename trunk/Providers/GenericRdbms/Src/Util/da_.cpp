 //
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
* Name                                                                  *
*   ut_da_* - Dynamic Array functions                                   *
*                                                                       *
* Synopsis                                                              *
*                                                                       *
*   #include <inc/stdar.h>                                              *
*   #include <inc/ut/da.h>                                              *
*                                                                       *
*   void *ut_da_append( ut_da_def *da, long num_el, void *data );       *
*   void *ut_da_insert( ut_da_def *da, long indx,   void *data );       *
*   void *ut_da_get(    ut_da_def *da, long indx );                     *
*   void *ut_da_get2D(  ut_da_def *da, long x, long y );                *
*   bool  ut_da_delete( ut_da_def *da, long indx, long num_el );        *
*   bool  ut_da_free(   ut_da_def *da );                                *
*   bool  ut_da_free2D( ut_da_def *da );                                *
*   bool  ut_da_check(  ut_da_def *da );                                *
*   bool  ut_da_presize( ut_da_def *da, long num_el );                  *
*   bool  ut_da_presize2D( ut_da_def *da, long el_size, long num_rows,  *
*                                         long num_columns );           *
*                                                                       *
*   Also see inc/ut/da.h for UT_DA_GET_FAST() and UT_DA_INDX().         *
*                                                                       *
* Description                                                           *
*                                                                       *
*       Functions in this module are for handling dynamic arrays.       *
*                                                                       *
*       This suite of functions stresses simplicity and low overhead.   *
*       There are only basic functions and the array structure is       *
*       visible to the caller.                                          *
*                                                                       *
*       Each function takes as a first parameter a pointer to a         *
*       ut_da_def structure (see inc/ut/da.h).  The caller initializes  *
*       this structure before the first call to any ut_da_*() function. *
*       Only the first member (element size) needs to be made non-zero. *
*                                                                       *
*       As elements are added to an array, the allocated space grows    *
*       as needed by periodically reallocating memory.  Thus, the       *
*       location of elements can change during updates.                 *
*                                                                       *
*       While insertion and deletion functions are provided, they       *
*       are merely a convenience.  There is no special method provided  *
*       here to get around the cost of shifting memory.  The mechanism  *
*       for shifting elements is memmove().                             *
*                                                                       *
*       ut_da_append() appends the given data to the end of the array.  *
*       ut_da_insert() inserts the given data at the given index.       *
*       ut_da_get() gets the address of the data at the given index.    *
*       ut_da_delete() deletes data starting at the given index.        *
*       ut_da_free() frees the array (but NOT the ut_da_def structure). *
*       ut_da_free2D() frees an array of arrays                         *
*       ut_da_check() performs sanity checks on the ut_da_def structure.*
*       ut_da_presize() pre-allocates and sizes an array with zeroed data*
*       ut_da_presize2D() pre-allocates an array of arrays              *
*                                                                       *
* Parameters                                                            *
*                                                                       *
*       da: update                                                      *
*           The Dynamic Array structure.                                *
*                                                                       *
*       num_el: input                                                   *
*           The number of elements.                                     *
*           For ut_da_delete(), give -1 to delete to end of array.      *
*                                                                       *
*       data: input                                                     *
*           The caller's data to be copied into the dynamic array.      *
*                                                                       *
*       indx: input                                                     *
*           The index value into the array.                             *
*                                                                       *
* Function value                                                        *
*                                                                       *
*   On success:                                                         *
*       ut_da_append() returns the address where the data was copied.   *
*       ut_da_insert() returns the address where the data was copied.   *
*       ut_da_get() returns the address of the requested data.          *
*       ut_da_delete() returns true.                                    *
*       ut_da_free() returns true.                                      *
*       ut_da_check() returns true.                                     *
*                                                                       *
*   On failure:                                                         *
*       ut_da_append() returns NULL.                                    *
*       ut_da_insert() returns NULL.                                    *
*       ut_da_get() returns NULL.                                       *
*       ut_da_delete() returns false.                                   *
*       ut_da_free() returns false.                                     *
*       ut_da_check() returns false.                                    *
*                                                                       *
* Example                                                               *
*                                                                       *
*       / * A simple function to read pairs of number from standard     *
*         * input and report them back.                                 *
*         * /                                                           *
*       #include <stdio.h>                                              *
*       #include <inc/stdar.h>                                          *
*       #include <inc/ut/da.h>                                          *
*                                                                       *
*       typedef struct { int a, int b } mystruct_def;                   *
*                                                                       *
*       static ut_da_def empty_list = { sizeof(mystruct_def),           *
*                                       NULL, 0, 0 };                   *
*       bool myfunc()                                                   *
*       {                                                               *
*           mystruct_def     mystr;                                     *
*           mystruct_def    *myptr;                                     *
*           ut_da_def        list = empty_list;                         *
*           long             i;                                         *
*           bool             status = false;                            *
*                                                                       *
*           while ( scanf( "%d %d", &mystr.a, &mystr.b ) != EOF )       *
*               if ( ut_da_append( &list, 1, (void *)&mystr ) == NULL ) *
*                   goto the_exit;                                      *
*                                                                       *
*           for ( i = 0;  i < list.size;  i++ ) {                       *
*               myptr = (mystruct_def *) ut_da_get( &list, i );         *
*               if ( myptr == NULL ) goto the_exit;                     *
*               printf( "pair # %d: a=%d, b=%d\n",                      *
*                       i+1, myptr->a, myptr->b );                      *
*           }                                                           *
*                                                                       *
*           status = true;                                              *
*         the_exit:                                                     *
*           if ( !status )                                              *
*               printf( "Internal error in myfunc()!\n" );              *
*           status &= ut_da_free( &list );                              *
*           return( status );                                           *
*       }                                                               *
*                                                                       *
* Remarks                                                               *
*       The caller is expected to initialize the ut_da_def structure    *
*       (as shown above) once, before the first call to this suite.     *
*       After that, the ut_da_def structure should be considered        *
*       read-only, with the `data' and `size' elements being the only   *
*       useful information.  The caller may examine and write to memory *
*       pointed to by `data', as long as array bounds are checked       *
*       carefully.  In a pinch, `size' may be manually decremented      *
*       by a caller to a non-negative value to truncate the array.      *
*                                                                       *
*       While ut_da_get() does provide a retrieval function that has    *
*       fairly low overhead and full array bounds checking, the caller  *
*       can access the data manually in performance-sensitive code.     *
*       In the above example, the call to ut_da_get() could be          *
*       replaced with:                                                  *
*                                                                       *
*           #define MYSTRUCT_GET_FAST( list, indx ) \                   *
*                   ( & ( (mystruct_def *) ((list)->data) ) [(indx)] )  *
*                                                                       *
*           if ( list.data == (void *)NULL ) goto the_exit;             *
*           myptr = MYSTRUCT_GET_FAST( &list, i );                      *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       This module issues no messages.                                 *
*       However, calls to the ut_vm_*() function may result in          *
*       error messages when appropriate.                                *
*                                                                       *
************************************************************************/

#ifdef unix
#include <unistd.h>
#endif
#include <Inc/ut.h>
#include <Inc/debugext.h>
#include <Inc/Util/da.h>
#include <string.h>
#define POLICY_DOUBLE   0               /* Double arrays until big enough   */
#define POLICY_EXACT    1               /* No extra space                   */

/*static*/
bool
ut_da_alloc_more( ut_da_def *da, long min_num_el, int policy );


/************************************************************************/

void *
ut_da_append(
    ut_da_def       *da,
    long             num_el,
    void            *data )
{
    char            *element;
    void            *new_location = NULL;

    if ( da == NULL || data == NULL || num_el <= 0 ) goto the_exit;

    if ( da->size + num_el > da->allocated ) {
        if ( !ut_da_alloc_more( da, num_el, POLICY_DOUBLE ) ) goto the_exit;
    }
    element = (char *)da->data;
    element += da->size * da->el_size;
    (void) memcpy( (void *)element, data, num_el * da->el_size );

    da->size += num_el;

    new_location = (void *)element;
  the_exit:
    return( new_location );
}

/************************************************************************/

void *
ut_da_insert(
    ut_da_def       *da,
    long             indx,
    void            *data )
{
    char            *element;
    char            *array_end;
    void            *new_location = NULL;

    if ( da == NULL || data == NULL || indx < 0 ) goto the_exit;

    if ( da->size >= da->allocated ) {
        if ( !ut_da_alloc_more( da, 1, POLICY_DOUBLE ) ) goto the_exit;
    }

    element = (char *)da->data;
    element += da->el_size * indx;

    if ( indx < da->size ) {
        (void) memmove( (void *)(element + da->el_size), (void *)element,
                        (da->size - indx) * da->el_size );
    } else if ( indx > da->size ) {
        array_end = (char *)da->data;
        array_end += da->el_size * da->size;
        (void) memset( (void *)array_end, 0,
                          (indx - da->size) * da->el_size );
    }
    (void) memcpy( (void *)element, data, da->el_size );

    da->size++;

    new_location = (void *)element;
  the_exit:
    return( new_location );
}

/************************************************************************/

void *
ut_da_get(
    ut_da_def       *da,
    long             indx )
{
    char            *element = NULL;

    if ( da == NULL || indx >= da->size || da->data == NULL ) goto the_exit;

    element = (char *)da->data + ( indx * da->el_size );

  the_exit:
    return( (void *)element );
}

/************************************************************************/

void *
ut_da_get2D(
    ut_da_def       *da,
    long             x,
    long             y )
{
    char            *element = NULL;
    ut_da_def       *row_ptr;

    if ( da == NULL || y >= da->size || da->data == NULL ||
         da->el_size != sizeof(ut_da_def) ) {

        goto the_exit;
    }

    row_ptr = ((ut_da_def *)da->data) + y;

    if ( x >= row_ptr->size || row_ptr->data == NULL ) goto the_exit;

    element = (char *)row_ptr->data + ( x * row_ptr->el_size );

  the_exit:
    return( (void *)element );
}

/************************************************************************/

bool
ut_da_delete(
    ut_da_def       *da,
    long             indx,
    long             num_el )
{
    char            *element;
    char            *trailing_element;
    bool             status = false;

    if ( num_el == -1 && da != NULL )
        num_el = da->size - indx;

    if ( da == NULL || indx < 0 || num_el <= 0 || (indx + num_el) > da->size )
        goto the_exit;

    if ( (indx + num_el) < da->size ) {
        element = (char *)da->data;
        element += da->el_size * indx;
        trailing_element = element + (da->el_size * num_el);
        (void) memmove( (void *)element, (void *)trailing_element,
                        (da->size - indx - num_el) * da->el_size );
    }

    da->size -= num_el;

    status = true;
  the_exit:
    return( status );
}

/************************************************************************/

bool
ut_da_free(
    ut_da_def       *da )
{
    static char     *func_name = "ut_da_free";
    bool             status = true;

    if ( da == NULL ) goto the_exit;

    if ( da->data != NULL ) {
        ut_vm_free( func_name, (char *)da->data );
        da->data = NULL;
    }
    da->size = 0;
    da->allocated = 0;

  the_exit:
    return( status );
}

/************************************************************************/

bool
ut_da_free2D(
    ut_da_def       *da )
{
    bool             status = true;

    if ( da == NULL || da->el_size != sizeof(ut_da_def) ) goto the_exit;

    if ( da->data != NULL ) {

        ut_da_def   *da2D = (ut_da_def*)da->data;
        long         indx;

        for ( indx = 0;  indx < da->size;  indx++, da2D++ )
        {
            status &= ut_da_free( da2D );
        }
    }
    status &= ut_da_free( da );

  the_exit:
    return( status );
}

/************************************************************************/

bool
ut_da_check(
    ut_da_def       *da )
{
#ifdef unix
    void            *sbrkval = sbrk(0);
#endif
    bool             status = true;

    debug_on( "ut_da_check_memory" );

    if ( da->el_size <= 0 ) {
        debug1( "bad element size %ld\n", da->el_size );
        status = false;
    }
    if ( da->size < 0 ) {
        debug1( "bad array size %ld\n", da->size );
        status = false;
    }
    if ( da->allocated < 0 ) {
        debug1( "bad allocation size %ld\n", da->allocated );
        status = false;
    }
    if ( da->size > da->allocated ) {
        debug2( "conflicting array size %ld and allocation size %ld\n",
                da->size, da->allocated );
    }
    if ( (da->data != NULL && da->allocated <= 0) ||
         (da->data == NULL && da->allocated > 0) ) {
        debug2( "conflicting data pointer 0x%x and allocation size %ld\n",
                (long)da->data, da->allocated );
    }
#ifdef unix
    if ( da->data > sbrkval ) {
        debug2( "bad data pointer 0x%lx (sbrk is %ld)\n",
                (long)da->data, (long)sbrkval );
        status = false;
    }
#endif
    debug_return( NULL, status );
}

/************************************************************************/

bool
ut_da_presize(
    ut_da_def       *da,
    long             num_el )
{
    bool             status = false;

    if ( da == NULL || num_el <= 0 ) goto the_exit;

    if ( num_el > da->allocated ) {
        if ( !ut_da_alloc_more( da, num_el - da->allocated, POLICY_EXACT ) )
            goto the_exit;
    }

    if ( num_el > da->size )
    {
        char            *newstuff_start;

        newstuff_start = (char *)da->data;
        newstuff_start += da->el_size * da->size;

        (void) memset( (void *)newstuff_start, 0,
                          (num_el - da->size) * da->el_size );
    }

    da->size = num_el;

    status = true;
  the_exit:
    return( status );
}

/************************************************************************/

bool
ut_da_presize2D(
    ut_da_def       *da,
    long             el_size,
    long             num_rows,
    long             num_columns )
{
    bool             status = false;
    long             row_indx;
    ut_da_def       *row;

    if ( da == NULL || el_size <= 0 || num_rows <= 0  || num_columns < 0 )
        goto the_exit;

    da->el_size = sizeof(ut_da_def);

    if ( !ut_da_presize( da, num_rows ) ) goto the_exit;

    for ( row_indx = 0;  row_indx < da->size;  row_indx++ ) {
        row = (ut_da_def *) ut_da_get( da, row_indx );
        if ( row == NULL ) goto the_exit;
        row->el_size = el_size;
        if ( num_columns > 0 ) {
            if ( !ut_da_presize( row, num_columns ) ) goto the_exit;
        }
    }

    status = true;
  the_exit:
    return( status );
}


/************************************************************************/

#define UT_DA_GROWTH_RATE   (2)
#define ut_max(a,b)             ((a) > (b) ? (a) : (b))
/* This function is not static in order to avoid confusing debuggers. */

/*static*/ bool ut_da_alloc_more(
    ut_da_def       *da,
    long             min_num_el,
    int              policy )
{
    long             new_num_el;
    int              alloc_size;
    static char     *func_name = "ut_da:alloc_more";
    bool             status = false;

    new_num_el = ut_max( da->allocated, 1 );
    if ( policy == POLICY_DOUBLE ) {
        while ( new_num_el < min_num_el + da->size )
            new_num_el *= UT_DA_GROWTH_RATE;
    } else { /* POLICY_EXACT */
        new_num_el = ut_max( new_num_el, min_num_el + da->size );
    }

    alloc_size = new_num_el * da->el_size;
    if ( da->data == NULL ) {
        da->data = (void *) malloc( alloc_size );
    } else {
        da->data = (void *) realloc( (char *)da->data,
                                            alloc_size );
    }
    if ( da->data == NULL ) {
        da->size = 0;
        da->allocated = 0;
        goto the_exit;
    }
    da->allocated = new_num_el;

    status = true;
  the_exit:
    return( status );
}
