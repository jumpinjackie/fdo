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

#include "mgICurveSegmentAbstract.h"

class FdoICurveSegmentAbstract;

BEGIN_NAMESPACE_OSGEO_GEOMETRY

interface class IEnvelope;
interface class IDirectPosition;

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The ICurveSegmentAbstractImp class is a concrete geometric Curve Segment object.  
/// This class is used strictly as a component of curves and, thus, does not inherit from IGeometry.
public ref class ICurveSegmentAbstractImp :
    public NAMESPACE_OSGEO_GEOMETRY::ICurveSegmentAbstract, public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs an ICurveSegmentAbstractImp managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	ICurveSegmentAbstractImp(IntPtr unmanaged, Boolean autoDelete);
	
public:
    /// \brief
    /// Gets the envelope for the curve segment.
    /// 
    /// \return
    /// Returns the envelope
    /// 
    virtual property NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ Envelope
    {
        NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ get();
    }
	
    /// \brief
    /// Gets the starting position of this curve segment.
    /// 
    /// \return
    /// Returns the starting position
    /// 
    virtual property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ StartPosition
    {
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get();
    }

    /// \brief
    /// Gets the ending position of this curve segment.
    /// 
    /// \return
    /// Returns the ending position
    /// 
    virtual property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ EndPosition
    {
        NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get();
    }
	
    /// \brief
    /// Gets the closure state for the curve segment.
    /// 
    /// \remarks
    /// The meaning behind this method is not guaranteed
    /// to be uniform between derived types or between implementations
    /// of this package.
    /// It may represent a computed value, an explicit attribute, or be true by definition.
    /// As a computed value, the result is typically from simply testing the starting and 
    /// ending positions for exact equality.  This is only reliable in floating
    /// point arithmetic if these data have identical origins.
    /// As an explicit attribute, it would be persisted with the Geometry and 
    /// typically denoted by a parameter in the relevant factory method.  
    /// Some Geometry types are closed by definition.
    /// 
    /// \return
    /// Returns 'true' if the curve is closed, and false otherwise
    /// 
    virtual property System::Boolean IsClosed
    {
        System::Boolean get();
    }
	
    /// \brief
    /// Gets the type of the most-derived interface 
    /// in the Geometry package for this object
    /// 
    /// \return
    /// Returns the derived type
    /// 
    virtual property NAMESPACE_OSGEO_COMMON::GeometryComponentType DerivedType
    {
        NAMESPACE_OSGEO_COMMON::GeometryComponentType get();
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
    virtual property System::Int32 Dimensionality
    {
        System::Int32 get();
    }

internal:
	FdoICurveSegmentAbstract *GetImpObj();
};

END_NAMESPACE_OSGEO_GEOMETRY


