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
#include <Rdbms/Override/ODBC/OdbcOvColumn.h>

FdoOdbcOvColumn* FdoOdbcOvColumn::Create()
{
    return new FdoOdbcOvColumn();
}

FdoOdbcOvColumn* FdoOdbcOvColumn::Create(FdoString *name)
{
    return new FdoOdbcOvColumn(name);
}

FdoOdbcOvColumn::FdoOdbcOvColumn()
{
}

FdoOdbcOvColumn::FdoOdbcOvColumn(FdoString* name) :
    FdoRdbmsOvColumn(name)
{
}

FdoOdbcOvColumn::~FdoOdbcOvColumn()
{
}

void FdoOdbcOvColumn::Dispose()
{
    delete this;
}

void FdoOdbcOvColumn::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvColumn::InitFromXml( pContext, attrs );
}

void FdoOdbcOvColumn::_writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
}


