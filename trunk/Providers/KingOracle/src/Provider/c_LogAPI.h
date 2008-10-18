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


#ifndef _c_LogAPI_h
#define _c_LogAPI_h

//#define _KGORA_EXTENDED_LOG

class c_LogAPI
{
public:
  c_LogAPI(void);
public:
  ~c_LogAPI(void);
  
  static void WriteLog0(const char* text);
  static void WriteLog(const char* text , ...);
  
protected:
  static	FdoCommonThreadMutex m_Mutex;  
};

#ifdef _DEBUG
  #define _KGORA_EXTENDED_LOG
#endif

#ifdef _KGORA_EXTENDED_LOG
  #define D_KGORA_ELOG_START_FUNC_TIME  clock_t elog_t1=clock();

  #define D_KGORA_ELOG_WRITE_FUNC_TIME(name)  clock_t elog_t2=clock();c_LogAPI::WriteLog("Execution time '%s' = %ld",name,(long)((elog_t2-elog_t1)* CLOCKS_PER_SEC / 1000));
  #define D_KGORA_ELOG_WRITE(Text) c_LogAPI::WriteLog0(Text);
  #define D_KGORA_ELOG_WRITE1(Text,p1) c_LogAPI::WriteLog(Text,p1);
  #define D_KGORA_ELOG_WRITE2(Text,p1,p2) c_LogAPI::WriteLog(Text,p1,p2);
  #define D_KGORA_ELOG_WRITE3(Text,p1,p2,p3) c_LogAPI::WriteLog(Text,p1,p2,p3);
  #define D_KGORA_ELOG_WRITE4(Text,p1,p2,p3,p4) c_LogAPI::WriteLog(Text,p1,p2,p3,p4);
  #define D_KGORA_ELOG_WRITE5(Text,p1,p2,p3,p4,p5) c_LogAPI::WriteLog(Text,p1,p2,p3,p4,p5);
#else
  #define D_KGORA_ELOG_START_FUNC_TIME
  #define D_KGORA_ELOG_WRITE_FUNC_TIME
  #define D_KGORA_ELOG_WRITE(Text)
  #define D_KGORA_ELOG_WRITE1(Text,p1)
  #define D_KGORA_ELOG_WRITE2(Text,p1,p2)
  #define D_KGORA_ELOG_WRITE3(Text,p1,p2,p3)
  #define D_KGORA_ELOG_WRITE4(Text,p1,p2,p3,p4,p5)
  #define D_KGORA_ELOG_WRITE5(Text,p1,p2,p3,p4,p5)
#endif




#endif
