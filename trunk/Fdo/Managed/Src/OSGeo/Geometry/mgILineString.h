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
#include "mgICurveAbstract.h"

BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc __interface IDirectPosition;
public __gc class DirectPositionCollection;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \brief
/// The ILineString class is a LineString Geometry type.  
/// The shape of ILineString is the set of positions defined by the contained collection, 
/// plus linear interpolation between consecutive points.
public __gc __interface ILineString : public NAMESPACE_OSGEO_GEOMETRY::ICurveAbstract
{
public:
    /// \brief
    /// Gets the number of positions in this object.
    /// 
    /// \return
    /// Returns the number of positions
    /// 
	__property System::Int32 get_Count();
	
    /// \brief
    /// Gets the position at the specified (zero-based) index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of positions.
    /// 
    /// \return
    /// Returns the position
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *get_Item(System::Int32 index);

    /// \brief
    /// Gets the position at the specified (zero-based) index, by values of its member data.
    /// This is in aid of higher performance for any implementation that
    /// does not internally use FdoIDirectPosition objects for storage, or for
    /// an application seeking to avoid overhead of accessor methods.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of positions.
    /// \param x 
    /// Output X ordinate value.
    /// \param y 
    /// Output Y ordinate value.
    /// \param z 
    /// Output Z ordinate value.
    /// \param m 
    /// Output M ordinate value.
    /// \param dimensionality 
    /// Output Dimensionality of ordinates in this position.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void GetItemByMembers(
		System::Int32 index, 
		System::Double &coordinateX, 
		System::Double &coordinateY, 
		System::Double &coordinateZ, 
		System::Double &coordinateM, 
		System::Int32 &dimensionality) = 0;
	
    /// \brief
    /// Gets a collection of all of the positions in this object.
    /// 
    /// \return
    /// Returns the positions
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection *get_Positions();
};

END_NAMESPACE_OSGEO_GEOMETRY


