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

#include "mgIDirectPosition.h"

class FdoIDirectPosition;

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \brief
/// The IDirectPositionImp class is a default implementation of the IDirectPosition object.  
/// IDirectPosition is a simple coordinate position in any allowed dimension.  
/// The actual object may have several implementations, depending on the dimension and other factors.
/// This is a lightweight helper type for Geometries in the Geometry package.
/// Note: It does not derive from IGeometry.
private __gc class IDirectPositionImp 
	: public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_GEOMETRY::IDirectPosition
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
	IDirectPositionImp(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public:

    /// \brief
    /// Gets the X ordinate.
    /// 
    /// \return
    /// Returns the X ordinate; default is numeric_limits::quiet_NaN()
    /// 
	__property System::Double get_X();

    /// \brief
    /// Gets the Y ordinate.
    /// 
    /// \return
    /// Returns the Y ordinate; default is numeric_limits::quiet_NaN()
    /// 
    __property System::Double get_Y();

    /// \brief
    /// Gets the Z ordinate.
    /// 
    /// \return
    /// Returns the Z ordinate; default is numeric_limits::quiet_NaN()
    /// 
    __property System::Double get_Z();

    /// \brief
    /// Gets the M ordinate.
    /// 
    /// \return
    /// Returns the M ordinate; default is numeric_limits::quiet_NaN()
    /// 
    __property System::Double get_M();

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

public private:
	FdoIDirectPosition *GetImpObj();
};

END_NAMESPACE_OSGEO_GEOMETRY


