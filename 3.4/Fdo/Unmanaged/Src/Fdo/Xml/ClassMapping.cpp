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
#include <Fdo/Xml/ClassMapping.h>

FdoXmlClassMapping* FdoXmlClassMapping::Create(
    FdoString* name, 
    FdoString* gmlName,
    FdoString* wkBaseName,
    FdoString* wkSchemaName
)
{
    return new FdoXmlClassMapping( name, gmlName, wkBaseName, wkSchemaName );
}


FdoXmlClassMapping::FdoXmlClassMapping(
    FdoString* name, 
    FdoString* gmlName,
    FdoString* wkBaseName,
    FdoString* wkSchemaName
) :
    FdoPhysicalClassMapping(name),
    mGmlName(gmlName),
    mWkBaseName(wkBaseName),
    mWkSchemaName(wkSchemaName)
{
}

FdoXmlClassMapping::~FdoXmlClassMapping()
{
}

FdoString* FdoXmlClassMapping::GetGmlName()
{
    return mGmlName;
}

FdoString* FdoXmlClassMapping::GetWkBaseName()
{
    return mWkBaseName;
}

FdoString* FdoXmlClassMapping::GetWkSchemaName()
{
    return mWkSchemaName;
}

FdoXmlElementMappingCollection* FdoXmlClassMapping::GetElementMappings()
{
    if ( !mElementMappings )
        mElementMappings = FdoXmlElementMappingCollection::Create(this);

    return FDO_SAFE_ADDREF( (FdoXmlElementMappingCollection*) mElementMappings );
}

void FdoXmlClassMapping::Dispose()
{
    delete this;
}

void FdoXmlClassMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    FdoPhysicalElementMapping::InitFromXml( pContext, attrs );

    //TODO deserialize class and schema name.
}


void FdoXmlClassMapping::_writeXml(
    FdoXmlWriter* xmlWriter, 
    const FdoXmlFlags* flags
)
{
    FdoInt32 idx;

    xmlWriter->WriteStartElement( L"complexType" );

    FdoPhysicalClassMapping::_writeXml( xmlWriter, flags );

    xmlWriter->WriteAttribute( 
        L"gmlName", 
        GetGmlName()
    );

    if ( wcslen(GetWkBaseName()) > 0 ) {
        // Encode names to handle characters not allowed in XML names.
        FdoStringP wkSchema = flags->GetNameAdjust() ? ((FdoString*) xmlWriter->EncodeName(GetWkSchemaName())) : GetWkSchemaName();

        xmlWriter->WriteAttribute( 
            L"wkSchema", 
            wkSchema 
        );

        // Encode names to handle characters not allowed in XML names.
        FdoStringP wkClass = flags->GetNameAdjust() ? ((FdoString*) xmlWriter->EncodeName(GetWkBaseName())) : GetWkBaseName();

        // TODO: centralize
        // Complex Type name is class name with "Type" suffix appended.
        if ( wkClass.GetLength() > 0 ) {
            wkClass += "Type";
        }

        xmlWriter->WriteAttribute( 
            L"wkClass", 
            wkClass
        );
    }

    for ( idx = 0; idx < mElementMappings->GetCount(); idx++ )
        FdoXmlElementMappingP( mElementMappings->GetItem(idx) )->_writeXml( xmlWriter, flags );

    xmlWriter->WriteEndElement();
}


