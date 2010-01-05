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

#ifndef _c_FdoOra_API2_h
#define _c_FdoOra_API2_h

#include "KingOracle/FdoKgOraOverrides.h"
#include "c_KgOraSridDesc.h"
#include "c_KgOraSpatialContext.h"
#include "c_KgOraSchemaDesc.h"

class FDOKGORA_API c_FdoSde_API
{
public:
  c_FdoSde_API(void);
public:
  ~c_FdoSde_API(void);
  
  //static c_KgOraSchemaDesc* c_FdoSde_API::DescribeSchema(c_Oci_Connection* OciConn,const char* OraSchema,const char* KingFdoViews);
  
 // static FdoByte* SdeGeom2FdoGeom(const c_KgOraSridDesc& SridDesc,int GeometryType,int NumOfPts,unsigned int SdeGeomLength,char* SdeGeom);

};

#endif
