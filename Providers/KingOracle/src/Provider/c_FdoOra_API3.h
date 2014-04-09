/*
* Copyright (C) 2008  SL-King d.o.o
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

#ifndef _c_FdoOra_API3_h
#define _c_FdoOra_API3_h

#include "KingOracle/FdoKgOraOverrides.h"
#include "c_KgOraSridDesc.h"
#include "c_KgOraSpatialContext.h"
#include "c_KgOraSchemaDesc.h"

class c_MapOraNameToFdoClass;

class FDOKGORA_API c_FdoOra_API3
{
public:
  c_FdoOra_API3(void);
public:
  ~c_FdoOra_API3(void);
  
  //static c_KgOraSchemaDesc* c_FdoOra_API3::DescribeSchema(c_Oci_Connection* OciConn,const char* OraSchema,const char* KingFdoViews);
  
  static c_KgOraSchemaDesc* DescribeSchema(c_Oci_Connection* OciConn,const wchar_t* ConnectionOraSchema,const wchar_t* UseOraSchema,const wchar_t* KingFdoViews,const wchar_t* SdeSchema);

  static bool OraTypeToFdoDataType(const char* OraType,int Precision,int Scale,int Length,FdoDataType & FdoType);
  
  static bool OraTypeToFdoDataType(ub2 OCiDataType,int Precision,int Scale,int Length,FdoDataType & FdoType);
  static bool OraTypeToFdoDataType(const wchar_t* OraType,int Precision,int Scale,int Length,FdoDataType & FdoType);
  static bool FdoPropertyToOraDataType(FdoPropertyDefinition* Property,FdoStringP& OraType);
  //static bool FdoDataTypeToOraDataType(FdoDataType DataTape,oracle::occi::Type& OraType);
  
  static bool SetOracleStatementData(c_Oci_Statement* Statement,int SqlParamNum,FdoDataValue* DataValue);
  static bool SetOracleStatementData(c_Oci_Statement* Statement,const wchar_t*SqlParamName,FdoDataValue* DataValue);

protected:
/*
  static void c_FdoOra_API3::DescribeSchemaSQL(oracle::occi::Connection * OraConn,oracle::occi::Statement* OraStm
                                              ,FdoClassCollection* FdoClasses,FdoKgOraClassCollection* PhysClasses,c_KgOraSpatialContextCollection* SC_Collection,long& AliasNum,bool IsOracle9=false);
*/                                              
  static bool DescribeTableProperties(c_Oci_Connection * OciConn,const wchar_t*Schema,const wchar_t*TableName,FdoPropertyDefinitionCollection* PropCollection,c_KgOraSpatialContextCollection* SC_Collection);
  static bool DescribeTableProperties(c_Oci_Connection * OciConn,const wchar_t*SqlColumns,bool BindOwner,const wchar_t* OraOwner ,c_MapOraNameToFdoClass& MapOraToClass);
  static bool DescribeTablePrimaryKey(c_Oci_Connection * OciConn,const wchar_t*SqlPkey,bool BindOwner,const wchar_t* OraOwner ,c_MapOraNameToFdoClass& MapOraToClass);
  //static void DescribeSchemaSQL(c_Oci_Connection * OciConn,FdoClassCollection* FdoClasses,FdoKgOraClassCollection* PhysClasses ,c_KgOraSpatialContextCollection* SC_Collection,long& AliasNum,bool IsOracle9 );
  static void DescribeSchemaSQL(c_Oci_Connection * OciConn,const wchar_t* SqlStringTables, const wchar_t* SqlStringColumns,const wchar_t* SqlStringPkey
                ,bool BindOwner,const wchar_t* ConnectionOraSchema,const wchar_t* Owner,FdoClassCollection* FdoClasses,FdoKgOraClassCollection* PhysClasses ,c_KgOraSpatialContextCollection* SC_Collection,long& AliasNum,bool IsOracle9 );
  static void DescribeSchemaSDE(c_Oci_Connection * OciConn,const wchar_t* Owner ,FdoClassCollection* FdoClasses,FdoKgOraClassCollection* PhysClasses ,c_KgOraSpatialContextCollection* SC_Collection,long& AliasNum );
  static bool CheckIfVersionedTableName(c_Oci_Connection * OciConn,const std::wstring& Owner,const std::wstring& ora_tablename,std::wstring& orig_tablename);
  static c_KgOraSpatialContext* CreateSpatialContext(c_Oci_Connection * OciConn,const wchar_t* Owner,const wchar_t* Table,const wchar_t* GeometryColumn
                                                    ,c_KgOraSpatialContextCollection* SC_Collection,bool& IsZ,bool& IsM);
  
};

#endif
