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

#include "mgIDirectPositionImp.h"

class FdoDirectPositionImpl;

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The DirectPositionImpl class is a default implementation of IDirectPosition.
/// DirectPositionImpl implements accessors from IDirectPosition, matching mutators, and simple data members.
/// Assignment and exact equality operators are also provided.
public ref class DirectPositionImpl : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_GEOMETRY::IDirectPosition
{
public:
    /// \brief
    /// Constructs a default instance of a DirectPositionImpl object.
    /// 
    /// \remarks
    /// Ordinate values default to System::Double::NaN.
    /// Dimensionality defaults to Dimensionality_XY.
    /// 
	DirectPositionImpl();

    /// \brief
    /// Constructs a 2D DirectPositionImpl object from X and Y ordinates.
    /// 
    /// \remarks
    /// Z and M ordinate values default to System::Double::NaN.
    /// Dimensionality is set to Dimensionality_XY.
    /// 
    /// \param coordinateX 
    /// Input X ordinate value
    /// \param coordinateY 
    /// Input Y ordinate value
    /// 
	DirectPositionImpl(System::Double coordinateX, System::Double coordinateY);

    /// \brief
    /// Constructs a 3D DirectPositionImpl object from X, Y and Z ordinates.
    /// 
    /// \remarks
    /// M ordinate value defaults to System::Double::NaN.
    /// Dimensionality is set to Dimensionality_XYZ.
    /// 
    /// \param coordinateX 
    /// Input X ordinate value
    /// \param coordinateY 
    /// Input Y ordinate value
    /// \param coordinateZ 
    /// Input Z ordinate value
    /// 
	DirectPositionImpl(System::Double coordinateX, System::Double coordinateY, System::Double coordinateZ);

    /// \brief
    /// Constructs a 4D DirectPositionImpl object from X, Y, Z and M ordinates.
    /// 
    /// \remarks
    ///  Dimensionality is set to Dimensionality_XYZ|Dimensionality_M.
    /// 
    /// \param coordinateX 
    /// Input X ordinate value
    /// \param coordinateY 
    /// Input Y ordinate value
    /// \param coordinateZ 
    /// Input Z ordinate value
    /// \param coordinateM 
    /// Input M ordinate value
    /// 
	DirectPositionImpl(System::Double coordinateX, System::Double coordinateY, System::Double coordinateZ, System::Double coordinateM);

    /// \brief
    /// Constructs a copy of a DirectPositionImpl
    /// 
    /// \param position 
    /// Input position to copy
    /// 
	DirectPositionImpl(DirectPositionImpl^ position);

    /// \brief
    /// Constructs a copy of a DirectPositionImpl
    /// 
    /// \remarks
    /// Copies position data using only public methods of IDirectPosition,
    /// thus supporting conversion from any implementation of the interface.
    /// 
    /// \param position 
    /// Input position to copy
    /// 
	DirectPositionImpl(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ position);

    /// \brief
    /// Assignment function for DirectPositionImpl from a IDirectPosition, using public methods.
    /// 
    /// \param left 
    /// Input Source position to copy
    /// 
    /// \param right 
    /// Input Destination position
    /// 
	DirectPositionImpl^ operator=(IDirectPosition^ right);

    /// \brief
    /// Equality function for FdoDirectPositionImpl.
    /// 
    /// \param obj 
    /// Input position for comparison (right-hand-side of equation)
    /// 
    /// \return
    /// Returns true if all ordinates and dimensionality are exactly equal.
    /// 
	virtual System::Boolean Equals(System::Object^ obj) override;

/// \cond DOXYGEN-IGNORE
	virtual System::Int32 GetHashCode() override;
/// \endcond

    /// \brief
    /// Gets the X ordinate.
    /// 
    /// \return
    /// Returns the X ordinate; default is numeric_limits::quiet_NaN()
    /// 
    /// \brief
    /// Sets the X ordinate.
    /// 
    /// \param value 
    /// Input X ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double X
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the Y ordinate.
    /// 
    /// \return
    /// Returns the Y ordinate; default is numeric_limits::quiet_NaN()
    /// 
    /// \brief
    /// Sets the Y ordinate.
    /// 
    /// \param value 
    /// Input Y ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double Y
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the Z ordinate.
    /// 
    /// \return
    /// Returns the Z ordinate; default is numeric_limits::quiet_NaN()
    /// 
    /// \brief
    /// Sets the Z ordinate.
    /// 
    /// \param value 
    /// Input Z ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double Z
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the M ordinate.
    /// 
    /// \return
    /// Returns the M ordinate; default is numeric_limits::quiet_NaN()
    /// 
    /// \brief
    /// Sets the M ordinate.
    /// 
    /// \param value 
    /// Input M ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double M
    {
        virtual System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the dimensionality of ordinates in this position.
    /// 
    /// \remarks
    /// Values are from the Dimensionality enumeration.
    /// A return type of "Int32" is used instead of the enumeration, catering to typical use with bit masking.
    /// 
    /// \return
    /// Returns the ordinate dimensionality
    /// 
    /// \brief
    /// Sets the dimensionality.
    /// 
    /// \remarks
    /// Use values (including bitwise combinations) from the Dimensionality enumeration.
    /// 
    /// \param value 
    /// Input dimensionality value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Int32 Dimensionality
    {
        virtual System::Int32 get();
        System::Void set(System::Int32 value);
    }

public:
    /// \brief
    /// Constructs an Position based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	DirectPositionImpl(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	FdoDirectPositionImpl* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
/// \endcond
};
END_NAMESPACE_OSGEO_GEOMETRY


