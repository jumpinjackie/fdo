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
#include <Sm/Ph/SOWriter.h>

FdoSmPhSOWriter::FdoSmPhSOWriter(
    FdoSmPhMgrP mgr
) : 
    FdoSmPhWriter( MakeWriter(mgr) )
{
}

FdoSmPhSOWriter::~FdoSmPhSOWriter(void)
{
}

FdoStringP FdoSmPhSOWriter::GetOwnerName()
{
	return(GetString(L"",L"ownername"));
}

FdoStringP FdoSmPhSOWriter::GetElementName()
{
	return(GetString(L"",L"elementname"));
}

FdoStringP FdoSmPhSOWriter::GetElementType()
{
	return(GetString(L"",L"elementtype"));
}

FdoStringP FdoSmPhSOWriter::GetName()
{
	return(GetString(L"",L"name"));
}

FdoStringP FdoSmPhSOWriter::GetValue()
{
	return(GetString(L"",L"value"));
}

void FdoSmPhSOWriter::SetOwnerName(FdoStringP sValue)
{
	SetString(L"",L"ownername", sValue);
}

void FdoSmPhSOWriter::SetElementName(FdoStringP sValue)
{
	SetString(L"",L"elementname", sValue);
}

void FdoSmPhSOWriter::SetElementType(FdoStringP sValue)
{
	SetString(L"",L"elementtype", sValue);
}

void FdoSmPhSOWriter::SetName(FdoStringP sValue)
{
	SetString(L"",L"name", sValue);
}

void FdoSmPhSOWriter::SetValue(FdoStringP sValue)
{
	SetString(L"",L"value", sValue);
}

void FdoSmPhSOWriter::Add()
{
    FdoSmPhWriter::Add();
}

void FdoSmPhSOWriter::Delete( FdoStringP sOwner, FdoStringP sName )
{
    FdoSmPhWriter::Delete(
        FdoStringP::Format( 
            L"where ownername=%ls and elementname=%ls",
			(FdoString*) GetManager()->FormatSQLVal( sOwner, FdoSmPhColType_String ),
			(FdoString*) GetManager()->FormatSQLVal( sName, FdoSmPhColType_String )
		)
    );
}

FdoSmPhRowP FdoSmPhSOWriter::MakeRow( FdoSmPhOwnerP owner )
{
    bool hasMs = owner->GetHasMetaSchema();
    FdoStringP optDefTable = FdoSmPhMgrP(owner->GetManager())->GetDcDbObjectName(L"f_schemaoptions");

    FdoSmPhRowP row = new FdoSmPhRow( 
        owner->GetManager(), 
        L"f_schemaoptions", 
        hasMs ? owner->FindDbObject(optDefTable) : FdoSmPhDbObjectP() 
    );

    // Each field adds itself to the row.
    FdoSmPhFieldP field = new FdoSmPhField( row, L"ownername" );
    field = new FdoSmPhField( row, L"elementname" );
    field = new FdoSmPhField( row, L"elementtype" );
    field = new FdoSmPhField( row, L"name" );
    field = new FdoSmPhField( row, L"value" );

    return( row );
}

FdoSmPhWriterP FdoSmPhSOWriter::MakeWriter( FdoSmPhMgrP mgr )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr->GetOwner()) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}







FdoSmPhElementSOWriter::FdoSmPhElementSOWriter( FdoSmPhMgrP mgr ) :
	FdoSmPhWriter((FdoSmPhWriter*) NULL)
{
	SetSubWriter(MakeWriter(mgr));
}

FdoSmPhElementSOWriter::~FdoSmPhElementSOWriter(void)
{
}


FdoStringP FdoSmPhElementSOWriter::GetOption(FdoString* optionName)
{
	SchemaOptionsMapIter iter = mOptionsMap.find(optionName);
	if (iter != mOptionsMap.end())
		return iter->second;
	else
		return L"";
}


void FdoSmPhElementSOWriter::SetOption(FdoStringP optionName, FdoStringP optionValue)
{
	SchemaOptionsMapIter iter = mOptionsMap.find(optionName);
	if (iter != mOptionsMap.end())
		iter->second = optionValue;  // update existing entry
	else
		mOptionsMap.insert(SchemaOptionsMap::value_type(optionName, optionValue));  // add new entry
}


void FdoSmPhElementSOWriter::Add(FdoStringP ownerName, FdoStringP elementName, FdoStringP elementType)
{
	// Add all SchemaOptions for this schema in one shot:
	for (SchemaOptionsMapIter iter = mOptionsMap.begin(); iter != mOptionsMap.end(); iter++)
	{
		Clear();
		SetString(L"", L"ownername", ownerName);
		SetString(L"", L"elementname", elementName);
		SetString(L"", L"elementtype", elementType);
		SetString(L"", L"name", iter->first);
		SetString(L"", L"value", iter->second);
		FdoSmPhWriter::Add();
	}

	mOptionsMap.clear(); //TODO: do I really need to clear all options now?
}

void FdoSmPhElementSOWriter::Modify(FdoStringP ownerName, FdoStringP elementName, FdoStringP elementType)
{
	// First, delete all old rows for this schema,
    // but don't clear the map until Add is called.
	mpSOWriter->Delete(ownerName, elementName);

	// Second, add all new rows for this schema:
	Add(ownerName, elementName, elementType);
}

void FdoSmPhElementSOWriter::Delete(FdoStringP ownerName, FdoStringP elementName, FdoStringP elementType)
{
	// Delete all rows associated to this schema:
	mpSOWriter->Delete(ownerName, elementName);

	mOptionsMap.clear(); //TODO: do I really need to clear all options now?
}


FdoSmPhWriterP FdoSmPhElementSOWriter::MakeWriter(FdoSmPhMgrP mgr)
{
	mpSOWriter = new FdoSmPhSOWriter(mgr);

	return mpSOWriter->SmartCast<FdoSmPhWriter>();
}
