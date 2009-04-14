/*
* Copyright (C) 2009  SL-King d.o.o
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
#include "stdafx.h"
#include "c_Ora_API2.h"

c_Ora_API2::c_Ora_API2(void)
{
}

c_Ora_API2::~c_Ora_API2(void)
{
}


wchar_t* c_Ora_API2::SdoGeomToStringW(c_SDO_GEOMETRY *SdoGeom)
{
  
  long ordnum = SdoGeom->GetSdoOrdinatesSize();
  long einfonum = SdoGeom->GetSdoElemInfoSize();
  long buffsize = ordnum * 24 + einfonum * 6  + 256;
  
  wchar_t* Buff = new wchar_t[buffsize];
  
  
  
  wchar_t bufnum[128];
  
  wcscpy(Buff,L"MDSYS.SDO_GEOMETRY(");
  
  if( SdoGeom->IsNullSdoGtype() )
  {
    wcscat(Buff,L"NULL");
  }
  else
  {
    FdoCommonOSUtil::swprintf(bufnum,128,L"%ld",(long)SdoGeom->GetSdoGtype());
    wcscat(Buff,bufnum);
  }
  
  wcscat(Buff,L",");
  
  if( SdoGeom->IsNullSdoSrid() )
  {
    wcscat(Buff,L"NULL");
  }
  else
  {
    FdoCommonOSUtil::swprintf(bufnum,128,L"%ld",(long)SdoGeom->GetSdoSrid());
    wcscat(Buff,bufnum);
  }
  
  wcscat(Buff,L",");
  
  //if( !SdoGeom->getSdo_point() )
  if( SdoGeom->IsNullSdoPoint() )
  {
    wcscat(Buff,L"NULL");
  }
  else
  {
    
    FdoCommonOSUtil::swprintf(bufnum,128,L"MDSYS.SDO_POINT_TYPE(%.8lf,%.8lf,%.8lf)",(double)SdoGeom->GetSdoPointX(),(double)SdoGeom->GetSdoPointY(),(double)SdoGeom->GetSdoPointZ());
    wcscat(Buff,bufnum);
  }
  
  wcscat(Buff,L",");

  // elem info
  wcscat(Buff,L"MDSYS.SDO_ELEM_INFO_ARRAY(");
  long size = (long)SdoGeom->GetSdoElemInfoSize();
  
  if( size > 0 )
  {
    FdoCommonOSUtil::swprintf(bufnum,128,L"%ld",(long)SdoGeom->GetSdoElemInfo(0));    
    wcscat(Buff,bufnum);    
    for(long ind=1;ind<size;ind++)
    {
      wcscat(Buff,L",");
      FdoCommonOSUtil::swprintf(bufnum,128,L"%ld",(long)SdoGeom->GetSdoElemInfo(ind));    
      wcscat(Buff,bufnum);    
    }
  }
  wcscat(Buff,L")");
  
  wcscat(Buff,L",");
  
  // ordinates
  wcscat(Buff,L"MDSYS.SDO_ORDINATE_ARRAY(");
  long sizeord = (long)SdoGeom->GetSdoOrdinatesSize();
  if( sizeord > 0 )
  {
    FdoCommonOSUtil::swprintf(bufnum,128,L"%.8lf",(double)SdoGeom->GetSdoOrdinate(0));    
    wcscat(Buff,bufnum);    
      
    for(long ind=1;ind<sizeord;ind++)
    {
      wcscat(Buff,L",");
      FdoCommonOSUtil::swprintf(bufnum,128,L"%.8lf",(double)SdoGeom->GetSdoOrdinate(ind));    
      wcscat(Buff,bufnum);    
    }
  }
  wcscat(Buff,L")");
  
  // end of SDO_GEOMETRY_TYPE(
  wcscat(Buff,L")");
  
  return Buff;
}//end of c_Ora_API2::SdoGeomToString

char* c_Ora_API2::SdoGeomToString(c_SDO_GEOMETRY *SdoGeom)
{
  
  long ordnum = SdoGeom->GetSdoOrdinatesSize();
  long einfonum = SdoGeom->GetSdoElemInfoSize();
  long buffsize = ordnum * 24 + einfonum * 6  + 256;
  
  char* Buff = new char[buffsize];
  
  
  
  char bufnum[128];
  
  strcpy(Buff,"MDSYS.SDO_GEOMETRY(");
  
  if( SdoGeom->IsNullSdoGtype() )
  {
    strcat(Buff,"NULL");
  }
  else
  {
    sprintf(bufnum,"%ld",(long)SdoGeom->GetSdoGtype());
    strcat(Buff,bufnum);
  }
  
  strcat(Buff,",");
  
  if( SdoGeom->IsNullSdoSrid() )
  {
    strcat(Buff,"NULL");
  }
  else
  {
    sprintf(bufnum,"%ld",(long)SdoGeom->GetSdoSrid());
    strcat(Buff,bufnum);
  }
  
  strcat(Buff,",");
  
  //if( !SdoGeom->getSdo_point() )
  if( SdoGeom->IsNullSdoPoint() )
  {
    strcat(Buff,"NULL");
  }
  else
  {
    
    sprintf(bufnum,"MDSYS.SDO_POINT_TYPE(%.8lf,%.8lf,%.8lf)",(double)SdoGeom->GetSdoPointX(),(double)SdoGeom->GetSdoPointY(),(double)SdoGeom->GetSdoPointZ());
    strcat(Buff,bufnum);
  }
  
  strcat(Buff,",");

  // elem info
  strcat(Buff,"MDSYS.SDO_ELEM_INFO_ARRAY(");
  long size = (long)SdoGeom->GetSdoElemInfoSize();
  
  if( size > 0 )
  {
    sprintf(bufnum,"%ld",(long)SdoGeom->GetSdoElemInfo(0));    
    strcat(Buff,bufnum);    
    for(long ind=1;ind<size;ind++)
    {
      strcat(Buff,",");
      sprintf(bufnum,"%ld",(long)SdoGeom->GetSdoElemInfo(ind));    
      strcat(Buff,bufnum);    
    }
  }
  strcat(Buff,")");
  
  strcat(Buff,",");
  
  // ordinates
  strcat(Buff,"MDSYS.SDO_ORDINATE_ARRAY(");
  long sizeord = (long)SdoGeom->GetSdoOrdinatesSize();
  if( sizeord > 0 )
  {
    sprintf(bufnum,"%.8lf",(double)SdoGeom->GetSdoOrdinate(0));    
    strcat(Buff,bufnum);    
      
    for(long ind=1;ind<sizeord;ind++)
    {
      strcat(Buff,",");
      sprintf(bufnum,"%.8lf",(double)SdoGeom->GetSdoOrdinate(ind));    
      strcat(Buff,bufnum);    
    }
  }
  strcat(Buff,")");
  
  // end of SDO_GEOMETRY_TYPE(
  strcat(Buff,")");
  
  return Buff;
}//end of c_Ora_API2::SdoGeomToString


c_SDO_GEOMETRY* c_Ora_API2::CreateOptimizedRect(c_Oci_Connection*Con,bool IsGeodeticCS,long OraSrid,double MinX,double MinY,double MaxX,double MaxY)
{
  c_SDO_GEOMETRY * sdogeom = c_SDO_GEOMETRY::Create(Con); //new SDO_GEOMETRY_TYPE; //SdoGeom->NewSdoGeometry();
  
  int ora_gtype = 2003;
  
  sdogeom->SetSdoGtype(ora_gtype);
  
  if( OraSrid > 0 )
  {
    sdogeom->SetSdoSrid(OraSrid);
  }
  else
  {
    sdogeom->SetNull_SdoSrid();
  }
  
  
  // elem info
  sdogeom->AppendElemInfoArray(1);
  sdogeom->AppendElemInfoArray(1003);
  sdogeom->AppendElemInfoArray(3);
  
  //o ordinates
  if( IsGeodeticCS && MinX < -179.99 ) MinX = -179.99;
  sdogeom->AppendSdoOrdinates(MinX);
  
  if( IsGeodeticCS && MinY < -89.99 ) MinY = -89.99;
  sdogeom->AppendSdoOrdinates(MinY);
  
  if( IsGeodeticCS && MaxX > 179.99 ) MaxX = 179.99;
  sdogeom->AppendSdoOrdinates(MaxX);
  
  if( IsGeodeticCS && MaxY > 89.99 ) MaxY = 89.99;
  sdogeom->AppendSdoOrdinates(MaxY);
  
  
  return sdogeom;
  
}//end of c_Ora_API2::CreateOptimizedRect


long c_Ora_API2::GetSrid(c_Oci_Connection*Conn,const wchar_t* CoordSysName)
{
  c_Oci_Statement* oci_stm=NULL;
  long srid = 0;

  oci_stm = Conn->CreateStatement();  
  
  std::wstring sqlstr;
  
  sqlstr = L" select SRID,CS_NAME,WKTEXT from MDSYS.cs_srs where cs_name = '";
  sqlstr = sqlstr + CoordSysName;
  sqlstr = sqlstr + L"'";
  
  oci_stm->Prepare(sqlstr.c_str());
  
  oci_stm->ExecuteSelectAndDefine();
  
  if( oci_stm->ReadNext() )
  {
    if( !oci_stm->IsColumnNull(1) )
    {
      srid = oci_stm->GetInteger(1);            
    }      
  }
  
  
  Conn->TerminateStatement (oci_stm);
  
  return srid;
  
}//end of c_Ora_API2::GetSrid


bool c_Ora_API2::GetCoordinateSystemWkt(c_Oci_Connection*Conn,long Srid,std::wstring& Wkt)
{
  c_Oci_Statement* oci_stm=NULL;
  

  oci_stm = Conn->CreateStatement();  
  
  std::wstring sqlstr;
  sqlstr = L" select WKTEXT,CS_NAME,SRID from MDSYS.cs_srs where SRID = :1";
  
  oci_stm->Prepare(sqlstr.c_str());
  oci_stm->BindLong(1,&Srid);
  
  oci_stm->ExecuteSelectAndDefine();
  
  bool found=false;
  if( oci_stm->ReadNext()  )
  {
    if( !oci_stm->IsColumnNull(1) )
    {
      Wkt = oci_stm->GetString(1);
      found=true;
    }      
  }
  
  
  Conn->TerminateStatement (oci_stm);
  
  return found;
  
}//end of c_Ora_API2::GetCoordinateSystemWkt

long c_Ora_API2::GetSequenceNextVal(c_Oci_Connection*Conn,const wchar_t* SequenceName)
{
  c_Oci_Statement* oci_stm=NULL;
  
  long nextval = 0;

  oci_stm = Conn->CreateStatement();  
  
  std::wstring sqlstr;
  sqlstr = L" select ";
  sqlstr = sqlstr + SequenceName + L".nextval from dual";
  
  
  oci_stm->Prepare(sqlstr.c_str());
  
  oci_stm->ExecuteSelectAndDefine();
  
  if( oci_stm->ReadNext()  )
  {
    if( !oci_stm->IsColumnNull(1) )
    {
      nextval = oci_stm->GetInteger(1);
    }      
  }
  
  Conn->TerminateStatement (oci_stm);
  
  return nextval;
  
}//end of c_Ora_API2::GetSequenceNextVal


bool c_Ora_API2::ResetSequence(c_Oci_Connection*Conn,const wchar_t* SequenceName,const wchar_t* FullTableName,const wchar_t* ColumnName)
{
  c_Oci_Statement* oci_stm=NULL;
  long nextval = 0;

try
{
  oci_stm = Conn->CreateStatement();  
  
  long maxid;
  
  std::wstring sql,strseq;
  strseq = SequenceName;
  
  std::wstring col = ColumnName;
  sql = L"SELECT MAX(";
  sql = sql + col + L") as MAXID from " + FullTableName;    
  
  oci_stm->Prepare(sql.c_str());
  
  oci_stm->ExecuteSelectAndDefine();
  
  if( oci_stm->ReadNext() )
  {
    if( !oci_stm->IsColumnNull(1) )
    {
     
      
      maxid = oci_stm->GetInteger(1);
      
      
      sql = L"SELECT " + strseq + L".nextval as CURR from dual";  
      oci_stm->Prepare(sql.c_str());  
      oci_stm->ExecuteSelectAndDefine();
      if( oci_stm->ReadNext() )
      {
        if( !oci_stm->IsColumnNull(1) )
        {
          long currval;
          
          currval = oci_stm->GetLong(1);
          
          
          
          long inc;
          inc = maxid -  currval;
          // now calculate increment
          if( (long)inc > 0 )
          {
            FdoStringP buff = FdoStringP::Format(L"%ld",inc);
            std::wstring incstr = (FdoString*)buff;
           
            sql = L"ALTER SEQUENCE " + strseq + L" INCREMENT BY " + incstr + L" MINVALUE 0";  
            oci_stm->Prepare(sql.c_str()); 
            oci_stm->ExecuteNonQuery(OCI_COMMIT_ON_SUCCESS); 
            
            sql = L"SELECT " + strseq + L".nextval from dual";    
            oci_stm->Prepare(sql.c_str()); 
            oci_stm->ExecuteSelectAndDefine();
            oci_stm->ReadNext();
            
            sql = L"ALTER SEQUENCE " + strseq + L" INCREMENT BY 1";     
            oci_stm->Prepare(sql.c_str()); 
            oci_stm->ExecuteNonQuery(OCI_COMMIT_ON_SUCCESS); 
                        
          }  
        }
      }
    }      
  }

  
  
  
  
  if( oci_stm ) Conn->TerminateStatement (oci_stm);
  
  return true;
}
catch(c_Oci_Exception* ex )
{
  if( oci_stm ) Conn->TerminateStatement (oci_stm);
  throw ex;
}
return false;  
  
}//end of c_Ora_API2::ResetSequence


bool c_Ora_API2::GetOracleVersion(c_Oci_Connection*Conn,int& MainVersion,int& SubVersion)
{

  c_Oci_Statement* oci_stm=NULL;
  
  long srid = 0;
  bool ret=false;
  
  MainVersion = 11;
  SubVersion = 1;     
try
{
  oci_stm = Conn->CreateStatement();  
  
  std::wstring sqlstr;
  sqlstr = L" SELECT * FROM V$VERSION  ";
  
  oci_stm->Prepare(sqlstr.c_str());  
  oci_stm->ExecuteSelectAndDefine();
  
  if( oci_stm->ReadNext() )
  {
    if( !oci_stm->IsColumnNull(1) )
    {
      std::wstring banner;
      banner = oci_stm->GetString(1);
      
      // find
      size_t pos = banner.find(L"Oracle");
      if( pos != std::wstring::npos )
      {
      // find numbers in it
        if( banner.find(L"10.2") != std::wstring::npos )
        {
          MainVersion = 10;
          SubVersion = 2;   
          ret = true;       
        }
        else
        {
          if( banner.find(L"10.1") != std::wstring::npos )
          {
            MainVersion = 10;
            SubVersion = 1;
            ret = true;
          } 
          else
          {
            if( banner.find(L"9.2") != std::wstring::npos )
            {
              MainVersion = 9;
              SubVersion = 2;
              ret = true;
            }
            else
            if( banner.find(L"9.1") != std::wstring::npos )
            {
              MainVersion = 9;
              SubVersion = 1;
              ret = true;
            }
          }
        }
      }
    }      
  }
  

  Conn->TerminateStatement (oci_stm);
  
  return ret;
}
catch(c_Oci_Exception* ex )
{
  if( oci_stm )
  {    
    Conn->TerminateStatement(oci_stm);  
  }
  throw ex;
}  

return false;
  
}//end of c_Ora_API2::GetSrid

bool c_Ora_API2::IsGeodeticCoordSystem(const wchar_t* CoordSysWkt)
{
  std::wstring cswkt;
  
  cswkt = CoordSysWkt;
  
  
  if( cswkt.length() > 6 )
  {
    std::wstring substr = cswkt.substr(0,6);
    if( substr.compare(L"GEOGCS")==0 )
    {
      return true;
    }
  }
  
  return false;
  
}//

int c_Ora_API2::GetTablePkeyColumns(c_Oci_Connection * OciConn,const wchar_t* Owner, const wchar_t* TableName,std::vector<std::wstring>& ColNames)
{
    int numcols=0;
    
  c_Oci_Statement *stm = OciConn->CreateStatement(); 
  
  
  stm->Prepare(L"SELECT acc.column_name"
              L" FROM all_constraints ac, all_cons_columns acc"
              L" WHERE ac.owner = :1 and ac.table_name = :2 AND ac.constraint_type = 'P'"
              L" AND ac.owner = acc.owner AND ac.constraint_name = acc.constraint_name"
             );
  
  stm->BindString(1,Owner);
  stm->BindString(2,TableName);
  
  stm->ExecuteSelectAndDefine();
  //ResultSet * rs = stm->executeQuery();
  while( stm->ReadNext() )
  {
    ColNames.push_back(stm->GetString(1));
    numcols++;
  }
  
  OciConn->TerminateStatement (stm);
        
  return numcols;
}//end of SdoGeom->GetTablePkeyColumns
