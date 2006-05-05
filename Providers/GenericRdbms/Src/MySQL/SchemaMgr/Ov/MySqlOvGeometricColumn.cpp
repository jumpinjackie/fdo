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
#include <Rdbms/Override/MySQL/MySqlOvGeometricColumn.h>

FdoMySQLOvGeometricColumn* FdoMySQLOvGeometricColumn::Create()
{
    return new FdoMySQLOvGeometricColumn();
}

FdoMySQLOvGeometricColumn* FdoMySQLOvGeometricColumn::Create(FdoString* name)
{
    return new FdoMySQLOvGeometricColumn(name);
}

FdoMySQLOvGeometricColumn::FdoMySQLOvGeometricColumn()
{
}

FdoMySQLOvGeometricColumn::FdoMySQLOvGeometricColumn(FdoString* name) :
    FdoRdbmsOvGeometricColumn(name)
{
}

FdoMySQLOvGeometricColumn::~FdoMySQLOvGeometricColumn()
{
}

void FdoMySQLOvGeometricColumn::Dispose()
{
    delete this;
}



void FdoMySQLOvGeometricColumn::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoRdbmsOvGeometricColumn::InitFromXml( pContext, attrs );
}

void FdoMySQLOvGeometricColumn::_writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
}
