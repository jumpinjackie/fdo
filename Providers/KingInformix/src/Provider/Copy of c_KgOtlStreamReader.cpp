#include "StdAfx.h"
#include "c_KgOtl.h"


c_KgOtlStreamReader::c_KgOtlStreamReader(otl_stream* Stream)
{
  m_OtlStream = Stream;
}

c_KgOtlStreamReader::~c_KgOtlStreamReader(void)
{
}

// Stream reading  
bool c_KgOtlStreamReader::ReadNext()
{
  if( !m_OtlStream ) return false;
  
  try
  {
    return (*m_OtlStream->ss)->next() != 0;    
  }
  catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
}//end of c_KgOtlStreamReader::ReadNext

bool c_KgOtlStreamReader::GetIsNull(int OraInd)  
{
if( !m_OtlStream ) return false;
  
try
{
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  
  return (*m_OtlStream->ss)->is_null() == 1;
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
}  

}//end of 

FdoDouble c_KgOtlStreamReader::GetDouble(int OraInd)
{
try
{
  double val;
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  
  (*m_OtlStream)>>val;
  
  return val;
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
} 
  
}//end of c_KgOtlStreamReader::GetDouble

FdoFloat c_KgOtlStreamReader::GetFloat(int OraInd)
{
try
{
  float val;
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  
  
  (*m_OtlStream)>>val;
  
  return val;
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
} 
  
}//end of c_KgOtlStreamReader::GetFloat

FdoInt16 c_KgOtlStreamReader::GetInt16(int OraInd)
{
try
{
  short val;
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  
  
  (*m_OtlStream)>>val;
  
  return val;
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
} 
  
}//end of c_KgOtlStreamReader::GetInt16

FdoInt32 c_KgOtlStreamReader::GetInt32(int OraInd)
{
try
{
  int val;
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  //
  
  (*m_OtlStream)>>val;
  
  return val;
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
} 
  
}//end of c_KgOtlStreamReader::GetInt32

FdoInt64 c_KgOtlStreamReader::GetInt64(int OraInd)
{
try
{
  long long val;
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  
  
  (*m_OtlStream)>>val;
  
  return val;
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
} 
  
}//end of c_KgOtlStreamReader::GetInt64


void c_KgOtlStreamReader::GetString(int OraInd,wstring& String)
{
try
{
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  //
  
  (*m_OtlStream)>>String;
  
  return ;
  
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
} 
  
}//end of c_KgOtlStreamReader::GetString

FdoDateTime c_KgOtlStreamReader::GetDateTime(int OraInd)
{
  
try
{
  TIMESTAMP_STRUCT val;
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  
  
  (*m_OtlStream)>>val;
  
  //FdoDateTime dt(
  
  return FdoDateTime(val.year,val.month,val.day,val.hour,val.minute,val.second);
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
} 
}

FdoByte c_KgOtlStreamReader::GetByte(int OraInd)
{
  return 0;
}
FdoBoolean c_KgOtlStreamReader::GetBoolean(int OraInd)
{
try
{
  wstring val;
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  
  
  (*m_OtlStream)>>val;
  
  if( _wcsicmp(val.c_str(),L"TRUE") == 0 ) return true;
  if( _wcsicmp(val.c_str(),L"1") == 0 ) return true;
  
  return false;
  
  
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
}

}


FdoByteArray* c_KgOtlStreamReader::GetGeometryFgf(int OraInd)
{
try
{
  otl_long_string val(70000); 
  (*m_OtlStream->ss)->cur_col = OraInd - 2;
  
  
  (*m_OtlStream)>>val;
  
  FdoPtr<FdoByteArray> bytearray = FdoByteArray::Create(val.v,val.len() );
  
  FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
  FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromWkb(bytearray);
  
  FdoPtr<FdoByteArray> fgf = gf->GetFgf(geom);
  
  return FDO_SAFE_ADDREF(fgf.p);    
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
}
}//end of c_KgOtlStreamReader::GetGeometryFgf
  
  

