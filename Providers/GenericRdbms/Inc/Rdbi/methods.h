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
*   methods.h - contains typedef for rdbms driver methods table         *
*                                                                       *
* Synopsis                                                              *
*   #include <inc/rdbi/methods.h>                                       *
*                                                                       *
* Description                                                           *
*       This  module  defines  the  table  of  addresses  of  the       *
*       routines  to  be used for rdb access.  In object-oriented       *
*       language these routines  are  the  methods that implement       *
*       the RDBI interface for a specific vendor.                       *
*                                                                       *
* Remarks                                                               *
*                                                                       *
************************************************************************/

#ifndef RDBI_METHODS_H
#define RDBI_METHODS_H

#include <Inc/Geometry/Fgf/AllGeometry_c.h>    /* pIGeometry_def           */
#include <Inc/Rdbi/vndr_info.h>
#include <stdlib.h>

#define   INVOKE_RDBI_FUNC( rdbifunc ) {\
    int rc = rdbifunc; \
    if( rc != RDBI_SUCCESS ) { \
        rdbi_get_msg( rdbi_context ); \
        throw FdoSchemaException::Create( rdbi_context->last_error_msg ); \
    } \
}

typedef struct _driver_capability_ {
    int    supports_sequence;
    int    supports_autoincrement;
	int    supports_unicode;
    int    supports_int64_binding;
} rdbi_driver_capabilities_def;

typedef struct rdbi_methods_def {
    int  (*connect)(void*,char*,char*,char*,char**,int*);
    int  (*disconnect)(void*,char**);
    int  (*est_cursor)(void*,char**);
    int  (*est_cursor_obj)(void*,char**);
    int  (*fre_cursor)(void*,char**);
    int  (*sql)(void*,char*,char*,int,char*,void*,char*);
	int  (*sqlW)(void*,char*,wchar_t*,int,char*,void*,char*);
    int  (*execute)(void*,char*,int,int,int*);
    int  (*exec_coc)(void*);
    int  (*define)(void*,char*,char*,int,int,char*,void*);
    int  (*bind)(void*,char*,char*,int,int,char*,void*);
    int  (*fetch)(void*,char*,int,int,int,int*);
    void  (*get_msg)(void*,char*);
    int  (*alcnullind)(void*,int,char**);
    void  (*set_null)(void*,void*,int,int);
    void  (*set_nnull)(void*,void*,int,int);
    int  (*is_null)(void*,void*,int);
    int  (*desc_slct)(void*,char*,int,int,char*,int*,int*,int*);
    int  (*desc_slctW)(void*,char*,int,int,wchar_t*,int*,int*,int*);
    int  (*desc_bind)(void*,char*,int,int,char*);
    int  (*dbswitch)(void*,int);
    char *(*vndr_name)(void*);
    int  (*commit)(void*,int);
    int  (*col_act)(void*,char*,char*,char*);
    int  (*col_get)(void*,char*,char*,int*,int*,int*,int*,int*,int*);
    int  (*col_deac)(void*);
    int  (*objects_act)(void*, const char *, const char *);
    int  (*objects_get)(void*, char *, char *, int*);
    int  (*objects_deac)(void*);
    int  (*pkeys_act)(void*, const char *);
    int  (*pkeys_get)(void*, char*, int*);
    int  (*pkeys_deac)(void*);
    int  (*users_act)(void*, const char *);
    int  (*users_get)(void*, char*, int*);
    int  (*users_deac)(void*);
    int  (*stores_act)(void*);
    int  (*stores_get)(void*, char*, int*);
    int  (*stores_deac)(void*);
    int  (*usr_exists)(void*,char*,int*);
    int  (*get_con_var)(void*,char*);
    int  (*do_break)(void*);
    int  (*set_schema)(void*,char*);
    int  (*set_schemaW)(void*,wchar_t*);
    int  (*vndr_info)(void*, rdbi_vndr_info_def *);
    int  (*geom_srid_set)(void*,char*,long);
    int  (*geom_dimens_set)(void*,char*,int);
    int  (*get_geoms_ext)( void *, char *, char *,  pIGeometry_def *);
    int  (*lob_create_ref)(void*,char*,void**);
    int  (*lob_destroy_ref)(void*,char*,void*);
    int  (*lob_get_size)(void*,char*,void*,unsigned int*);
    int  (*lob_write_next)(void*,char*,void*,int,unsigned int,char*);
    int  (*lob_read_next)(void*,char*,void*,int,unsigned int,char*,unsigned int*,int*);
    int  (*lob_open)(void*,char*,void*,int);
    int  (*lob_close)(void*,char*,void*);
    int  (*term)(void*);
    int  (*run_sql)(void*,char*,int,int*);
	int  (*run_sqlW)(void*,wchar_t*,int,int*);
    int  (*get_gen_id)(void*,char*,int*);
	int  (*autocommit_on)(void*);
	int  (*autocommit_off)(void*);
	int  (*autocommit_mode)(void*);
    rdbi_driver_capabilities_def    capabilities;

} rdbi_methods_def, *rdbi_methods;

#endif /* RDBI_METHODS_H */

