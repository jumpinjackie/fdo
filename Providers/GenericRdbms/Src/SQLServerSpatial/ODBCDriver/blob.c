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

#include <ctype.h>
#include <math.h>
#include <Inc/debugext.h>
#include <Inc/ut.h>                     /* ut_vm_malloc()           */
#include <Inc/rdbi.h>                   /* rdbi status values       */
#include <Inc/Geometry/Fgf/AllGeometry_c.h>    /* pByteArray_def           */

#include "context.h"
#include "constants.h"
#include "structs.h"
#include "proto_p.h"

static bool col_list_free_blob_S(odbcdr_context_def *context, odbcdr_blob_col_list_def *list);
static odbcdr_blob_col_list_def *col_list_create_blob_S(void);
static int col_list_addColumn_blob_S(odbcdr_blob_col_list_def *list, int position_I, pByteArray_def *address_I);
static int col_list_setNumRows_blob_S(odbcdr_blob_col_list_def *list, int numRows_I);
static odbcdr_blob_col_def *col_list_getColumnByIndex_blob_S(odbcdr_blob_col_list_def *list, long indx);
static bool col_list_freeSqlServerBlobs_S(odbcdr_context_def *context, odbcdr_blob_col_list_def *list);
static int blob_checkFetchStatusRow_S( odbcdr_cursor_def  *cursor, int idxBlob_I, int numRow_I );
static int get_blobSqlServer(odbcdr_context_def *context, odbcdr_cursor_def *cursor, int position, int blobNo, 
                             int recNo, int blobValid, pByteArray_def *visionBlob_O, pByteArray_def *l_visionBlob_0);

int odbcdr_blob_convertBlobsToByteArray(odbcdr_context_def *context, odbcdr_cursor_def *cursor, int num_rows)
{
    odbcdr_blob_col_list_def* columnList = cursor->defined_blobs;
    int rdbi_status = RDBI_GENERIC_ERROR;

    if (num_rows < 1 || columnList == NULL)
        goto the_exit;

    for (long i = 0;  i < columnList->size;  i++ )
    {
        long                 j;
        odbcdr_blob_def     **sqlserverBlob;
        odbcdr_blobNI_def   **sqlserverBlobNI;
        pByteArray_def      *visionBlob;
		pByteArray_def      *l_visionBlob;   
        odbcdr_blob_col_def  *column;

        column = col_list_getColumnByIndex_blob_S( columnList, i );
        if ( column == NULL || column->blob_list.size < num_rows )
            goto the_exit;

        sqlserverBlob = (odbcdr_blob_def **)ut_da_get( &column->blob_list, 0L );
        sqlserverBlobNI = (odbcdr_blobNI_def **)ut_da_get( &column->blobNI_list, 0L );
        visionBlob = column->address;
        l_visionBlob = column->l_address;

        if ( sqlserverBlob == NULL || sqlserverBlobNI == NULL || visionBlob == NULL ) goto the_exit;

        // Process each blob in the dataset. The blob are bound.
        // In case a large blob is hit (data truncation), the blobs are unbound and 
        // the current blob is fetched again as a blob.
        // After the entire batch is processed the blobs are bound again.
        int  unbound = false; 
        int wantedDim = -1;

        for ( j = 0;  j < num_rows;  j++ )
        {
            debug2( "converting blob column pos '%d', row %d",
                    column->position, j );

            IByteArray_Release(visionBlob[j]);
            visionBlob[j] = NULL;
            l_visionBlob[j] = NULL;

            // Check the status of this blob.
            int blobValid = blob_checkFetchStatusRow_S( cursor, i, j );

            // Unbind if not already unbound.
            if ( !blobValid && !unbound)
            {
                ODBCDR_ODBC_ERR( SQLBindCol( cursor->hStmt,
	                        (SQLUSMALLINT) column->position,
	                        (SQLSMALLINT) SQL_C_BINARY,
                            (SQLPOINTER) (char*)NULL,
                            (SQLINTEGER) 0,
	                        (SQLLEN *) NULL),
                    SQL_HANDLE_STMT, cursor->hStmt,
                    "SQLBindCol", "unbind" );  

                debug0("UNBOUND!\n");
                unbound = true;
            }

            if ( unbound )
            {
                // Advance 
		        ODBCDR_ODBC_ERR( SQLSetPos(	cursor->hStmt,
									        (SQLUSMALLINT) j+1, // Row # is 1 based
									        SQL_POSITION,
									        SQL_LOCK_NO_CHANGE),
								         SQL_HANDLE_STMT,cursor->hStmt,
						                 "SQLSetPos", "set position");
            }

		    /* Remember the last blob fetched in order to be released when the cursor is freed*/
            ODBCDR_RDBI_ERR( get_blobSqlServer( context, 
                                                      cursor,
                                                      column->position,
                                                      i,
                                                      j,
                                                      blobValid,
                                                      &visionBlob[j],
												      &l_visionBlob[j] ));
        }

        // Rebind back if the case.            
        if (unbound)
        {
            int offset = i * ODBCDR_MAX_ARRAY_SIZE;
            ODBCDR_ODBC_ERR( SQLBindCol( cursor->hStmt,
			            (SQLUSMALLINT) column->position,
			            (SQLSMALLINT) SQL_C_BINARY,
                        (SQLPOINTER) (char *)&cursor->odbcdr_blob[offset * ODBCDR_UBLOB_CHUNK_SIZE],
                        (SQLINTEGER) ODBCDR_UBLOB_CHUNK_SIZE,
			            (SQLLEN *) (char *)&cursor->odbcdr_blobNI[offset]),
	            SQL_HANDLE_STMT, cursor->hStmt,
	            "SQLBindCol", "rebind" );   
        }   
    }
	
    rdbi_status = RDBI_SUCCESS;

  the_exit:
    debug_return( NULL, rdbi_status );
}

int odbcdr_blob_freeAllColumns(
	odbcdr_context_def	*context,
    odbcdr_cursor_def   *cursor_U
    )
{
    int                      rdbi_status = RDBI_GENERIC_ERROR;
    bool                     freeingStatus = TRUE;

    if ( cursor_U == NULL ) goto the_exit;

    if ( cursor_U->defined_blobs != NULL )
    {
        freeingStatus &= col_list_free_blob_S(context, cursor_U->defined_blobs);

        free( (char *) cursor_U->defined_blobs );

        cursor_U->defined_blobs = NULL;
    }

    if ( freeingStatus )
    {
        rdbi_status = RDBI_SUCCESS;
    }

  the_exit:
    return( rdbi_status );
}

int odbcdr_blob_defineColumn(
	odbcdr_context_def	*context,
    odbcdr_cursor_def *      cursor_U,
    int                     position_I,
    char *                  address_I
    )
{
	pByteArray_def			*pUserAddr = (pByteArray_def *)address_I;
    int                     rdbi_status = RDBI_GENERIC_ERROR;

    if ( cursor_U == NULL ||
         position_I < 1 ||
         address_I == NULL )
    {
        goto the_exit;
    }

    if ( cursor_U->defined_blobs == NULL )
    {
        cursor_U->defined_blobs = col_list_create_blob_S();
        
        if ( cursor_U->defined_blobs == NULL )
        {
            rdbi_status = RDBI_MALLOC_FAILED;
            goto the_exit;
        }
    }

    ODBCDR_RDBI_ERR( col_list_addColumn_blob_S( cursor_U->defined_blobs,
                                          position_I,
                                          (pByteArray_def *) address_I ) );
	*pUserAddr = NULL;

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    return( rdbi_status );
}

int odbcdr_blob_setNumRows(
	odbcdr_context_def	*context,
    odbcdr_cursor_def *      cursor_U,
    int                     numRows_I
    )
{
    int                     rdbi_status = RDBI_GENERIC_ERROR;

    if ( cursor_U == NULL ||
         numRows_I < 1 )
    {
        goto the_exit;
    }

    if ( cursor_U->defined_blobs != NULL )
    {
        ODBCDR_RDBI_ERR( col_list_setNumRows_blob_S( cursor_U->defined_blobs,
                                               numRows_I ) );
    }

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    return( rdbi_status );
}

static odbcdr_blob_col_list_def  initialisedColumnList_blob_S =
{   sizeof(odbcdr_blob_col_def), NULL, 0L, 0L };

static odbcdr_blob_col_list_def *
col_list_create_blob_S(
    void
    )
{
    odbcdr_blob_col_list_def *   newColumnList = NULL;

    newColumnList = (odbcdr_blob_col_list_def *) ut_vm_malloc(
                        "odbcdr_blob:col_list_create_blob_S",
                        sizeof(odbcdr_blob_col_list_def) );

    if ( newColumnList != NULL )
    {
        *newColumnList = initialisedColumnList_blob_S;
    }

    return( newColumnList );
}

static odbcdr_blob_col_def initialisedColumn_blob_S = {
    0,                                              // position
    NULL,                                           // address
    NULL,
    { sizeof(odbcdr_blob_def *), NULL, 0L, 0L },     // blob_list
    { sizeof(odbcdr_blobNI_def *), NULL, 0L, 0L }    // blobNI_list
};

static int
col_list_addColumn_blob_S(
    odbcdr_blob_col_list_def *list,
    int                      position_I,
    pByteArray_def          *address_I
    )
{
    odbcdr_blob_col_def       newColumn = initialisedColumn_blob_S;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    debug_on2( "odbcdr_blob:col_list_addColumn_blob_S", "pos %d, address 0x%lx",
                                            position_I, (long) address_I );

    if ( list == NULL || position_I < 1 || address_I == NULL ) goto the_exit;

    newColumn.position = position_I;
    newColumn.address  = address_I;

    if ( ut_da_append( list, 1L, (void *) &newColumn ) == NULL ) {
        rdbi_status = RDBI_MALLOC_FAILED;
        goto the_exit;
    }

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    debug_return( NULL, rdbi_status );
}

static int
col_list_setNumRows_blob_S(
    odbcdr_blob_col_list_def *list,
    int                      numRows_I
    )
{
    long                     i;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    debug_on1( "odbcdr_blob:col_list_setNumRows_blob_S", "numRows %d", numRows_I );

    if ( list == NULL || numRows_I < 1 ) goto the_exit;

    for ( i = 0;  i < list->size;  i++ ) {

        odbcdr_blob_col_def  *column = col_list_getColumnByIndex_blob_S( list, i );

        if ( !ut_da_presize( &column->blob_list, numRows_I ) ) {
            rdbi_status = RDBI_MALLOC_FAILED;
            goto the_exit;
        }

        if ( !ut_da_presize( &column->blobNI_list, numRows_I ) ) {
            rdbi_status = RDBI_MALLOC_FAILED;
            goto the_exit;
        }

		if ( column->l_address == NULL ) {
			column->l_address = (pByteArray_def *) malloc (numRows_I * sizeof(pByteArray_def));
			if ( column->l_address == NULL ) {
				rdbi_status = RDBI_MALLOC_FAILED;
				goto the_exit;
			}
			memset( column->l_address, '\0', numRows_I * sizeof(pByteArray_def));
		}
    }

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    debug_return(NULL, rdbi_status );
}

static odbcdr_blob_col_def *
col_list_getColumnByIndex_blob_S(
    odbcdr_blob_col_list_def *list,
    long                     indx
    )
{
    odbcdr_blob_col_def      *column = NULL;

    column = (odbcdr_blob_col_def *) ut_da_get( list, indx );

    return column;
}

static bool
col_list_free_blob_S(
	odbcdr_context_def	*context,
    odbcdr_blob_col_list_def *list
    )
{
    long                     i;
    bool                     status = true;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    debug_on1( "odbcdr_blob:col_list_free_blob_S", "%ld columns",
                (list==NULL)? -1 : list->size );

    if ( list == NULL ) goto the_exit;

    if ( !col_list_freeSqlServerBlobs_S( context, list ) ) goto the_exit;

    for ( i = 0;  i < list->size;  i++ ) {

        odbcdr_blob_col_def  *column = col_list_getColumnByIndex_blob_S( list, i );

        /* Free the arrays themselves. */
        debug1( "Freeing pointers for %ld blobs.",
                column->blob_list.size );

		for ( int j = 0; j < column->blob_list.size; j++ ) {
			IByteArray_Release( column->l_address[j] );	
		}

		ut_vm_free( _db_function, column->l_address );

        status &= ut_da_free( &(column->blob_list) );
        status &= ut_da_free( &(column->blobNI_list) );
    }

    rdbi_status = RDBI_SUCCESS;

    debug1( "Freeing %ld blob columns.", list->size );
    status &= ut_da_free( list ); 

  the_exit:

    status &= ( rdbi_status == RDBI_SUCCESS );
    
    debug_return( NULL, status );
}

static bool
col_list_freeSqlServerBlobs_S(
	odbcdr_context_def	*context,
    odbcdr_blob_col_list_def *list
    )
{
    long                     i, j;
    bool                     status = true;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    debug_on1( "odbcdr_blob:col_list_freeSqlServerBlobs_S", "%ld columns",
                (list==NULL)? -1 : list->size );

    if ( list == NULL ) goto the_exit;

    for ( i = 0;  i < list->size;  i++ ) {

        odbcdr_blob_col_def  *column = col_list_getColumnByIndex_blob_S( list, i );

        for ( j = 0; j < column->blob_list.size; j++ )
        {
			odbcdr_blob_def *blob = *((odbcdr_blob_def **)ut_da_get( &column->blob_list, j ));
			odbcdr_blobNI_def *blobNI = *((odbcdr_blobNI_def **)ut_da_get( &column->blobNI_list, j ));
        }

        /* Just zero out the pointers. */
        debug1( "Zeroing out pointers for %ld blobs.",
                column->blob_list.size );
        (void) memset( column->blob_list.data, 0,
             (size_t) column->blob_list.el_size * column->blob_list.size );
        (void) memset( column->blobNI_list.data, 0,
             (size_t) column->blobNI_list.el_size * column->blobNI_list.size );
    }

    rdbi_status = RDBI_SUCCESS;

  the_exit:

    status &= ( rdbi_status == RDBI_SUCCESS );
    
    debug_return( NULL, status );
}

static int
blob_checkFetchStatusRow_S (
    odbcdr_cursor_def   *cursor,
    int                 idxBlob_I,
    int                 numRow_I
    )
{
    int             status = true;
    SQLLEN     nullInd = cursor->odbcdr_blobNI[ idxBlob_I * ODBCDR_MAX_ARRAY_SIZE + numRow_I];

    if ( nullInd == SQL_NULL_DATA)
        status = true;
    else if ( nullInd >= ODBCDR_UBLOB_CHUNK_SIZE )
        status = false;

    return status;
}

static int
get_blobSqlServer(
	odbcdr_context_def		*context,
    odbcdr_cursor_def       *cursor,
    int                     position,
    int                     blobNo,
    int                     recNo,
    int                     blobValid,
    pByteArray_def          *visionBlob_O,
	pByteArray_def		    *l_visionBlob_0
    )
{
    odbcdr_connData_def	    *connData = NULL;
    int                     rdbi_status = RDBI_GENERIC_ERROR;
    PBYTE                   pData = NULL;
    BYTE                    test[1];
    pByteArray_def          wkb = NULL;
    SQLLEN                  count;
    SQLRETURN               rc;

    debug_on( "odbcdr_blob:blob_convertFromSqlServer_S" );

    // Bound arrays offset 
    int offset = blobNo * ODBCDR_MAX_ARRAY_SIZE + recNo;

    if ( blobValid )
    {
        if ( cursor->odbcdr_blobNI[offset] == SQL_NULL_DATA)
        {
            *visionBlob_O = NULL;
            *l_visionBlob_0 = NULL;
            debug0( "Blob is NULL." );
            rdbi_status = RDBI_SUCCESS;
            goto the_exit;
        }

        debug0( "Blob is not NULL." );

        pData = (PBYTE)&cursor->odbcdr_blob[offset * ODBCDR_BLOB_CHUNK_SIZE];
     
        count = cursor->odbcdr_blobNI[offset];
    }
    else
    {
        // Fetch as blob. First get the size.
        rc = SQLGetData( cursor->hStmt, 
                                     position, 
                                     SQL_C_BINARY, 
                                     test, 
                                     0, 
                                     &count);

        if ( rc != SQL_SUCCESS_WITH_INFO ) {
            ODBCDR_ODBC_ERR( rc,  
                             SQL_HANDLE_STMT, cursor->hStmt,
                            "SQLGetData", "getData");
        }
        
        if ( count <= 0 ) 
        {
            *visionBlob_O = NULL;
            *l_visionBlob_0 = NULL;
            debug0( "Blob is NULL." );
            rdbi_status = RDBI_SUCCESS;
            goto the_exit;
        }

        debug0( "Blob is not NULL." );

        // Allocate the buffer
        int  allocMore = true;
        if ( cursor->odbcdr_blob_tmp == NULL )
            cursor->odbcdr_blob_tmp = (PBYTE)malloc( count );
        else if ( cursor->odbcdr_blob_tmp_size < count )
            cursor->odbcdr_blob_tmp = (PBYTE)realloc( cursor->odbcdr_blob_tmp, count );
        else
            allocMore = false;

        if ( allocMore )
            cursor->odbcdr_blob_tmp_size = (int) count;

        pData = cursor->odbcdr_blob_tmp;

        SQLLEN lenOrIndex;
        ODBCDR_ODBC_ERR( SQLGetData( cursor->hStmt, 
                                        position, 
                                        SQL_C_BINARY, 
                                        pData,
                                        count, 
                                        &lenOrIndex),
                            SQL_HANDLE_STMT, cursor->hStmt,
                            "SQLGetData", "getData");
        cursor->odbcdr_blobNI[offset] = lenOrIndex;
    }

    // Create a byte array from the array of bytes. Note 'count' is an initial size.
    if ( NULL == ( wkb = IByteArray_Create( pData, (int) count) ) )
        goto the_exit;

    *visionBlob_O = *l_visionBlob_0 = wkb;

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    debug_return( NULL, rdbi_status );
}
