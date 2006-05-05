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
*   rdbi_nom_db - Set the default user name to prefix tables by         *
*                                                                       *
* Synopsis                                                              *
*   void rdbi_nom_db(db_name)                                           *
*   char *db_name;                                                      *
*                                                                       *
* Description                                                           *
*                                                                       *
* Parameters                                                            *
*   db_name: input                                                      *
*       String containing the database name.                            *
*                                                                       *
* Remarks                                                               *
*       When  rdbi  encounters  a  substring  in  an  SQL command       *
*       indicating  the active default database ("&&DB"), it will       *
*       replace that  substring with the database name.  The name       *
*       given here sets that database name.                             *
*                                                                       *
* Function value                                                        *
*       void                                                            *
*                                                                       *
* Remarks                                                               *
*                                                                       *
*   IMPORTANT NOTE:                                                     *
*       All messages in this source file have been externalized. Do not *
*       add any more msg_issue() calls, use nls_msg_issue() instead.    *
*       If you modify or delete messages, be sure to modify or delete   *
*       the corresponding message in $root/nls/msf/rdbi.msf.        *
*                                                                       *
************************************************************************/

#include <Inc/debugext.h>
extern char *rdbi_cat;

#include    <Inc/Nls/rdbi_msg.h>
#include	<Inc/Rdbi/context.h>
#include <Inc/ut.h>

#include "global.h"             /* rdbi global area     */

void rdbi_nom_db(
	rdbi_context_def *context,
    char *db_name)
{


    debug_on1("rdbi_nom_db", "db_name : %s", db_name);
    /*
     * Save for future reference
     */
    strcpy(context->rdbi_cnct->db_name, db_name);

    debug_return_void(NULL);
}

