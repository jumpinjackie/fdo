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
#include <FdoStd.h>
#include <Fdo/Commands/CommandException.h>
#include <Fdo/Commands/Schema/PhysicalClassMapping.h>

FdoPhysicalClassMapping::FdoPhysicalClassMapping() : 
    FdoPhysicalElementMapping()
{
}


FdoPhysicalClassMapping::FdoPhysicalClassMapping(FdoString* name/*, FdoString* description */) :
    FdoPhysicalElementMapping(name)

{
}

FdoPhysicalClassMapping::~FdoPhysicalClassMapping()
{
}

void FdoPhysicalClassMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    FdoPhysicalElementMapping::InitFromXml( pContext, attrs );

    // Decode the complexType name
    FdoStringP className = GetName();
    if ( fdoContext ) 
        className = fdoContext->DecodeName(className);

    // If complexType name has "Type" suffix, removed it from class name
    if ( (className.GetLength() > 4) && (className.Mid(className.GetLength() - 4, 999) == L"Type") ) {
        className = className.Mid( 0, className.GetLength() - 4 );
    }

    SetName( className );
}


void FdoPhysicalClassMapping::_writeXml(
    FdoXmlWriter* xmlWriter, 
    const FdoXmlFlags* flags
)
{
    FdoPhysicalElementMapping::_writeXml( xmlWriter, flags );

    // Encode names to handle characters not allowed in XML names.
    FdoStringP typeName = flags->GetNameAdjust() ? ((FdoString*) xmlWriter->EncodeName(GetName())) : GetName();

    // Complex Type name is class name with "Type" suffix appended.
    if ( typeName.GetLength() > 0 ) {
        typeName += "Type";
    }

    xmlWriter->WriteAttribute( 
        L"name", 
        typeName
    );
}


