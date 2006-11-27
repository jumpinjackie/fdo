/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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
 *
 */

#include "stdafx.h"
#include <Sm/Error.h>
#include <Sm/Ph/ClassWriter.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/ClassSOWriter.h>


FdoSmPhClassWriter::FdoSmPhClassWriter(FdoSmPhMgrP mgr) : 
	FdoSmPhWriter( MakeWriter(mgr) )
{
	// Determine which table/field names to use, depending on if F_SCHEMAOPTIONS exists:
	mbSchemaOptionsTableDefined = (mgr->FindDbObject(mgr->GetDcDbObjectName(L"f_schemaoptions")) != NULL);
	if (mbSchemaOptionsTableDefined)
		mpClassSOWriter = new FdoSmPhClassSOWriter(mgr);
}

FdoSmPhClassWriter::~FdoSmPhClassWriter(void)
{
}

FdoStringP FdoSmPhClassWriter::GetName()
{
	return(GetString(L"", L"classname"));
}

FdoInt64 FdoSmPhClassWriter::GetId()
{
	return( GetInt64(L"", L"classid") );
}

FdoStringP FdoSmPhClassWriter::GetSchemaName()
{
	return(GetString(L"", L"schemaname"));
}

FdoStringP FdoSmPhClassWriter::GetBaseName()
{
	return(GetString(L"", L"parentclassname"));
}

FdoStringP FdoSmPhClassWriter::GetDescription()
{
	return(GetString(L"", L"description"));
}

FdoStringP FdoSmPhClassWriter::GetTableName()
{
	return(GetString(L"", L"tablename"));
}

FdoStringP FdoSmPhClassWriter::GetRootTableName()
{
	return(GetString(L"", L"roottablename"));
}

FdoStringP FdoSmPhClassWriter::GetTableMapping()
{
	return(GetString(L"", L"tablemapping"));
}

FdoStringP FdoSmPhClassWriter::GetTableStorage()
{
	if (mbSchemaOptionsTableDefined)
		return mpClassSOWriter->GetOption(L"tablestorage");
	else
		return(GetString(L"", L"tablespacename"));
}

FdoStringP FdoSmPhClassWriter::GetClassType()
{
	return(GetString(L"", L"classtype"));
}

bool FdoSmPhClassWriter::GetIsAbstract()
{
	return(GetBoolean(L"", L"isabstract"));
}

bool FdoSmPhClassWriter::GetIsFixedTable()
{
	return(GetBoolean(L"", L"isfixedtable"));
}

bool FdoSmPhClassWriter::GetIsTableCreator()
{
	return(GetBoolean(L"", L"istablecreator"));
}

FdoStringP FdoSmPhClassWriter::GetDatabase()
{
	return(GetString(L"", L"tablelinkname"));
}

FdoStringP FdoSmPhClassWriter::GetOwner()
{
	return(GetString(L"", L"tableowner"));
}

FdoStringP FdoSmPhClassWriter::GetGeometryProperty()
{
	return(GetString(L"", L"geometryproperty"));
}

FdoStringP FdoSmPhClassWriter::GetOption( FdoStringP optionName )
{
	if (mbSchemaOptionsTableDefined)
		return mpClassSOWriter->GetOption(optionName);
	else
		return L"";
}

void FdoSmPhClassWriter::SetName( FdoStringP sValue )
{
	SetString(L"",  L"classname", sValue );
}

void FdoSmPhClassWriter::SetId( FdoInt64 lValue)
{
	SetInt64(L"", L"classid", lValue);
}

void FdoSmPhClassWriter::SetSchemaName( FdoStringP sValue )
{
	SetString(L"",  L"schemaname", sValue );
}

void FdoSmPhClassWriter::SetBaseName( FdoStringP sValue )
{
	SetString(L"",  L"parentclassname", sValue );
}

void FdoSmPhClassWriter::SetDescription( FdoStringP sValue )
{
	SetString(L"",  L"description", sValue );
}

void FdoSmPhClassWriter::SetTableName( FdoStringP sValue )
{
	SetString(L"",  L"tablename", GetManager()->DbObject2MetaSchemaName(sValue) );
}

void FdoSmPhClassWriter::SetRootTableName( FdoStringP sValue )
{
	SetString(L"",  L"roottablename", GetManager()->DbObject2MetaSchemaName(sValue) );
}

void FdoSmPhClassWriter::SetTableMapping(FdoStringP sValue)
{
	SetString(L"", L"tablemapping", sValue);
}

void FdoSmPhClassWriter::SetTableStorage(FdoStringP sValue)
{
	if (mbSchemaOptionsTableDefined)
		mpClassSOWriter->SetOption(L"tablestorage", sValue);
	else
		SetString(L"", L"tablespacename", sValue);
}

void FdoSmPhClassWriter::SetClassType( FdoStringP sValue )
{
	SetString(L"",  L"classtype", sValue );
}

void FdoSmPhClassWriter::SetIsAbstract( bool bValue )
{
	SetBoolean(L"",  L"isabstract", bValue );
}

void FdoSmPhClassWriter::SetDatabase( FdoStringP sValue )
{
	SetString(L"",  L"tablelinkname", sValue );
}

void FdoSmPhClassWriter::SetOwner( FdoStringP sValue )
{
	SetString(L"",  L"tableowner", sValue );
}

void FdoSmPhClassWriter::SetGeometryProperty( FdoStringP sValue )
{
	SetString(L"",  L"geometryproperty", sValue );
}

void FdoSmPhClassWriter::SetIsFixedTable( bool bValue )
{
	SetBoolean(L"",  L"isfixedtable", bValue );
}

void FdoSmPhClassWriter::SetIsTableCreator( bool bValue )
{
    FdoSmPhFieldP pField = GetField( L"", L"istablecreator" );

    if ( pField && pField->GetColumn() ) 
    	SetBoolean(L"",  L"istablecreator", bValue );
}

void FdoSmPhClassWriter::SetOption( FdoStringP optionName, FdoStringP optionValue )
{
	if (mbSchemaOptionsTableDefined)
		mpClassSOWriter->SetOption(optionName, optionValue);
	else
		throw FdoSchemaException::Create (
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_364), 
                L"F_SCHEMAOPTIONS" 
			)
		);
}

void FdoSmPhClassWriter::Add()
{
    // Classes are handled specially. A dbi function is invoked,
    // unstead of a GQL insert statement, in order to generate classId.

    FdoSmPhMgrP mgr = GetManager();
    FdoStringP classTypeTable = mgr->GetDcDbObjectName(L"f_classType");

    // Translate class type enum into the datastore class type id.

    FdoSmPhRowsP pFroms = new FdoSmPhRowCollection();
    FdoSmPhRowP pFrom = new FdoSmPhRow( mgr, L"f_classType", mgr->FindDbObject(classTypeTable) );
    pFroms->Add(pFrom);
    
    FdoSmPhFieldP pField = new FdoSmPhField( pFrom, L"classtype", (FdoSmPhColumn*) NULL, L"", false );

	FdoSmPhRdQueryReaderP rdr = mgr->CreateQueryReader(
        pFroms,
		FdoStringP::Format( L"where classtypename = '%ls'", 
		    (FdoString*) GetClassType()
		)
	);

	if ( !rdr->ReadNext() )
		throw FdoSchemaException::Create (
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_122), 
                (FdoString*) ( GetSchemaName() + L":" + GetName() ), 
                (FdoString*) GetClassType()
			)
		);

    SetClassType( rdr->GetString(L"", L"classtype") );

    FdoSmPhWriter::Add();

	if (mbSchemaOptionsTableDefined)
		mpClassSOWriter->Add(GetSchemaName(), GetName());
}

void FdoSmPhClassWriter::Modify( FdoString* schemaName, FdoString* className, FdoInt64 classId )
{
    // No DBI function for modifying a class so do a GQL statement.
    FdoSmPhWriter::Modify(
#ifdef _WIN32
   		FdoStringP::Format( L"where classid = %I64d", classId )
#else
   		FdoStringP::Format( L"where classid = %lld", classId )
#endif
	);

	if (mbSchemaOptionsTableDefined)
		mpClassSOWriter->Modify(schemaName, className);
}

void FdoSmPhClassWriter::Delete( FdoString* schemaName, FdoString* className, FdoInt64 classId )
{
    if ( classId > 0 ) {
        FdoSmPhWriter::Delete( 
            FdoStringP::Format(
#ifdef _WIN32
                L"where classid = %I64d",
#else
                L"where classid = %lld",
#endif
                classId
            ) 
        );
	}

	if (mbSchemaOptionsTableDefined)
		mpClassSOWriter->Delete(schemaName, className);
}

FdoSmPhRowP FdoSmPhClassWriter::MakeRow( FdoSmPhMgrP mgr )
{
    bool hasMs = FdoSmPhOwnerP(mgr->GetOwner())->GetHasMetaSchema();
    FdoStringP classDefTable = mgr->GetDcDbObjectName(L"f_classdefinition");

    FdoSmPhRowP row = new FdoSmPhRow( 
        mgr, 
        L"f_classdefinition", 
        hasMs ? mgr->FindDbObject(classDefTable) : FdoSmPhDbObjectP() 
    );

    // Each field adds itself to the row
    FdoSmPhFieldP field = new FdoSmPhField( row, L"classid" );
    field = new FdoSmPhField( row, L"classname");
    field = new FdoSmPhField( row, L"classtype" );
    field = new FdoSmPhField( row, L"parentclassname" );
    field = new FdoSmPhField( row, L"schemaname" );
    field = new FdoSmPhField( row, L"description" );
    field = new FdoSmPhField( row, L"tablename" );
    field = new FdoSmPhField( row, L"isabstract" );
    field = new FdoSmPhField( row, L"isfixedtable", row->CreateColumnBool(L"isfixedtable",true) );
    field = new FdoSmPhField( row, L"istablecreator", row->CreateColumnBool(L"istablecreator",true) );
    field = new FdoSmPhField( row, L"tablelinkname", row->CreateColumnDbObject(L"tablelinkname",true) );
    field = new FdoSmPhField( row, L"tableowner", row->CreateColumnDbObject(L"tableowner",true) );
    field = new FdoSmPhField( row, L"roottablename", row->CreateColumnDbObject(L"roottablename",true) );
    field = new FdoSmPhField( row, L"tablemapping", row->CreateColumnDbObject(L"tablemapping",true) );
    field = new FdoSmPhField( row, L"geometryproperty", row->CreateColumnChar(L"geometryproperty", true, 4000) );
    field = new FdoSmPhField( row, L"tablespacename", row->CreateColumnDbObject(L"tablespacename",true) );

    return( row );
}

FdoSmPhRowP FdoSmPhClassWriter::MakeClassTypeRow( FdoSmPhMgrP mgr )
{
    bool hasMs = FdoSmPhOwnerP(mgr->GetOwner())->GetHasMetaSchema();
    FdoStringP classTypeTable = mgr->GetDcDbObjectName(L"f_classType");

    FdoSmPhRowP row = new FdoSmPhRow( 
        mgr, 
        L"f_classtype", 
        hasMs ? mgr->FindDbObject(classTypeTable) : FdoSmPhDbObjectP() 
    );

    FdoSmPhFieldP field = new FdoSmPhField( row, L"classtypename" );

    return( row );
}

FdoSmPhWriterP FdoSmPhClassWriter::MakeWriter( FdoSmPhMgrP mgr )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}
