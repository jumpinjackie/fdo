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
* Name																	*
*	odbcdr_rdbi_init - Initialize the ODBC vendor-specific routine		*
*					 addresses											*
*																		*
* Synopsis																*
*	#include <Inc/Rdbi/methods.h>										*
*	odbcdr_rdbi_init(methods)											*
*	rdbi_methods	methods;											*
*																		*
* Description															*
*		This  module  defines the addresses of the routines to be		*
*		used for rdb  access.	In object-oriented language these		*
*		routines  are  the	methods  that	implement	the  RDBI		*
*		interface for a specific vendor.								*
*																		*
* Parameters															*
*	methods: output 													*
*		Table  of  routine	addresses  to be filled  in  by  this		*
*		routine.														*
*																		*
* Function value														*
*	Returns TRUE if the table was initialized, FALSE otherwise. 		*
*																		*
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <Inc/ut.h>
#include	<Inc/debugext.h>
#include <Inc/Rdbi/methods.h>	/* rdbi methods table */
#include "proto_p.h"

static char *getparm( char *p );
static void options_init(odbcdr_context_def  *context);
static void context_init(odbcdr_context_def  *context);


int
odbcdr_rdbi_init(
    odbcdr_context_def **contextp,
	rdbi_methods	methods
	)
{
    int                 ret = RDBI_GENERIC_ERROR;
    odbcdr_context_def  *context;

	debug_on("odbcdr_rdbi_init");

    *contextp = NULL;

	context = (odbcdr_context_def *)ut_vm_malloc ("odbcdr_rdbi_init", sizeof (odbcdr_context_def));
    if ((odbcdr_context_def *)NULL == context)
        ret = RDBI_MALLOC_FAILED;
    else
    {
        context_init (context);
	    options_init(context);

	    methods->connect	= (int (*)(void*, char*, char*, char*, char**, int*))odbcdr_connect;
		methods->close_cursor = (int (*)(void*,char*))odbcdr_close_cursor;
	    methods->disconnect = (int (*)(void*, char**))odbcdr_disconnect;
	    methods->est_cursor = (int (*)(void*, char**))odbcdr_est_cursor;
	    methods->est_cursor_obj = (int (*)(void*, char**))odbcdr_est_cursor;
	    methods->get_con_var = (int  (*)(void*,char*))odbcdr_get_con_var;
	    methods->fre_cursor = (int (*)(void*, char**))odbcdr_fre_cursor;
	    methods->sql		= (int (*)(void*, char*, char*, int, char*, void*, char*))odbcdr_sql;
		methods->sqlW		= (int (*)(void*, char*,wchar_t*, int, char*, void*, char*))odbcdr_sqlW;
	    methods->exec_coc	= NULL;
	    methods->execute	= (int (*)(void*, char*, int, int, int*))odbcdr_execute;
	    methods->define 	= (int (*)(void*, char*, char*, int, int, char*, void*))odbcdr_define;
	    methods->bind		= (int (*)(void*, char*, char*, int, int, char*, void*))odbcdr_bind;
	    methods->fetch		= (int (*)(void*, char*, int, int, int, int*))odbcdr_fetch2;
	    methods->alcnullind = (int (*)(void*, int, char**))odbcdr_alcnullind; 
	    methods->set_null	= (void (*)(void*, void*, int, int))odbcdr_set_null;
	    methods->set_nnull	= (void (*)(void*, void*, int, int))odbcdr_set_nnull;
	    methods->is_null	= (int (*)(void*, void*, int))odbcdr_is_null;
	    methods->commit 	= (int (*)(void*, int))odbcdr_commit;
	    methods->vndr_name	= (char*(*)(void*))odbcdr_vndr_name;
        methods->vndr_info  = (int  (*)(void*, rdbi_vndr_info_def *))odbcdr_vndr_info;
	    methods->dbswitch	= (int (*)(void*,int))odbcdr_switch;
	    methods->get_msg	= (void (*)(void*, char*))odbcdr_get_msg;
	    methods->usr_exists = (int  (*)(void*,char*,int*))odbcdr_usr_exists;
	    methods->col_act	= (int (*)(void*,char*,char*,char*))odbcdr_col_act;
	    methods->col_get	= (int (*)(void*,char*,char*,int*,int*,int*,int*,int*,int*))odbcdr_col_get;
	    methods->col_deac	= (int  (*)(void*))odbcdr_col_deac;
	    methods->objects_act	= (int  (*)(void*, const char *, const char *))odbcdr_objects_act;
	    methods->objects_get	= (int  (*)(void*, char *, char *, int*))odbcdr_objects_get;
	    methods->objects_deac	= (int  (*)(void*))odbcdr_objects_deac;
	    methods->pkeys_act	= (int  (*)(void*, const char *, const char *))odbcdr_pkeys_act;
	    methods->pkeys_get	= (int  (*)(void*, char*, int*))odbcdr_pkeys_get;
	    methods->pkeys_deac	= (int  (*)(void*))odbcdr_pkeys_deac;
	    methods->users_act	= (int  (*)(void*, const char *))odbcdr_users_act;
	    methods->users_get	= (int  (*)(void*, char*, int*))odbcdr_users_get;
	    methods->users_deac	= (int  (*)(void*))odbcdr_users_deac;
	    methods->stores_act	= (int  (*)(void*))odbcdr_stores_act;
	    methods->stores_get	= (int  (*)(void*, char*, int*))odbcdr_stores_get;
	    methods->stores_deac	= (int  (*)(void*))odbcdr_stores_deac;
	    methods->desc_slct	= (int  (*)(void*,char*,int,int,char*,int*,int*,int*))odbcdr_desc_slct;
		methods->desc_slctW	= (int  (*)(void*,char*,int,int,wchar_t*,int*,int*,int*))odbcdr_desc_slctW;
	    methods->desc_bind	= (int  (*)(void*,char*,int,int,char*))odbcdr_desc_bind;
        methods->set_schema = (int  (*)(void*,char*))odbcdr_set_schema;
        methods->set_schemaW = (int  (*)(void*,wchar_t*))odbcdr_set_schemaW;
        methods->run_sql    = (int  (*)(void*,char*,int,int*))odbcdr_run_sql;
		methods->run_sqlW    = (int  (*)(void*,wchar_t*,int,int*))odbcdr_run_sqlW;
		methods->lob_create_ref = NULL;

        methods->capabilities.supports_sequence = 0;
        methods->capabilities.supports_autoincrement = 1;
#ifdef _WIN32
		methods->capabilities.supports_unicode = 1;
#else
                // Linux defaults wchar_t to UTF32, but Easysoft driver expects UTF16 (like on Windows).
		methods->capabilities.supports_unicode = 0;
#endif
        methods->capabilities.supports_int64_binding = 0;

        methods->get_gen_id = (int  (*)(void*,char*,int*))odbcdr_get_gen_id;
		methods->autocommit_on = (int  (*)(void*))odbcdr_autocommit_on;
		methods->autocommit_off = (int  (*)(void*))odbcdr_autocommit_off;
		methods->autocommit_mode = (int  (*)(void*))odbcdr_autocommit_mode;

        // Many others are missing...
        methods->term        = (int  (*)(void*))odbcdr_term;

        *contextp = context;
        ret = RDBI_SUCCESS; 
    }
	debug_return( NULL, ret );
}

static char *getparm(
	char	*p
	)
{
	char	envname[556];

	/*
	 *	Prepend our standard prefix.
	 *	Doing it this way makes it more difficult to find
	 *	our private env. vars. with the strings(1) command.
	 */
	strcpy(envname,"VISION");
	strcat(envname, "_");
	strcat(envname,"ODBCDR");
	strcat(envname, "_");
	strcat(envname, p);
	return getenv(envname);
}

static void options_init( odbcdr_context_def *context)
{
	char		*p;

	if (context->odbcdr_ddlexec == -1) {

		p = getparm("DDLEXEC");

		/*
		 *	Default to ddlexec off (suppress exec for DDL stmts.)
		 */
		if(p && (_stricmp(p, "yes")==0 || _stricmp(p, "y")==0))
			context->odbcdr_ddlexec = 1;
		else
			context->odbcdr_ddlexec = 0;
	}

	if (context->odbcdr_ccache_size == -1) {

		p = getparm("CCACHE_SIZE");

		/*
		 *	Default cursor cache size to 10 cursors.
		 */
		if(p) {
			context->odbcdr_ccache_size = atoi(p);
			if(context->odbcdr_ccache_size < 0)
				context->odbcdr_ccache_size = 0;
		} else
			context->odbcdr_ccache_size = 10;
	}
}

static void context_init( odbcdr_context_def *context)
{
    int i;

    context->odbcdr_connect_count = 0; /* number of open connections */
    context->odbcdr_current_connect = 0; /* index into odbcdr_conns */
 
    context->odbcdr_env = INIT_NULL;

    context->odbcdr_connect_count = INIT_ZERO;
    context->odbcdr_current_connect = INIT_M_ONE;			/* Index into odbcdr_lda */

    context->odbcdr_ddlexec = INIT_M_ONE;
    context->odbcdr_ccache_size = INIT_M_ONE;

    for( i = 0; i < RDBI_MAX_CONNECTS; i++ )
        context->odbcdr_conns[i] = (odbcdr_connData_def *)NULL;

    strcpy(context->odbcdr_automatic_logon_user, INIT_SLASH );	/* For default logon	*/

    context->odbcdr_last_rc = INIT_ZERO;

	context->odbcdr_last_err_msg[0] = '\0';

	context->odbcdr_nameListNextPosition_cols = -1;
	context->odbcdr_nameListNextPosition_pkeys = -1;
	context->odbcdr_nameListNextPosition_objs = -1;
	context->odbcdr_nameListNextPosition_stores = -1;
	context->odbcdr_nameListNextPosition_users = -1;

	context->odbcdr_singletonNameInUse = false;
	context->odbcdr_singletonNameReturned = false;

	memset(&context->odbcdr_nameList_objs, 0, sizeof(ut_da_def));
	context->odbcdr_nameList_objs.el_size = sizeof(odbcdr_NameListEntry_obj_def);

	memset(&context->odbcdr_nameList_users, 0, sizeof(ut_da_def));
	context->odbcdr_nameList_users.el_size = sizeof(odbcdr_NameListEntry_user_def);

	memset(&context->odbcdr_nameList_cols, 0, sizeof(ut_da_def));
	context->odbcdr_nameList_cols.el_size = sizeof(odbcdr_NameListEntry_col_def);

	memset(&context->odbcdr_nameList_stores, 0, sizeof(ut_da_def));
	context->odbcdr_nameList_stores.el_size = sizeof(odbcdr_NameListEntry_store_def);

	memset(&context->odbcdr_nameList_pkeys, 0, sizeof(ut_da_def));
	context->odbcdr_nameList_pkeys.el_size = sizeof(odbcdr_NameListEntry_pkey_def);

}

