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
#include "mgIRingAbstractImp.h"
#include "mgILinearRing.h"

class FdoILinearRing;

BEGIN_NAMESPACE_OSGEO_GEOMETRY
interface class IDirectPosition;
ref class DirectPositionCollection;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \ingroup (OSGeoFDOGeometry)
/// \brief
/// The ILinearRingImp class is a linear ring helper type.
/// The shape of ILinearRingImp is the set of positions defined by the contained collection, 
/// plus linear interpolation between consecutive points.
public ref class ILinearRingImp : 
    public NAMESPACE_OSGEO_GEOMETRY::IRingAbstractImp, public NAMESPACE_OSGEO_GEOMETRY::ILinearRing
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
	ILinearRingImp(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	FdoILinearRing* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

public:
    /// \brief
    /// Gets the number of positions in this object.
    /// 
    /// \return
    /// Returns the number of positions
    /// 
    property System::Int32 Count
    {
        virtual System::Int32 get();
    }
	
    /// \brief
    /// Gets the position at the specified (zero-based) index.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of positions.
    /// 
    /// \return
    /// Returns the position
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ default[System::Int32]
    {
        virtual NAMESPACE_OSGEO_GEOMETRY::IDirectPosition^ get(System::Int32 index);
    }
	
    /// \brief
    /// Gets the position at the specified (zero-based) index, by values of its member data.
    /// This is in aid of higher performance for any implementation that
    /// does not internally use FdoIDirectPosition objects for storage, or for
    /// an application seeking to avoid overhead of accessor methods.
    /// 
    /// \param index 
    /// Input Zero-based index in the object's list of positions.
    /// \param coordinateX 
    /// Output X ordinate value.
    /// \param coordinateY 
    /// Output Y ordinate value.
    /// \param coordinateZ 
    /// Output Z ordinate value.
    /// \param coordinateM 
    /// Output M ordinate value.
    /// \param dimensionality 
    /// Output Dimensionality of ordinates in this position.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void GetItemByMembers(
		System::Int32 index, 
		System::Double% coordinateX, 
		System::Double% coordinateY,
		System::Double% coordinateZ,
		System::Double% coordinateM,
		System::Int32% dimensionality);
		
    /// \brief
    /// Gets a collection of all of the positions in this object.
    /// 
    /// \return
    /// Returns the positions
    /// 
    property NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ Positions
    {
        virtual NAMESPACE_OSGEO_GEOMETRY::DirectPositionCollection^ get();
    }
};

END_NAMESPACE_OSGEO_GEOMETRY


