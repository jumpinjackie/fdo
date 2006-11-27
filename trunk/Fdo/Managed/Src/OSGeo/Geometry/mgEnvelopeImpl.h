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

#include "mgIEnvelopeImp.h"

class FdoEnvelopeImpl;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc __interface IEnvelope;
public __gc __interface IDirectPosition;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \brief
/// The EnvelopeImpl class is a default implementation of IEnvelope.
/// EnvelopeImpl implements accessors from IEnvelope, matching mutators, and simple data members.
/// Assignment, exact equality operators, and utility methods are also provided.
public __gc class EnvelopeImpl : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_GEOMETRY::IEnvelope
{
public:
	static System::Boolean op_Inequality(EnvelopeImpl *left, EnvelopeImpl* right);

    /// \brief
    /// Constructs a default instance of a EnvelopeImpl object.
    /// 
    /// \remarks
    ///  Ordinate values default to System::Double::NaN.
    /// 
    /// \return
    /// Returns nothing
    /// 
    EnvelopeImpl();

    /// \brief
    /// Constructs a 2D EnvelopeImpl object from X and Y ordinates.
    /// 
    /// \remarks
    ///  Z ordinate value defaults to System::Double::NaN.
    /// 
    /// \param minX 
    /// Input Lower-left X ordinate
    /// \param minY 
    /// Input Lower-left Y ordinate
    /// \param maxX 
    /// Input Upper-right X ordinate
    /// \param maxY 
    /// Input Upper-right Y ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
	EnvelopeImpl(System::Double minX, System::Double minY,
		System::Double maxX, System::Double maxY);

    /// \brief
    /// Constructs a 3D EnvelopeImpl object from X, Y and Z ordinates.
    /// 
    /// \param minX 
    /// Input Lower-left X ordinate
    /// \param minY 
    /// Input Lower-left Y ordinate
    /// \param minZ 
    /// Input Lower-left, rear Z ordinate
    /// \param maxX 
    /// Input Upper-right X ordinate
    /// \param maxY 
    /// Input Upper-right Y ordinate
    /// \param maxZ 
    /// Input Upper-right, front Z ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
	EnvelopeImpl(System::Double minX, System::Double minY, System::Double minZ,
		System::Double maxX, System::Double maxY, System::Double maxZ);

    /// \brief
    /// Constructs a copy of a EnvelopeImpl.
    /// 
    /// \param envelopeImpl 
    /// Input envelope to copy
    /// 
    /// \return
    /// Returns nothing
    /// 
    EnvelopeImpl(EnvelopeImpl* envelopeImpl);

    /// \brief
    /// Constructs a EnvelopeImpl object from two positions.
    /// 
    /// \param lowerLeft 
    /// Input Lower-left position
    /// \param upperRight 
    /// Input Upper-right position
    /// 
    /// \return
    /// Returns nothing
    /// 
	EnvelopeImpl(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* lowerLeft, NAMESPACE_OSGEO_GEOMETRY::IDirectPosition * upperRight);

    /// \brief
    ///  Constructs a EnvelopeImpl object by copying from an array of ordinates.
    /// 
    /// \param dimensionType 
    /// Input dimensionality of the ordinates (see Dimensionality)
    /// \param ordinates 
    /// Input ordinate array representing two positions
    /// 
    /// \return
    /// Returns nothing
    /// 
	EnvelopeImpl(System::Int32 dimensionType, System::Double ordinates __gc[]);

    /// \brief
    /// Equality operator for EnvelopeImpl.
    /// 
    /// \param right 
    /// Input envelope for comparison (right-hand-side of equation)
    /// 
    /// \return
    /// Returns true if all non-NaN ordinates exactly equal.  All NaN values compare as equal to each other.
    /// 
	static System::Boolean op_Equality(EnvelopeImpl *left, EnvelopeImpl* right);

	System::Boolean Equals(System::Object* obj);

	System::Int32 GetHashCode();

    /// \brief
    /// Gets the minimum X coordinate value.
    /// 
    /// \return
    /// Returns the minimum X ordinate; numeric_limits<double>::quiet_NaN() by default
    /// 
	__property System::Double get_MinX();

    /// \brief
    /// Sets the minimum X coordinate value.
    /// 
    /// \param minX 
    /// Input Lower-left X ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_MinX(System::Double minX);

    /// \brief
    /// Gets the minimum Y coordinate value.
    /// 
    /// \return
    /// Returns the minimum Y ordinate; numeric_limits<double>::quiet_NaN() by default
    /// 
    __property System::Double get_MinY();

    /// \brief
    /// Sets the minimum Y coordinate value.
    /// 
    /// \param minY 
    /// Input Lower-left Y ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    __property System::Void set_MinY(System::Double minY);

    /// \brief
    /// Gets the minimum Z coordinate value.
    /// 
    /// \return
    /// Returns the minimum Z ordinate; numeric_limits<double>::quiet_NaN() by default
    /// 
	__property System::Double get_MinZ();

    /// \brief
    /// Sets the minimum Z coordinate value.
    /// 
    /// \param minZ 
    /// Input Lower-left, rear Z ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    __property System::Void set_MinZ(System::Double minZ);

    /// \brief
    /// Gets the maximum X coordinate value.
    /// 
    /// \return
    /// Returns the maximum X ordinate; numeric_limits<double>::quiet_NaN() by default
    /// 
	__property System::Double get_MaxX();

    /// \brief
    /// Sets the maximum X coordinate value.
    /// 
    /// \param maxX 
    /// Input Upper-right X ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    __property System::Void set_MaxX(System::Double maxX);

    /// \brief
    /// Gets the maximum Y coordinate value.
    /// 
    /// \return
    /// Returns the maximum Y ordinate; numeric_limits<double>::quiet_NaN() by default
    /// 
	__property System::Double get_MaxY();
    /// \brief
    /// Sets the maximum Y coordinate value.
    /// 
    /// \param maxY 
    /// Input Upper-right Y ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    __property System::Void set_MaxY(System::Double maxY);

    /// \brief
    /// Gets the maximum Z coordinate value.
    /// 
    /// \return
    /// Returns the maximum Z ordinate; numeric_limits<double>::quiet_NaN() by default
    /// 
	__property System::Double get_MaxZ();

    /// \brief
    /// Sets the maximum Z coordinate value.
    /// 
    /// \param maxZ 
    /// Input Upper-right, front Z ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    __property System::Void set_MaxZ(System::Double maxZ);

    /// \brief
    /// Indicates whether the envelope's extents are set.
    /// 
    /// \return
    /// Returns 'true' if none of the X, Y and Z extents have been set; 'false' otherwise
    /// 
	__property System::Boolean get_IsEmpty();

    /// \brief
    /// Expands the envelope's extents to include a given position.
    /// 
    /// \param position 
    /// Input position to include in extent
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Expand(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition* position);

    /// \brief
    /// Expands the envelope's extents to include another envelope.
    /// 
    /// \param envelope 
    /// Input envelope to include in extent
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Expand(NAMESPACE_OSGEO_GEOMETRY::IEnvelope* envelope);

public:
	EnvelopeImpl(System::IntPtr unmanaged, System::Boolean autoDelete);

protected:
    System::Void ReleaseUnmanagedObject();

public private:
	FdoEnvelopeImpl* GetImpObj();
};

END_NAMESPACE_OSGEO_GEOMETRY


