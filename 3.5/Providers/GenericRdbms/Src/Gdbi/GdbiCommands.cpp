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
 *
 *
 */
#include "stdafx.h"
#include "GdbiCommands.h"
#include "FdoCommonOSUtil.h"
#ifdef HAVE_GEOM_INFO_TYPE
#include <inc/geometry/fgf/AllGeometry_c.h>
#endif

GdbiCommands::GdbiCommands( rdbi_context_def* rdbi_context ):
m_pRdbiContext( rdbi_context )
{
    mFeatureSeq.size = 0;
    mFeatureSeq.next = 0;

	rdbi_vndr_info_def info;
	rdbi_vndr_info( m_pRdbiContext, &info );
	mArraySize = info.maxFetchSize;
}

GdbiCommands::~GdbiCommands(void)
{
}

void GdbiCommands::CheckDB()
{
    if( m_pRdbiContext== NULL || m_pRdbiContext->rdbi_cnct == NULL || ! m_pRdbiContext->rdbi_cnct->in_use )
            throw GdbiException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));
    if( m_pRdbiContext->last_error_msg != NULL )
	{
		free( m_pRdbiContext->last_error_msg );
		m_pRdbiContext->last_error_msg = NULL;
	}
}

void GdbiCommands::ThrowException()
{
    if( m_pRdbiContext->last_error_msg == NULL )
        ::rdbi_get_msg (m_pRdbiContext);
    
    throw GdbiException::Create( m_pRdbiContext->last_error_msg, m_pRdbiContext->rdbi_last_status);
}

int GdbiCommands::err_stat()
{
	return m_pRdbiContext->rdbi_last_status;
}

int GdbiCommands::run_sql( FdoStringP sql, bool isDDL,  int *rows_processed  )
{
    CheckDB();
    int rc = RDBI_GENERIC_ERROR;

	if( SupportsUnicode() )
        rc = ::rdbi_run_sqlW (m_pRdbiContext, sql, isDDL ? 1 : 0, rows_processed );
	else
		rc = ::rdbi_run_sql (m_pRdbiContext, sql, isDDL ? 1 : 0, rows_processed );

    if( rc == RDBI_SUCCESS )
        return rc;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::sql( FdoStringP sql,  int *qid  )
{
    CheckDB();
    int rc = RDBI_GENERIC_ERROR;

    rc = ::rdbi_est_cursor (m_pRdbiContext, qid);

    if( rc == RDBI_SUCCESS )
	{
		if( SupportsUnicode() )
			rc = ::rdbi_sqlW (m_pRdbiContext, *qid, sql );
		else
			rc = ::rdbi_sql (m_pRdbiContext, *qid, sql );
	}

    if( rc == RDBI_SUCCESS )
        return rc;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::execute( int qid, int count, int offset )
{
    CheckDB();
    int rc = ::rdbi_execute (m_pRdbiContext, qid, count, offset);

    if(  rc == RDBI_SUCCESS )
        return rc;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::free_cursor( int qid  )
{
	if( m_pRdbiContext == NULL )
		return RDBI_SUCCESS;  // This is called by a cleanup code invoked after the connection is closed

    CheckDB();

    if( ::rdbi_fre_cursor (m_pRdbiContext, qid) == RDBI_SUCCESS )
        return RDBI_SUCCESS;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

void GdbiCommands::set_null( GDBI_NI_TYPE *null_ind, int start, int end)
{
    ::rdbi_set_null(m_pRdbiContext, (void *)null_ind, start, end);
}

int GdbiCommands::alcnullind(int n, GDBI_NI_TYPE **null_ind)
{
	int rc = rdbi_alcnullind(m_pRdbiContext, n, (char **)null_ind);
	if (rc != RDBI_SUCCESS)
		ThrowException();
	return (rc == RDBI_SUCCESS);
}

void GdbiCommands::set_nnull( GDBI_NI_TYPE *null_ind, int start, int end)
{
    ::rdbi_set_nnull(m_pRdbiContext, (void *)null_ind, start, end);
}

int GdbiCommands::is_null( GDBI_NI_TYPE *null_ind, int offset )
{
    return ::rdbi_is_null(m_pRdbiContext, (void *)null_ind, offset );
}

int GdbiCommands::bind(
    int   cursorId,        /* index into cursor array                  */
    char *name,         /* column/expression position               */
    int   datatype,     /* A data type from Inc/rdbi.h              */
    int   size,         /* binary size                              */
    char *address,      /* data address                             */
    GDBI_NI_TYPE *null_ind
    )
{
	int   loc_datatype = datatype;
    int   loc_size = size;
    char *loc_address = address;

    CheckDB();

	if( datatype == RDBI_WSTRING && ! SupportsUnicode() )
	{
		throw new GdbiException(L"Cannot bind widechar strings; target RDBMS does not support widechar strings");
	}

    if( ::rdbi_bind(m_pRdbiContext, cursorId, name, loc_datatype,  loc_size, loc_address, (void *)null_ind) == RDBI_SUCCESS )
        return RDBI_SUCCESS;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::tran_begin( char    *tran_id )
{
    CheckDB();

    if( ::rdbi_tran_begin( m_pRdbiContext, tran_id) )
        return RDBI_SUCCESS;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::tran_end( char    *tran_id )
{
    CheckDB();

    if( ::rdbi_tran_end( m_pRdbiContext, tran_id ) )
        return RDBI_SUCCESS;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::tran_rolbk( )
{
    return ::rdbi_tran_rolbk( m_pRdbiContext );
}

int GdbiCommands::define(
        int   sqlid,        /* index into cursor array                  */
        char *name,         /* column/expression position               */
        int   datatype,     /* A data type from Inc/rdbi.h              */
        int   size,         /* binary size                              */
        char *address,      /* data address                             */
        GDBI_NI_TYPE *null_ind  /* pointer to null indicator variables      */
    )
{
    CheckDB();

    if( ::rdbi_define(m_pRdbiContext, sqlid, name, datatype, size, address, (void *)null_ind ) == RDBI_SUCCESS )
        return RDBI_SUCCESS;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::desc_slct(
        int   sqlid,        /* Index into ptrs to Oracle work areas */
        int   pos,          /* position within select clause        */
        int   name_len,     /* The maximum size of name             */
        wchar_t *name,         /* Name of this field                   */
        int  *rdbi_type,    /* rdbi constant                        */
        int  *binary_size,  /* bytes of memory to store the field   */
        int  *null_ok )
{
    CheckDB();
	int rc;
	if( this->SupportsUnicode() )
		rc = ::rdbi_desc_slctW(m_pRdbiContext, sqlid, pos, name_len, name, rdbi_type, binary_size, null_ok );
    else
	{
		char colName[GDBI_COLUMN_SIZE];
		rc = ::rdbi_desc_slct(m_pRdbiContext, sqlid, pos, name_len, colName, rdbi_type, binary_size, null_ok );
		wcsncpy(name,(const wchar_t*)FdoStringP( colName ), name_len );
		name[name_len-1]='\0';
	}
    if(  rc == RDBI_SUCCESS || rc == RDBI_NOT_IN_DESC_LIST )
        return rc;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::end_select( int     sqlid )
{
	if( m_pRdbiContext == NULL )
		return RDBI_SUCCESS;  // This is called by a cleanup code invoked after the connection is closed

    CheckDB();

    if( ::rdbi_end_select( m_pRdbiContext, sqlid ) == RDBI_SUCCESS )
        return RDBI_SUCCESS;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::fetch( int  sqlid, int  count, int *rows_processed )
{
    CheckDB();

    int rc = ::rdbi_fetch( m_pRdbiContext, sqlid, count, rows_processed );
    if(  rc == RDBI_SUCCESS || rc == RDBI_END_OF_FETCH )
        return rc;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

int GdbiCommands::crsr_nrows(  int sqlid )
{
    return ::rdbi_crsr_nrows( m_pRdbiContext, sqlid );
}

int GdbiCommands::vndr_info(
    rdbi_vndr_info_def *info
)
{
    int rc = ::rdbi_vndr_info( m_pRdbiContext, info );
    if(  rc == RDBI_SUCCESS  )
        return rc;

    ThrowException();
    return RDBI_GENERIC_ERROR; // to supress a compiler warning
}

//
// Use the RDBMS sequence number capability(i.e Oracle )
// TODO: need to cache the sequences for all sequences. Currently it will only work with one sequence for feature ids
long GdbiCommands::NextSequenceNumber(  FdoString* dbiSequenceName )
{
    FdoString* dbiSeqName = dbiSequenceName;
    FdoString* adbSeqName;

    if( dbiSequenceName == NULL || dbiSequenceName[0] == '\0' )
        dbiSeqName = DBI_FEATURE_SEQUENCEW;

    if ( wcscmp( dbiSeqName, DBI_FEATURE_SEQUENCEW ) == 0 )
        adbSeqName = ADB_FEATURESEQW;
    else if ( wcscmp( dbiSeqName, DBI_CLASS_SEQUENCEW ) == 0 )
        adbSeqName = ADB_CLASSSEQW;
    else if ( wcscmp( dbiSeqName, DBI_PLAN_SEQUENCEW ) == 0 )
        adbSeqName = ADB_PLANSEQW;
    else if ( wcscmp( dbiSeqName, DBI_PLAN_GROUP_SEQUENCEW ) == 0 )
        adbSeqName = ADB_PLANGROUPSEQW;
    else // Todo: Throw exception.
        adbSeqName = dbiSeqName;

    // Call specific provider implementation to get a number
    if( m_pRdbiContext->dispatch.capabilities.supports_sequence )
        return NextRDBMSSequenceNumber( adbSeqName );
    else if ( m_pRdbiContext->dispatch.capabilities.supports_autoincrement )
        return NextRDBMSAutoincrementNumber( adbSeqName );
    else
        return NextGDBISequenceNumber( adbSeqName );
}

//
// In case sequence is not supported the method simulate a sequence allocation scheme.
// The caller should start a transaction if one is not started. Apply Schema does.

long GdbiCommands::NextGDBISequenceNumber( FdoString* adbSequenceName )
{
    bool                rc = false; 
    FdoStringP          strUse;
    int                 cursor;
    int                 select_begun = FALSE;
    int                 number = 0;
    int                 rows_proc;
    double              doubleVal;
    gdbi_full_seq_def   *gptr = &mFeatureSeq;
    CheckDB();

    if (gptr->next < gptr->size)
    {
        number = gptr->sequence[gptr->next];
        gptr->next++;
        rc = true;
        goto the_exit;
    }

    /* build SQL update command */
    strUse = FdoStringP::Format( L"update %ls set startnum=startnum+%d where seqid='%ls';", ADB_SEQUENCE_TABLEW, ADB_SN_ALLOC_INCREMENT, adbSequenceName);

    if( sql( strUse, &cursor ) == RDBI_SUCCESS )
        this->execute( cursor );

    if( cursor != - 1 )
        this->free_cursor( cursor );

     /* build SQL update command */
    strUse = FdoStringP::Format( L"select startnum from %ls where seqid='%ls';", ADB_SEQUENCE_TABLEW, adbSequenceName);

    if( ::rdbi_est_cursor(m_pRdbiContext, &cursor) != RDBI_SUCCESS )
        goto the_exit;

    if (SupportsUnicode())
    {
        if( ::rdbi_sql_vaW( m_pRdbiContext, RDBI_VA_EXEC, cursor, strUse,
                                /* bind variables */
                                RDBI_VA_EOL,
                                /* define variables */
                                RDBI_DOUBLE, 0, &doubleVal,
                                RDBI_VA_EOL ) != RDBI_SUCCESS )
                    goto the_exit;
    }
    else
    {
        if( ::rdbi_sql_va( m_pRdbiContext, RDBI_VA_EXEC, cursor, strUse,
                                /* bind variables */
                                RDBI_VA_EOL,
                                /* define variables */
                                RDBI_DOUBLE, 0, &doubleVal,
                                RDBI_VA_EOL ) != RDBI_SUCCESS )
                    goto the_exit;
    }
    if( ::rdbi_fetch (m_pRdbiContext, cursor, 1, &rows_proc) != RDBI_SUCCESS )
            goto the_exit;

    if(rows_proc == 0) goto the_exit;

    number = (long)doubleVal;
    gptr->size = ADB_SN_ALLOC_INCREMENT;
    for(int i=0; i<ADB_SN_ALLOC_INCREMENT;i++)
        gptr->sequence[i] = number--;

    gptr->next = 0;
    select_begun = TRUE;
    number = gptr->sequence[gptr->next];
    gptr->next++;

    rc = true;
the_exit:
    if (select_begun)
        rdbi_end_select(m_pRdbiContext, cursor);

    if( ! rc )
        ThrowException();

    return number;
}

// In case sequence is not supported this method simulate a sequence allocation scheme.
long GdbiCommands::NextRDBMSAutoincrementNumber( FdoString* adbSequenceName )
{
    FdoStringP tableName;
    int                 number = 0;
    bool                seqSupported = true;

    CheckDB();

    // Get the table name from the sequence name.
    if ( wcscmp( adbSequenceName, ADB_CLASSSEQW ) == 0 )
        tableName = ADB_CLASSDEF_TABW;
    else if ( wcscmp( adbSequenceName, ADB_PLANSEQW ) == 0 )
        tableName = ADB_SPATIAL_CONTEXT_TABLEW;
    else if ( wcscmp( adbSequenceName, ADB_PLANGROUPSEQW ) == 0 )
        tableName = ADB_SPATIAL_CONTEXT_GROUP_TABLEW;
	else if ( wcscmp( adbSequenceName, ADB_FEATURESEQW ) == 0 )
	    tableName = L"";
    else 
        seqSupported = false;

    if ( !seqSupported )
    {
		// Should not happen since we handle all known SEQ cases
		ThrowException();
    }
    else
    {
        if (SupportsUnicode())
        {
            if ( ::rdbi_get_gen_idW ( m_pRdbiContext, tableName, &number ) != RDBI_SUCCESS )
                ThrowException();
        }
        else
        {
            if ( ::rdbi_get_gen_id ( m_pRdbiContext, tableName, &number ) != RDBI_SUCCESS )
                ThrowException();
        }
        // Note: The number is the current generated one.
    }

    return number;
}

long GdbiCommands::NextRDBMSSequenceNumber( FdoString* adbSequenceName )
{

    bool                rc = false;                     /* return code */
    int                 switched_to_2 = FALSE;
    int                 tran_begun = FALSE;
    int                 select_begun = FALSE;
    int                 sqlid_coc = -1;
    FdoStringP          sql_buf;   /* place to format SQL command */
    int                 rows_proc;
    gdbi_full_seq_def   *gptr = &mFeatureSeq;
    long                number = -1;
    int                 CURSOR = -1;

    CheckDB();

    if ( (gptr->next < gptr->size) && (FdoCommonOSUtil::wcsicmp(adbSequenceName, gptr->seq_name) == 0) )
    {
        number = gptr->sequence[gptr->next];
        gptr->next++;
        rc = true;
        goto the_exit;
    }

    if (CURSOR == -1)
    {
        if( ::rdbi_est_cursor(m_pRdbiContext, &CURSOR) != RDBI_SUCCESS )
            goto the_exit;

        select_begun = TRUE;

        /* build SQL select command */
        sql_buf = FdoStringP::Format(L"select %ls.nextval from %ls", adbSequenceName, ADB_SEQUENCE_TABLEW);

        /* add the where clause for other than 'feature' numbers */
        if (FdoCommonOSUtil::wcsicmp(adbSequenceName, ADB_FEATURESEQW) != 0)
            sql_buf += FdoStringP::Format(L" where %ls = '%ls'", ADB_SEQID_COLUMNW, adbSequenceName);
        if (SupportsUnicode())
        {
            if( ::rdbi_sql_vaW( m_pRdbiContext, 0, CURSOR, sql_buf,
                            /* bind variables */
                            RDBI_VA_EOL,
                            /* define variables */
                            RDBI_LONG, 0, gptr->sequence,
                            RDBI_VA_EOL ) != RDBI_SUCCESS )
                goto the_exit;
        }
        else
        {
            if( ::rdbi_sql_va( m_pRdbiContext, 0, CURSOR, sql_buf,
                            /* bind variables */
                            RDBI_VA_EOL,
                            /* define variables */
                            RDBI_LONG, 0, gptr->sequence,
                            RDBI_VA_EOL ) != RDBI_SUCCESS )
                goto the_exit;
        }
    }

    if (   (0 == FdoCommonOSUtil::wcsicmp (adbSequenceName, ADB_FEATURESEQW))
        || (0 == FdoCommonOSUtil::wcsicmp (adbSequenceName, ADB_USERSEQW)))
    {
        /* select next feature ids */
        gptr->size = ADB_SN_ALLOC_INCREMENT;
        if( ::rdbi_exec_fetch(m_pRdbiContext, CURSOR, gptr->size, FALSE, &rows_proc) != RDBI_SUCCESS )
            goto the_exit;

        if(rows_proc == 0) goto the_exit;

        gptr->next = 0;
    }
    else
    {

        if( ::rdbi_exec_fetch(m_pRdbiContext, CURSOR, 1, FALSE, &rows_proc) != RDBI_SUCCESS )
            goto the_exit;

        if(rows_proc == 0) goto the_exit;

        gptr->size = 1;
        if(rows_proc == 0) goto the_exit;
        gptr->next = 0;
    }

    select_begun = TRUE;

    number = gptr->sequence[gptr->next];
    gptr->next++;
    wcscpy( gptr->seq_name, adbSequenceName );

    rc = true;
the_exit:
    if (select_begun) {
        ::rdbi_end_select(m_pRdbiContext, CURSOR);
        ::rdbi_fre_cursor(m_pRdbiContext, CURSOR);
    }

    if( ! rc )
        ThrowException();

    return number;
}

bool GdbiCommands::IsFieldWriteable( FdoStringP tableName, FdoStringP columnName )
{
    bool    isWriteable = true;

    if ( m_pRdbiContext->dispatch.capabilities.supports_autoincrement )
    {
        // All the columns are writeable except INDENTITY/AUTO_INCREMENT columns
        if ( ( wcscmp( tableName, L"f_classdefinition" ) == 0 && wcscmp( columnName, L"classid" ) == 0 ) ||
             ( wcscmp( tableName, L"f_spatialcontext" ) == 0 && wcscmp( columnName, L"scid" ) == 0 ) ||
             ( wcscmp( tableName, L"f_spatialcontextgroup" ) == 0 && wcscmp( columnName, L"scgid" ) == 0 ) )
            isWriteable = false;
    }

    return isWriteable;
}
void GdbiCommands::set_array_size(int arraySize)
{
    rdbi_vndr_info_def info;
	rdbi_vndr_info( m_pRdbiContext, &info );
    if (arraySize <= info.maxFetchSize)
	    mArraySize = arraySize;
}

int GdbiCommands::get_array_size()
{
	return mArraySize;
}
int GdbiCommands::lob_create_ref( int  sqlid, void **lob_ref )
{
	int rc = ::rdbi_lob_create_ref(m_pRdbiContext, sqlid, lob_ref);
	if (rc == RDBI_SUCCESS)
		return rc;

	return RDBI_GENERIC_ERROR;
}
int GdbiCommands::lob_destroy_ref( int sqlid, void *lob_ref )
{
	int rc = ::rdbi_lob_destroy_ref(m_pRdbiContext, sqlid, lob_ref);
	if (rc == RDBI_SUCCESS)
		return rc;

	return RDBI_GENERIC_ERROR;
}
int GdbiCommands::lob_get_size(int sqlid, void *lob_ref, unsigned int *size)
{
	int rc = ::rdbi_lob_get_size(m_pRdbiContext, sqlid, lob_ref, size);
	if (rc == RDBI_SUCCESS)
		return rc;

	return RDBI_GENERIC_ERROR;
}
int GdbiCommands::lob_read_next(int sqlid, void *lob_ref, int rdbi_lob_type, unsigned int block_size, char *block, unsigned int *block_size_out, int *eol)
{
	int rc = ::rdbi_lob_read_next(m_pRdbiContext, sqlid, lob_ref, rdbi_lob_type, block_size, block, block_size_out, eol);
	if (rc == RDBI_SUCCESS)
		return rc;

	return RDBI_GENERIC_ERROR;
}
int GdbiCommands::autocommit_on()
{
	int rc = ::rdbi_autocommit_on(m_pRdbiContext);

	return rc;
}
int GdbiCommands::autocommit_off()
{
	int rc = ::rdbi_autocommit_off(m_pRdbiContext);
	
	return rc;
}

int GdbiCommands::autocommit_mode()
{
	int autoCmt = ::rdbi_autocommit_mode(m_pRdbiContext);

	return autoCmt;
}

int GdbiCommands::geom_to_fgf ( 
    int             sqlid,
    void *          rdbmsGeometryInfo_I,
    int             defaultDim,
    void **         fgfGeometryByteArray_O )
{
#ifdef HAVE_GEOM_INFO_TYPE
    RdbmsGeometryValueInfo * geomInfo2 = (RdbmsGeometryValueInfo *) rdbmsGeometryInfo_I;

    int rc = ::rdbi_geom_to_fgf(m_pRdbiContext, sqlid, geomInfo2, defaultDim, (pByteArray_def *)fgfGeometryByteArray_O);

    return rc;
#else
    return FALSE;
#endif
}

int GdbiCommands::geom_from_fgf ( 
    int             sqlid,
    long            srid,
    void *          fgfGeometryByteArray_I,
    void **         rdbmsGeometryInfo_O)
{
#ifdef HAVE_GEOM_INFO_TYPE
    RdbmsGeometryValueInfo ** geomInfo2 = (RdbmsGeometryValueInfo **) rdbmsGeometryInfo_O;

    (*geomInfo2) = new RdbmsGeometryValueInfo;

    int rc = ::rdbi_geom_from_fgf(m_pRdbiContext, sqlid, srid, (pByteArray_def)fgfGeometryByteArray_I, (*geomInfo2));

    return rc;
#else
    return FALSE;
#endif
}

int GdbiCommands::geom_srid_set(
	int				sqlid,
	char			*geom_col_name,
	long			srid)
{
	int rc = ::rdbi_geom_srid_set(m_pRdbiContext, sqlid, geom_col_name, srid);
	if (rc == RDBI_SUCCESS)
		return rc;

	return RDBI_GENERIC_ERROR;
}
