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
#include <Sm/Ph/SADWriter.h>

FdoSmPhSADWriter::FdoSmPhSADWriter(
    FdoSmPhMgrP mgr
) : 
    FdoSmPhWriter( MakeWriter(mgr) )
{
}

FdoSmPhSADWriter::~FdoSmPhSADWriter(void)
{
}

FdoStringP FdoSmPhSADWriter::GetOwnerName()
{
	return(GetString(L"",L"ownername"));
}

FdoStringP FdoSmPhSADWriter::GetElementName()
{
	return(GetString(L"",L"elementname"));
}

FdoStringP FdoSmPhSADWriter::GetElementType()
{
	return(GetString(L"",L"elementtype"));
}

FdoStringP FdoSmPhSADWriter::GetName()
{
	return(GetString(L"",L"name"));
}

FdoStringP FdoSmPhSADWriter::GetValue()
{
	return(GetString(L"",L"value"));
}

void FdoSmPhSADWriter::SetOwnerName(FdoStringP sValue)
{
	SetString(L"",L"ownername", sValue);
}

void FdoSmPhSADWriter::SetElementName(FdoStringP sValue)
{
	SetString(L"",L"elementname", sValue);
}

void FdoSmPhSADWriter::SetElementType(FdoStringP sValue)
{
	SetString(L"",L"elementtype", sValue);
}

void FdoSmPhSADWriter::SetName(FdoStringP sValue)
{
	SetString(L"",L"name", sValue);
}

void FdoSmPhSADWriter::SetValue(FdoStringP sValue)
{
	SetString(L"",L"value", sValue);
}

void FdoSmPhSADWriter::Add()
{
    FdoSmPhWriter::Add();
}

void FdoSmPhSADWriter::Delete( FdoStringP sOwner, FdoStringP sName )
{
    FdoSmPhWriter::Delete(
        FdoStringP::Format( 
            L"where ownername=%ls and elementname=%ls",
			(FdoString*) GetManager()->FormatSQLVal( sOwner, FdoSmPhColType_String ),
			(FdoString*) GetManager()->FormatSQLVal( sName, FdoSmPhColType_String )
		)
    );
}

FdoSmPhRowP FdoSmPhSADWriter::MakeRow( FdoSmPhMgrP mgr )
{
    bool hasMs = FdoSmPhOwnerP(mgr->GetOwner())->GetHasMetaSchema();
    FdoStringP sadDefTable = mgr->GetDcDbObjectName(L"f_sad");

    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"f_sad", hasMs ? mgr->FindDbObject(sadDefTable) : FdoSmPhDbObjectP() );

    // Each field adds itself to the row.
    FdoSmPhFieldP field = new FdoSmPhField( row, L"ownername" );
    field = new FdoSmPhField( row, L"elementname" );
    field = new FdoSmPhField( row, L"elementtype" );
    field = new FdoSmPhField( row, L"name" );
    field = new FdoSmPhField( row, L"value" );

    return( row );
}

FdoSmPhWriterP FdoSmPhSADWriter::MakeWriter( FdoSmPhMgrP mgr )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}

