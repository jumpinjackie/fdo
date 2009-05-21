#ifndef UT_THREAD_H
#define UT_THREAD_H

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

#ifdef _WIN32
#include <windows.h>
#endif

typedef enum
{
    MUTEX_CURREVISION,
    MUTEX_ROLLBACKCACHE,
    MUTEX_CONNECTIONSEQ,
    MUTEX_NLSMSGGET
} ut_thread_mutex_enum;

#define UT_THREAD_MUTUX_NUM  4       /* number of mutex */

#ifdef _WIN32
extern int tlsIndex;
extern CRITICAL_SECTION ut_mutex[];
#else
#include <pthread.h>
extern pthread_key_t key;
extern pthread_mutex_t ut_mutex[];
#endif

void ut_thread_mutex_init();
void ut_thread_mutex_enter(int index);
void ut_thread_mutex_leave(int index);
void ut_thread_mutex_destroy();

#endif

