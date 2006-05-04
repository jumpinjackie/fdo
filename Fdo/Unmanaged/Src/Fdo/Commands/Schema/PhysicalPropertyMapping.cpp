// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <Std.h>
#include <Fdo/Commands/Schema/PhysicalPropertyMapping.h>

FdoPhysicalPropertyMapping::FdoPhysicalPropertyMapping() : 
    FdoPhysicalElementMapping()
{
}


FdoPhysicalPropertyMapping::FdoPhysicalPropertyMapping(FdoString* name/*, FdoString* description */) :
    FdoPhysicalElementMapping(name)

{
}

FdoPhysicalPropertyMapping::~FdoPhysicalPropertyMapping()
{
}

void FdoPhysicalPropertyMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    FdoPhysicalElementMapping::InitFromXml( pContext, attrs );

    // Decode the property name.
    FdoStringP elementName = GetName();

    if ( fdoContext ) 
        elementName = fdoContext->DecodeName(elementName);

    SetName( elementName );
}


void FdoPhysicalPropertyMapping::_writeXml(
    FdoXmlWriter* xmlWriter, 
    const FdoXmlFlags* flags
)
{
    FdoPhysicalElementMapping::_writeXml( xmlWriter, flags );

    // Encode names to handle characters not allowed in XML names.
    FdoStringP elementName = flags->GetNameAdjust() ? ((FdoString*) xmlWriter->EncodeName(GetName())) : GetName();

    xmlWriter->WriteAttribute( 
        L"name", 
        elementName 
    );
}


