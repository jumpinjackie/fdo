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
#include <Sm/Ph/SADReader.h>
#include <Sm/Ph/SADWriter.h>
#include <Sm/Ph/Rd/QueryReader.h>

FdoSmPhSADReader::FdoSmPhSADReader(
    FdoStringP elementType, 
	FdoSmPhMgrP mgr,
	FdoStringP schemaName,
	FdoStringP className,
	FdoStringP propertyName
) : 
    FdoSmPhReader( MakeReader(elementType, mgr, schemaName, className, propertyName) )
{
}

FdoSmPhSADReader::~FdoSmPhSADReader(void)
{
}

bool FdoSmPhSADReader::ReadNext()
{
    return FdoSmPhReader::ReadNext();
}

FdoStringP FdoSmPhSADReader::GetOwnerName()
{
	return(GetString(L"",L"ownername"));
}

FdoStringP FdoSmPhSADReader::GetElementName()
{
	return(GetString(L"",L"elementname"));
}

FdoStringP FdoSmPhSADReader::GetElementType()
{
	return(GetString(L"",L"elementtype"));
}

FdoStringP FdoSmPhSADReader::GetName()
{
	return(GetString(L"",L"name"));
}

FdoStringP FdoSmPhSADReader::GetValue()
{
	return(GetString(L"",L"value"));
}

FdoSmPhReaderP FdoSmPhSADReader::MakeReader( 
    FdoStringP elementType, 
	FdoSmPhMgrP mgr,
	FdoStringP schemaName,
	FdoStringP className,
	FdoStringP propertyName
)
{
    FdoSmPhReaderP pSubReader;

    // Create the row collection describing class attributes.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    FdoSmPhRowP sadRow = FdoSmPhSADWriter::MakeRow(mgr);
    rows->Add( sadRow );

    if ( sadRow->GetDbObject()->GetExists() ) {
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


        // F_SAD exists, read from MetaSchema
        pSubReader = mgr->CreateQueryReader( rows, where ).p->SmartCast<FdoSmPhReader>();
    }
    else {
        // F_SAD does not exist, nothing to read.
        pSubReader = new FdoSmPhReader( mgr, rows);
    }

    return pSubReader;
}
