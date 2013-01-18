/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvTable.h>

FdoSqlServerOvTable* FdoSqlServerOvTable::Create()
{
    return new FdoSqlServerOvTable();
}

FdoSqlServerOvTable* FdoSqlServerOvTable::Create(FdoString *name)
{
    return new FdoSqlServerOvTable(name);
}

FdoSqlServerOvTable::FdoSqlServerOvTable()
{
    Init();
}

FdoSqlServerOvTable::FdoSqlServerOvTable(FdoString *name) : FdoRdbmsOvTable(name)
{
    Init();
}

FdoSqlServerOvTable::~FdoSqlServerOvTable()
{
}


FdoString *FdoSqlServerOvTable::GetTableFilegroup()
{
    return mTableFilegroup;
}

void FdoSqlServerOvTable::SetTableFilegroup(FdoString *tableFilegroup)
{
    mTableFilegroup = tableFilegroup;
}

FdoString *FdoSqlServerOvTable::GetTextFilegroup()
{
    return mTextFilegroup;
}

void FdoSqlServerOvTable::SetTextFilegroup(FdoString *textFilegroup)
{
    mTextFilegroup = textFilegroup;
}

FdoString *FdoSqlServerOvTable::GetIndexFilegroup()
{
    return mIndexFilegroup;
}

void FdoSqlServerOvTable::SetIndexFilegroup(FdoString *indexFilegroup)
{
    mIndexFilegroup = indexFilegroup;
}

SqlServerOvTextInRowOption FdoSqlServerOvTable::GetTextInRow()
{
    return mTextInRow;
}

void FdoSqlServerOvTable::SetTextInRow(SqlServerOvTextInRowOption textInRow)
{
    mTextInRow = textInRow;
}

FdoString* FdoSqlServerOvTable::GetDatabase()
{
    return mDatabase;
}

void FdoSqlServerOvTable::SetDatabase(FdoString *database)
{
    mDatabase = database;
}

FdoString *FdoSqlServerOvTable::GetOwner()
{
    return mOwner;
}

void FdoSqlServerOvTable::SetOwner(FdoString *owner)
{
    mOwner = owner;
}

void FdoSqlServerOvTable::SetParent(FdoPhysicalElementMapping* value)
{
    FdoPhysicalElementMapping::SetParent(value);
}

void FdoSqlServerOvTable::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvTable::InitFromXml( pContext, attrs );

    FdoXmlAttributeP att = attrs->FindItem( L"database" );
    if ( att ) {
        mDatabase = att->GetValue();
    }

    att = attrs->FindItem( L"owner" );
    if ( att ) {
        mOwner = att->GetValue();
    }

    att = attrs->FindItem( L"tableFilegroup" );
    if ( att ) {
        mTableFilegroup = att->GetValue();
    }

    att = attrs->FindItem( L"textFilegroup" );
    if ( att ) {
        mTextFilegroup = att->GetValue();
    }

    att = attrs->FindItem( L"indexFilegroup" );
    if ( att ) {
        mIndexFilegroup = att->GetValue();
    }

    att = attrs->FindItem( L"textInRow" );
    if ( att ) {
        mTextInRow = TextInRow_StringToEnum(att->GetValue(), pContext);
    }
}

void FdoSqlServerOvTable::_writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    if ( mDatabase.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"database", mDatabase );

    if ( mOwner.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"owner", mOwner );

    if ( mTableFilegroup.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"tableFilegroup", mTableFilegroup );

    if ( mTextFilegroup.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"textFilegroup", mTextFilegroup );

    if ( mIndexFilegroup.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"indexFilegroup", mIndexFilegroup );

    if ( mTextInRow != SqlServerOvTextInRowOption_Default)
        xmlWriter->WriteAttribute( L"textInRow", TextInRow_EnumToString(mTextInRow) );
}

void FdoSqlServerOvTable::Dispose()
{
    delete this;
}


void FdoSqlServerOvTable::Init()
{
    mTextInRow = SqlServerOvTextInRowOption_NotInRow;
}