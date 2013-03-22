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
#include <Rdbms/Override/RdbmsOvGeometricColumn.h>


FdoRdbmsOvGeometricColumn::FdoRdbmsOvGeometricColumn()
{
}

FdoRdbmsOvGeometricColumn::FdoRdbmsOvGeometricColumn(FdoString* name) :
    FdoPhysicalElementMapping(name)
{
}

FdoRdbmsOvGeometricColumn::~FdoRdbmsOvGeometricColumn()
{
}

void FdoRdbmsOvGeometricColumn::SetParent(FdoPhysicalElementMapping* value)
{
    FdoPhysicalElementMapping::SetParent(value);
}

void FdoRdbmsOvGeometricColumn::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    xmlWriter->WriteStartElement( L"GeometricColumn" );

    FdoPhysicalElementMapping::_writeXml( xmlWriter, flags );

    _writeXmlContents( xmlWriter, flags );

    xmlWriter->WriteEndElement();
}

void FdoRdbmsOvGeometricColumn::_writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
}

FdoXmlSaxHandler* FdoRdbmsOvGeometricColumn::XmlStartElement(
    FdoXmlSaxContext* context,
    FdoString* uri,
    FdoString* name,
    FdoString* qname,
    FdoXmlAttributeCollection* atts
)
{
    // Handle generic sub-elements.
    FdoXmlSaxHandler* pRet = FdoPhysicalElementMapping::XmlStartElement(context, uri, name, qname, atts);

    if ( !pRet )
        // report unexpected sub-elements.
        pRet = SubElementError( context, L"GeometricColumn", name );

    return(pRet);
}

