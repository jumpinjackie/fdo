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
*   rdbi_geom_dimens_set - helper function in aid of ocidr to set the   *
*                        geometry dimensionality                        *
*                                                                       *
* Synopsis                                                              *
*   rdbi_geom_dimens_set( int sqlid, int wantedDim)                     *
*       srid - input                                                    *                                                                    *
*                                                                       *
* Description                                                           *
*       Calls ocidr related function to set the desired dimensionality  *
*       for the geometry columns.                                       *
*                                                                       *
* Parameters                                                            *
*   sqlid: input                                                        *
*       cursor id.                                                      *
*                                                                       *
*   wantedDim: input                                                    *
*       Desired geometry dimensionality for insert/fetch when different *
*       from the storage. It is one of DBI_COORD_NATIVE_DIM (default)   *
*       DBI_XY, DBI_XYZ. It causes padding with zeros on insert and     *
*       unpadding on fetch geometries.                                  *
*                                                                       *
* Function value                                                        *
*                                                                       *
* Remarks                                                               *
*                                                                       *
*                                                                       *
************************************************************************/

#include <Inc/ut.h>
#include <Inc/debugext.h>
#include    "global.h"                 /* rdbi global area         */
#include    <Inc/Rdbi/proto.h>
#include    <Inc/Rdbi/cursor.h>    
#include    <Inc/Rdbi/context.h>

int rdbi_geom_dimens_set ( rdbi_context_def *context, int sqlid, int wantedDim ) 
{
    rdbi_cursor_def *cursor;        /* for convenience          */
    int     rc = FALSE;

    debug_on2("rdbi_geom_dimens_set", "sqlid: %d  wantedDim: %%d", sqlid, wantedDim);

    cursor = context->rdbi_cursor_ptrs[sqlid];
     
    rc = (*(context->dispatch.geom_dimens_set))( context->drvr, cursor->vendor_data, wantedDim );

    debug_return(NULL, rc );
}

