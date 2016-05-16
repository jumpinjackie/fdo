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
#include "IndexPropertyWriter.h"
#include "PhysicalSchema.h"

FdoRdbmsPhIndexPropertyWriter::FdoRdbmsPhIndexPropertyWriter( FdoRdbmsPhysicalSchema* physicalSchema) : 
    FdoRdbmsPhCommandWriter(
        GetFromInto(physicalSchema),
        physicalSchema
    )
{
}

FdoRdbmsPhIndexPropertyWriter::~FdoRdbmsPhIndexPropertyWriter(void)
{
}

FdoStringP FdoRdbmsPhIndexPropertyWriter::GetIndexName()
{
	return(GetString(L"indexname"));
}

FdoStringP FdoRdbmsPhIndexPropertyWriter::GetAttributeName()
{
	return(GetString(L"attributename"));
}

int FdoRdbmsPhIndexPropertyWriter::GetPosition()
{
	return((int) GetInteger(L"position"));
}

void FdoRdbmsPhIndexPropertyWriter::SetIndexName( FdoStringP sValue )
{
	SetString(L"indexname", sValue.Upper());
}

void FdoRdbmsPhIndexPropertyWriter::SetAttributeName( FdoStringP sValue )
{
	SetString(L"attributename", sValue);
}

void FdoRdbmsPhIndexPropertyWriter::SetPosition( int iValue )
{
	SetInteger(L"position", iValue);
}

void FdoRdbmsPhIndexPropertyWriter::Add()
{
    FdoRdbmsPhCommandWriter::Add();
}

void FdoRdbmsPhIndexPropertyWriter::Modify( FdoStringP sIndexName, int position )
{
    FdoRdbmsPhCommandWriter::Modify(
        FdoStringP::Format( 
            L"where indexname = %ls and position = %d",
			(FdoString*) FdoRdbmsPhysicalSchema::FormatSQLStringVal( sIndexName ),
			position
		)
	);
}

void FdoRdbmsPhIndexPropertyWriter::Delete( FdoStringP sIndexName, int position )
{
    FdoRdbmsPhCommandWriter::Delete(
        FdoStringP::Format( 
            L"where indexname = %ls and position = %d",
			(FdoString*) FdoRdbmsPhysicalSchema::FormatSQLStringVal( sIndexName ),
			position
		)
	);
}

void FdoRdbmsPhIndexPropertyWriter::Delete( FdoStringP sIndexName )
{
    FdoRdbmsPhCommandWriter::Delete(
        FdoStringP::Format( 
            L"where indexname = %ls",
			(FdoString*) FdoRdbmsPhysicalSchema::FormatSQLStringVal( sIndexName )
		)
	);
}

FdoRdbmsPhFromInto* FdoRdbmsPhIndexPropertyWriter::GetFromInto( FdoRdbmsPhysicalSchema* physicalSchema )
{
    FdoRdbmsPhFromInto* pFrom = new FdoRdbmsPhFromInto( physicalSchema, L"F_INDEXATTRIBUTE", NULL  );
    FdoRdbmsPhFieldCollection* pFields = pFrom->GetFields();

    FdoRdbmsPhField* pField = new FdoRdbmsPhField( physicalSchema, pFrom, L"indexname",L"", NULL );
    pFields->Add( pField );
    pField->Release();

    pField = new FdoRdbmsPhField( physicalSchema, pFrom, L"attributename", L"", NULL );
    pFields->Add( pField );
    pField->Release();

    pField = new FdoRdbmsPhField( physicalSchema, pFrom, L"position", L"0", NULL );
    pFields->Add( pField );
    pField->Release();

    return( pFrom );
}


