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
* Name                                                                  *
*   rdbi_get_geoms_ext - Retrieve existing geometries extents.          *
*                                                                       *
* Synopsis                                                              *
*   int rdbi_get_geoms_ext (char *table_name_I,                         *
*                            char *geom_col_name_I,                     *
*                            pIGeometry_def *pGeom_O)                   *
*                                                                       *
* Description                                                           *
*   See  rdbi_get_geoms_ext() for details.                              *
*                                                                       *
* Parameters                                                            *
*   table_name_I: input                                                 *
*       The table name.                                                 *
*                                                                       *
*   geom_col_name_I: input                                              *
*       The geometry column name.                                       *
*                                                                       *
*  pGeom_O: output                                                      *
*       The geometry returned by Oracle representing the extents.       *
*                                                                       *
* Function value                                                        *
*   Returns an rdbi status code from Inc/rdbi.h                         *
*                                                                       *
* Remarks                                                               *
*                                                                       *
************************************************************************/

#include <Inc/debugext.h>

#include <Inc/Nls/rdbi_msg.h>
#include <Inc/Geometry/Fgf/AllGeometry_c.h>    /* pIGeometry_def           */
#include "global.h"
#include	<Inc/Rdbi/context.h>

int rdbi_get_geoms_ext (rdbi_context_def *context, char *table_name, char *geom_col_name, pIGeometry_def *pGeom)
{
    debug_on("rdbi_get_geoms_ext");

    context->rdbi_last_status = (*(context->dispatch.get_geoms_ext))(context->drvr, table_name, geom_col_name, pGeom);

    debug_return(NULL, context->rdbi_last_status);
}

