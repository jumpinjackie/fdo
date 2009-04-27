/*
 * 
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
/*NOTUSED*/
#include "func.h"
/*
 *  Use multiple static message buffers to mitigate the fact that nsl_msg_get()
 *  is not reentrant.  Make the number of message buffers a power of 2 since
 *  this is least inefficient way to index a multi-dimensional array.
 */
#define MSG_BUFFERS 8
static wchar_t msg_buf[MSG_BUFFERS][NLS_BUFFSZ + 1];
static wchar_t msg_bufW[MSG_BUFFERS][NLS_BUFFSZ + 1];
static unsigned msg_buf_index = 0;

#ifndef _WIN32
#include <stdarg.h>                 /* for defn of ANSI va_start(), ... */
#include "pthread.h"
#include <wchar.h>
#include <stdio.h>

extern char *nls_cat;


extern pthread_mutex_t NlsMsgGetCriticalSection;

void nls_msg_close_handles()
{
}

wchar_t* nls_msg_get_W2(wchar_t *msg_string,
                        const char *cat_name,
                        int  set_num,
                        int  msg_num,
                        const char *default_msg,
                        va_list argp)
{
    char            *fmt_str = NULL;
    unsigned        msg_size;
    wchar_t         *msg_bufp;
    wchar_t         *Wtext;

    pthread_mutex_lock(&NlsMsgGetCriticalSection);

    /*
     *  If catalog is specified, then ...
     */
    if (cat_name != NULL) {
        /*
         *  Open catalog.
         */
        nl_catd cat_desc = nls_cat_open(cat_name);
        if (cat_desc != (nl_catd) -1)  {
            /*
             *  Get catalog string.
             */
            fmt_str = catgets(cat_desc, set_num, msg_num, default_msg);
        }
    }
    /*
     *  If no catalog or no catalog string, then use default_msg.
     */
    if (fmt_str == NULL || *fmt_str == '\0')
        fmt_str = default_msg;
    /*
     *  Allocate next message buffer.
     */
    Wtext = msg_bufW[msg_buf_index % MSG_BUFFERS];
    Wtext[0] = 0;
    msg_bufp = msg_buf[msg_buf_index++ % MSG_BUFFERS];
    mbstowcs ( msg_bufp, fmt_str, NLS_BUFFSZ);

    /*
     *  Format variadic arguments.
     */
    msg_size = vswprintf(Wtext, NLS_BUFFSZ, msg_bufp, argp);
    /*
     *  If we overflowed the msg buffer, panic.
     */
    if (msg_size >= NLS_BUFFSZ) {
        fprintf(stderr,
                    "memory may be corrupted while formating a message.\n");
        fprintf(stderr,
                    "probably caused by a invalid externalised message.\n");
    }
the_exit:
    pthread_mutex_unlock(&NlsMsgGetCriticalSection);
    return Wtext;
}

#else

#include <windows.h>

// cache three catalogs; actually many more without paths on catalog name
#define MAX_REF 3
typedef struct _Reference
{
    HINSTANCE hLib;           // handle to the messagetable DLL
    struct _Reference *pNext; // pointer to next Reference in chain
    char szCatalog[1];        // name of the DLL
} Reference;

static char Cache[(sizeof (Reference) + MAX_PATH) * MAX_REF];
extern CRITICAL_SECTION CacheCriticalSection;
extern CRITICAL_SECTION NlsMsgGetCriticalSection;

// prepend the catalog onto the reference chain
static int Prepend (HINSTANCE handle, const char* catalog)
{
    size_t size;
    size_t remainder;
    Reference* rover;
    Reference* previous;
    Reference ref;
    int ret;

    ret = FALSE;

    size = sizeof (Reference) + strlen (catalog);
    // check for enough space given we can use the whole thing
    if (size <= sizeof (Cache))
    {
        // prep the structure
        ref.hLib = handle;
        ref.pNext = NULL;
        ref.szCatalog[0] = '\0';

        // clear a spot
        rover = (Reference*)Cache;
        if (0 == rover->hLib) // non-zero hLib means a valid first entry
            rover = NULL;
        if (NULL != rover)
        {
            // search for last entry
            previous = NULL;
            while (NULL != rover->pNext)
            {
                previous = rover;
                rover = rover->pNext;
            }

            // calculate remaining area
            remainder = sizeof (Cache); // total available
            remainder -= ((char*)rover - Cache); // entries up to last
            remainder -= sizeof (Reference); // last entry size 
            remainder -= strlen (rover->szCatalog); // and it's string

            if (size < remainder)
            {
                // move everything back
                memmove (Cache + size, Cache, sizeof (Cache) - size);
                ref.pNext = (Reference*)(Cache + size);
                rover = (Reference*)Cache;
                *rover = ref;
                strcpy (rover->szCatalog, catalog);

                // adjust pointers
                rover = ref.pNext;
                while (NULL != rover->pNext)
                {
                    rover->pNext = (Reference*)(((char*)(rover->pNext)) + size);
                    rover = rover->pNext;
                }

                // successfully cached
                ret= TRUE;
            }
            else
            {
                // drop least recently allocated entry and recurse
                FreeLibrary (rover->hLib);
                if (NULL == previous)
                    memset (Cache, 0, sizeof (Cache));
                else
                {
                    previous->pNext = NULL;
                    memset (rover, 0, sizeof (Cache) - ((char*)rover - Cache));
                }
                ret = Prepend (handle, catalog);
            }
        }
        else
        {
            // store in first available memory
            rover = (Reference*)Cache;
            *rover = ref;
            strcpy (rover->szCatalog, catalog);

            // successfully cached
            ret = TRUE;
        }
    }

    return (ret);
}

// get the message from the catalog
static int LoadMessage (const char* catalog, DWORD number, wchar_t* message, size_t size)
{
    Reference* rover;
    HINSTANCE handle;
    int transient;
    wchar_t* buffer;
    int ret;

    ret = FALSE;

    EnterCriticalSection (&CacheCriticalSection);

    // search for the catalog in the cache
    rover = (Reference*)Cache;
    if (0 == rover->hLib) // non-zero hLib means a valid first entry
        rover = NULL;
    while (NULL != rover)
        if (0 == strcmp (catalog, rover->szCatalog))
            break;
        else
            rover = rover->pNext;

    transient = FALSE;
    if (NULL != rover)
        handle = rover->hLib;
    else
    {
        // load the resource library without calling any entry points since
        // this is a resource-only DLL
        handle = LoadLibraryEx (catalog, NULL, LOAD_LIBRARY_AS_DATAFILE);
        if (NULL != handle)
            if (!Prepend (handle, catalog))
                transient = TRUE;
    }

    if (NULL != handle)
    {
        if (0 != FormatMessageW (
                    FORMAT_MESSAGE_FROM_HMODULE |
                    FORMAT_MESSAGE_MAX_WIDTH_MASK |
                    FORMAT_MESSAGE_IGNORE_INSERTS |
                    FORMAT_MESSAGE_ALLOCATE_BUFFER,
                    handle,
                    number,
                    LANG_USER_DEFAULT, // GetUserDefaultLangID (),
                    (wchar_t*)&buffer, 
                    0,
                    NULL))
        {
            if (wcslen (buffer) < size - 1)
            {
                wcscpy (message, buffer);
                ret = TRUE;
            }
            LocalFree (buffer);
        }
        if (transient)
            FreeLibrary (handle);
    }

    LeaveCriticalSection (&CacheCriticalSection);

    return (ret);
}

void nls_msg_close_handles()
{
    Reference* rover = (Reference*)Cache;
    while (NULL != rover)
    {
        if (rover->hLib != NULL)
        {
            FreeLibrary (rover->hLib);
            rover->hLib = NULL;
        }
        rover = rover->pNext;
    }
}

wchar_t * nls_msg_get_W2(wchar_t *msg_string,
             const char *cat_name,
             int  set_num,
             DWORD  msg_num,
             const char *default_msg,
             va_list arguments)
{
    static wchar_t      tmp_buf[NLS_BUFFSZ +1];
    static wchar_t      buffer[NLS_BUFFSZ +1];
    wchar_t             *ptr_src = NULL;
    wchar_t             *ptr_dst= buffer;
    wchar_t             *Wtext;
    int                 length;

    EnterCriticalSection (&NlsMsgGetCriticalSection);

    /*
     *  Allocate next message buffer.
     */
    Wtext = msg_bufW[msg_buf_index++ % MSG_BUFFERS];

    if (LoadMessage (cat_name, msg_num, tmp_buf, NLS_BUFFSZ ))
        ptr_src = tmp_buf;
    else
    {
        length = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, default_msg,
                                (int)strlen(default_msg), &tmp_buf[0], NLS_BUFFSZ);
        if( length )    
        {
            tmp_buf[length] = 0;
            ptr_src = tmp_buf;
        }
    }
    // else use the default message

    /* The following will search for strings of the form "%a$b"
            where   a is the digit 1 to 9 and
                b is 's', 'd', 'x', 'lu', 'ld', 'u'

        and convert the string to "%a!b!"

        eg. "Error opening file '%1$s' --> "Error opening file '%1!s!');
        It also converts printf newline escape '\n' to FormatMessage
        newline escape '%n' 
    */

    while(*ptr_src) {
        if (*ptr_src == '%') {
            *(ptr_dst++) = *(ptr_src++);
            *(ptr_dst++) = *(ptr_src++);
            if( *(ptr_src++) == '$' )
                *(ptr_dst)++ = '!';
            else {
                *(ptr_dst)++ = *(ptr_src - 1);
                *(ptr_dst)++ = '!';
            }
            if( *ptr_src == 'l' ) {
                *(ptr_dst++) = *(ptr_src++);
                *(ptr_dst++) = *(ptr_src++);
            }
            else    {
                *(ptr_dst++) = *(ptr_src++);
            }
            *(ptr_dst++) = '!';
        }
        else    {
            if (*ptr_src == '\\' && *(ptr_src+1)== 'n' ) {
                *(ptr_dst++) = '%';
                *(ptr_dst++) = 'n';
                ptr_src+=2;
            }
            *(ptr_dst++) = *(ptr_src++);
        }   
    }
    *ptr_dst = '\0';

    FormatMessageW(
        FORMAT_MESSAGE_FROM_STRING | 
        FORMAT_MESSAGE_MAX_WIDTH_MASK,
        buffer, 0, 0, Wtext, 
        NLS_BUFFSZ, (va_list*) &arguments);
       
    LeaveCriticalSection (&NlsMsgGetCriticalSection);
    return Wtext;
}

#endif

