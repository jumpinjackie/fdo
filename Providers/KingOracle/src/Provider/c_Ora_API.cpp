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
#include "c_Ora_API.h"

c_Ora_API::c_Ora_API(void)
{
}

c_Ora_API::~c_Ora_API(void)
{
}


char* c_Ora_API::SdoGeomToString(SDO_GEOMETRY *SdoGeom)
{
  
  long ordnum = (long)SdoGeom->getSdo_ordinates().size();
  long einfonum = (long)SdoGeom->getSdo_elem_info().size();
  long buffsize = ordnum * 24 + einfonum * 6  + 256;
  
  char* Buff = new char[buffsize];
  
  
  
  char bufnum[128];
  
  strcpy(Buff,"MDSYS.SDO_GEOMETRY(");
  
  if( SdoGeom->getSdo_gtype().isNull() )
  {
    strcat(Buff,"NULL");
  }
  else
  {
    sprintf(bufnum,"%ld",(long)SdoGeom->getSdo_gtype());
    strcat(Buff,bufnum);
  }
  
  strcat(Buff,",");
  
  if( SdoGeom->getSdo_srid().isNull() )
  {
    strcat(Buff,"NULL");
  }
  else
  {
    sprintf(bufnum,"%ld",(long)SdoGeom->getSdo_srid());
    strcat(Buff,bufnum);
  }
  
  strcat(Buff,",");
  
  if( !SdoGeom->getSdo_point() )
  {
    strcat(Buff,"NULL");
  }
  else
  {
    
    sprintf(bufnum,"MDSYS.SDO_POINT_TYPE(%.8lf,%.8lf,%.8lf)",(double)SdoGeom->getSdo_point()->getX(),(double)SdoGeom->getSdo_point()->getY(),(double)SdoGeom->getSdo_point()->getZ());
    strcat(Buff,bufnum);
  }
  
  strcat(Buff,",");

  // elem info
  strcat(Buff,"MDSYS.SDO_ELEM_INFO_ARRAY(");
  long size = (long)SdoGeom->getSdo_elem_info().size();
  
  if( size > 0 )
  {
    sprintf(bufnum,"%ld",(long)SdoGeom->getSdo_elem_info()[0]);    
    strcat(Buff,bufnum);    
    for(long ind=1;ind<size;ind++)
    {
      strcat(Buff,",");
      sprintf(bufnum,"%ld",(long)SdoGeom->getSdo_elem_info()[ind]);    
      strcat(Buff,bufnum);    
    }
  }
  strcat(Buff,")");
  
  strcat(Buff,",");
  
  // ordinates
  strcat(Buff,"MDSYS.SDO_ORDINATE_ARRAY(");
  long sizeord = (long)SdoGeom->getSdo_ordinates().size();
  if( sizeord > 0 )
  {
    sprintf(bufnum,"%.8lf",(double)SdoGeom->getSdo_ordinates()[0]);    
    strcat(Buff,bufnum);    
      
    for(long ind=1;ind<sizeord;ind++)
    {
      strcat(Buff,",");
      sprintf(bufnum,"%.8lf",(double)SdoGeom->getSdo_ordinates()[ind]);    
      strcat(Buff,bufnum);    
    }
  }
  strcat(Buff,")");
  
  // end of SDO_GEOMETRY(
  strcat(Buff,")");
  
  return Buff;
}//end of c_Ora_API::SdoGeomToString


SDO_GEOMETRY* c_Ora_API::CreateOptimizedRect(long OraSrid,double MinX,double MinY,double MaxX,double MaxY)
{
  SDO_GEOMETRY * sdogeom = new SDO_GEOMETRY;
  
  int ora_gtype = 2003;
  
  sdogeom->setSdo_gtype(ora_gtype);
  
  oracle::occi::Number orasr;
  if( OraSrid > 0 )
  {
    orasr = OraSrid;
  }
  else
  {
    orasr.setNull();
  }
  sdogeom->setSdo_srid(orasr);
  
  // elem info
  sdogeom->getSdo_elem_info().push_back( 1 );
  sdogeom->getSdo_elem_info().push_back(1003 );
  sdogeom->getSdo_elem_info().push_back(3 );
  
  //o ordinates
  sdogeom->getSdo_ordinates().push_back( MinX );
  sdogeom->getSdo_ordinates().push_back( MinY );
  sdogeom->getSdo_ordinates().push_back( MaxX );
  sdogeom->getSdo_ordinates().push_back( MaxY );
  
  return sdogeom;
  
}//end of c_Ora_API::CreateOptimizedRect

long c_Ora_API::GetSrid(oracle::occi::Connection*Conn,const char* CoordSysName)
{
  oracle::occi::Statement* occi_stm=NULL;
  oracle::occi::ResultSet* occi_rset=NULL;
  long srid = 0;

  occi_stm = Conn->createStatement();  
  
  string sqlstr;
  sqlstr = " select SRID,CS_NAME,WKTEXT from MDSYS.cs_srs where cs_name = '";
  sqlstr = sqlstr + CoordSysName;
  sqlstr = sqlstr + "'";
  
  occi_stm->setSQL(sqlstr);
  
  occi_rset = occi_stm->executeQuery();
  
  if( occi_rset->next() != oracle::occi::ResultSet::END_OF_FETCH )
  {
    if( !occi_rset->isNull(1) )
    {
      srid = occi_rset->getInt(1);
    }      
  }
  
  occi_stm->closeResultSet(occi_rset);
  Conn->terminateStatement (occi_stm);
  
  return srid;
  
}//end of c_Ora_API::GetSrid


bool c_Ora_API::GetOracleVersion(oracle::occi::Connection*Conn,int& MainVersion,int& SubVersion)
{

  oracle::occi::Statement* occi_stm=NULL;
  oracle::occi::ResultSet* occi_rset=NULL;
  long srid = 0;

try
{
  occi_stm = Conn->createStatement();  
  
  string sqlstr;
  sqlstr = " SELECT * FROM V$VERSION  ";
  
  occi_stm->setSQL(sqlstr);
  
  occi_rset = occi_stm->executeQuery();
  
  if( occi_rset && occi_rset->next() != oracle::occi::ResultSet::END_OF_FETCH )
  {
    if( !occi_rset->isNull(1) )
    {
      string banner;
      banner = occi_rset->getString(1);
      
      // find
      size_t pos = banner.find("Oracle");
      if( pos != string::npos )
      {
      // find numbers in it
        if( banner.find("10.2") != string::npos )
        {
          MainVersion = 10;
          SubVersion = 1;
          return true;
        }
        if( banner.find("10.1") != string::npos )
        {
          MainVersion = 10;
          SubVersion = 1;
          return true;
        }
        if( banner.find("9.2") != string::npos )
        {
          MainVersion = 9;
          SubVersion = 2;
          return true;
        }
        if( banner.find("9.1") != string::npos )
        {
          MainVersion = 9;
          SubVersion = 1;
          return true;
        }
      }
    }      
  }
  
  occi_stm->closeResultSet(occi_rset);
  Conn->terminateStatement (occi_stm);
  
  return false;
}
catch(oracle::occi::SQLException& )
{
  if( occi_stm )
  {    
    if( occi_rset )
    {
      occi_stm->closeResultSet(occi_rset);
    }
    Conn->terminateStatement(occi_stm);  
  }
  return false;
}  

return false;
  
}//end of c_Ora_API::GetSrid

bool c_Ora_API::IsGeodeticCoordSystem(const char* CoordSysWkt)
{
  string cswkt;
  
  cswkt = CoordSysWkt;
  
  
  if( cswkt.length() > 6 )
  {
    string substr = cswkt.substr(0,6);
    if( substr.compare("GEOGCS")==0 )
    {
      return true;
    }
  }
  
  return false;
  
}//