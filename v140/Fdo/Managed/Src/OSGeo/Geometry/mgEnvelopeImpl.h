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
interface class IEnvelope;
interface class IDirectPosition;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The EnvelopeImpl class is a default implementation of IEnvelope.
/// EnvelopeImpl implements accessors from IEnvelope, matching mutators, and simple data members.
/// Assignment, exact equality operators, and utility methods are also provided.
public ref class EnvelopeImpl : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_GEOMETRY::IEnvelope
{
public:
    /// \brief
    /// Constructs a default instance of a EnvelopeImpl object.
    /// 
    /// \remarks
    /// Ordinate values default to System::Double::NaN.
    /// 
    EnvelopeImpl();

    /// \brief
    /// Constructs a 2D EnvelopeImpl object from X and Y ordinates.
    /// 
    /// \remarks
    /// Z ordinate value defaults to System::Double::NaN.
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
	EnvelopeImpl(System::Double minX, System::Double minY, System::Double minZ,
		System::Double maxX, System::Double maxY, System::Double maxZ);

    /// \brief
    /// Constructs a copy of a EnvelopeImpl.
    /// 
    /// \param envelopeImpl 
    /// Input envelope to copy
    /// 
    EnvelopeImpl(EnvelopeImpl^ envelopeImpl);

    /// \brief
    /// Constructs a EnvelopeImpl object from two positions.
    /// 
    /// \param lowerLeft 
    /// Input Lower-left position
    /// \param upperRight 
    /// Input Upper-right position
    /// 
	EnvelopeImpl(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ lowerLeft, NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ upperRight);

    /// \brief
    /// Constructs a EnvelopeImpl object by copying from an array of ordinates.
    /// 
    /// \param dimensionType 
    /// Input dimensionality of the ordinates (see Dimensionality)
    /// \param ordinates 
    /// Input ordinate array representing two positions
    /// 
	EnvelopeImpl(System::Int32 dimensionType, array<System::Double>^ ordinates);

    /// \brief
    /// Equality function for EnvelopeImpl.
    /// 
    /// \param obj 
    /// Input envelope for comparison (right-hand-side of equation)
    /// 
    /// \return
    /// Returns true if all non-NaN ordinates exactly equal to this instance.  All NaN values compare as equal to each other.
    /// 
	virtual System::Boolean Equals(System::Object^ obj) override;

/// \cond DOXYGEN-IGNORE
	virtual System::Int32 GetHashCode() override;
/// \endcond

    /// \brief
    /// Gets the minimum X coordinate value.
    /// 
    /// \return
    /// Returns the minimum X ordinate; numeric_limits::quiet_NaN() by default
    /// 
    /// \brief
    /// Sets the minimum X coordinate value.
    /// 
    /// \param minX 
    /// Input Lower-left X ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double MinX
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the minimum Y coordinate value.
    /// 
    /// \return
    /// Returns the minimum Y ordinate; numeric_limits::quiet_NaN() by default
    /// 
    /// \brief
    /// Sets the minimum Y coordinate value.
    /// 
    /// \param minY 
    /// Input Lower-left Y ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double MinY
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the minimum Z coordinate value.
    /// 
    /// \return
    /// Returns the minimum Z ordinate; numeric_limits::quiet_NaN() by default
    /// 
    /// \brief
    /// Sets the minimum Z coordinate value.
    /// 
    /// \param minZ 
    /// Input Lower-left, rear Z ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double MinZ
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the maximum X coordinate value.
    /// 
    /// \return
    /// Returns the maximum X ordinate; numeric_limits::quiet_NaN() by default
    /// 
    /// \brief
    /// Sets the maximum X coordinate value.
    /// 
    /// \param maxX 
    /// Input Upper-right X ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double MaxX
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the maximum Y coordinate value.
    /// 
    /// \return
    /// Returns the maximum Y ordinate; numeric_limits::quiet_NaN() by default
    /// 
    /// \brief
    /// Sets the maximum Y coordinate value.
    /// 
    /// \param maxY 
    /// Input Upper-right Y ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double MaxY
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the maximum Z coordinate value.
    /// 
    /// \return
    /// Returns the maximum Z ordinate; numeric_limits::quiet_NaN() by default
    /// 
    /// \brief
    /// Sets the maximum Z coordinate value.
    /// 
    /// \param maxZ 
    /// Input Upper-right, front Z ordinate
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double MaxZ
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Indicates whether the envelope's extents are set.
    /// 
    /// \return
    /// Returns 'true' if none of the X, Y and Z extents have been set; 'false' otherwise
    /// 
    property System::Boolean IsEmpty
    {
        virtual System::Boolean get();
    }

    /// \brief
    /// Expands the envelope's extents to include a given position.
    /// 
    /// \param position 
    /// Input position to include in extent
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Expand(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ position);

    /// \brief
    /// Expands the envelope's extents to include another envelope.
    /// 
    /// \param envelope 
    /// Input envelope to include in extent
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Expand(NAMESPACE_OSGEO_GEOMETRY::IEnvelope^ envelope);

public:
    /// \brief
    /// Constructs an Envelope based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed element should be automatically deleted 
    /// once it no longer referenced.
    /// 
	EnvelopeImpl(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	FdoEnvelopeImpl* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
/// \endcond
};

END_NAMESPACE_OSGEO_GEOMETRY


