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
#include <Fdo/Xml/SchemaMapping.h>

FdoXmlElementMapping* FdoXmlElementMapping::Create(
    FdoString* name
)
{
    return new FdoXmlElementMapping( name );
}


FdoXmlElementMapping::FdoXmlElementMapping(
    FdoString* name
) :
    FdoPhysicalElementMapping(name)
{
}

FdoXmlElementMapping::~FdoXmlElementMapping()
{
}

void FdoXmlElementMapping::SetClassMapping(FdoXmlClassMapping* classMapping)
{
    mClassMapping = FDO_SAFE_ADDREF(classMapping);
    // Reference to class mapping is no longer dangling.
    mClassName = L"";
    mSchemaName = L"";
}

FdoXmlClassMapping* FdoXmlElementMapping::GetClassMapping()
{
    return FDO_SAFE_ADDREF( (FdoXmlClassMapping*) mClassMapping );
}

void FdoXmlElementMapping::SetClassName( FdoString* className )
{
    // reference to class is now dangling.
    mClassMapping = NULL;
    mClassName = className;
}

FdoString* FdoXmlElementMapping::GetClassName()
{
    // Initialize to dangling reference
    FdoString* className = mClassName;

    // Get class name from class mapping object if present.
    if ( mClassMapping ) 
        className = mClassMapping->GetName();

    return className;
}

void FdoXmlElementMapping::SetSchemaName( FdoString* schemaName )
{
    // reference to class is now dangling.
    mClassMapping = NULL;
    mSchemaName = schemaName;
}

FdoString* FdoXmlElementMapping::GetSchemaName()
{
    // Initialize to dangling reference
    FdoString* schemaName = mSchemaName;

    if ( mClassMapping ) {
        // Class Mapping object set, get schema name from it.
        FdoPhysicalElementMappingP parent = mClassMapping->GetParent();
        if ( parent ) 
            schemaName = parent->GetName();
    }
    else {
        // Otherwise, if the element class is defined, default the schema 
        // name to this element mapping's schema.
        if ( (wcslen(schemaName) == 0) && (mClassName != L"") ) {
            FdoPhysicalSchemaMappingP schemaMapping = GetSchemaMapping();
            if ( schemaMapping ) 
                schemaName = schemaMapping->GetName();
        }
    }

    return schemaName;
}

void FdoXmlElementMapping::SetGmlUri( FdoString* gmlUri )
{
    mGmlUri = gmlUri;
}

FdoString* FdoXmlElementMapping::GetGmlUri()
{
    FdoString* gmlUri = mGmlUri;

    if ( wcslen(gmlUri) == 0 ) {
        FdoXmlSchemaMappingP schemaMapping = (FdoXmlSchemaMapping*) GetSchemaMapping();
        if ( schemaMapping ) 
            gmlUri = schemaMapping->GetTargetNamespace();
    }

    return gmlUri;
}

void FdoXmlElementMapping::SetGmlLocalName( FdoString* gmlLocalName )
{
    mGmlLocalName = gmlLocalName;
}

FdoString* FdoXmlElementMapping::GetGmlLocalName()
{
    FdoString* gmlLocalName = mGmlLocalName;

    if ( wcslen(gmlLocalName) == 0 ) 
        gmlLocalName = GetName();

    return gmlLocalName;
}

void FdoXmlElementMapping::Dispose()
{
    delete this;
}

void FdoXmlElementMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    FdoPhysicalElementMapping::InitFromXml( pContext, attrs );

    //TODO deserialize class and schema name.
}


void FdoXmlElementMapping::_writeXml(
    FdoXmlWriter* xmlWriter, 
    const FdoXmlFlags* flags
)
{
    xmlWriter->WriteStartElement( L"element" );

    FdoPhysicalElementMapping::_writeXml( xmlWriter, flags );

    // Encode names to handle characters not allowed in XML names.
    FdoStringP classSchema = flags->GetNameAdjust() ? ((FdoString*) xmlWriter->EncodeName(GetSchemaName())) : GetSchemaName();

    xmlWriter->WriteAttribute( 
        L"classSchema", 
        classSchema 
    );

    // Encode names to handle characters not allowed in XML names.
    FdoStringP className = flags->GetNameAdjust() ? ((FdoString*) xmlWriter->EncodeName(GetClassName())) : GetClassName();

    // TODO: centralize
    // Complex Type name is class name with "Type" suffix appended.
    if ( className.GetLength() > 0 ) {
        className += "Type";
    }

    xmlWriter->WriteAttribute( 
        L"className", 
        className
    );

    if ( mGmlUri != L"" ) {
        xmlWriter->WriteAttribute( 
            L"gmlUri", 
            mGmlUri
        );
    }

    if ( mGmlLocalName != L"" ) {
        xmlWriter->WriteAttribute( 
            L"gmlLocalName", 
            mGmlLocalName
        );
    }

    xmlWriter->WriteEndElement();
}


