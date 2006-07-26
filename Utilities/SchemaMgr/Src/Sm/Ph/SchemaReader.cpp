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
#include <Sm/Ph/SchemaReader.h>
#include <Sm/Ph/SchemaWriter.h>
#include <Sm/Ph/Mt/SchemaReader.h>
#include <Sm/Ph/Rd/SchemaReader.h>
#include <Sm/Ph/Cfg/SchemaReader.h>
#include <Sm/Ph/SchemaSOWriter.h>


FdoSmPhSchemaReader::FdoSmPhSchemaReader(FdoSmPhOwnerP owner, bool dsInfo) : 
	FdoSmPhReader( MakeReader(owner, dsInfo) )
{
   	mpSOReader = new FdoSmPhSOReader(FdoSmPhMgr::SchemaType, owner);
}

FdoSmPhSchemaReader::~FdoSmPhSchemaReader(void)
{
}

FdoStringP FdoSmPhSchemaReader::GetName()
{
	return(GetString(L"", L"schemaname"));
}

FdoStringP FdoSmPhSchemaReader::GetDescription()
{
	return(GetString(L"", L"description"));
}

double FdoSmPhSchemaReader::GetVersion()
{
	return(GetDouble(L"", L"schemaversionid"));
}

FdoStringP FdoSmPhSchemaReader::GetDatabase()
{
    return(GetString(L"", L"tablelinkname"));
}

FdoStringP FdoSmPhSchemaReader::GetOwner()
{
    // Owner column already used for specifying owning user so needed a different
    // name for owning Oracle schema
	return(GetString(L"", L"tableowner"));
}

FdoStringP FdoSmPhSchemaReader::GetTableMapping()
{
	return(GetString(L"", L"tablemapping"));
}

FdoStringP FdoSmPhSchemaReader::GetUser()
{
	return(GetString(L"", L"owner"));
}

FdoStringP FdoSmPhSchemaReader::GetTableStorage()
{
	if (mbSchemaOptionsTableDefined && mbReadFromMetadata)
		return mpSchemaSOReader->GetOption(L"tablestorage");  // read from F_SCHEMAOPTIONS instead of F_SCHEMAINFO
	else
		return(GetString(L"", L"tablespacename"));
}

FdoStringP FdoSmPhSchemaReader::GetIndexStorage()
{
	if (mbSchemaOptionsTableDefined && mbReadFromMetadata)
		return mpSchemaSOReader->GetOption(L"indexstorage");  // read from F_SCHEMAOPTIONS instead of F_SCHEMAINFO
	else
		return FdoStringP(); // return empty string; no such option in F_SCHEMAINFO
}

FdoStringP FdoSmPhSchemaReader::GetTextStorage()
{
	if (mbSchemaOptionsTableDefined && mbReadFromMetadata)
		return mpSchemaSOReader->GetOption(L"textstorage");  // read from F_SCHEMAOPTIONS instead of F_SCHEMAINFO
	else
		return FdoStringP(); // return empty string; no such option in F_SCHEMAINFO
}

FdoStringP FdoSmPhSchemaReader::GetTextInRow()
{
	if (mbSchemaOptionsTableDefined && mbReadFromMetadata)
		return mpSchemaSOReader->GetOption(L"textinrow");  // read from F_SCHEMAOPTIONS instead of F_SCHEMAINFO
	else
		return FdoStringP(); // return empty string; no such option in F_SCHEMAINFO
}

FdoStringP FdoSmPhSchemaReader::GetTableStorageEngine()
{
	if (mbSchemaOptionsTableDefined && mbReadFromMetadata)
		return mpSchemaSOReader->GetOption(L"tablestorageengine");  // read from F_SCHEMAOPTIONS instead of F_SCHEMAINFO
	else
		return FdoStringP(); // return empty string; no such option in F_SCHEMAINFO
}


FdoSmPhReaderP FdoSmPhSchemaReader::MakeReader( FdoSmPhOwnerP owner, bool dsInfo )
{
    FdoSmPhMgrP mgr = owner->GetManager();
    FdoSmPhReaderP pSubReader;

	mbSchemaOptionsTableDefined = false;
    mbReadFromMetadata = false;

    // Create the rows describing the fields to read.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();
    FdoSmPhRowP row = FdoSmPhSchemaWriter::MakeRow( owner );
    rows->Add( row );

	// Determine which table/field names to use, depending on if F_SCHEMAOPTIONS exists:
    mbSchemaOptionsTableDefined =
        (owner->GetHasMetaSchema() && owner->FindDbObject(mgr->GetDcDbObjectName(L"f_schemaoptions")) != NULL);

	// Create the appropriate schema reader:
    if ( mgr->GetConfigMappings() && (!dsInfo) ) {
        // A Config document is set, read schemas from it.
        // However, the config doc does not have datastore info so try one of the
        // other readers when dsInfo=true.
        pSubReader = mgr->CreateCfgSchemaReader( rows ).p->SmartCast<FdoSmPhReader>();
    }
    else {

        if ( row->GetDbObject()->GetExists() ) {
            // F_SCHEMAINFO exists, read from MetaSchema
            mbReadFromMetadata = true;
            pSubReader = MakeMtReader( rows, owner, dsInfo );
        }
        else {
            // F_SCHEMAINFO does not exist, read from native physical schema
            pSubReader = owner->GetManager()->CreateRdSchemaReader( rows, owner, dsInfo );
        }
    }

    return(pSubReader);
}

FdoSmPhReaderP FdoSmPhSchemaReader::MakeMtReader( FdoSmPhRowsP rows, FdoSmPhOwnerP owner, bool dsInfo )
{
    return new FdoSmPhMtSchemaReader( rows, owner, dsInfo );
}

bool FdoSmPhSchemaReader::ReadNext()
{
	bool ret = FdoSmPhReader::ReadNext();

	// Create new schema options reader for this schema (wrapped around mpSOReader):
	if (ret)
	{
		mpSchemaSOReader = new FdoSmPhSchemaSOReader(GetName(), mpSOReader);
		mpSchemaSOReader->ReadNext();
	}
	else
		mpSchemaSOReader = NULL;

	return ret;
}
