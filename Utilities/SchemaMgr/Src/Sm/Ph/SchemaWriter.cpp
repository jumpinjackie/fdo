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
#include <Sm/Ph/SchemaWriter.h>
#include <Sm/Ph/SOWriter.h>
#include <Sm/Error.h>


FdoSmPhSchemaWriter::FdoSmPhSchemaWriter(FdoSmPhMgrP mgr) : 
	FdoSmPhWriter( MakeWriter( mgr ) )
{
    FdoStringP optionsTable = mgr->GetDcDbObjectName(L"f_schemaoptions");

    // Determine which table/field names to use, depending on if F_SCHEMAOPTIONS exists:
	mbSchemaOptionsTableDefined = (mgr->FindDbObject(optionsTable) != NULL);
	if (mbSchemaOptionsTableDefined)
		mpSchemaSOWriter = new FdoSmPhSchemaSOWriter(mgr);
}

FdoSmPhSchemaWriter::~FdoSmPhSchemaWriter(void)
{
}

FdoStringP FdoSmPhSchemaWriter::GetName()
{
	return(GetString(L"", L"schemaname"));
}

FdoStringP FdoSmPhSchemaWriter::GetDescription()
{
	return(GetString(L"", L"description"));
}

FdoStringP FdoSmPhSchemaWriter::GetDatabase()
{
	return(GetString(L"", L"tablelinkname"));
}

FdoStringP FdoSmPhSchemaWriter::GetOwner()
{
    // Owner column already used for specifying owning user so needed a different
    // name for owning Oracle schema
	return(GetString(L"", L"tableowner"));
}

FdoStringP FdoSmPhSchemaWriter::GetTableStorage()
{
	// Corresponds to tablespace in Oracle, filegroup in SQL Server, etc.
	if (mbSchemaOptionsTableDefined)
		return mpSchemaSOWriter->GetOption(L"tablestorage");
	else
		return(GetString(L"", L"tablespacename"));
}

FdoStringP FdoSmPhSchemaWriter::GetTableMapping()
{
	return(GetString(L"", L"tablemapping"));
}

FdoStringP FdoSmPhSchemaWriter::GetIndexStorage()
{
	if (mbSchemaOptionsTableDefined)
		return mpSchemaSOWriter->GetOption(L"indexstorage");
	else
		return L"";
}

FdoStringP FdoSmPhSchemaWriter::GetTextStorage()
{
	if (mbSchemaOptionsTableDefined)
		return mpSchemaSOWriter->GetOption(L"textstorage");
	else
		return L"";
}

FdoStringP FdoSmPhSchemaWriter::GetTextInRow()
{
	if (mbSchemaOptionsTableDefined)
		return mpSchemaSOWriter->GetOption(L"textinrow");
	else
		return L"";
}

FdoStringP FdoSmPhSchemaWriter::GetTableStorageEngine()
{
	if (mbSchemaOptionsTableDefined)
		return mpSchemaSOWriter->GetOption(L"tablestorageengine");
	else
		return L"";
}


FdoStringP FdoSmPhSchemaWriter::GetUser()
{
	return(GetString(L"", L"owner"));
}

void FdoSmPhSchemaWriter::SetName(FdoStringP sValue)
{
	SetString(L"", L"schemaname", sValue);
}

void FdoSmPhSchemaWriter::SetDescription(FdoStringP sValue)
{
	SetString(L"", L"description", sValue);
}

void FdoSmPhSchemaWriter::SetDatabase(FdoStringP sValue)
{
	SetString(L"", L"tablelinkname", sValue.Upper());
}

void FdoSmPhSchemaWriter::SetOwner(FdoStringP sValue)
{
	SetString(L"", L"tableowner", sValue.Upper());
}

void FdoSmPhSchemaWriter::SetTableStorage(FdoStringP sValue)
{
	if (mbSchemaOptionsTableDefined)
		mpSchemaSOWriter->SetOption(L"tablestorage", sValue.Upper());
	else
		SetString(L"", L"tablespacename", sValue.Upper());
}

void FdoSmPhSchemaWriter::SetTableMapping(FdoStringP sValue)
{
	SetString(L"", L"tablemapping", sValue);
}

void FdoSmPhSchemaWriter::SetIndexStorage(FdoStringP sValue)
{
	if (mbSchemaOptionsTableDefined)
		mpSchemaSOWriter->SetOption(L"indexstorage", sValue);
	else
        //Table %1$ls is not in the current datastore; datastore schema needs to be upgraded
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_364),
			    L"F_SCHEMAOPTIONS"
            )
		);
}

void FdoSmPhSchemaWriter::SetTextStorage(FdoStringP sValue)
{
	if (mbSchemaOptionsTableDefined)
		mpSchemaSOWriter->SetOption(L"textstorage", sValue);
	else
        //Table %1$ls is not in the current datastore; datastore schema needs to be upgraded
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_364),
			    L"F_SCHEMAOPTIONS"
            )
		);
}

void FdoSmPhSchemaWriter::SetTextInRow(FdoStringP sValue)
{
	if (mbSchemaOptionsTableDefined)
		mpSchemaSOWriter->SetOption(L"textinrow", sValue);
	else
		//Table %1$ls is not in the current datastore; datastore schema needs to be upgraded
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_364),
			    L"F_SCHEMAOPTIONS"
            )
		);
}

void FdoSmPhSchemaWriter::SetTableStorageEngine(FdoStringP sValue)
{
	if (mbSchemaOptionsTableDefined)
		mpSchemaSOWriter->SetOption(L"tablestorageengine", sValue);
	else
		//Table %1$ls is not in the current datastore; datastore schema needs to be upgraded
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_364),
			    L"F_SCHEMAOPTIONS"
            )
		);
}

void FdoSmPhSchemaWriter::SetUser(FdoStringP sValue)
{
	SetString(L"", L"owner", sValue);
}

void FdoSmPhSchemaWriter::Add()
{
    // creation date is always current date.
    SetString( L"", L"creationdate", GetManager()->FormatCurrentDateField() );
    FdoSmPhWriter::Add();

	if (mbSchemaOptionsTableDefined)
		mpSchemaSOWriter->Add(GetName());
}

void FdoSmPhSchemaWriter::Modify( FdoStringP sName )
{
    FdoSmPhWriter::Modify( 
        FdoStringP::Format(
               L"where schemaname = %ls", 
               (FdoString*) GetManager()->FormatSQLVal( sName, FdoSmPhColType_String )
        ) 
       );

	if (mbSchemaOptionsTableDefined)
		mpSchemaSOWriter->Modify(sName);
}

void FdoSmPhSchemaWriter::Delete( FdoStringP sName )
{
    FdoSmPhWriter::Delete( 
        FdoStringP::Format(
               L"where schemaname = %ls", 
               (FdoString*) GetManager()->FormatSQLVal( sName, FdoSmPhColType_String )
        ) 
       );

	if (mbSchemaOptionsTableDefined)
		mpSchemaSOWriter->Delete(sName);
}

FdoSmPhRowP FdoSmPhSchemaWriter::MakeRow( FdoSmPhOwnerP owner )
{
    bool hasMs = owner->GetHasMetaSchema();
    FdoStringP infoTable = owner->GetManager()->GetDcDbObjectName(L"f_schemainfo");

    FdoSmPhRowP row = new FdoSmPhRow( 
        owner->GetManager(), 
        L"f_schemainfo", 
        hasMs ? owner->FindDbObject(infoTable) : FdoSmPhDbObjectP() 
    );

    // Each field adds itself to the row.
    FdoSmPhFieldP field = new FdoSmPhField( row, L"schemaname" );
    field = new FdoSmPhField( row, L"description" );
    field = new FdoSmPhField( row, L"owner" );
    field = new FdoSmPhField( row, L"creationdate", (FdoSmPhColumn*) NULL, L"", false );
    field = new FdoSmPhField( row, L"schemaversionid", (FdoSmPhColumn*) NULL, L"0.0" );

    field = new FdoSmPhField(row, L"tablelinkname", row->CreateColumnDbObject(L"tablelinkname",true));
    field = new FdoSmPhField(row, L"tableowner", row->CreateColumnDbObject(L"tableowner",true));
    field = new FdoSmPhField(row, L"tablemapping", row->CreateColumnDbObject(L"tablemapping",true));
    field = new FdoSmPhField(row, L"tablespacename", row->CreateColumnDbObject(L"tablespacename",true));

    return( row );
}

FdoSmPhWriterP FdoSmPhSchemaWriter::MakeWriter( FdoSmPhMgrP mgr )
{
	FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter(MakeRow(mgr->GetOwner()));

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}
