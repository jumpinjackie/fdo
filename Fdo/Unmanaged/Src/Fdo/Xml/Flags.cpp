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
#include <Fdo/Xml/Flags.h>
#include <Fdo/Schema/SchemaException.h>
#include <Fdo/Commands/Schema/PhysicalSchemaMappingCollection.h>

FdoXmlFlags* FdoXmlFlags::Create( FdoString* url, ErrorLevel errorLevel, FdoBoolean nameAdjust )
{
    return new FdoXmlFlags( url, errorLevel, nameAdjust );
}

FdoXmlFlags::FdoXmlFlags( FdoString* url, ErrorLevel errorLevel, FdoBoolean nameAdjust )
{
    SetErrorLevel( errorLevel );
    SetUrl( url );
    SetNameAdjust( nameAdjust );
    mSchemaNameAsPrefix = false;
    mUseGmlId = false;
    mSchemaMappings = NULL;
}

FdoXmlFlags::~FdoXmlFlags() 
{
    FDO_SAFE_RELEASE( mSchemaMappings );
}

void FdoXmlFlags::SetUrl( FdoString* url )
{
    mUrl = url;
}

FdoString* FdoXmlFlags::GetUrl() const
{
    return mUrl;
}

void FdoXmlFlags::SetErrorLevel( ErrorLevel errorLevel )
{
    mErrorLevel = errorLevel;
}

FdoXmlFlags::ErrorLevel FdoXmlFlags::GetErrorLevel() const
{
    return mErrorLevel;
}

void FdoXmlFlags::SetNameAdjust( FdoBoolean nameAdjust )
{
    mNameAdjust = nameAdjust;
}

FdoBoolean FdoXmlFlags::GetNameAdjust() const
{
    return mNameAdjust;
}

void FdoXmlFlags::SetSchemaNameAsPrefix( FdoBoolean schemaNameAsPrefix )
{
    if ( schemaNameAsPrefix && (mErrorLevel != ErrorLevel_VeryLow) ) 
        throw FdoSchemaException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(SCHEMA_59_SCHEMAFROMPREFIX)
            )
        );

    mSchemaNameAsPrefix = schemaNameAsPrefix;
}

FdoBoolean FdoXmlFlags::GetSchemaNameAsPrefix() const
{
    return mSchemaNameAsPrefix;
}

void FdoXmlFlags::SetUseGmlId( FdoBoolean useGmlId )
{
    mUseGmlId = useGmlId;
}

FdoBoolean FdoXmlFlags::GetUseGmlId() const
{
    return mUseGmlId;
}

void FdoXmlFlags::SetSchemaMappings( FdoPhysicalSchemaMappingCollection* mappings )
{
    mSchemaMappings = FDO_SAFE_ADDREF( mappings );
}

FdoPhysicalSchemaMappingCollection* FdoXmlFlags::GetSchemaMappings() const
{
    return FDO_SAFE_ADDREF( mSchemaMappings );
}

void FdoXmlFlags::Dispose()
{
    delete this;
}

