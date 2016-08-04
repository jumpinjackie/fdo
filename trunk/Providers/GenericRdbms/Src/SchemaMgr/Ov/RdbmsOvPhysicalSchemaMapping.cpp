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
#include <Utilities/SchemaMgr/Overrides/TableMappingType.h>
#include <Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h>
#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>


FdoRdbmsOvPhysicalSchemaMapping::FdoRdbmsOvPhysicalSchemaMapping()
{
    mMappingType = FdoSmOvTableMappingType_Default;
    mDefaultGeometricColumnType = FdoSmOvGeometricColumnType_Default;
    mDefaultGeometricContentType = FdoSmOvGeometricContentType_Default;
    mClasses = FdoRdbmsOvClassCollection::Create( this );
}

FdoRdbmsOvPhysicalSchemaMapping::FdoRdbmsOvPhysicalSchemaMapping(FdoString *name)
{
    SetName(name);
    mMappingType = FdoSmOvTableMappingType_Default;
    mDefaultGeometricColumnType = FdoSmOvGeometricColumnType_Default;
    mDefaultGeometricContentType = FdoSmOvGeometricContentType_Default;
    mClasses = FdoRdbmsOvClassCollection::Create( this );
}

FdoRdbmsOvPhysicalSchemaMapping::~FdoRdbmsOvPhysicalSchemaMapping()
{
}

FdoRdbmsOvReadOnlyClassCollection *FdoRdbmsOvPhysicalSchemaMapping::GetClasses()
{
    return FdoRdbmsOvReadOnlyClassCollection::Create(mClasses);
}

FdoSmOvTableMappingType FdoRdbmsOvPhysicalSchemaMapping::GetTableMapping()
{
    return mMappingType;
}

void FdoRdbmsOvPhysicalSchemaMapping::SetTableMapping(FdoSmOvTableMappingType mappingType)
{
    mMappingType = mappingType;
}

FdoSmOvGeometricColumnType FdoRdbmsOvPhysicalSchemaMapping::GetGeometricColumnType()
{
    return mDefaultGeometricColumnType;
}

void FdoRdbmsOvPhysicalSchemaMapping::SetGeometricColumnType(FdoSmOvGeometricColumnType columnType)
{
    mDefaultGeometricColumnType = columnType;
}

FdoSmOvGeometricContentType FdoRdbmsOvPhysicalSchemaMapping::GetGeometricContentType()
{
    return mDefaultGeometricContentType;
}

void FdoRdbmsOvPhysicalSchemaMapping::SetGeometricContentType(FdoSmOvGeometricContentType contentType)
{
    mDefaultGeometricContentType = contentType;
}

FdoRdbmsOvSchemaAutoGeneration* FdoRdbmsOvPhysicalSchemaMapping::GetAutoGeneration()
{
    return FDO_SAFE_ADDREF(mAutoGeneration.p);
}

void FdoRdbmsOvPhysicalSchemaMapping::SetAutoGeneration(FdoRdbmsOvSchemaAutoGeneration* autoGeneration)
{
    mAutoGeneration = FDO_SAFE_ADDREF(autoGeneration);
}

void FdoRdbmsOvPhysicalSchemaMapping::SetParent(FdoPhysicalElementMapping* value)
{
    FdoPhysicalElementMapping::SetParent(value);
}

void FdoRdbmsOvPhysicalSchemaMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Get current error level

    FdoXmlFlags::ErrorLevel errorLevel = FdoXmlFlags::ErrorLevel_VeryLow;
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    if ( fdoContext )
        errorLevel = FdoXmlFlagsP(fdoContext->GetFlags())->GetErrorLevel();

    FdoPhysicalSchemaMapping::InitFromXml( pContext, attrs );

    FdoXmlAttributeP att = attrs->FindItem( L"tableMapping" );
    if ( att ) {
        try {
            mMappingType = FdoSmOvTableMappingTypeMapper::String2Type(att->GetValue());
        }
        catch ( FdoCommandException* ex ) {
            // No errors logged at very low error level.
            if ( errorLevel != FdoXmlFlags::ErrorLevel_VeryLow )
                pContext->AddError( ex );
            ex->Release();
        }
    }
}

void FdoRdbmsOvPhysicalSchemaMapping::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    FdoInt32 idx;

    xmlWriter->WriteStartElement( L"SchemaMapping" );
    xmlWriter->WriteAttribute( L"xmlns:rdb", L"http://fdordbms.osgeo.org/schemas" );

    _writeXmlAttributes( xmlWriter, flags );

    if ( mMappingType != FdoSmOvTableMappingType_Default )
        xmlWriter->WriteAttribute( L"tableMapping", FdoSmOvTableMappingTypeMapper::Type2String(mMappingType) );

    FdoPhysicalSchemaMapping::_writeXml( xmlWriter, flags );

    if ( mAutoGeneration )
        mAutoGeneration->_writeXml( xmlWriter, flags );

    for ( idx = 0; idx < mClasses->GetCount(); idx++ )
        FdoRdbmsOvClassP( mClasses->GetItem(idx) )->_writeXml( xmlWriter, flags );

    _writeXmlElements( xmlWriter, flags );

    xmlWriter->WriteEndElement();
}

FdoXmlSaxHandler* FdoRdbmsOvPhysicalSchemaMapping::XmlStartElement(
    FdoXmlSaxContext* context,
    FdoString* uri,
    FdoString* name,
    FdoString* qname,
    FdoXmlAttributeCollection* atts
)
{
    // Handle generic sub-elements.
    FdoXmlSaxHandler* pRet = FdoPhysicalSchemaMapping::XmlStartElement(context, uri, name, qname, atts);

    if ( !pRet ) {
        // Not a generic sub-element so must be a class; handle it here.

        // Handle sub-elements for classes
        if ( wcscmp(name, L"complexType") == 0 ) {
            // Check if class already in schema
            FdoRdbmsOvClassP pClass = CreateClass( context, atts );
            FdoRdbmsOvClassP pDupClass = mClasses->FindItem( pClass->GetName() );
            if ( pDupClass ) {
                // Set parent so that right error message is generated.
                pClass->SetParent(this);
                pRet = DuplicateSubElementError( context, L"SchemaMapping", name, pClass->GetName() );
            }
            else {
                AddClass(pClass);
                pRet = pClass;
            }
        }

        // Handle Schema Autogeneration settings.
        if ( wcscmp(name, L"AutoGeneration") == 0 ) {
            if ( mAutoGeneration ) {
                // Maximum of 1 AutoGeneration setting allowed.
                pRet = DuplicateSubElementError( context, L"SchemaMapping", name, L"" );
            }
            else {
                mAutoGeneration = FdoRdbmsOvSchemaAutoGeneration::Create();
                mAutoGeneration->InitFromXml( context, atts );
                pRet = mAutoGeneration;
            }
        }
    }

    if ( !pRet )
        // report unexpected sub-elements.
        pRet = SubElementError( context, L"SchemaMapping", name );

    return(pRet);
}

void FdoRdbmsOvPhysicalSchemaMapping::_writeXmlAttributes(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
}

void FdoRdbmsOvPhysicalSchemaMapping::_writeXmlElements(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
}

FdoRdbmsOvClassCollection *FdoRdbmsOvPhysicalSchemaMapping::GetRdbmsClasses()
{
    return FDO_SAFE_ADDREF(mClasses.p);
}

