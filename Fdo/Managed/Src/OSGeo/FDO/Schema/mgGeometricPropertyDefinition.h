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

#include "FDO\Schema\mgPropertyDefinition.h"
#include "FDO\Schema\mgPropertyType.h"

class FdoGeometricPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// GeometricPropertyDefinition derives from PropertyDefinition and represents a
/// geometric value. Geometric properties can take on any of the geometric types
/// listed in the GeometricType enumeration. 
/// Note: Geometric properties do
/// not actually store the coordinate system or measure units, this is to enable
/// the use of the same schema in different geometric locals. GeometricType
/// informs the client application what types of geometric primitives it can
/// expect when reading a particular geometric property (and what types they can
/// store in the property on insert/update). The client application can
/// interpret the actual geometry values as it wants, e.g., displaying and
/// manipulating the vertices of a Polyline as a group of independent points.
public ref class GeometricPropertyDefinition : public NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an GeometricPropertyDefinition.
    /// 
	GeometricPropertyDefinition();

    /// \brief
    /// Constructs an instance of an GeometricPropertyDefinition using the
    /// specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
	GeometricPropertyDefinition(System::String^ name, System::String^ description);

    /// \brief
    /// Constructs an instance of an GeometricPropertyDefinition using the
    /// specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// \param system 
    /// Input true if this is a system generated property, false otherwise.
    /// <p><b>Note:</b> A client would never set system to true, only a provider.
    /// 
	GeometricPropertyDefinition(System::String^ name, System::String^ description, System::Boolean system);

    /// \brief
    /// Gets the concrete property type.
    /// 
    /// \return
    /// Returns the concrete property type
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType PropertyType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get();
    }

    /// \brief
    /// Gets the GeometricTypes that can be stored in this geometric property.
    /// The returned value may be any combination of the values from the
    /// GeometricType enumeration combined via a bit-wise or operation.
    /// 
    /// \return
    /// Returns the GeometricTypes that can be stored in this geometric property
    /// 
    /// \brief
    /// Sets the GeometricTypes that can be stored in this geometric property.
    /// The value may be any combination of the values from GeometricType
    /// enumeration combined via a bit-wise or operation.
    /// 
    /// \param value 
    /// Input the GeometricTypes that can be stored in this geometric property
    /// 
    property System::Int32 GeometryTypes
    {
        System::Int32 get();
        System::Void set(System::Int32 value);
    }

    /// \brief
    /// Gets the specific geometry types that can be stored in this geometric
    /// property. The returned value is a list of geometry types that are
    /// supported, with length being the number of types returned.
    /// 
    /// \param length 
    /// The parameter is used to return the number of geometry types being
    /// returned by the function.
    /// 
    /// \return
    /// Returns a list of geometry types that are supported.
    /// 
    /// \brief
    /// Sets the specific set of geometry types that can be stored in this
    /// geometric property. The provided value is a list of geometry types
    /// that are supported. Usually, one specific type is supported, but
    /// there can be more than one. 
    /// 
    /// \param types
    /// The specific set of geometry types that can be stored in this
    /// geometric property.
    ///
    /// \param length 
    /// Indicates the number of geometry types being stored in the provided
    /// set of geometry types.
    /// 
    property array<NAMESPACE_OSGEO_COMMON::GeometryType>^ SpecificGeometryTypes[System::Int32%]
    {
        array<NAMESPACE_OSGEO_COMMON::GeometryType>^ get(System::Int32% length);
        System::Void set(System::Int32% length, array<NAMESPACE_OSGEO_COMMON::GeometryType>^ types);
    }

    /// \brief
    /// Returns a Boolean value that indicates if this geometric property is
    /// read-only.
    /// 
    /// \return
    /// Returns a Boolean value that specifies whether this geometric property is
    /// read-only
    /// 
    /// \brief
    /// Sets a Boolean value that specifies whether this geometric property is
    /// read-only.
    /// 
    /// \param value 
    /// Input a Boolean value that specifies whether this geometric property is
    /// read-only
    /// 
    property System::Boolean ReadOnly
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    /// Gets a Boolean value that indicates if the geometry of this property
    /// include elevation values.
    /// 
    /// \return
    /// Returns a Boolean value that determines if the geometry of this property
    /// includes elevation values
    /// 
    /// \brief
    /// Sets a Boolean value that determines if the geometry of this property
    /// includes elevation values.
    /// 
    /// \param value 
    /// Input a Boolean value that determines if the geometry of this property
    /// includes elevation values
    /// 
    property System::Boolean HasElevation
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    /// Gets a Boolean value that indicates if the geometry of this property
    /// includes measurement values that can be used for dynamic segmentation.
    /// 
    /// \return
    /// Returns a Boolean value that indicates if the geometry of this property
    /// includes measurement values
    /// 
    /// \brief
    /// Sets a Boolean value that determines if the geometry of this property
    /// includes measurement values that can be used for dynamic segmentation.
    /// 
    /// \param value 
    /// Input a Boolean value that determines if the geometry of this property
    /// includes measurement values
    /// 
    property System::Boolean HasMeasure
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    /// Sets/add a Spatial Context association to this geometric property.
    /// 
    /// \param value 
    /// Input the Spatial Context name to be added/set. 
    /// Defaults to the active Spatial Context.
    /// 
    /// \brief
    /// Gets the Spatial Context name associated to this geometric property.
    /// 
    /// \return
    /// Returns a String value representing the Spatial Context name.
    /// If nullptr then the geometric property is associated with all the Spatial Contexts
    /// in the datastore (supports multiple geometric representations via this property).
    /// 
    property System::String^ SpatialContextAssociation
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }
    /// \brief
    /// Constructs a GeometricPropertyDefinition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	GeometricPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(unmanaged, autoDelete)
	{

	}

internal:
	inline FdoGeometricPropertyDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


