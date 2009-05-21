/*
* Copyright (C) 2006  SL-King d.o.o
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

#include "StdAfx.h"
#include <stdarg.h>
#include "c_LogAPI.h"



extern wchar_t g_LogFileName[MAX_PATH];

FdoCommonThreadMutex c_LogAPI::m_Mutex;

c_LogAPI::c_LogAPI(void)
{
}

c_LogAPI::~c_LogAPI(void)
{
}

void c_LogAPI::WriteLog0(const char* Text)
{
  
    
    m_Mutex.Enter();
    
    FILE * GisDebugFile = _wfopen(g_LogFileName, L"a+");
    if( GisDebugFile )
    {
    // add new line date and time
      struct tm *newtime;
      
      __time64_t long_time;

      _time64( &long_time );           // Get time as 64-bit integer.
                                       // Convert to local time.
      newtime = _localtime64( &long_time ); // C4996
      fprintf(GisDebugFile, "\n<%d-%d-%d %d:%d:%d>",newtime->tm_yday,newtime->tm_mon,newtime->tm_mday,newtime->tm_hour,newtime->tm_min,newtime->tm_sec);

      fprintf(GisDebugFile, Text);
      fclose(GisDebugFile);
      
      
    }

    m_Mutex.Leave();
    
}//end of c_LogAPI::WriteLog


void c_LogAPI::WriteLog(const char* text,...)
{
  
    
    va_list args;
    va_start(args,text);
    
    int i = va_arg( args, int);    
    va_end(args);
    
    va_start(args,text);
    long ld = va_arg( args, long);    
    va_end(args);
    
    va_start(args,text);
    const char* t1 = va_arg( args, const char*);    
    
    va_start(args,text);

     m_Mutex.Enter();
    FILE * GisDebugFile = _wfopen(g_LogFileName, L"a+");
    if( GisDebugFile )
    {
    // add new line date and time
      struct tm *newtime;
      
      __time64_t long_time;

      _time64( &long_time );           // Get time as 64-bit integer.
                                       // Convert to local time.
      newtime = _localtime64( &long_time ); // C4996
      fprintf(GisDebugFile, "\n<%d-%d-%d %d:%d:%d>",newtime->tm_yday,newtime->tm_mon,newtime->tm_mday,newtime->tm_hour,newtime->tm_min,newtime->tm_sec);

      char* cbuff = new char[2048];
      cbuff[2047]=0;
      vsnprintf(cbuff,2040,text,args);
      
      fprintf(GisDebugFile, cbuff);
      fclose(GisDebugFile);
      
      delete[] cbuff;
    }
     m_Mutex.Leave();
    
    va_end(args);
    
}//end of c_LogAPI::WriteLog
