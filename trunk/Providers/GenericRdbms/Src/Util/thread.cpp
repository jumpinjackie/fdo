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
 *
 */

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <Inc/Util/thread.h>

#ifdef _WIN32
int tlsIndex;
CRITICAL_SECTION ut_mutex[UT_THREAD_MUTUX_NUM];

#else
pthread_key_t key;
pthread_mutex_t ut_mutex[UT_THREAD_MUTUX_NUM];
static pthread_mutex_t temp =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#endif


void ut_thread_mutex_init()
{
    int i;
    for (i=0; i<UT_THREAD_MUTUX_NUM; i++)
    {
#ifdef _WIN32
        InitializeCriticalSection(&ut_mutex[i]);
#else
        ut_mutex[i] = temp;
#endif
    }
}

void ut_thread_mutex_enter(int index)
{
    if (index < 0 || index>= UT_THREAD_MUTUX_NUM)
        return;

#ifdef _WIN32
        EnterCriticalSection(&ut_mutex[index]);
#else
        pthread_mutex_lock(&ut_mutex[index]);

#endif
}

void ut_thread_mutex_leave(int index)
{
    if (index < 0 || index>= UT_THREAD_MUTUX_NUM)
        return;

#ifdef _WIN32
        LeaveCriticalSection(&ut_mutex[index]);
#else
        pthread_mutex_unlock( &ut_mutex[index]);
#endif
}

void ut_thread_mutex_destroy()
{
    int i;

    for (i=0; i<UT_THREAD_MUTUX_NUM; i++)
    {
#ifdef _WIN32
        DeleteCriticalSection(&ut_mutex[i]);
#else
        pthread_mutex_destroy(&ut_mutex[i]);
#endif
    }
}
