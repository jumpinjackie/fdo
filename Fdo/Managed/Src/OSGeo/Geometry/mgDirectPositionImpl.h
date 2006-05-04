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

/// \brief
/// The DirectPositionImpl class is a default implementation of IDirectPosition.
/// DirectPositionImpl implements accessors from IDirectPosition, matching mutators, and simple data members.
/// Assignment and exact equality operators are also provided.
public __gc class DirectPositionImpl : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_GEOMETRY::IDirectPosition
{
public:
	static System::Boolean op_Inequality(DirectPositionImpl *left, DirectPositionImpl *right);

    /// \brief
    /// Constructs a default instance of a DirectPositionImpl object.
    /// 
    /// \remarks
    ///  Ordinate values default to System::Double::NaN.
    /// Dimensionality defaults to Dimensionality_XY.
    /// 
    /// \return
    /// Returns nothing
    /// 
	DirectPositionImpl();

    /// \brief
    /// Constructs a 2D DirectPositionImpl object from X and Y ordinates.
    /// 
    /// \remarks
    ///  Z and M ordinate values default to System::Double::NaN.
    /// Dimensionality is set to Dimensionality_XY.
    /// 
    /// \param x 
    /// Input X ordinate value
    /// \param y 
    /// Input Y ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
	DirectPositionImpl(System::Double coordinateX, System::Double coordinateY);

    /// \brief
    /// Constructs a 3D DirectPositionImpl object from X, Y and Z ordinates.
    /// 
    /// \remarks
    ///  M ordinate value defaults to System::Double::NaN.
    /// Dimensionality is set to Dimensionality_XYZ.
    /// 
    /// \param x 
    /// Input X ordinate value
    /// \param y 
    /// Input Y ordinate value
    /// \param z 
    /// Input Z ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
	DirectPositionImpl(System::Double coordinateX, System::Double coordinateY, System::Double coordinateZ);

    /// \brief
    /// Constructs a 4D DirectPositionImpl object from X, Y, Z and M ordinates.
    /// 
    /// \remarks
    ///  Dimensionality is set to Dimensionality_XYZ|Dimensionality_M.
    /// 
    /// \param x 
    /// Input X ordinate value
    /// \param y 
    /// Input Y ordinate value
    /// \param z 
    /// Input Z ordinate value
    /// \param m 
    /// Input M ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
	DirectPositionImpl(System::Double coordinateX, System::Double coordinateY, System::Double coordinateZ, System::Double coordinateM);

    /// \brief
    /// Constructs a copy of a DirectPositionImpl
    /// 
    /// \param position 
    /// Input position to copy
    /// 
    /// \return
    /// Returns nothing
    /// 
	DirectPositionImpl(DirectPositionImpl *position);

    /// \brief
    /// Constructs a copy of a DirectPositionImpl
    /// 
    /// \remarks
    /// Copies position data using only public methods of IDirectPosition,
    /// thus supporting conversion from any implementation of the interface.
    /// 
    /// \param pos 
    /// Input position to copy
    /// 
    /// \return
    /// Returns nothing
    /// 
	DirectPositionImpl(NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *position);

    /// \brief
    /// Assignment operator for DirectPositionImpl.
    /// 
    /// \param position 
    /// Input position to copy
    /// 
    /// \return
    /// Returns a position
    /// 
	static System::Void op_Assign(DirectPositionImpl *left, DirectPositionImpl *right);

    /// \brief
    /// Assignment operator for DirectPositionImpl from a IDirectPosition, using public methods.
    /// 
    /// \param position 
    /// Input position to copy
    /// 
    /// \return
    /// Returns a position
    /// 
	static System::Void op_Assign(DirectPositionImpl *left, IDirectPosition *right);

	System::Boolean Equals(System::Object* obj);

	System::Int32 GetHashCode();

    /// \brief
    /// Equality operator for DirectPositionImpl.
    /// 
    /// \param right 
    /// Input position for comparison (right-hand-side of equation)
    /// 
    /// \return
    /// Returns true if all ordinates and dimensionality are exactly equal.
    /// 
	static System::Boolean op_Equality(DirectPositionImpl *left, DirectPositionImpl *right);

    /// \brief
    /// Gets the X ordinate.
    /// 
    /// \return
    /// Returns the X ordinate; default is numeric_limits<double>::quiet_NaN()
    /// 
	__property System::Double get_X();

    /// \brief
    /// Sets the X ordinate.
    /// 
    /// \param x 
    /// Input X ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_X(System::Double value);

    /// \brief
    /// Gets the Y ordinate.
    /// 
    /// \return
    /// Returns the Y ordinate; default is numeric_limits<double>::quiet_NaN()
    /// 
	__property System::Double get_Y();

    /// \brief
    /// Sets the Y ordinate.
    /// 
    /// \param y 
    /// Input Y ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Y(System::Double value);

    /// \brief
    /// Gets the Z ordinate.
    /// 
    /// \return
    /// Returns the Z ordinate; default is numeric_limits<double>::quiet_NaN()
    /// 
	__property System::Double get_Z();

    /// \brief
    /// Sets the Z ordinate.
    /// 
    /// \param z 
    /// Input Z ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Z(System::Double value);

    /// \brief
    /// Gets the M ordinate.
    /// 
    /// \return
    /// Returns the M ordinate; default is numeric_limits<double>::quiet_NaN()
    /// 
	__property System::Double get_M();

    /// \brief
    /// Sets the M ordinate.
    /// 
    /// \param m 
    /// Input M ordinate value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_M(System::Double value);

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
	__property System::Int32 get_Dimensionality();

    /// \brief
    /// Sets the dimensionality.
    /// 
    /// \remarks
    ///  Use values (including bitwise combinations) from the Dimensionality enumeration.
    /// 
    /// \param dimensionality 
    /// Input dimensionality value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Dimensionality(System::Int32 value);

public:
	DirectPositionImpl(System::IntPtr unmanaged, System::Boolean autoDelete);

protected:
    System::Void Dispose(System::Boolean disposing);

public private:
	FdoDirectPositionImpl *GetImpObj();
};
END_NAMESPACE_OSGEO_GEOMETRY


