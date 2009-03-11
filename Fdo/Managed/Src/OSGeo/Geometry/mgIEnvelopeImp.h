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

#include "mgIEnvelope.h"

class FdoIEnvelope;

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The IEnvelopeImp class is a three-dimensional, axis-aligned box.  This is a
/// helper type for Geometries, and does not itself inherit from
/// IGeometry.
/// \remarks
/// This type is typically used to record the "extents" of a shape, also
/// known in 2 dimensions as an MBR ("Minimum Bounding Rectangle").
/// The box may be initially empty, meaning that no dimensions have an
/// assigned value. Each individual extent may be unassigned, in which case using its
/// accessor will return numeric_limits::quiet_NaN().
public ref class IEnvelopeImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_GEOMETRY::IEnvelope
{
public:
    /// \brief
    /// Constructs an IEnvelopeImp managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	IEnvelopeImp(System::IntPtr unmanaged, System::Boolean autoDelete);

public:
    /// \brief
    /// Gets the minimum X coordinate value.
    /// 
    /// \remarks
    /// This is the smallest X ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the X coordinate for the point in the rear, lower-left
    /// corner of the envelope.
    /// 
    /// \return
    /// Returns the miniumum X ordinate; numeric_limits::quiet_NaN() by default
    /// 
    virtual property System::Double MinX
    {
        System::Double get();
    }

    /// \brief
    /// Gets the minimum Y coordinate value.
    /// 
    /// \remarks
    /// This is the smallest Y ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the Y coordinate for the point in the rear, lower-left
    /// corner of the envelope.
    /// 
    /// \return
    /// Returns the miniumum Y ordinate; numeric_limits::quiet_NaN() by default
    /// 
    virtual property System::Double MinY
    {
        System::Double get();
    }

    /// \brief
    /// Gets the minimum Z coordinate value.
    /// 
    /// \remarks
    /// This is the smallest Z ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the Z coordinate for the point in the rear, lower-left
    /// corner of the envelope.
    /// 
    /// \return
    /// Returns the miniumum Z ordinate; numeric_limits::quiet_NaN() by default
    /// 
    virtual property System::Double MinZ
    {
        System::Double get();
    }

    /// \brief
    /// Gets the maximum X coordinate value.
    /// 
    /// \remarks
    /// This is the largest X ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the X coordinate for the point in the front, upper-right
    /// corner of the envelope.
    /// 
    /// \return
    /// Returns the maxiumum X ordinate; numeric_limits::quiet_NaN() by default
    /// 
    virtual property System::Double MaxX
    {
        System::Double get();
    }

    /// \brief
    /// Gets the maximum Y coordinate value.
    /// 
    /// \remarks
    /// This is the largest Y ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the Y coordinate for the point in the front, upper-right
    /// corner of the envelope.
    /// 
    /// \return
    /// Returns the maxiumum Y ordinate; numeric_limits::quiet_NaN() by default
    /// 
    virtual property System::Double MaxY
    {
        System::Double get();
    }

    /// \brief
    /// Gets the maximum Z coordinate value.
    /// 
    /// \remarks
    /// This is the largest Z ordinate value for the envelope, regardless
    /// of the sense of the axes.  In a conventional right-handed system,
    /// this would be the Z coordinate for the point in the front, upper-right
    /// corner of the envelope.
    /// 
    /// \return
    /// Returns the maxiumum Z ordinate; numeric_limits::quiet_NaN() by default
    /// 
    virtual property System::Double MaxZ
    {
        System::Double get();
    }

    /// \brief
    /// Indicates whether the envelope's extents are set.
    /// 
    /// \return
    /// Returns 'true' if none of the X, Y and Z extents have been set; 'false' otherwise
    /// 
    virtual property System::Boolean IsEmpty
    {
        System::Boolean get();
    }

internal:
	FdoIEnvelope* GetImpObj();
};

END_NAMESPACE_OSGEO_GEOMETRY


