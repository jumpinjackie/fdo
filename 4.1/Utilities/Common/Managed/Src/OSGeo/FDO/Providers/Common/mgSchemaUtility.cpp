/*
* Copyright (C) 2004-2009  Autodesk, Inc.
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
#include "mgSchemaUtility.h"
#include "FdoCommonSchemaUtil.h"

/// Deep-copy an FDO schema collection.
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoFeatureSchemas(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection ^ schemas, System::String^  schemaName)
{
    FdoFeatureSchemaCollection* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas(static_cast<FdoFeatureSchemaCollection*>(schemas->GetDisposableObject().ToPointer()), StringToUni(schemaName));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection(IntPtr(result), true);
}

/// Deep-copy an FDO schema.
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoFeatureSchema(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema ^ schema)
{
    FdoFeatureSchema* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchema(static_cast<FdoFeatureSchema*>(schema->GetDisposableObject().ToPointer()));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema(IntPtr(result), true);
}

/// Deep-copy an FDO class Definition.
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoClassDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition ^ sourceClass)
{
    FdoClassDefinition* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(static_cast<FdoClassDefinition*>(sourceClass->GetDisposableObject().ToPointer()));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(IntPtr(result), true);
}

/// Deep-copy an FDO class (Non-Feature).
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::Class ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoClass(NAMESPACE_OSGEO_FDO_SCHEMA::Class ^ sourceClass)
{
    FdoClass* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoClass(static_cast<FdoClass*>(sourceClass->GetDisposableObject().ToPointer()));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::Class(IntPtr(result), true);
}

/// Deep-copy an FDO Feature class.
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoFeatureClass(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass ^ sourceClass)
{
    FdoFeatureClass* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoFeatureClass(static_cast<FdoFeatureClass*>(sourceClass->GetDisposableObject().ToPointer()));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass(IntPtr(result), true);
}

/// Deep-copy an FDO Property Definition.
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition ^ prop)
{
    FdoPropertyDefinition* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoPropertyDefinition(static_cast<FdoPropertyDefinition*>(prop->GetDisposableObject().ToPointer()));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(IntPtr(result), true);
}

/// Deep-copy an FDO Data Property Definition.
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoDataPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition ^ prop)
{
    FdoDataPropertyDefinition* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoDataPropertyDefinition(static_cast<FdoDataPropertyDefinition*>(prop->GetDisposableObject().ToPointer()));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition(IntPtr(result), true);
}

/// Deep-copy an FDO Geometric Property Definition.
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoGeometricPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition ^ prop)
{
    FdoGeometricPropertyDefinition* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoGeometricPropertyDefinition(static_cast<FdoGeometricPropertyDefinition*>(prop->GetDisposableObject().ToPointer()));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition(IntPtr(result), true);
}

/// Deep-copy an FDO Raster Property Definition.
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoRasterPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition ^ prop)
{
    FdoRasterPropertyDefinition* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoRasterPropertyDefinition(static_cast<FdoRasterPropertyDefinition*>(prop->GetDisposableObject().ToPointer()));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition(IntPtr(result), true);
}

/// Deep-copy an FDO Association Property Definition.
/// This is guaranteed to never return NULL (will throw a Exception on error).
NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition ^ NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON::SchemaUtility::DeepCopyFdoAssociationPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition ^ prop)
{
    FdoAssociationPropertyDefinition* result = nullptr;
    result = FdoCommonSchemaUtil::DeepCopyFdoAssociationPropertyDefinition(static_cast<FdoAssociationPropertyDefinition*>(prop->GetDisposableObject().ToPointer()));
    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition(IntPtr(result), true);
}
