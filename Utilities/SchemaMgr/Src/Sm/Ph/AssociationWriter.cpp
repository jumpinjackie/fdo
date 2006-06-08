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
#include <Sm/Ph/AssociationWriter.h>

FdoSmPhAssociationWriter::FdoSmPhAssociationWriter(FdoSmPhMgrP mgr) : 
	FdoSmPhWriter( MakeWriter(mgr) )
{
}

FdoSmPhAssociationWriter::~FdoSmPhAssociationWriter(void)
{
}

FdoStringP FdoSmPhAssociationWriter::GetPkTableName()
{
	return(GetString(L"", L"pktablename"));
}

FdoSmPhColumnListP FdoSmPhAssociationWriter::GetPkColumnNames()
{
    // Parse the column list into a collection of column names.
    return( FdoSmPhColumnList::Create(GetManager(), GetString(L"", L"pkcolumnnames"), L" ") );
}

FdoStringP FdoSmPhAssociationWriter::GetFkTableName()
{
	return(GetString(L"", L"fktablename"));
}

FdoSmPhColumnListP FdoSmPhAssociationWriter::GetFkColumnNames()
{
    // Parse the column list into a collection of column names.
    return( FdoSmPhColumnList::Create(GetManager(), GetString(L"", L"fkcolumnnames"), L" ") );
}

FdoStringP FdoSmPhAssociationWriter::GetMultiplicity()
{
	return(GetString(L"", L"multiplicity"));
}

FdoStringP FdoSmPhAssociationWriter::GetReverseMultiplicity()
{
	return(GetString(L"", L"reversemultiplicity"));
}

FdoStringP FdoSmPhAssociationWriter::GetReverseName()
{
	return(GetString(L"", L"reverseName"));
}

bool FdoSmPhAssociationWriter::GetCascadeLock()
{
	return(GetBoolean(L"", L"cascadelock"));
}

int FdoSmPhAssociationWriter::GetDeleteRule()
{
	return GetInteger(L"", L"deleterule");
}

FdoStringP FdoSmPhAssociationWriter::GetPseudoColumnName()
{
	return(GetString(L"", L"pseudocolname"));
}

void FdoSmPhAssociationWriter::SetPkTableName(FdoStringP sValue)
{
	SetString(L"", L"pktablename", sValue);
}

void FdoSmPhAssociationWriter::SetPkColumnNames(FdoSmPhColumnListP sValues)
{
	SetString(L"", L"pkcolumnnames", sValues->ToString(L" ") );
}

void FdoSmPhAssociationWriter::SetFkTableName(FdoStringP sValue)
{
	SetString(L"", L"fktablename", sValue);
}

void FdoSmPhAssociationWriter::SetFkColumnNames(FdoSmPhColumnListP sValues)
{
	SetString(L"", L"fkcolumnnames", sValues->ToString(L" ") );
}

void FdoSmPhAssociationWriter::SetMultiplicity(FdoStringP sValue)
{
	SetString(L"", L"multiplicity", sValue);
}

void FdoSmPhAssociationWriter::SetReverseMultiplicity(FdoStringP sValue)
{
	SetString(L"", L"reversemultiplicity", sValue);
}

void FdoSmPhAssociationWriter::SetReverseName(FdoStringP sValue)
{
	SetString(L"", L"reversename", sValue);
}

void FdoSmPhAssociationWriter::SetCascadeLock(bool bValue)
{
	SetBoolean(L"", L"cascadelock", bValue);
}

void FdoSmPhAssociationWriter::SetDeleteRule(int iValue)
{
	SetInteger(L"", L"deleterule", iValue);
}

void FdoSmPhAssociationWriter::SetPseudoColumnName(FdoStringP sValue)
{
	SetString(L"", L"pseudocolname", sValue);
}

void FdoSmPhAssociationWriter::Add()
{
    FdoSmPhWriter::Add();
}

void FdoSmPhAssociationWriter::Modify( FdoStringP pkTableName, FdoStringP fkTableName )
{
    FdoSmPhWriter::Modify(
        FdoStringP::Format( 
            L"where pktablename = %ls and fktablename = %ls",
            (FdoString*) GetManager()->FormatSQLVal(pkTableName,FdoSmPhColType_String),
            (FdoString*) GetManager()->FormatSQLVal(fkTableName,FdoSmPhColType_String) 
		)
	);
}

void FdoSmPhAssociationWriter::Delete( FdoStringP pkTableName, FdoStringP fkTableName )
{
    FdoSmPhWriter::Delete(
        FdoStringP::Format( 
            L"where pktablename = %ls and fktablename = %ls",
            (FdoString*) GetManager()->FormatSQLVal(pkTableName,FdoSmPhColType_String),
            (FdoString*) GetManager()->FormatSQLVal(fkTableName,FdoSmPhColType_String) 
		)
    );
}

FdoSmPhRowP FdoSmPhAssociationWriter::MakeRow( FdoSmPhMgrP mgr )
{
    FdoStringP assocDefTable = mgr->GetDcDbObjectName(L"f_associationdefinition");
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"f_associationdefinition", mgr->FindDbObject(assocDefTable) );

    // Each field adds itself to the row.
    FdoSmPhFieldP field = new FdoSmPhField( row, L"pseudocolname");
    field = new FdoSmPhField( row, L"pktablename" );
    field = new FdoSmPhField( row, L"pkcolumnnames" );
    field = new FdoSmPhField( row, L"fktablename" );
    field = new FdoSmPhField( row, L"fkcolumnnames" );
    field = new FdoSmPhField( row, L"multiplicity" );
    field = new FdoSmPhField( row, L"reversemultiplicity" );
    field = new FdoSmPhField( row, L"cascadelock" );
    field = new FdoSmPhField( row, L"deleterule" );
    field = new FdoSmPhField( row, L"reversename" );

    return( row );
}

FdoSmPhWriterP FdoSmPhAssociationWriter::MakeWriter( FdoSmPhMgrP mgr )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}


