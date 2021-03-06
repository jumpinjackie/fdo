#include "StdAfx.h"
#include "c_KgOtl.h"


c_KgOtlStreamReader::c_KgOtlStreamReader(otl_stream* Stream)
{
  m_Stream = Stream;
  
  m_Attached=false;
}

c_KgOtlStreamReader::~c_KgOtlStreamReader(void)
{
  //if( m_Stream ) m_Rs.detach();
}

void c_KgOtlStreamReader::Close(void)
{
try
  {
  if( m_Stream ) m_Rs.detach();
  }
  catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
}

// Stream reading  
bool c_KgOtlStreamReader::ReadNext()
{
  if( !m_Stream ) return false;
  
  try
  {
    //return (*m_OtlStream->ss)->next() != 0;  
    
    int ret;
    
    
    if( !m_Attached ) 
    {
      //ret = (*m_Stream->ss)->open();          
      m_Stream->rewind();
      m_Stream->create_var_desc();
      m_Rs.attach(*m_Stream); // attach the iterator "rs" to the stream "i".
      
      ret = m_Rs.next_row();
      m_Attached = true;
    }
    else
    {
      ret = m_Rs.next_row();
    }
    return ret;
   
    
  }
  catch(otl_exception& ea)
  {   
    throw c_KgOtlException(ea.msg,ea.stm_text);
  }
}//end of c_KgOtlStreamReader::ReadNext

bool c_KgOtlStreamReader::GetIsNull(int OraInd)  
{
if( !m_Stream ) return false;
  
try
{
  //(*m_OtlStream->ss)->cur_col = OraInd - 2;
  //return (*m_OtlStream->ss)->is_null() == 1;
  return m_Rs.is_null(OraInd);
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
  //(*m_OtlStream->ss)->cur_col = OraInd - 2;  
  //(*m_OtlStream)>>val;
  m_Rs.get(OraInd,val);
  
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
  //(*m_OtlStream->ss)->cur_col = OraInd - 2;
  //(*m_OtlStream)>>val;
  m_Rs.get(OraInd,val);
  
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
  //(*m_OtlStream->ss)->cur_col = OraInd - 2;
  //(*m_OtlStream)>>val;
  m_Rs.get(OraInd,val);
  
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
  //(*m_OtlStream->ss)->cur_col = OraInd - 2;
  //(*m_OtlStream)>>val;
  m_Rs.get(OraInd,val);
  
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
  //(*m_OtlStream->ss)->cur_col = OraInd - 2;
  //(*m_OtlStream)>>val;
  m_Rs.get(OraInd,val);
  
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
  //(*m_OtlStream->ss)->cur_col = OraInd - 2;
  //(*m_OtlStream)>>String;
  
  string str;
  m_Rs.get(OraInd,str);
  
  FdoStringP fstr;
  fstr = str.c_str();
  
  String = fstr;
  
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
  //TIMESTAMP_STRUCT val;
  //(*m_OtlStream->ss)->cur_col = OraInd - 2;
  //(*m_OtlStream)>>val;
  otl_datetime val; // gets an otl_datetime
  m_Rs.get(OraInd,val);
  
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
  string val;
  //(*m_OtlStream->ss)->cur_col = OraInd - 2;
  //(*m_OtlStream)>>val;
  
  m_Rs.get(OraInd,val);
  
  if( _strcmpi(val.c_str(),"TRUE") == 0 ) return true;
  if( _strcmpi(val.c_str(),"1") == 0 ) return true;
  
  //if( _wcsicmp(val.c_str(),L"TRUE") == 0 ) return true;
  //if( _wcsicmp(val.c_str(),L"1") == 0 ) return true;
  
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
  m_Rs.get(OraInd,val);
  FdoPtr<FdoByteArray> barray = FdoByteArray::Create(val.v,val.len() );
  
  FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
  FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromWkb(barray);
  
  FdoPtr<FdoByteArray> fgf = gf->GetFgf(geom);
  return FDO_SAFE_ADDREF(fgf.p); 
  
 
 /* string val;
  
  m_Rs.get(OraInd,val);
  
  int found=val.find_first_of(" ");
  val = val.insert(found+1,"XY");
  //FdoPtr<FdoByteArray> bytearray = FdoByteArray::Create( (FdoByte*)val.c_str(), val.length() );
  
  FdoStringP fstr,f2;
  fstr = val.c_str();
  f2 = L"GeomFromText('";
  f2 += fstr; 
  f2 += L"')";
  
  FdoGeometryValue* geom2 = dynamic_cast<FdoGeometryValue*>(FdoExpression::Parse(f2));
  FdoByteArray* barray = geom2->GetGeometry();
  */
  //return barray;
  /*
  
 
  
  return FDO_SAFE_ADDREF(fgf.p);    
  */
}
catch(otl_exception& ea)
{   
  throw c_KgOtlException(ea.msg,ea.stm_text);
}
}//end of c_KgOtlStreamReader::GetGeometryFgf
  
  

