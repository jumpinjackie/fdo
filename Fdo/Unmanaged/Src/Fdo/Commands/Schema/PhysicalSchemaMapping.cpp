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
#include <Fdo/Commands/Schema/PhysicalSchemaMapping.h>

void FdoPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    FdoPhysicalElementMapping::InitFromXml( pContext, attrs );

    // Decode the schema name.
    FdoStringP schemaName = GetName();

    if ( fdoContext ) 
        schemaName = fdoContext->DecodeName(schemaName);

    SetName( schemaName );
}

void FdoPhysicalSchemaMapping::_writeXml(
  	FdoXmlWriter* xmlWriter, 
   	const FdoXmlFlags* flags
)
{
    xmlWriter->WriteAttribute( L"provider", GetProvider() );

    FdoPhysicalElementMapping::_writeXml( xmlWriter, flags );

    // Encode names to handle characters not allowed in XML names.
    FdoStringP schemaName = flags->GetNameAdjust() ? ((FdoString*) xmlWriter->EncodeName(GetName())) : GetName();

    xmlWriter->WriteAttribute( 
        L"name", 
        schemaName 
    );

}

