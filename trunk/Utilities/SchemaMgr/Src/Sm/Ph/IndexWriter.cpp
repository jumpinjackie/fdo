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
#include "IndexWriter.h"
#include "PhysicalSchema.h"

FdoRdbmsPhIndexWriter::FdoRdbmsPhIndexWriter( FdoRdbmsPhysicalSchema* physicalSchema) : 
    FdoRdbmsPhCommandWriter(
        GetFromInto(physicalSchema),
        physicalSchema
    )
{
}

FdoRdbmsPhIndexWriter::~FdoRdbmsPhIndexWriter(void)
{
}

FdoStringP FdoRdbmsPhIndexWriter::GetName()
{
	return(GetString(L"indexname"));
}

dbi_cn_id_t FdoRdbmsPhIndexWriter::GetClassId()
{
	return(GetInteger(L"classid"));
}

bool FdoRdbmsPhIndexWriter::GetIsUnique()
{
	return(GetBoolean(L"isunique"));
}

void FdoRdbmsPhIndexWriter::SetName( FdoStringP sValue )
{
	SetString(L"indexname", sValue.Upper());
}

void FdoRdbmsPhIndexWriter::SetClassId( dbi_cn_id_t lValue )
{
	SetInteger(L"classid", lValue);
}

void FdoRdbmsPhIndexWriter::SetIsUnique( bool bValue )
{
	SetBoolean(L"isunique", bValue);
}

void FdoRdbmsPhIndexWriter::Add()
{
    FdoRdbmsPhCommandWriter::Add();
}

void FdoRdbmsPhIndexWriter::Modify( FdoStringP sName )
{
    FdoRdbmsPhCommandWriter::Modify(
        FdoStringP::Format( 
            L"where indexname = %ls",
			(FdoString*) FdoRdbmsPhysicalSchema::FormatSQLStringVal( sName )
		)
	);
}

void FdoRdbmsPhIndexWriter::Delete( FdoStringP sName )
{
    FdoRdbmsPhIndexPropertyWriter* pPropWriter = GetPhysicalSchema()->GetIndexPropertyWriter();

    pPropWriter->Delete( sName );

    FdoRdbmsPhCommandWriter::Delete(
        FdoStringP::Format( 
            L"where indexname = %ls",
			(FdoString*) FdoRdbmsPhysicalSchema::FormatSQLStringVal( sName )
		)
    );
}

FdoRdbmsPhFromInto* FdoRdbmsPhIndexWriter::GetFromInto( FdoRdbmsPhysicalSchema* physicalSchema )
{
    FdoRdbmsPhFromInto* pFrom = new FdoRdbmsPhFromInto( physicalSchema, L"F_INDEXDEFINITION", NULL  );
    FdoRdbmsPhFieldCollection* pFields = pFrom->GetFields();

    FdoRdbmsPhField* pField = new FdoRdbmsPhField( physicalSchema, pFrom, L"indexname", L"", NULL );
    pFields->Add( pField );
    pField->Release();

    pField = new FdoRdbmsPhField( physicalSchema, pFrom, L"classid", L"", NULL );
    pFields->Add( pField );
    pField->Release();

    pField = new FdoRdbmsPhField( physicalSchema, pFrom, L"isunique", L"", NULL );
    pFields->Add( pField );
    pField->Release();

    return( pFrom );
}


