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
*   rdbi_get_size - Get size for rdbi types found in Inc/rdbi.h         *
*                                                                       *
* Synopsis                                                              *
*   rdbi_get_size(datatype)                                             *
*   int datatype;                                                       *
*                                                                       *
* Description                                                           *
*       This  function  returns  a  size  given an rdbi data type       *
*       found in in Inc/rdbi.h.  A size  is  not returned for for       *
*       varying data types, for example RDBI_STRING.                    *
*                                                                       *
* Parameters                                                            *
*   datatype: input                                                     *
*       RDBI data type found in Inc/rdbi.h .                            *
*                                                                       *
* Function Value                                                        *
*       Data type size or 0 if data type  could  not be resolved.       *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.            *
*                                                                       *
************************************************************************/


#include <Inc/Nls/rdbi_msg.h>
#include <Inc/debugext.h>
#include <Inc/rdbi.h>
#include <Inc/Geometry/Fgf/AllGeometry_c.h>
#include	<Inc/Rdbi/context.h>

int
rdbi_get_size(
	rdbi_context_def *context,
    int             datatype )
{
    int             size = 0;


    debug_on("rdbi_get_size");


    /****************************************************************
    *   Establish size from KNOWN types (keep this in one place)    *
    *   This shouldn't be repeated by code above rdbi               *
    ****************************************************************/
    switch(datatype) {
        case RDBI_CHAR:
            size = sizeof(char);
            break;
        case RDBI_SHORT:
            size = sizeof(short);
            break;
        case RDBI_INT:
            size = sizeof(int);
            break;
        case RDBI_LONG:
            size = sizeof(long);
            break;
        case RDBI_FLOAT:
            size = sizeof(float);
            break;
        case RDBI_DOUBLE:
            size = sizeof(double);
            break;
        case RDBI_BLOB_REF:
            size = sizeof(void *);
            break;
        case RDBI_GEOMETRY:
            size = sizeof(pIGeometry_def);
            break;
    }


    debug_return(NULL, size);
}

