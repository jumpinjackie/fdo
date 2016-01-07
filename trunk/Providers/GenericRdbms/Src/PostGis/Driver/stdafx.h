/*
 * Copyright (C) 2006 Refractions Research, Inc. 
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
 */

#ifndef POSTGIS_DRIVER_STDAFX_H
#define POSTGIS_DRIVER_STDAFX_H

#ifdef _WIN32
/* Exclude rarely-used stuff from Windows headers. */
# define WIN32_LEAN_AND_MEAN 
# include <windows.h>
#endif /* _WIN32 */

/* Standard library */
#include <stdio.h>  /* snprintf() */
#include <string.h>

/* RDBI interface */
#include <Inc/rdbi.h>

/* PostgreSQL client library */
#include <libpq-fe.h>

/*
 * POSTGISDR_CALL
 *
 * The following macro allowis various calling conventions
 * on the RDBI PostGIS API driver.
 */
#ifdef _WIN32
# define POSTGISDR_CALL   __declspec(dllexport)
#else
# define POSTGISDR_CALL
#endif /* _WIN32 */


/*
 * Helper macro that determines the number of elements
 * in a statically-sized array.
 * NOTE: This doesn't work for dynamically-sized arrays.
 */
#define ELEMENTS(x) (sizeof(x)/sizeof(x[0]))


/*
 * DEF_PGPORT
 *
 * NOTE: This macro has been copied directly from pg_config.h file,
 * from PostgreSQL sources.
 *
 * Define to the default TCP port number on which the server listens and to
 * which clients will try to connect.
 */
#define DEF_PGPORT 5432
#define DEF_PGPORT_STR "5432"

/*
 * C/POSIX prototypes
 */

#ifdef _WIN32
#define snprintf _snprintf
#else
#undef libintl_sprintf
#define libintl_sprintf sprintf
#endif

#ifdef _WIN32
    typedef __int64 rdbi_int64_t;
#else
    typedef long long rdbi_int64_t;
#endif /* _WIN32 */


#endif /* POSTGIS_DRIVER_STDAFX_H */
