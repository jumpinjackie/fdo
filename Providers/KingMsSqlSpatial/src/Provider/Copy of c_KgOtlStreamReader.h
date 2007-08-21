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
  otl_stream* m_OtlStream; // connect object    
  
public:
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

