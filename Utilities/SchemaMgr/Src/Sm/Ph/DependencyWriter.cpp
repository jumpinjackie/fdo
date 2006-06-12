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
#include <Sm/Ph/CommandWriter.h>
#include <Sm/Ph/DependencyWriter.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhDependencyWriter::FdoSmPhDependencyWriter( FdoSmPhMgrP mgr ) : 
	FdoSmPhWriter( MakeWriter(mgr) )
{
}

FdoSmPhDependencyWriter::~FdoSmPhDependencyWriter(void)
{
}

FdoStringP FdoSmPhDependencyWriter::GetPkTableName()
{
	return(GetString(L"",L"pktablename"));
}

FdoSmPhColumnListP FdoSmPhDependencyWriter::GetPkColumnNames()
{
    // Parse the column list into a collection of column names.
    return( FdoSmPhColumnList::Create( GetManager(), GetString(L"",L"pkcolumnnames"), L" ,") );
}

FdoStringP FdoSmPhDependencyWriter::GetFkTableName()
{
	return(GetString(L"",L"fktablename"));
}

FdoSmPhColumnListP FdoSmPhDependencyWriter::GetFkColumnNames()
{
   	// Parse the column list into a collection of column names.
    return( FdoSmPhColumnList::Create( GetManager(), GetString(L"",L"fkcolumnnames"), L" ,") );
}

FdoStringP FdoSmPhDependencyWriter::GetIdentityColumn()
{
	return(GetString(L"",L"identitycolumn"));
}

FdoStringP FdoSmPhDependencyWriter::GetOrderType()
{
	return(GetString(L"",L"ordertype"));
}

long FdoSmPhDependencyWriter::GetCardinality()
{
	return((long) GetDouble(L"",L"fkcardinality"));
}

void FdoSmPhDependencyWriter::SetPkTableName(FdoStringP sValue)
{
	SetString(L"",L"pktablename", GetManager()->DbObject2MetaSchemaName(sValue));
}

void FdoSmPhDependencyWriter::SetPkColumnNames(FdoSmPhColumnListP sValues)
{
	SetString(L"", L"pkcolumnnames", sValues->ToString(L" ") );
}

void FdoSmPhDependencyWriter::SetFkTableName(FdoStringP sValue)
{
	SetString(L"",L"fktablename", GetManager()->DbObject2MetaSchemaName(sValue));
}

void FdoSmPhDependencyWriter::SetFkColumnNames(FdoSmPhColumnListP sValues)
{
	SetString(L"",L"fkcolumnnames", sValues->ToString(L" ") );
}

void FdoSmPhDependencyWriter::SetIdentityColumn(FdoStringP sValue)
{
	SetString(L"",L"identitycolumn", sValue);
}

void FdoSmPhDependencyWriter::SetOrderType(FdoStringP sValue)
{
	SetString(L"",L"ordertype", sValue);
}

void FdoSmPhDependencyWriter::SetCardinality(long lValue)
{
	SetDouble(L"",L"fkcardinality", lValue);
}

void FdoSmPhDependencyWriter::Modify( FdoStringP pkTableName, FdoStringP fkTableName )
{
    FdoStringP localPkTableName = GetManager()->DbObject2MetaSchemaName(pkTableName);
    FdoStringP localFkTableName = GetManager()->DbObject2MetaSchemaName(fkTableName);

    FdoSmPhWriter::Modify(
        FdoStringP::Format( 
            L"where pktablename in ( %ls, %ls ) and fktablename in ( %ls, %ls )",
            (FdoString*) GetManager()->FormatSQLVal(pkTableName,FdoSmPhColType_String),
            (FdoString*) GetManager()->FormatSQLVal(localPkTableName,FdoSmPhColType_String),
            (FdoString*) GetManager()->FormatSQLVal(fkTableName,FdoSmPhColType_String), 
            (FdoString*) GetManager()->FormatSQLVal(localFkTableName,FdoSmPhColType_String) 
		)
	);
}

void FdoSmPhDependencyWriter::Delete( FdoStringP pkTableName, FdoStringP fkTableName )
{
    FdoStringP localPkTableName = GetManager()->DbObject2MetaSchemaName(pkTableName);
    FdoStringP localFkTableName = GetManager()->DbObject2MetaSchemaName(fkTableName);

    FdoSmPhWriter::Delete(
        FdoStringP::Format( 
            L"where pktablename in ( %ls, %ls ) and fktablename in ( %ls, %ls )",
            (FdoString*) GetManager()->FormatSQLVal(pkTableName,FdoSmPhColType_String),
            (FdoString*) GetManager()->FormatSQLVal(localPkTableName,FdoSmPhColType_String),
            (FdoString*) GetManager()->FormatSQLVal(fkTableName,FdoSmPhColType_String), 
            (FdoString*) GetManager()->FormatSQLVal(localFkTableName,FdoSmPhColType_String) 
		)
    );
}

FdoSmPhRowP FdoSmPhDependencyWriter::MakeRow( FdoSmPhMgrP mgr )
{
    FdoStringP depDefTable = mgr->GetDcDbObjectName(L"f_attributedependencies");

    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"f_attributedependencies", mgr->FindDbObject(depDefTable) );

    // Each field adds itself to the row
    FdoSmPhFieldP field = new FdoSmPhField( row, L"pktablename" );
    field = new FdoSmPhField( row, L"pkcolumnnames");
    field = new FdoSmPhField( row, L"fktablename" );
    field = new FdoSmPhField( row, L"fkcolumnnames" );
    field = new FdoSmPhField( row, L"fkcardinality" );
    field = new FdoSmPhField( row, L"identitycolumn" );
    field = new FdoSmPhField( row, L"ordertype" );

    return( row );
}

FdoSmPhWriterP FdoSmPhDependencyWriter::MakeWriter( FdoSmPhMgrP mgr )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}

