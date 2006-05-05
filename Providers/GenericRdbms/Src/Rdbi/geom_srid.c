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
*   rdbi_geom_srid_set - helper function in aid of ocidr to set the     *
*                        geometry srid (Spatial Reference Id)           *
*                                                                       *
* Synopsis                                                              *
*   rdbi_geom_srid_set( long srid)                                      *
*       srid - input                                                    *                                                                    *
*                                                                       *
* Description                                                           *
*       Call ocidr related function to set the current SRID.            *
*                                                                       *
* Parameters                                                            *
*                                                                       *
* Function value                                                        *
*                                                                       *
* Remarks                                                               *
*                                                                       *
************************************************************************/
#include <Inc/ut.h>
#include <Inc/debugext.h>
#include "global.h"                 /* rdbi global area         */
#include <Inc/Rdbi/proto.h>

#include <Inc/Rdbi/cursor.h>   
#include	<Inc/Rdbi/context.h>

int rdbi_geom_srid_set ( rdbi_context_def *context, int sqlid, long srid ) 
{
    rdbi_cursor_def *cursor;        /* for convenience          */
    int     rc = FALSE;

    debug_on1("rdbi_geom_srid_set", "sqlid: %d", sqlid);

    cursor = context->rdbi_cursor_ptrs[sqlid];
     
    rc = (*(context->dispatch.geom_srid_set))( context->drvr, cursor->vendor_data, srid );

    debug_return(NULL, rc );
}

