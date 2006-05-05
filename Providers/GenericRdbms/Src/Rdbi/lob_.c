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
*   rdbi_lob_* - suite of function for handling LOBs  by-reference      *
*                                                                       *
* Synopsis                                                              *
*   rdbi_geom_srid_set( long srid)                                      *
*       srid - input                                                    *                                                                    *
*                                                                       *
*   int rdbi_lob_create_ref( int sqlid, void **lob_ref )                *
*   int rdbi_lob_destroy_ref( int sqlid, void *lob_ref )                *
*   int rdbi_lob_get_size( int sqlid, void *lob_ref, unsigned int *size)*
*   int rdbi_lob_write_next( int sqlid, void *lob_ref, int rdbi_lob_type,*
*                            unsigned int block_size, char* block )     *
*   int rdbi_lob_read_next( int sqlid, void *lob_ref, int rdbi_lob_type,*
*                           unsigned int block_size, char *block,       * 
*                           unsigned int *block_size_out, int *eol )    *
*   int rdbi_lob_open(int sqlid, void *lob_ref, int for_read)           *
*   int rdbi_lob_close( int sqlid, void *lob_ref)                       *
*                                                                       *
* Description                                                           *
*    Calls ocidr related functions with the proper cursor.              *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*                                                                       *
* Remarks                                                               *
*    See dbi_lob_* for more detailed description.                       *
*                                                                       *
************************************************************************/
#include <Inc/ut.h>
#include <Inc/debugext.h>
#include "global.h"                 /* rdbi global area         */
#include <Inc/Rdbi/proto.h>

#include <Inc/Rdbi/cursor.h>    
#include	<Inc/Rdbi/context.h>

/************************************************************************/
int rdbi_lob_create_ref( rdbi_context_def *context, int sqlid, void **lob_ref)
{
    rdbi_cursor_def *cursor; 

    debug_on1("rdbi_lob_create_ref", "sqlid: %d", sqlid);

    cursor = context->rdbi_cursor_ptrs[sqlid];
     
    context->rdbi_last_status = (*(context->dispatch.lob_create_ref))( context->drvr, cursor->vendor_data, lob_ref );

    debug_return( NULL, context->rdbi_last_status );
}

/************************************************************************/
int rdbi_lob_destroy_ref( rdbi_context_def *context, int sqlid, void *lob_ref)
{
    rdbi_cursor_def *cursor; 

    debug_on1("rdbi_lob_destroy_ref", "sqlid: %d", sqlid);

    cursor = context->rdbi_cursor_ptrs[sqlid];
     
    context->rdbi_last_status = (*(context->dispatch.lob_destroy_ref))( context->drvr, cursor->vendor_data, lob_ref );

    debug_return( NULL, context->rdbi_last_status );
}

/************************************************************************/
int rdbi_lob_get_size(rdbi_context_def *context, int sqlid, void *lob_ref, unsigned int *size)
{
    rdbi_cursor_def *cursor;

    debug_on1("rdbi_lob_get_size", "sqlid: %d", sqlid);

    cursor = context->rdbi_cursor_ptrs[sqlid];
     
    context->rdbi_last_status = (*(context->dispatch.lob_get_size))( context->drvr, cursor->vendor_data, lob_ref, size );

    debug_return( NULL, context->rdbi_last_status );
}

/************************************************************************/
int rdbi_lob_write_next(rdbi_context_def *context, int sqlid, void *lob_ref, int rdbi_lob_type, unsigned int block_size, char* block )
{
    int             rdbi_last_status = RDBI_SUCCESS;
    rdbi_cursor_def *cursor; 

    debug_on1("rdbi_lob_write", "sqlid: %d", sqlid);

    if ( block_size > 0 ) {
        cursor = context->rdbi_cursor_ptrs[sqlid];
    
        rdbi_last_status = (*(context->dispatch.lob_write_next))( context->drvr, cursor->vendor_data, lob_ref, rdbi_lob_type, block_size, block);
    }

    debug_return( NULL, rdbi_last_status );
}

/************************************************************************/
int rdbi_lob_read_next(rdbi_context_def *context, int sqlid, void *lob_ref, int rdbi_lob_type, unsigned int block_size, char *block, unsigned int *block_size_out, int *eol)
{
    rdbi_cursor_def *cursor; 

    debug_on1("rdbi_lob_read_next", "sqlid: %d", sqlid);

    cursor = context->rdbi_cursor_ptrs[sqlid];
     
    context->rdbi_last_status = (*(context->dispatch.lob_read_next))( context->drvr, cursor->vendor_data, lob_ref, rdbi_lob_type, block_size, block, block_size_out, eol );

    debug_return( NULL, context->rdbi_last_status );
}

/************************************************************************/
int rdbi_lob_open(rdbi_context_def *context, int sqlid, void *lob_ref, int for_read)
{
    rdbi_cursor_def *cursor; 

    debug_on1("rdbi_lob_open", "sqlid: %d", sqlid);

    cursor = context->rdbi_cursor_ptrs[sqlid];
     
    context->rdbi_last_status = (*(context->dispatch.lob_open))( context->drvr, cursor->vendor_data, lob_ref, for_read );

    debug_return( NULL, context->rdbi_last_status );
}

/************************************************************************/
int rdbi_lob_close(rdbi_context_def *context, int sqlid, void *lob_ref)
{
    rdbi_cursor_def *cursor; 

    debug_on1("rdbi_lob_close", "sqlid: %d", sqlid);

    cursor = context->rdbi_cursor_ptrs[sqlid];
     
    context->rdbi_last_status = (*(context->dispatch.lob_close))( context->drvr, cursor->vendor_data, lob_ref );

    debug_return( NULL, context->rdbi_last_status );
}


