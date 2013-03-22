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
#include "IndexReader.h"
#include "IndexWriter.h"

FdoRdbmsPhIndexReader::FdoRdbmsPhIndexReader(FdoStringP schemaName, FdoRdbmsPhysicalSchema* physicalSchema) : 
    FdoRdbmsPhQueryReader( 
        MakeTables( physicalSchema ),
        MakeClauses( schemaName, physicalSchema ),
		physicalSchema 
    )
{
}

FdoRdbmsPhIndexReader::~FdoRdbmsPhIndexReader(void)
{
}

FdoStringP FdoRdbmsPhIndexReader::GetFeatureClassName()
{
	return(GetString(L"classname"));
}

FdoStringP FdoRdbmsPhIndexReader::GetIndexName()
{
	return(GetString(L"indexname"));
}

bool FdoRdbmsPhIndexReader::GetIsUnique()
{
	return(GetBoolean(L"isunique"));
}

FdoStringP FdoRdbmsPhIndexReader::GetAttributeName()
{
	return(GetString(L"attributename"));
}

int FdoRdbmsPhIndexReader::GetAttributePosition()
{
	return((int) GetDouble(L"position"));
}

FdoRdbmsPhFromIntoCollection* FdoRdbmsPhIndexReader::MakeTables( FdoRdbmsPhysicalSchema* physicalSchema )
{
    FdoRdbmsPhFromIntoCollection* pTables = new FdoRdbmsPhFromIntoCollection();

    FdoRdbmsPhFromInto* pFrom = FdoRdbmsPhIndexPropertyWriter::GetFromInto(physicalSchema);
    pTables->Add( pFrom );

    pFrom->Release();

    pFrom = new FdoRdbmsPhFromInto( physicalSchema, L"F_INDEXDEFINITION", NULL  );
    FdoRdbmsPhFieldCollection* pFields = pFrom->GetFields();

    FdoRdbmsPhField* pField = new FdoRdbmsPhField( physicalSchema, pFrom, L"isunique", L"", NULL );
    pFields->Add( pField );
    pField->Release();

    pTables->Add( pFrom );
    pFrom->Release();

    pFrom = new FdoRdbmsPhFromInto( physicalSchema, L"F_CLASSDEFINITION", NULL  );
    pFields = pFrom->GetFields();

    pField = new FdoRdbmsPhField( physicalSchema, pFrom, L"classname", L"", NULL );
    pFields->Add( pField );
    pField->Release();

    pTables->Add( pFrom );
    pFrom->Release();

    return pTables;
}

FdoStringP FdoRdbmsPhIndexReader::MakeClauses( FdoStringP schemaName, FdoRdbmsPhysicalSchema* physicalSchema )
{
    return (
		FdoStringP::Format( 
            L"where f_indexattribute.indexname = f_indexdefinition.indexname and f_indexdefinition.classid = f_classdefinition.classid and schemaname = %ls order by classname, indexname, position", 
			(FdoString*) FdoRdbmsPhysicalSchema::FormatSQLStringVal(schemaName)
        )
    );
}


