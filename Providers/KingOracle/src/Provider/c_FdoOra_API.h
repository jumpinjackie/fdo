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

#ifndef _c_FdoOra_API_h
#define _c_FdoOra_API_h

class c_FdoOra_API
{
public:
  c_FdoOra_API(void);
public:
  ~c_FdoOra_API(void);
  
  static c_KgOraSchemaDesc* c_FdoOra_API::DescribeSchema(c_KgOraConnection* KgOraConn,const char* OraSchema);
  
  static bool c_FdoOra_API::OraTypeToFdoDataType(const char* OraType,int Scale,int Length,FdoDataType & FdoType);
  static bool OraTypeToFdoDataType(oracle::occi::Type OraType,int Scale,int Length,FdoDataType & FdoType);
  
  static bool FdoPropertyToOraDataType(FdoPropertyDefinition* Property,FdoStringP& OraType);
  static bool FdoDataTypeToOraDataType(FdoDataType DataTape,oracle::occi::Type& OraType);
  
  static bool SetOracleStatementData(oracle::occi::Statement* Statement,int SqlParamNum,FdoDataValue* DataValue);
  
protected:
  static bool DescribeTableProperties(oracle::occi::Connection * OcciConnection,const char*Schema,const char*TableName,FdoPropertyDefinitionCollection* PropCollection);
};

#endif
