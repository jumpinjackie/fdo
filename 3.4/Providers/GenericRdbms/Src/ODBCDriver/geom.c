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

/************************************************************************
*
* Name
*   odbcdr_geom_* - geometry functions
*
* Synopsis
*
*   #include "constants.h"
*   #include "structs.h"
*   #include "global.h"
*   #include "proto_p.h"
*   #include <Inc/rdbi.h>
*   #include <Inc/stdar.h>
*   #include <Inc/ut/da.h>
*
*   int odbcdr_geom_bindColumn( odbcdr_cursor_def *cursor_U,
*                              int position_I,
*                              char *address_I );
*
*   int odbcdr_geom_defineColumn( odbcdr_cursor_def *cursor_U,
*                                int position_I,
*                                char *address_I );
*
*   int odbcdr_geom_setNumRows( odbcdr_cursor_def *cursor_U,
*                              int numRows_I );
*
*   int odbcdr_geom_getSqlServerBuffAddr( odbcdr_geom_col_list_def *columnList_I,
*                                     int position_I,
*                                     odbcdr_geom_def ***buffer_O,
*                                     odbcdr_geomNI_def ***ni_O );
*
*   int odbcdr_geom_convertBoundToSqlServer( odbcdr_cursor_def *cursor_U,
*                                        int numRows_I );
*
*   int odbcdr_geom_convertDefinedFromSqlServer( odbcdr_cursor_def *cursor_U,
*                                            int numRows_I );
*
*   int odbcdr_geom_freeSqlServerGeometries( odbcdr_cursor_def *cursor );
*
*   int odbcdr_geom_freeAllColumns( odbcdr_cursor_def *cursor );
*
* Description
*
*       Functions in this module are for handling geometries.  They
*       handle the conversions between FdoGeometry types and SqlServer
*       geometries (stored as BLOBS).
*
*       There are several subtopics:
*           - SqlServer object-relational mapping
*           - data buffering
*           - SqlServer/FdoGeometry data conversion
*
*       Data buffering is required in order to hide the SqlServer-specific
*       handling of geometries from ODBC callers.  As far as the outside
*       is concerned, OCIDR supports a column type of RDBI_GEOMETRY, which
*       can be specified in calls to odbcdr_bind() and odbcdr_define() on
*       ODBC connections.  The RDBI_GEOMETRY typecode corresponds
*       to the pIGeometry_def type, which is just a C wrapper around
*       C++ classes in the FdoGeometry package.  While the caller may bind or
*       define with pIGeometry_def's, we actually map such columns to
*       SqlServer mdsys.sdo_geometry's, using buffering supported by the
*       following functions:
*           - odbcdr_geom_bindColumn()
*           - odbcdr_geom_defineColumn()
*           - odbcdr_geom_setNumRows()
*           - odbcdr_geom_getSqlServerBuffAddr()
*           - odbcdr_geom_freeSqlServerGeometries()
*           - odbcdr_geom_freeAllColumns()
*
*       SqlServer/FdoGeometry data conversion is performed between caller-provided
*       pIGeometry_def buffers and locally-buffered geometry buffers.
*       This conversion happens in the following functions:
*           - odbcdr_geom_convertBoundToSqlServer()
*           - odbcdr_geom_convertDefinedFromSqlServer()
*
* Parameters
*
*       cursor_U: A cursor structure obtained from odbcdr_est_cursor_obj().
*
*       position_I: A column position in an SQL statement.
*
*       address_I: The address of a buffer of pIGeometry_def pointers.
*
*       numRows_I: The index value into the array.                              *
*       columnList_I: List of geometry columns to process (bound or defined)
*
*       buffer_O: SqlServer geometry conversion buffer
*
*       ni_O: NULL indicators for SqlServer geometry conversion buffer
*
* Function value
*
*   On success, all of these functions return RDBI_SUCCESS.
*   On failure, They will return one of several error RDBI_* error codes.
*   See inc/rdbi.h.
*
***************************************************************************/

#include <ctype.h>
#include <math.h>
#include <Inc/debugext.h>
#include <Inc/ut.h>                     /* ut_vm_malloc()           */
#include <Inc/rdbi.h>                   /* rdbi status values       */
#include <Inc/Geometry/Fgf/AllGeometry_c.h>    /* pIGeometry_def           */

#include "context.h"
#include "constants.h"
#include "structs.h"
#include "proto_p.h"

#define CONVERSION_TO_SQLSERVER        (0)
#define CONVERSION_FROM_SQLSERVER      (1)


/***************************************************************************/


static int geom_convert_S( odbcdr_context_def *context, odbcdr_cursor_def *cursor, odbcdr_geom_col_list_def *columnList_I,
                           int numRows_I, int conversionCode, long srid, int wantedDim );
static odbcdr_geom_col_list_def *col_list_create_S( void );
static int col_list_addColumn_S( odbcdr_geom_col_list_def *list, int position_I, pIGeometry_def *address_I );
static bool col_list_free_S( odbcdr_context_def	*context, odbcdr_geom_col_list_def *list, bool release_geoms );
static bool col_list_freeSqlServerGeometries_S( odbcdr_context_def	*context, odbcdr_geom_col_list_def *list );
static int col_list_setNumRows_S( odbcdr_geom_col_list_def *list, int numRows_I );
static odbcdr_geom_col_def *col_list_getColumnByIndex_S( odbcdr_geom_col_list_def *list, long indx );
static odbcdr_geom_col_def *col_list_getColumnByPosition_S( odbcdr_geom_col_list_def *list, int position_I );

static int geom_convertFromSqlServer_S( odbcdr_context_def	*context, odbcdr_cursor_def	*cursor, 
                                     int position,
                                     int geomNo,
                                     int recNo,
                                     odbcdr_geom_def *sqlserverGeom_I,
                                     odbcdr_geomNI_def *sqlserverGeomNI_I,
                                     int wantedDim,
                                     int geomValid,
                                     pIGeometry_def *visionGeom_O,
									 pIGeometry_def *l_visionGeom_O );
static int geom_convertToSqlServer_S( odbcdr_context_def *context, 
                                   odbcdr_cursor_def *cursor,
                                   int  position,
                                   pIGeometry_def visionGeom_I,
                                   long srid,
                                   int  wantedDim,
                                   odbcdr_geom_def **sqlserverGeom_O,
                                   odbcdr_geomNI_def **sqlserverGeomNI_O );
static int geom_checkFetchStatusRow_S( odbcdr_cursor_def  *cursor, int idxGeom_I, int numRow_I );

/*************************************************************************/

int odbcdr_geom_srid_set ( 
	odbcdr_context_def	*context,
    char   *cursor,                     /* cursor associated with SQL stmnt */
	char	*col_name,
    long srid ) 
{
    odbcdr_cursor_def    *c;
    int                 rdbi_status = RDBI_GENERIC_ERROR;
    int                 rc = FALSE;

    ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, cursor, &c ) );

    rc = TRUE;
the_exit:
    return rc;
}

/*************************************************************************/

int odbcdr_geom_dimens_set ( 
    odbcdr_context_def	*context,
    char   *cursor,                     /* cursor associated with SQL stmnt */
    int     wantedDim ) 
{
    odbcdr_cursor_def    *c;
    int                 rdbi_status = RDBI_GENERIC_ERROR;
    int                 rc = FALSE;

    ODBCDR_RDBI_ERR( odbcdr_get_cursor( context, cursor, &c ) );

    rc = TRUE;
the_exit:
    return rc;
}


/**************************************************************************
 *  Bind a geometry column.  Adds entry to cursor's list of bound geometries.
 *  This does NOT actually allocate the conversion buffer.
 *  See odbcdr_geom_setNumRows().
 */

int odbcdr_geom_bindColumn(
	odbcdr_context_def	*context,
    odbcdr_cursor_def *      cursor_U,
    int                     position_I,
    char *                  address_I
    )
{
    int                     rdbi_status = RDBI_GENERIC_ERROR;

    if ( cursor_U == NULL ||
         position_I < 1 ||
         address_I == NULL )
    {
        goto the_exit;
    }

    if ( cursor_U->bound_geometries == NULL )
    {
        cursor_U->bound_geometries = col_list_create_S();
        
        if ( cursor_U->bound_geometries == NULL )
        {
            rdbi_status = RDBI_MALLOC_FAILED;
            goto the_exit;
        }
    }

    ODBCDR_RDBI_ERR( col_list_addColumn_S( cursor_U->bound_geometries,
                                          position_I,
                                          (pIGeometry_def *) address_I ) );
    rdbi_status = RDBI_SUCCESS;

  the_exit:
    return( rdbi_status );
}


/**************************************************************************
 *  Define a geometry column.  Adds entry to cursor's list of defined
 *  geometries.
 *  This does NOT actually allocate the conversion buffer.
 *  See odbcdr_geom_setNumRows().
 */

int odbcdr_geom_defineColumn(
	odbcdr_context_def	*context,
    odbcdr_cursor_def *      cursor_U,
    int                     position_I,
    char *                  address_I
    )
{
	pIGeometry_def			*pUserAddr = (pIGeometry_def *)address_I;
    int                     rdbi_status = RDBI_GENERIC_ERROR;

    if ( cursor_U == NULL ||
         position_I < 1 ||
         address_I == NULL )
    {
        goto the_exit;
    }

    if ( cursor_U->defined_geometries == NULL )
    {
        cursor_U->defined_geometries = col_list_create_S();
        
        if ( cursor_U->defined_geometries == NULL )
        {
            rdbi_status = RDBI_MALLOC_FAILED;
            goto the_exit;
        }
    }

    ODBCDR_RDBI_ERR( col_list_addColumn_S( cursor_U->defined_geometries,
                                          position_I,
                                          (pIGeometry_def *) address_I ) );
	*pUserAddr = NULL;
	//memset(address_I, '\0', sizeof(pIGeometry_def));

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    return( rdbi_status );
}


/**************************************************************************
 *  Allocate the conversion buffers for any bound or defined geometries
 *  related to the given cursor.  This should happen before any 
 *  'execute' or 'fetch' operations.  In fact, it is recommended to do this
 *  once with a large maximum size at bind/define time, then again when
 *  needed with the actual size.  This assures that the buffer will not
 *  need reallocation, and hence will not need rebinding or redefining.
 */

int odbcdr_geom_setNumRows(
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

    if ( cursor_U->bound_geometries != NULL )
    {
        ODBCDR_RDBI_ERR( col_list_setNumRows_S( cursor_U->bound_geometries,
                                               numRows_I ) );
    }

    if ( cursor_U->defined_geometries != NULL )
    {
        ODBCDR_RDBI_ERR( col_list_setNumRows_S( cursor_U->defined_geometries,
                                               numRows_I ) );
    }

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    return( rdbi_status );
}


/**************************************************************************
 *  Get SqlServer conversion buffer address.
 *  This would be used as the substitute address for a 'bind' or 'define'
 *  operation, rather than the caller-provided address of a pIGeometry_def* buffer.
 */

int odbcdr_geom_getSqlServerBuffAddr(
	odbcdr_context_def	*context,
    odbcdr_geom_col_list_def *columnList_I,
    int                      position_I,
    odbcdr_geom_def        ***buffer_O,
    odbcdr_geomNI_def      ***ni_O
    )
{
    int                      rdbi_status = RDBI_GENERIC_ERROR;
    odbcdr_geom_col_def      *column;
    odbcdr_geom_def         **sqlserverGeom;
    odbcdr_geomNI_def       **sqlserverGeomNI;

    column = col_list_getColumnByPosition_S( columnList_I, position_I );
    if ( column == NULL ) goto the_exit;

    sqlserverGeom = (odbcdr_geom_def **) ut_da_get( &column->geom_list, 0L );
    if ( sqlserverGeom == NULL ) goto the_exit;

    sqlserverGeomNI = (odbcdr_geomNI_def **) ut_da_get( &column->geomNI_list, 0L );
    if ( sqlserverGeomNI == NULL ) goto the_exit;

    *buffer_O = sqlserverGeom;
    *ni_O = sqlserverGeomNI;

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    return( rdbi_status );
}


/**************************************************************************
 *  Convert bound geometry columns from FdoGeometry to SqlServer.
 *  Caller-provided pIGeometry_def buffers will be converted to odbcdr-provided
 *  buffers.  This would be called just before the 'execute'
 *  of any 'update' or 'insert' SQL verbs.
 */

int odbcdr_geom_convertBoundToSqlServer(
	odbcdr_context_def	*context,
    odbcdr_cursor_def        *cursor_U,
    int                      numRows_I
    )
{
    odbcdr_geom_col_list_def *columnList = NULL;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    if ( cursor_U == NULL )
        goto the_exit;

    columnList = cursor_U->bound_geometries;

    rdbi_status = geom_convert_S( context, cursor_U, columnList, numRows_I, CONVERSION_TO_SQLSERVER, 0, -1 );

  the_exit:
    return( rdbi_status );
}


/**************************************************************************
 *  Convert defined geometry columns from SqlServer to FdoGeometry.
 *  odbcdr-provided  buffers will be converted to caller-provided
 *  pIGeometry_def buffers.  This would be called just after the 'execute'
 *  or 'fetch' of an SQL 'select' verb.
 */

int odbcdr_geom_convertDefinedFromSqlServer(
	odbcdr_context_def		*context,
    odbcdr_cursor_def        *cursor_U,
    int                      numRows_I
    )
{
    odbcdr_geom_col_list_def *columnList = NULL;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    if ( cursor_U == NULL )
        goto the_exit;

    columnList = cursor_U->defined_geometries;

    rdbi_status = geom_convert_S( context, cursor_U, columnList, numRows_I, CONVERSION_FROM_SQLSERVER, 0L, -1 );

  the_exit:
    return( rdbi_status );
}


/**************************************************************************
 *  Free SqlServer Geometries
 *  This will free any SqlServer Geometry or intermediate objects in 
 *  the conversion buffers that are stored in the cursor structure.
 *  It will leave the pointer array buffers intact.
 */

int odbcdr_geom_freeSqlServerGeometries(
	odbcdr_context_def		*context,
    odbcdr_cursor_def       *cursor_U
    )
{
    int                      rdbi_status = RDBI_GENERIC_ERROR;
    bool                     freeingStatus = TRUE;

    if ( cursor_U == NULL ) goto the_exit;

    if ( cursor_U->bound_geometries != NULL )
    {
        freeingStatus &= col_list_freeSqlServerGeometries_S(context,
                                        cursor_U->bound_geometries );
    }

    if ( cursor_U->defined_geometries != NULL )
    {
        freeingStatus &= col_list_freeSqlServerGeometries_S(context,
                                        cursor_U->defined_geometries );
    }

    if ( freeingStatus )
    {
        rdbi_status = RDBI_SUCCESS;
    }

  the_exit:
    return( rdbi_status );
}


/**************************************************************************
 *  Free all columns.
 *  This will free the conversion buffers for geometries that have been
 *  stored in the cursor structure.
 */

int odbcdr_geom_freeAllColumns(
	odbcdr_context_def	*context,
    odbcdr_cursor_def   *cursor_U
    )
{
    int                      rdbi_status = RDBI_GENERIC_ERROR;
    bool                     freeingStatus = TRUE;

    if ( cursor_U == NULL ) goto the_exit;

    if ( cursor_U->bound_geometries != NULL )
    {
        freeingStatus &= col_list_free_S( context, cursor_U->bound_geometries, FALSE );

        free( (char *) cursor_U->bound_geometries );

        cursor_U->bound_geometries = NULL;
    }

    if ( cursor_U->defined_geometries != NULL )
    {
        freeingStatus &= col_list_free_S( context, cursor_U->defined_geometries, TRUE );

        free( (char *) cursor_U->defined_geometries );

        cursor_U->defined_geometries = NULL;
    }

    if ( freeingStatus )
    {
        rdbi_status = RDBI_SUCCESS;
    }

  the_exit:
    return( rdbi_status );
}


/**************************************************************************/

static int
geom_convert_S(
	odbcdr_context_def		*context,
    odbcdr_cursor_def        *cursor,	       
    odbcdr_geom_col_list_def *columnList,
    int                      numRows_I,
    int                      conversionCode,
    long                     srid,
    int                      wantedDim
    )
{
	SQLSMALLINT			     boundParmID;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    debug_on4( "odbcdr_geom:geom_convert_S", "%d columns, %d rows, code=%d (%s)",
        columnList==NULL ? 0 : columnList->size,
        numRows_I,
        conversionCode,
        conversionCode==CONVERSION_TO_SQLSERVER ? "to" : "from" );

    if ( numRows_I < 1 ) goto the_exit;

    if ( columnList != NULL )
    {
        long                 i;

        for ( i = 0;  i < columnList->size;  i++ )
        {
            long                 j;
            odbcdr_geom_def     **sqlserverGeom;
            odbcdr_geomNI_def   **sqlserverGeomNI;
            pIGeometry_def      *visionGeom;    /* "FdoGeometry" from/to caller.    */
			pIGeometry_def       *l_visionGeom;   
            odbcdr_geom_col_def  *column;

            column = col_list_getColumnByIndex_S( columnList, i );
            if ( column == NULL || column->geom_list.size < numRows_I )
                goto the_exit;

            sqlserverGeom = (odbcdr_geom_def **)ut_da_get( &column->geom_list, 0L );
            sqlserverGeomNI = (odbcdr_geomNI_def **)ut_da_get( &column->geomNI_list, 0L );
            visionGeom = column->address;
			l_visionGeom = column->l_address;

            if ( sqlserverGeom == NULL || sqlserverGeomNI == NULL || visionGeom == NULL ) goto the_exit;

            // Process each geometry in the dataset. The geometries are bound.
            // In case a large geometry is hit (data truncation), the geometries are unbound and 
            // the current geometry is fetched again as a blob.
            // After the entire batch is processed the geometries are bound again.
            int  unbound = false; 

            for ( j = 0;  j < numRows_I;  j++ )
            {
                debug2( "converting geometry column pos '%d', row %d",
                        column->position, j );

                if ( conversionCode == CONVERSION_TO_SQLSERVER )
                {
                    ODBCDR_RDBI_ERR( geom_convertToSqlServer_S( context, 
                                                            cursor,
                                                            column->position,
                                                            visionGeom[j],
                                                            srid, 
                                                            wantedDim,
                                                            &sqlserverGeom[j],
                                                            &sqlserverGeomNI[j])); 
                }
                else    /* conversionCode == CONVERSION_FROM_SQLSERVER */
                {

                    IGeometry_Release(visionGeom[j]);
                    visionGeom[j] = NULL;

                    // Check the status of this geometry.
                    int geomValid = geom_checkFetchStatusRow_S( cursor, i, j );

                    // Unbind if not already unbound.
                    if ( !geomValid && !unbound)
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

				    /* Remember the last geometry fetched in order to be released when the cursor is freed*/
                    ODBCDR_RDBI_ERR( geom_convertFromSqlServer_S( context, 
                                                              cursor,
                                                              column->position,
                                                              i,
                                                              j,
                                                              sqlserverGeom[j], // Not used
                                                              sqlserverGeomNI[j],
                                                              wantedDim,
                                                              geomValid,
                                                              &visionGeom[j],
														      &l_visionGeom[j] ));
                }
            }

            // Rebind back if the case.            
            if ( (conversionCode == CONVERSION_FROM_SQLSERVER ) && unbound )
            {
                int offset = i * ODBCDR_MAX_ARRAY_SIZE;
                ODBCDR_ODBC_ERR( SQLBindCol( cursor->hStmt,
				            (SQLUSMALLINT) column->position,
				            (SQLSMALLINT) SQL_C_BINARY,
                            (SQLPOINTER) (char *)&cursor->odbcdr_geom[offset * ODBCDR_BLOB_CHUNK_SIZE],
                            (SQLINTEGER) ODBCDR_BLOB_CHUNK_SIZE,
				            (SQLLEN *) (char *)&cursor->odbcdr_geomNI[offset]),
		            SQL_HANDLE_STMT, cursor->hStmt,
		            "SQLBindCol", "rebind" );   
            }   

        }   /* end for (i < columnList->size) */
		
		// Make final SQLParamData call. SQLParamData executes the SQL and
		// can return errors like SQLExecute so, check for errors.

		if ( conversionCode == CONVERSION_TO_SQLSERVER ) {
			int rc;

			rc = SQLParamData(cursor->hStmt, (SQLPOINTER*)&boundParmID);
			if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
			{
				rdbi_status = odbcdr_xlt_status( context, rc, SQL_HANDLE_STMT, cursor->hStmt);
				context->odbcdr_last_rc = rc;
				goto the_exit;
			}
		}
    }   /* end if (columnList != NULL) */

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    debug_return( NULL, rdbi_status );
}


/**************************************************************************/

static odbcdr_geom_col_list_def  initialisedColumnList_S =
{   sizeof(odbcdr_geom_col_def), NULL, 0L, 0L };


/**************************************************************************/

static odbcdr_geom_col_list_def *
col_list_create_S(
    void
    )
{
    odbcdr_geom_col_list_def *   newColumnList = NULL;

    newColumnList = (odbcdr_geom_col_list_def *) ut_vm_malloc(
                        "odbcdr_geom:col_list_create_S",
                        sizeof(odbcdr_geom_col_list_def) );

    if ( newColumnList != NULL )
    {
        *newColumnList = initialisedColumnList_S;
    }

    return( newColumnList );
}


/**************************************************************************/

static odbcdr_geom_col_def   initialisedColumn_S = {
    0,                                              /* position         */
    NULL,                                           /* address          */
	NULL,
    { sizeof(odbcdr_geom_def *), NULL, 0L, 0L },     /* geom_list        */
    { sizeof(odbcdr_geomNI_def *), NULL, 0L, 0L }    /* geomNI_list      */
};


/**************************************************************************/

static int
col_list_addColumn_S(
    odbcdr_geom_col_list_def *list,
    int                      position_I,
    pIGeometry_def          *address_I
    )
{
    odbcdr_geom_col_def       newColumn = initialisedColumn_S;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    debug_on2( "odbcdr_geom:col_list_addColumn_S", "pos %d, address 0x%lx",
                                            position_I, (long) address_I );

    if ( list == NULL || position_I < 1 || address_I == NULL ) goto the_exit;

    newColumn.position = position_I;
    newColumn.address  = address_I;

    if ( ut_da_append( list, 1L, (void *) &newColumn ) == NULL ) {
        rdbi_status = RDBI_MALLOC_FAILED;
        goto the_exit;
    }

    /* Note that the buffer itself (newColumn.geom_list) is left empty.
     * We do not know the number of rows affected (it could be an array
     * operation), so the buffer will be allocated as needed with a call
     * to odbcdr_geom_setNumRows().
     */

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    debug_return( NULL, rdbi_status );
}


/**************************************************************************/

static bool
col_list_freeSqlServerGeometries_S(
	odbcdr_context_def	*context,
    odbcdr_geom_col_list_def *list
    )
{
    long                     i, j;
    bool                     status = true;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    debug_on1( "odbcdr_geom:col_list_freeSqlServerGeometries_S", "%ld columns",
                (list==NULL)? -1 : list->size );

    if ( list == NULL ) goto the_exit;

    for ( i = 0;  i < list->size;  i++ ) {

        odbcdr_geom_col_def  *column = col_list_getColumnByIndex_S( list, i );

        for ( j = 0; j < column->geom_list.size; j++ )
        {
			odbcdr_geom_def *geom = *((odbcdr_geom_def **)ut_da_get( &column->geom_list, j ));
			odbcdr_geomNI_def *geomNI = *((odbcdr_geomNI_def **)ut_da_get( &column->geomNI_list, j ));
        }

        /* Just zero out the pointers. */
        debug1( "Zeroing out pointers for %ld geometries.",
                column->geom_list.size );
        (void) memset( column->geom_list.data, 0,
             (size_t) column->geom_list.el_size * column->geom_list.size );
        (void) memset( column->geomNI_list.data, 0,
             (size_t) column->geomNI_list.el_size * column->geomNI_list.size );
    }

    rdbi_status = RDBI_SUCCESS;

  the_exit:

    status &= ( rdbi_status == RDBI_SUCCESS );
    
    debug_return( NULL, status );
}
/**************************************************************************/

static bool
col_list_free_S(
	odbcdr_context_def	*context,
    odbcdr_geom_col_list_def *list,
    bool release_geoms
    )
{
    long                     i;
    bool                     status = true;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    debug_on1( "odbcdr_geom:col_list_free_S", "%ld columns",
                (list==NULL)? -1 : list->size );

    if ( list == NULL ) goto the_exit;

    if ( !col_list_freeSqlServerGeometries_S( context, list ) ) goto the_exit;

    for ( i = 0;  i < list->size;  i++ ) {

        odbcdr_geom_col_def  *column = col_list_getColumnByIndex_S( list, i );

        /* Free the arrays themselves. */
        debug1( "Freeing pointers for %ld geometries.",
                column->geom_list.size );

        if ( release_geoms ) {
			for ( int j = 0; j < column->geom_list.size; j++ ) {
   				IGeometry_Release( column->l_address[j] );	
			}
        }

		ut_vm_free( _db_function, column->l_address );

        status &= ut_da_free( &(column->geom_list) );
        status &= ut_da_free( &(column->geomNI_list) );
    }

    rdbi_status = RDBI_SUCCESS;

    debug1( "Freeing %ld geometry columns.", list->size );
    status &= ut_da_free( list ); 

  the_exit:

    status &= ( rdbi_status == RDBI_SUCCESS );
    
    debug_return( NULL, status );
}

/**************************************************************************/

static int
col_list_setNumRows_S(
    odbcdr_geom_col_list_def *list,
    int                      numRows_I
    )
{
    long                     i;
    int                      rdbi_status = RDBI_GENERIC_ERROR;

    debug_on1( "odbcdr_geom:col_list_setNumRows_S", "numRows %d", numRows_I );

    if ( list == NULL || numRows_I < 1 ) goto the_exit;

    for ( i = 0;  i < list->size;  i++ ) {

        odbcdr_geom_col_def  *column = col_list_getColumnByIndex_S( list, i );

        if ( !ut_da_presize( &column->geom_list, numRows_I ) ) {
            rdbi_status = RDBI_MALLOC_FAILED;
            goto the_exit;
        }

        if ( !ut_da_presize( &column->geomNI_list, numRows_I ) ) {
            rdbi_status = RDBI_MALLOC_FAILED;
            goto the_exit;
        }

		if ( column->l_address == NULL ) {
			column->l_address = (pIGeometry_def *) malloc (numRows_I * sizeof(pIGeometry_def));
			if ( column->l_address == NULL ) {
				rdbi_status = RDBI_MALLOC_FAILED;
				goto the_exit;
			}
			memset( column->l_address, '\0', numRows_I * sizeof(pIGeometry_def));
		}
    }

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    debug_return(NULL, rdbi_status );
}


/**************************************************************************/

static odbcdr_geom_col_def *
col_list_getColumnByIndex_S(
    odbcdr_geom_col_list_def *list,
    long                     indx
    )
{
    odbcdr_geom_col_def      *column = NULL;

    column = (odbcdr_geom_col_def *) ut_da_get( list, indx );

    return column;
}


/**************************************************************************/

static odbcdr_geom_col_def *
col_list_getColumnByPosition_S(
    odbcdr_geom_col_list_def *list,
    int                      position_I
    )
{
    long                     i;
    odbcdr_geom_col_def      *column = NULL;

    if ( list != NULL )
    {
        odbcdr_geom_col_def  *searchCol;
        int                  found = FALSE;

        for ( i = 0;  !found && i < list->size;  i++ )
        {
            searchCol = (odbcdr_geom_col_def *) ut_da_get( list, i );
            if ( searchCol != NULL && searchCol->position == position_I )
            {
                found = TRUE;
                column = searchCol;
            }
        }
    }

    return column;
}


/**************************************************************************/

static int
geom_convertFromSqlServer_S(
	odbcdr_context_def		*context,
    odbcdr_cursor_def       *cursor,
    int                     position,
    int                     geomNo,
    int                     recNo,
    odbcdr_geom_def         *sqlserverGeom_I,
    odbcdr_geomNI_def       *sqlserverGeomNI_I,
    int                     wantedDim,
    int                     geomValid,
    pIGeometry_def          *visionGeom_O,
	pIGeometry_def		    *l_visionGeom_0
    )
{
    odbcdr_connData_def	    *connData = NULL;
    int                     rdbi_status = RDBI_GENERIC_ERROR;
    PBYTE                   pData = NULL;
    BYTE                    test[1];
    pByteArray_def          fgf = NULL;
    SQLLEN                  count;
    SQLRETURN               rc;

    debug_on( "odbcdr_geom:geom_convertFromSqlServer_S" );

    // Bound arrays offset 
    int offset = geomNo * ODBCDR_MAX_ARRAY_SIZE + recNo;

    if ( geomValid )
    {
        if ( cursor->odbcdr_geomNI[offset] == SQL_NULL_DATA)
        {
            *visionGeom_O = NULL;
            debug0( "Geometry is NULL." );
            rdbi_status = RDBI_SUCCESS;
            goto the_exit;
        }

        debug0( "Geometry is not NULL." );

        pData = (PBYTE)&cursor->odbcdr_geom[offset * ODBCDR_BLOB_CHUNK_SIZE];
     
        count = cursor->odbcdr_geomNI[offset];
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
            *visionGeom_O = NULL;
            debug0( "Geometry is NULL." );
            rdbi_status = RDBI_SUCCESS;
            goto the_exit;
        }

        debug0( "Geometry is not NULL." );

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
        cursor->odbcdr_geomNI[offset] = lenOrIndex;
    }

    // Create a byte array from the array of bytes.
    if ( NULL == ( fgf = IByteArray_Create( pData, count) ) )
        goto the_exit;

    // Create the geometry
    if ( !IGeometry_CreateGeometryFromFgf( fgf, visionGeom_O, l_visionGeom_0 ) )
    {
		rdbi_status = RDBI_GEOMETRY_CONVERION_ERROR;
        goto the_exit;
	}

	// Release intermediate data
	IByteArray_Release( fgf );

    debug_area() {
        const wchar_t * fgft;
        if ( IGeometry_GetFgftString(*visionGeom_O, &fgft)) {
            debug1( "FgftGeometry = '%ls'", fgft );
        }
    }

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    debug_return( NULL, rdbi_status );
}


/**************************************************************************/

static int
geom_convertToSqlServer_S(
	odbcdr_context_def		*context,
    odbcdr_cursor_def       *cursor,
    int                     position,
    pIGeometry_def          visionGeom_I,
    long                    srid,
    int                     wantedDim,
    odbcdr_geom_def         **sqlserverGeom_O,
    odbcdr_geomNI_def       **sqlserverGeomNI_O
    )
{
    static char             *funcName = "odbcdr_geom:geom_convertToSqlServer_S";
    odbcdr_connData_def	    *connData = NULL;
    pIGeometry_def          gissdoGeom = NULL;
	pByteArray_def          fgf = NULL;
    PBYTE                   pData; // It's the "data" not the FdoByteArray
    int                     rdbi_status = RDBI_GENERIC_ERROR;
    SQLSMALLINT             boundParmID;
    SDWORD                  count;
    SQLRETURN               rc;

    debug_on( funcName );

    *sqlserverGeom_O = NULL;
    *sqlserverGeomNI_O = NULL;

    // Get the akwb.
    if (NULL != visionGeom_I)
    {
        debug_area() {
            const wchar_t *fgft;
            if ( IGeometry_GetFgftString(visionGeom_I, &fgft) ) {
                debug1( "FgfdGeometry = '%ls'", fgft );
            }
        }

        // Get the ByteArray
        if ( !IGeometry_GetFgf( visionGeom_I, &fgf ) )
        {
			rdbi_status = RDBI_GEOMETRY_CONVERION_ERROR;
            goto the_exit;
		}

        // Get the data, as a regular array of bytes
        if ( !IGeometry_GetByteArrayData( fgf,  &pData, (int*)&count) )
		{
			rdbi_status = RDBI_GEOMETRY_CONVERION_ERROR;
            goto the_exit;
		}

        // Set output as a byte array but we'll store it as byte[]
        *sqlserverGeom_O = (PBYTE *)fgf;
	}
    else
    {
        debug0( "Geometry is NULL" );
		pData = NULL;
		count = 0;
        *sqlserverGeom_O = NULL;
    }

	// Effectively store data. Assumes it is called after an Execute.  
    rc = SQLParamData(cursor->hStmt, (SQLPOINTER*)&boundParmID);

    if ( rc == SQL_NEED_DATA ) {
        
        // Store the bytes in one chunk
        ODBCDR_ODBC_ERR( SQLPutData( cursor->hStmt, 
                            (SQLPOINTER) pData, 
                            count ),
                            SQL_HANDLE_STMT, cursor->hStmt,
					        "SQLPutData", "putGeometry" ) ;

    }

	// Release intermediate data
	IByteArray_Release( fgf );

    rdbi_status = RDBI_SUCCESS;

  the_exit:
    debug_return( NULL, rdbi_status );
}

/**************************************************************************/
static int
geom_checkFetchStatusRow_S (
    odbcdr_cursor_def   *cursor,
    int                 idxGeom_I,
    int                 numRow_I
    )
{
    int             status = true;
    SQLLEN      nullInd = cursor->odbcdr_geomNI[ idxGeom_I * ODBCDR_MAX_ARRAY_SIZE + numRow_I];

    if ( nullInd == SQL_NULL_DATA)
        status = true;
    else if ( nullInd >= ODBCDR_BLOB_CHUNK_SIZE )
        status = false;

    return status;
}
