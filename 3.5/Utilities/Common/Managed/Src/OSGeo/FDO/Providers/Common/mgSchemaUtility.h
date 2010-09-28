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
*
*/

#pragma once

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON

public ref class SchemaUtility
{
public:
    /// Deep-copy an FDO schema collection.
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection ^ DeepCopyFdoFeatureSchemas(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection ^ schemas, System::String^  schemaName);

    /// Deep-copy an FDO schema.
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema ^ DeepCopyFdoFeatureSchema(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema ^ schema);

    /// Deep-copy an FDO class Definition.
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition ^ DeepCopyFdoClassDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition ^ sourceClass);

    /// Deep-copy an FDO class (Non-Feature).
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::Class ^ DeepCopyFdoClass(NAMESPACE_OSGEO_FDO_SCHEMA::Class ^ sourceClass);

    /// Deep-copy an FDO Feature class.
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass ^ DeepCopyFdoFeatureClass(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass ^ sourceClass);

    /// Deep-copy an FDO Property Definition.
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition ^ DeepCopyFdoPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition ^ prop);

    /// Deep-copy an FDO Data Property Definition.
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition ^ DeepCopyFdoDataPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition ^ prop);

    /// Deep-copy an FDO Geometric Property Definition.
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition ^ DeepCopyFdoGeometricPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition ^ prop);

    /// Deep-copy an FDO Raster Property Definition.
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition ^ DeepCopyFdoRasterPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition ^ prop);

    /// Deep-copy an FDO Association Property Definition.
    /// This is guaranteed to never return NULL (will throw a Exception on error).
    static NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition ^ DeepCopyFdoAssociationPropertyDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition ^ prop);
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_COMMON