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
#include <Sm/Ph/SOReader.h>
#include <Sm/Ph/SOWriter.h>
#include <Sm/Ph/Rd/QueryReader.h>

FdoSmPhSOReader::FdoSmPhSOReader(
    FdoStringP elementType, 
	FdoSmPhOwnerP owner,
	FdoStringP schemaName,
	FdoStringP className,
	FdoStringP propertyName
) : 
    FdoSmPhReader( MakeReader(elementType, owner, schemaName, className, propertyName) )
{
}

FdoSmPhSOReader::~FdoSmPhSOReader(void)
{
}

bool FdoSmPhSOReader::ReadNext()
{
    return FdoSmPhReader::ReadNext();
}

FdoStringP FdoSmPhSOReader::GetOwnerName()
{
	return(GetString(L"",L"ownername"));
}

FdoStringP FdoSmPhSOReader::GetElementName()
{
	return(GetString(L"",L"elementname"));
}

FdoStringP FdoSmPhSOReader::GetElementType()
{
	return(GetString(L"",L"elementtype"));
}

FdoStringP FdoSmPhSOReader::GetName()
{
	return(GetString(L"",L"name"));
}

FdoStringP FdoSmPhSOReader::GetValue()
{
	return(GetString(L"",L"value"));
}

FdoSmPhReaderP FdoSmPhSOReader::MakeReader( 
    FdoStringP elementType, 
	FdoSmPhOwnerP owner,
	FdoStringP schemaName,
	FdoStringP className,
	FdoStringP propertyName
)
{
    FdoSmPhMgrP mgr = owner->GetManager();
    FdoSmPhReaderP pSubReader;

    // Create the row collection describing class attributes.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    FdoSmPhRowP soRow = FdoSmPhSOWriter::MakeRow(owner);
    rows->Add( soRow );

    if ( FdoSmPhDbObjectP(soRow->GetDbObject())->GetExists() ) {
        FdoStringP where;
	    FdoStringP ownerName;
	    FdoStringP ownerCmp(L"=");
	    FdoStringP elementName;
	    FdoStringP elementCmp(L"=");
	    FdoStringP kwd(L" where ");

	    // Figure out the ownername and elementname for the rows to retrieve, 
	    // based on the element type

	    if ( wcscmp( elementType, FdoSmPhMgr::SchemaType ) == 0 ) {
		    // Schema elements have no owner so just default it to the schema name.
		    ownerName = schemaName;
		    elementName = schemaName;
	    }

	    if ( wcscmp( elementType, FdoSmPhMgr::ClassType ) == 0 ) {
		    ownerName = schemaName;
		    elementName = className;
	    }

	    if ( wcscmp( elementType, FdoSmPhMgr::PropertyType ) == 0 ) {
		    // property element owner is <schemaName>:<className>

		    if ( schemaName.GetLength() > 0 ) {
			    ownerName = schemaName + L":";
		    }
		    else {
			    // When schemaName not specified, wildcard it and use "like" operator in
			    // the select. This selects properties for all schemas
			    ownerName = L"%:";
			    ownerCmp = L"like";
		    }

		    if ( className.GetLength() > 0 ) {
			    ownerName += className;
		    }
		    else {
			    // When className not specified, wildcard it and use "like" operator in
			    // the select. This selects all properties or all properties in a given
			    // schema.
			    ownerName += L"%";
			    ownerCmp = L"like";
		    }

		    elementName = propertyName;
		    elementCmp = L"=";
	    }

	    // Build the where clause from the specified columns

	    if ( elementType.GetLength() > 0 ) {
		    where += kwd + FdoStringP::Format( L"elementtype = %ls", (FdoString*) mgr->FormatSQLVal(elementType,FdoSmPhColType_String) );
		    kwd = L" and ";
	    }

	    if ( ownerName.GetLength() > 0 ) {
		    FdoStringP fmtOwner = mgr->FormatSQLVal(ownerName,FdoSmPhColType_String);
		    where += kwd + FdoStringP::Format( L"ownername %ls %ls", (FdoString*) ownerCmp, (FdoString*) fmtOwner );
		    kwd = L" and ";
	    }

	    if ( elementName.GetLength() > 0 ) {
		    where += kwd + FdoStringP::Format( L"elementname %ls %ls", (FdoString*) elementCmp, (FdoString*) mgr->FormatSQLVal(elementName,FdoSmPhColType_String) );
		    kwd = L" and ";
	    }

	    // Ordering is important. This particular ordering is expected by callers.

	    where += L" order by ownername, elementname, elementtype, name ";


        // F_SCHEMAOPTIONS exists, read from MetaSchema
        pSubReader = mgr->CreateQueryReader( rows, where ).p->SmartCast<FdoSmPhReader>();
    }
    else {
        // F_SCHEMAOPTIONS does not exist, nothing to read.
        pSubReader = new FdoSmPhReader( mgr, rows);
    }

    return pSubReader;
}


FdoSmPhElementSOReader::FdoSmPhElementSOReader(FdoStringP groupName, FdoSmPhSOReaderP pSOReader) :
	FdoSmPhGroupReader( groupName, pSOReader.p->SmartCast<FdoSmPhReader>()),
	mpSOReader( pSOReader ),
	mbBeforeFirstReadNext(true)
{
}

FdoSmPhElementSOReader::~FdoSmPhElementSOReader()
{
	;
}

bool FdoSmPhElementSOReader::ReadNext()
{
	// We always act as if there is exactly one row of data for this schema, regardless
	// of how many rows actually exist for this schema in F_SCHEMAOPTIONS
	if (mbBeforeFirstReadNext)
	{
		// load all schema options for this schema:
		FdoStringP optionName;
		FdoStringP optionValue;
		mOptionsMap.clear();
		while (FdoSmPhGroupReader::ReadNext())
		{
			optionName = mpSOReader->GetName();
			optionValue = mpSOReader->GetValue();
			mOptionsMap.insert(SchemaOptionsMap::value_type(optionName, optionValue));
		}

		mbBeforeFirstReadNext = false;
		return true;
	}
	else
		return false;
}

FdoStringP FdoSmPhElementSOReader::GetOption(FdoString* optionName)
{
	SchemaOptionsMapIter iter = mOptionsMap.find(optionName);
	if (iter != mOptionsMap.end())
		return iter->second;
	else
		return L"";
}
