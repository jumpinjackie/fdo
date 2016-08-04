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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvColumn.h>

FdoSqlServerOvColumn* FdoSqlServerOvColumn::Create()
{
    return new FdoSqlServerOvColumn();
}

FdoSqlServerOvColumn* FdoSqlServerOvColumn::Create(FdoString *name)
{
    return new FdoSqlServerOvColumn(name);
}

FdoSqlServerOvColumn::FdoSqlServerOvColumn()
{
}

FdoSqlServerOvColumn::FdoSqlServerOvColumn(FdoString* name) :
    FdoRdbmsOvColumn(name)
{
}

FdoSqlServerOvColumn::~FdoSqlServerOvColumn()
{
}

FdoString* FdoSqlServerOvColumn::GetFormula()
{
    return mFormula;
}

void FdoSqlServerOvColumn::SetFormula(FdoString* formula)
{
    mFormula = formula;
}

void FdoSqlServerOvColumn::Dispose()
{
    delete this;
}

void FdoSqlServerOvColumn::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvColumn::InitFromXml( pContext, attrs );

    FdoXmlAttributeP att = attrs->FindItem( L"formula" );
    if ( att ) {
        mFormula = att->GetValue();
    }
}

void FdoSqlServerOvColumn::_writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    if ( mFormula.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"formula", mFormula );
}


