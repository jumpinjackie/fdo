/*
* Copyright (C) 2007  Haris Kurtagic
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
#pragma once

#include "c_KgOtl.h"


// Ora ind is index on which I am looking for property
// beacuse it is from 1 i need to decrement by 2 before calling get_netx
#define D_OTL_SET_CURRENT_COL_ORAIND(OtlStream,OraInd) (*OtlStream->ss)->cur_col = OraInd-2;(*OtlStream->ss)->get_next();


class c_KgOtlStreamReader
{
public:
  c_KgOtlStreamReader(otl_stream* Stream);
  ~c_KgOtlStreamReader(void);

protected:
  //otl_connect* m_OtlConnect; // connect object  
  otl_stream* m_Stream; // connect object    
  
  otl_stream_read_iterator<otl_stream,otl_exception,otl_lob_stream> m_Rs; 
  
  bool m_Attached;
  
public:
  void Close(void);
  // Stream reading  
  bool ReadNext(); // read next record from stream
  FdoByte GetByte(int OraInd);
  FdoBoolean GetBoolean(int OraInd);
  FdoDateTime GetDateTime(int OraInd);
  FdoDouble GetDouble(int OraInd);
  FdoFloat GetFloat(int OraInd);
  FdoInt16 GetInt16(int OraInd);
  FdoInt32 GetInt32(int OraInd);
  FdoInt64 GetInt64(int OraInd);
  //static FdoString* GetString(otl_stream* OtlStream,int OraInd);
  void GetString(int OraInd,wstring& String);
  bool GetIsNull(int OraInd);
  FdoByteArray* GetGeometryFgf(int OraInd);
  
};

