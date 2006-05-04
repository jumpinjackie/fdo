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

#pragma  once

#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Schema\mgClassType.h"

class FdoFeatureClass;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class GeometricPropertyDefinition;

/// \brief
/// The FdoFeatureClass class can be used to represent spatial or non-spatial features.
/// Spatial features must have at lease one geometric property. The FdoFeatureClass
/// type has one pre-defined base property "Bounds". The "Bounds" read-only
/// property can be used to get the geometric boundary of the spatial feature.
/// The value of this property will be null if the feature is not a spatial
/// feature.
public __gc class FeatureClass : public NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an FdoFeatureClass.
    /// 
	FeatureClass();

    /// \brief
    /// Constructs an instance of an FdoFeatureClass using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
	FeatureClass(System::String* name, System::String* description);

    /// \brief
    /// Gets the concrete class type.
    /// 
    /// \return
    /// Returns class type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassType get_ClassType();

    /// \brief
    /// Gets the geometric property that defines the geometry for this spatial 
    /// feature class.
    /// 
    /// \return
    /// Returns the geometric property
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition* get_GeometryProperty();

    /// \brief
    /// Sets the geometric property that defines the geometry for this spatial 
    /// feature class. The specified FdoGeometricPropertyDefinition must be a member 
    /// of either the FdoClassDefinition.GetBaseProperties or FdoClassDefinition.GetProperties 
    /// collection.
    /// 
    /// \param value 
    /// Input the geometric property
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_GeometryProperty(NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition* value);

	FeatureClass(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoFeatureClass* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


