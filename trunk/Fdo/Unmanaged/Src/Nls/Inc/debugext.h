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
*   debugext - Declare pointers, flags required for debug statements.   *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/debugext.h>                                           *
*                                                                       *
* Description                                                           *
*                                                                       *
* Remarks                                                               *
*                                                                       *
*                                                                       *
************************************************************************/
#ifndef DEBUGEXT_H
#define DEBUGEXT_H  1

#include <Inc/tfn.h>
#include <Inc/os.h>

#ifndef NO_DEBUG /* set NO_DEBUG to something to compile out debug */
#include <stdio.h>
#endif

#ifdef C_PLUSPLUS
extern "C"
{
#endif

#if defined(FULLPROTO) || defined(c_plusplus) || defined(__cplusplus)

void debug_command(
        int     command,
        char *  pattern
        );
int debug(
        char * routine,
        char * string
        );
int debug_dump(
        void
        );
void debug_indent(
        char * function_name,
        char * debug_string
        );
void debug_init(
        int * count,
        char * parms[]
        );
int debug_initialize(
        char * version,
        int * count,
        char * parms[]
        );
int debug_null_msg(
        char * function_name,
        char * fmt_string,
        long value
        );
int debug_prompt(
        char * prompt,
        char * buf,
        int bufsize
        );
void debug_query(
        void
        );
int debug_read(
        char * fname
        );
char * debug_store_exe(
        char * name
        );
int debug_this(
        char * routine,
        int entry_point
        );
void debug_trace_in(
        char * function
        );
void debug_trace_out(
        char * function
        );
int debug_keep(
        char *      string
        );
void debug_tab_col(
        char *      routine,
        char *      table,
        char *      column
        );
int debug_dumpfds(
        int print_it
        );

#else

extern void debug_command();
extern int debug();
extern int debug_dump();
extern void debug_indent();
extern void debug_init();
extern int debug_initialize();
extern int debug_null_msg();
extern int debug_prompt();
extern void debug_query();
extern int debug_read();
extern char * debug_store_exe();
extern int debug_this();
extern void debug_trace_in();
extern void debug_trace_out();
extern int debug_keep();
extern void debug_tab_col();
extern int debug_dumpfds();

#endif /* ifdef FULLPROTO */


#ifdef DEBUG_VERSION
#define debug_init(ac,av) debug_initialize(DEBUG_VERSION,ac,av)
#else
#define debug_init(ac,av) debug_initialize("Unknown",ac,av)
#endif

#define BUFFER_SIZE 4096
/* Define command types for debug_command function.                     */
#define DEBUG_CLEAR     0       /* Clear module list (all includes etc) */
#define DEBUG_INCLUDE   1       /* Add include pattern to list          */
#define DEBUG_EXCLUDE   2       /* Add exclude pattern to list          */
#define DEBUG_TRACE     3       /* Add trace pattern to list            */
#define DEBUG_BREAK     4       /* Add break pattern to list            */
#define DEBUG_ON        5       /* Turn debugging on.                   */
#define DEBUG_OFF       6       /* Turn debugging off                   */
#define DEBUG_DUMP      7       /* Generate symbolic stack dump         */

extern int  debug_level;        /* global flag to indicate whether or not to execute debugging statements */
extern long profile_flag;       /* global flag to indicate whether or not to call profiling */
extern char *format,            /* pointer to a format string. Space for this will be malloc'd. */
            *debug_buf;         /* pointer to output string to display parameters in debug statements */
extern char debug_fmt[];        /* new format string and buffer ... */
extern char debug_msg[];        /* ... not allocated each time */
extern int  debug_dots_i;       /* current number of dots counter */
extern int  debug_function;     /* if TRUE, function name is printed */

/* define a macro to start debug in a module, it must be the first line
   in the module since it defines a variable */
#ifdef NO_DEBUG /* set NO_DEBUG to something to compile out debug */

#define debug_main(ac, av) debug_init(ac, av)
#define debug_on(func)
#define debug_on1(func, fmt, p1)
#define debug_on2(func, fmt, p1, p2)
#define debug_on3(func, fmt, p1, p2, p3)
#define debug_on4(func, fmt, p1, p2, p3, p4)
#define debug_on5(func, fmt, p1, p2, p3, p4, p5)
#define debug_on6(func, fmt, p1, p2, p3, p4, p5, p6)
#define debug_on7(func, fmt, p1, p2, p3, p4, p5, p6, p7)
#define debug_on8(func, fmt, p1, p2, p3, p4, p5, p6, p7, p8)
#define debug_on9(func, fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9)
#define debug_off(p1)
#define debug_return(fmt, p1)                               \
    { return(p1); }
#define debug_return_void(p1)                               \
    { return; }
#define debug0(fmt)
#define debug1(fmt, p1)
#define debug2(fmt, p1, p2)
#define debug3(fmt, p1, p2, p3)
#define debug4(fmt, p1, p2, p3, p4)
#define debug5(fmt, p1, p2, p3, p4, p5)
#define debug6(fmt, p1, p2, p3, p4, p5, p6)
#define debug7(fmt, p1, p2, p3, p4, p5, p6, p7)
#define debug8(fmt, p1, p2, p3, p4, p5, p6, p7, p8)
#define debug9(fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9)
#define debug_out0(fmt)
#define debug_out1(fmt, p1)
#define debug_out2(fmt, p1, p2)
#define debug_out3(fmt, p1, p2, p3)
#define debug_out4(fmt, p1, p2, p3, p4)
#define debug_out5(fmt, p1, p2, p3, p4, p5)
#define debug_out6(fmt, p1, p2, p3, p4, p5, p6)
#define debug_out7(fmt, p1, p2, p3, p4, p5, p6, p7)
#define debug_out8(fmt, p1, p2, p3, p4, p5, p6, p7, p8)
#define debug_out9(fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9)

#else

/************************************************************************
* Define macros for comparing the relative magnitude of debug_level and *
*   _local_debug_level.  The different possibilities are:               *
*                                                                       *
*   _local_debug_level < debug_level                                    *
*       Call   debug_this   (again)  to  determine  what  if  any       *
*       debugging is   enabled                                          *
*                                                                       *
*   _local_debug_level == debug_level                                   *
*       Debugging is disabled for module                                *
*                                                                       *
*   _local_debug_level == debug_level + 1                               *
*       Debugging is enabled for module                                 *
*                                                                       *
*   _local_debug_level == debug_level + 2                               *
*       Either a break or trace point is set for the module, call       *
*      debug_this to find out which.                                    *
*                                                                       *
*  The two macros DEBUG_TEST_ENTRY and DEBUG_TEST are due to different  *
*  handling of things when we enter a module compared to all other      *
*  debugging statements (we only look for trace and break points on     *
*  entry).                                                              *
*                                                                       *
************************************************************************/

#define DEBUG_TEST_ENTRY                                                    \
    ((_local_debug_level == debug_level + 1) ||                             \
     (_local_debug_level != debug_level &&                                  \
      (_local_debug_level = debug_level + debug_this(_db_function,TRUE))    \
       != debug_level))

#define DEBUG_TEST                                                          \
    ((_local_debug_level > debug_level) ||                                  \
     (_local_debug_level < debug_level &&                                   \
      (_local_debug_level = debug_level + debug_this(_db_function,FALSE))   \
       > debug_level))


#define debug_main(ac, av)                                                  \
    char *_db_function = "main";                                            \
    static int _local_debug_level;                                          \
    debug_init(ac, av);                                                     \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) debug_indent(_db_function, (char *)0);                   \
        }                                                                   \
    }
#define debug_on(func)                                                      \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) debug_indent(_db_function, (char *)0);                   \
        }                                                                   \
    }
#define debug_on1(func, fmt, p1)                                            \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) sprintf(debug_msg, fmt, p1);                             \
            (void) debug_indent(_db_function, debug_msg);                   \
        }                                                                   \
    }
#define debug_on2(func, fmt, p1, p2)                                        \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) sprintf(debug_msg, fmt, p1, p2);                         \
            (void) debug_indent(_db_function, debug_msg);                   \
        }                                                                   \
    }
#define debug_on3(func, fmt, p1, p2, p3)                                    \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) sprintf(debug_msg, fmt, p1, p2, p3);                     \
            (void) debug_indent(_db_function, debug_msg);                   \
        }                                                                   \
    }
#define debug_on4(func, fmt, p1, p2, p3, p4)                                \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) sprintf(debug_msg, fmt, p1, p2, p3, p4);                 \
            (void) debug_indent(_db_function, debug_msg);                   \
        }                                                                   \
    }
#define debug_on5(func, fmt, p1, p2, p3, p4, p5)                            \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5);             \
            (void) debug_indent(_db_function, debug_msg);                   \
        }                                                                   \
    }
#define debug_on6(func, fmt, p1, p2, p3, p4, p5, p6)                        \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6);         \
            (void) debug_indent(_db_function, debug_msg);                   \
        }                                                                   \
    }
#define debug_on7(func, fmt, p1, p2, p3, p4, p5, p6, p7)                    \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6, p7);     \
            (void) debug_indent(_db_function, debug_msg);                   \
        }                                                                   \
    }
#define debug_on8(func, fmt, p1, p2, p3, p4, p5, p6, p7, p8)                \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6, p7, p8); \
            (void) debug_indent(_db_function, debug_msg);                   \
        }                                                                   \
    }
#define debug_on9(func, fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9)            \
    char *_db_function = func;                                              \
    static int _local_debug_level;                                          \
    if ( debug_level ) {                                                    \
        debug_trace_in(_db_function);                                       \
        if ( DEBUG_TEST_ENTRY ) {                                           \
            debug_dots_i += 2;                                              \
            (void) sprintf(debug_msg,fmt,p1,p2,p3,p4,p5,p6,p7,p8,p9);       \
            (void) debug_indent(_db_function, debug_msg);                   \
        }                                                                   \
    }

#define debug_off(p1)                                       \
    if (debug_level && DEBUG_TEST ) {                       \
        debug_dots_i -= 2;                                  \
        (void) debug_indent(_db_function, p1);              \
    }

#define debug_return(fmt, p1)                               \
    {                                                       \
        if ( debug_level ) {                                \
            if ( DEBUG_TEST ) {                             \
                debug_null_msg(_db_function, (char *) fmt, (long) (p1));    \
            }                                               \
            debug_trace_out(_db_function);                  \
        }                                                   \
        return(p1);                                         \
    }

#define debug_return_void(p1)                                   \
    {                                                           \
        if ( debug_level ) {                                    \
            if ( DEBUG_TEST ) {                                 \
                debug_dots_i -= 2;                              \
                (void) debug_indent(_db_function, (char *) p1); \
            }                                                   \
            debug_trace_out(_db_function);                      \
        }                                                       \
        return;                                                 \
    }

#define debug0(fmt)                                         \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) debug_indent(_db_function, fmt);             \
    }
#define debug1(fmt, p1)                                     \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(debug_msg, fmt, p1);                 \
        (void) debug_indent(_db_function, debug_msg);       \
    }
#define debug2(fmt, p1, p2)                                 \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(debug_msg, fmt, p1, p2);             \
        (void) debug_indent(_db_function, debug_msg);       \
    }
#define debug3(fmt, p1, p2, p3)                             \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(debug_msg, fmt, p1, p2, p3);         \
        (void) debug_indent(_db_function, debug_msg);       \
    }
#define debug4(fmt, p1, p2, p3, p4)                         \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4);     \
        (void) debug_indent(_db_function, debug_msg);       \
    }
#define debug5(fmt, p1, p2, p3, p4, p5)                     \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5); \
        (void) debug_indent(_db_function, debug_msg);       \
    }
#define debug6(fmt, p1, p2, p3, p4, p5, p6)                     \
    if (debug_level && DEBUG_TEST ) {                           \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6); \
        (void) debug_indent(_db_function, debug_msg);           \
    }
#define debug7(fmt, p1, p2, p3, p4, p5, p6, p7)                     \
    if (debug_level && DEBUG_TEST ) {                               \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6, p7); \
        (void) debug_indent(_db_function, debug_msg);               \
    }
#define debug8(fmt, p1, p2, p3, p4, p5, p6, p7, p8)                     \
    if (debug_level && DEBUG_TEST ) {                                   \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6, p7, p8); \
        (void) debug_indent(_db_function, debug_msg);                   \
    }
#define debug9(fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9)                     \
    if (debug_level && DEBUG_TEST ) {                                       \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9); \
        (void) debug_indent(_db_function, debug_msg);                       \
    }
#define debug_out0(fmt)                                     \
    if (debug_level && DEBUG_TEST ) {                       \
        debug_function = FALSE;                             \
        debug(_db_function, fmt);                           \
        debug_function = TRUE;                              \
    }
#define debug_out1(fmt, p1)                                 \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(debug_msg, fmt, p1);                 \
        debug_function = FALSE;                             \
        debug(_db_function, debug_msg);                     \
        debug_function = TRUE;                              \
    }
#define debug_out2(fmt, p1, p2)                             \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(debug_msg, fmt, p1, p2);             \
        debug_function = FALSE;                             \
        debug(_db_function, debug_msg);                     \
        debug_function = TRUE;                              \
    }
#define debug_out3(fmt, p1, p2, p3)                         \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(debug_msg, fmt, p1, p2, p3);         \
        debug_function = FALSE;                             \
        debug(_db_function, debug_msg);                     \
        debug_function = TRUE;                              \
    }
#define debug_out4(fmt, p1, p2, p3, p4)                     \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(&debug_msg, fmt, p1, p2, p3, p4);    \
        debug_function = FALSE;                             \
        debug(_db_function, debug_msg);                     \
        debug_function = TRUE;                              \
    }
#define debug_out5(fmt, p1, p2, p3, p4, p5)                 \
    if (debug_level && DEBUG_TEST ) {                       \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5); \
        debug_function = FALSE;                             \
        debug(_db_function, debug_msg);                     \
        debug_function = TRUE;                              \
    }
#define debug_out6(fmt, p1, p2, p3, p4, p5, p6)                 \
    if (debug_level && DEBUG_TEST ) {                           \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6); \
        debug_function = FALSE;                                 \
        debug(_db_function, debug_msg);                         \
        debug_function = TRUE;                                  \
    }
#define debug_out7(fmt, p1, p2, p3, p4, p5, p6, p7)                 \
    if (debug_level && DEBUG_TEST ) {                               \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6, p7); \
        debug_function = FALSE;                                     \
        debug(_db_function, debug_msg);                             \
        debug_function = TRUE;                                      \
    }
#define debug_out8(fmt, p1, p2, p3, p4, p5, p6, p7, p8)                 \
    if (debug_level && DEBUG_TEST ) {                                   \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6, p7, p8); \
        debug_function = FALSE;                                         \
        debug(_db_function, debug_msg);                                 \
        debug_function = TRUE;                                          \
    }
#define debug_out9(fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9)                 \
    if (debug_level && DEBUG_TEST ) {                                       \
        (void) sprintf(debug_msg, fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9); \
        debug_function = FALSE;                                             \
        debug(_db_function, debug_msg);                                     \
        debug_function = TRUE;                                              \
    }

#endif

#define debug_area()                                        \
    if (debug_level && DEBUG_TEST )

/* Macro that returns the ptr, or the string "NULL" if it's null    */
#define ISNULL(ptr) ((ptr)?(ptr):"NULL")

/*  Macro that returns the strings "TRUE" or "FALSE" depending on   */
/*  the boolean value of the given variable.                        */
#define ISTRUE(bool) ((bool)?"TRUE":"FALSE")

/*  Macros that returns the ascii string value of *ptr, or the      */
/*  string "NULL" if it's null.  These macros  are to be used       */
/*  only when ptr points to the following scalar types:             */
/*      int, long, float or double.                                 */
/*  Note: include ut.h if you use these things...                   */
#define ISIPTR(ptr) ((ptr)?ut_itoa(*ptr, (char *) NULL):"NULL")
#define ISLPTR(ptr) ((ptr)?ut_ltoa(*ptr, (char *) NULL):"NULL")
#define ISFPTR(ptr) ((ptr)?ut_dtoa_p((double) *ptr, 8):"NULL")
#define ISDPTR(ptr) ((ptr)?ut_dtoa_p(*ptr, 12):"NULL")

/*  Macros that return the ascii decimal string value of ptr, or    */
/*  the string "NULL" if it's null.  Hex output might be more       */
/*  desirable, but its decimal for now!?                            */
#define ISNPTR(ptr) ((ptr)?ut_ultoa(ptr, (char *) NULL):"NULL")

#ifdef C_PLUSPLUS
}
#endif


#endif

