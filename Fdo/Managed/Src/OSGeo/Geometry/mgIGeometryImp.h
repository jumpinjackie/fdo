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


#pragma once

#include "mgIGeometry.h"

class FdoIGeometry;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class IEnvelope;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The IGeometryImp class defines the properties and methods common to all geometric types. 
/// IGeometryImp is a concreate implementation of IGeometry.
public ref class IGeometryImp :
    public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_GEOMETRY::IGeometry
{
public:
    /// \brief
    /// Constructs a managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed element should be automatically deleted 
    /// once it no longer referenced.
    /// 
	IGeometryImp(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    /// \brief
    /// Gets the envelope for the Geometry.
    /// 
    /// \return
    /// Returns the envelope
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ Envelope
    {
        virtual NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ get();
    }

    /// \brief
    /// Gets the dimensionality of ordinates in this object.
    /// 
    /// \remarks
    /// Values are from the Dimensionality enumeration.
    /// A return type of "Int32" is used instead of the enumeration, catering to typical use with bit masking.
    /// 
    /// \return
    /// Returns the ordinate dimensionality
    /// 
    property System::Int32 Dimensionality
    {
        virtual System::Int32 get();
    }

    /// \brief
    /// Gets the type of the most-derived interface 
    /// in the Geometry package for this object
    /// 
    /// \remarks
    /// The derived type indicates to what class the object may safely be cast.
    /// 
    /// \return
    /// Returns the derived type
    /// 
    property NAMESPACE_OSGEO_COMMON::GeometryType DerivedType
    {
        virtual NAMESPACE_OSGEO_COMMON::GeometryType get();
    }

    /// \brief
    /// Gets the text string representation of this Geometry.
    /// 
    /// \remarks
    /// This object retains ownership of the string.
    /// The caller should NOT attempt to free it.
    /// The text string may be retained in memory by the object indefinitely.
    /// Calling this method on a large number of Geometries that are retained
    /// in memory may cause a noticable increase in memory consumption.
    /// 
    /// \return
    /// Returns the text string
    /// 
    property System::String^ Text
    {
        virtual System::String^ get();
    }

internal:
	FdoIGeometry* GetImpObj();
};

END_NAMESPACE_OSGEO_GEOMETRY


