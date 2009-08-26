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
#define _POSIX_ 1
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <Inc/debugext.h>

static  int      Strace_line = 0;


#include <sys\timeb.h>
#include <time.h>

static double get_time()
{

   struct _timeb        timebuffer;
   double               our_time;

   _ftime( &timebuffer );
   our_time = ((double) timebuffer.time) + timebuffer.millitm /1000.0;
   return our_time;
}

static void output_message(
    FILE *out_file,
    char *msg,
	wchar_t *msgW,
    double elapsed,
    double own_utime,
    double own_stime,
    double child_utime,
    double child_stime
    );


int debug_trace(
    char    *msg,
	wchar_t *msgW,
    int     *line_num
    )
{

    static  FILE    *Strace_fp   = NULL;
    double          elapsed = 0;
    static  double  Selapsed;
    double          own_utime = 0;
    double          own_stime = 0;
    double          child_utime = 0;
    double          child_stime = 0;
    char            normal_name[PATH_MAX + 1];
    char *          type = NULL;
	FILETIME user_rp, kernel_rp, temp_rp;
	SYSTEMTIME SystemTime, SystemTime2;

    if ( Strace_line == 0 ) {
        char        *file_name;

        file_name = getenv("FDO_TRACE_FILE");
        if ( file_name != NULL ) {
            if (*file_name == '+') {
                file_name++;
                type = "a";
            }
            else {
                type = "w";
            }
            strcpy(normal_name,file_name);
            Strace_fp = fopen(normal_name, type);
        }
		Selapsed = get_time();
    }
    Strace_line++;

	GetProcessTimes(GetCurrentProcess(), &temp_rp, &temp_rp, &kernel_rp, &user_rp);
	FileTimeToSystemTime(&user_rp, &SystemTime);
	FileTimeToSystemTime(&kernel_rp, &SystemTime2);

	own_utime = ((SystemTime.wHour*60)+SystemTime.wMinute)*60+SystemTime.wSecond + SystemTime.wMilliseconds/1000.0;
	own_stime = ((SystemTime2.wHour*60)+SystemTime2.wMinute)*60+SystemTime2.wSecond + SystemTime2.wMilliseconds/1000.0;

    if ( Strace_fp != NULL ) {
		elapsed = get_time() - Selapsed;
        if ( Strace_fp != NULL ) {
            output_message(Strace_fp,msg, msgW, elapsed,
                            own_utime,own_stime,child_utime,child_stime);
        }
    }

    if ( line_num != NULL )
        *line_num = Strace_line;

    return(0);

}


static void output_message(
    FILE *out_file,
    char *msg,
	wchar_t *msgW,
    double elapsed,
    double own_utime,
    double own_stime,
    double child_utime,
    double child_stime
    )
{                           
    fprintf(out_file, "Cum. E: %.2lf, CPU: U %.2lf, S %.2lf",
                                        elapsed, own_utime, own_stime);
    if ( child_utime > 0.0 || child_stime > 0.0 )
        fprintf(out_file, ", child user %.2lf, child system %.2lf",
                                        child_utime, child_stime);
	if (msg)
		fprintf(out_file, "\n\n!!%d: %s\n%s\n", Strace_line,"FDO", msg);
	else
		fwprintf(out_file, L"\n\n!!%d: %s\n%s\n", Strace_line, L"FDO", msgW);

    fflush(out_file);
}
#else
int debug_trace(
    char    *msg,
	wchar_t *msgW,
    int     *line_num
    )
{
 return 0;
}
#endif
