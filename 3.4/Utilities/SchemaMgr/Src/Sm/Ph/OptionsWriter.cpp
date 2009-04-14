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
#include <Sm/Ph/OptionsWriter.h>
#include <Sm/Ph/Rd/QueryReader.h>


FdoSmPhOptionsWriter::FdoSmPhOptionsWriter(FdoSmPhMgrP mgr, FdoStringP owner) : 
	FdoSmPhWriter( MakeWriter(mgr, owner) )
{
}

FdoSmPhOptionsWriter::~FdoSmPhOptionsWriter(void)
{
}

FdoStringP FdoSmPhOptionsWriter::GetName()
{
	return(GetString(L"", L"name"));
}

FdoStringP FdoSmPhOptionsWriter::GetValue()
{
	return(GetString(L"", L"value"));
}

void FdoSmPhOptionsWriter::SetName( FdoStringP sValue )
{
	SetString(L"",  L"name", sValue );
}

void FdoSmPhOptionsWriter::SetValue( FdoStringP sValue )
{
	SetString(L"",  L"value", sValue );
}

FdoSmPhRowP FdoSmPhOptionsWriter::MakeRow( FdoSmPhMgrP mgr, FdoStringP owner )
{
    FdoStringP optDefTable = mgr->GetDcDbObjectName(L"f_options");

    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"f_options", mgr->FindDbObject(optDefTable, owner) );

    // Each field adds itself to the row
    FdoSmPhFieldP field = new FdoSmPhField( row, L"name" );
    field = new FdoSmPhField( row, L"value");
    return( row );
}

FdoSmPhWriterP FdoSmPhOptionsWriter::MakeWriter( FdoSmPhMgrP mgr, FdoStringP owner )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr, owner) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}
