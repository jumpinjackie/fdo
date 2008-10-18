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

#include <Rdbms/Override/RdbmsOvGeometricPropertyDefinition.h>
#include "Utilities/SchemaMgr/Overrides/GeometricContentTypeMapper.h"
#include "Utilities/SchemaMgr/Overrides/GeometricColumnTypeMapper.h"

FdoRdbmsOvGeometricPropertyDefinition::FdoRdbmsOvGeometricPropertyDefinition()
:
    mGeometricColumnType(FdoSmOvGeometricColumnType_Default),
    mGeometricContentType(FdoSmOvGeometricContentType_Default)
{
}

FdoRdbmsOvGeometricPropertyDefinition::FdoRdbmsOvGeometricPropertyDefinition(FdoString* name)
:
    FdoRdbmsOvPropertyDefinition(name),
    mGeometricColumnType(FdoSmOvGeometricColumnType_Default),
    mGeometricContentType(FdoSmOvGeometricContentType_Default)
{
}

FdoRdbmsOvGeometricPropertyDefinition::~FdoRdbmsOvGeometricPropertyDefinition()
{
}

FdoRdbmsOvGeometricColumn* FdoRdbmsOvGeometricPropertyDefinition::GetColumn()
{
    return FDO_SAFE_ADDREF((FdoRdbmsOvGeometricColumn*) mColumn);
}

void FdoRdbmsOvGeometricPropertyDefinition::SetColumn(FdoRdbmsOvGeometricColumn* geometricColumn)
{
    mColumn = FDO_SAFE_ADDREF(geometricColumn);
    mColumn->SetParent(this);
}

FdoSmOvGeometricColumnType FdoRdbmsOvGeometricPropertyDefinition::GetGeometricColumnType()
{
    return mGeometricColumnType;
}

void FdoRdbmsOvGeometricPropertyDefinition::SetGeometricColumnType(FdoSmOvGeometricColumnType columnType)
{
    mGeometricColumnType = columnType;
}

FdoSmOvGeometricContentType FdoRdbmsOvGeometricPropertyDefinition::GetGeometricContentType()
{
    return mGeometricContentType;
}

void FdoRdbmsOvGeometricPropertyDefinition::SetGeometricContentType(FdoSmOvGeometricContentType contentType)
{
    mGeometricContentType = contentType;
}

FdoString * FdoRdbmsOvGeometricPropertyDefinition::GetXColumnName()
{
    return mXColumnName;
}

void FdoRdbmsOvGeometricPropertyDefinition::SetXColumnName(FdoString * columnName)
{
    mXColumnName = columnName;
}

FdoString * FdoRdbmsOvGeometricPropertyDefinition::GetYColumnName()
{
    return mYColumnName;
}

void FdoRdbmsOvGeometricPropertyDefinition::SetYColumnName(FdoString * columnName)
{
    mYColumnName = columnName;
}

FdoString * FdoRdbmsOvGeometricPropertyDefinition::GetZColumnName()
{
    return mZColumnName;
}

void FdoRdbmsOvGeometricPropertyDefinition::SetZColumnName(FdoString * columnName)
{
    mZColumnName = columnName;
}

void FdoRdbmsOvGeometricPropertyDefinition::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    xmlWriter->WriteStartElement( L"element" );

    FdoPhysicalPropertyMapping::_writeXml( xmlWriter, flags );

    if (FdoSmOvGeometricColumnType_Default != mGeometricColumnType)
        xmlWriter->WriteAttribute( L"GeometricColumnType", FdoSmOvGeometricColumnTypeMapper::Type2String(mGeometricColumnType) );

    if (FdoSmOvGeometricContentType_Default != mGeometricContentType)
        xmlWriter->WriteAttribute( L"GeometricContentType", FdoSmOvGeometricContentTypeMapper::Type2String(mGeometricContentType) );

    if ( mXColumnName.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"xColumnName", mXColumnName );

    if ( mYColumnName.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"yColumnName", mYColumnName );

    if ( mZColumnName.GetLength() > 0 )
        xmlWriter->WriteAttribute( L"zColumnName", mZColumnName );

    _writeXmlContents( xmlWriter, flags );

    xmlWriter->WriteEndElement();
}

void FdoRdbmsOvGeometricPropertyDefinition::_writeXmlContents(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    if ( mColumn )
        mColumn->_writeXml( xmlWriter, flags );
}

void FdoRdbmsOvGeometricPropertyDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Get the current error level

    FdoXmlFlags::ErrorLevel errorLevel = FdoXmlFlags::ErrorLevel_VeryLow;
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    if ( fdoContext )
        errorLevel = FdoXmlFlagsP(fdoContext->GetFlags())->GetErrorLevel();

    FdoRdbmsOvPropertyDefinition::InitFromXml( pContext, attrs );

    FdoXmlAttributeP att = attrs->FindItem( L"GeometricColumnType" );
    if ( att ) {
        try {
            this->mGeometricColumnType = FdoSmOvGeometricColumnTypeMapper::String2Type(att->GetValue());
        }
        catch ( FdoCommandException* ex ) {
            // No errors logged when level is very low.
            if ( errorLevel != FdoXmlFlags::ErrorLevel_VeryLow )
                pContext->AddError( ex );
            ex->Release();
        }
    }

    att = attrs->FindItem( L"GeometricContentType" );
    if ( att ) {
        try {
            this->mGeometricContentType = FdoSmOvGeometricContentTypeMapper::String2Type(att->GetValue());
        }
        catch ( FdoCommandException* ex ) {
            // No errors logged when level is very low.
            if ( errorLevel != FdoXmlFlags::ErrorLevel_VeryLow )
                pContext->AddError( ex );
            ex->Release();
        }
    }

    att = attrs->FindItem( L"xColumnName" );
    if ( att ) {
        this->mXColumnName = att->GetValue();
    }

    att = attrs->FindItem( L"yColumnName" );
    if ( att ) {
        this->mYColumnName = att->GetValue();
    }

    att = attrs->FindItem( L"zColumnName" );
    if ( att ) {
        this->mZColumnName = att->GetValue();
    }
}

