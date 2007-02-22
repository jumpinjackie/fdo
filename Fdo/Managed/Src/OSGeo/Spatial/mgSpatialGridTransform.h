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

class FdoSpatialGridTransform;

BEGIN_NAMESPACE_OSGEO_SPATIAL

/// \ingroup (OSGeoFDOSpatial)
/// \brief
/// Class to handle transformations of ordinates between double precision
/// and integers.  The integer format models a fine grid.
public __gc class SpatialGridTransform: public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Factory method for GridTransformer.
    /// 
    /// \remarks
    /// This models a 2D coordinate system on a grid, using 64-bit integers as ordinates.
    /// Each unit of integer is equivalent to one unit of resolution in floating point
    /// numbers.  The unit of resolution is taken to be twice the given tolerance.
    /// 
    /// \param extents 
    /// Input Extents of floating-point XY plane.
    /// \param tolerance 
    /// Tolerance of ordinate values.
    /// 
	SpatialGridTransform(NAMESPACE_OSGEO_GEOMETRY::IEnvelope* extents, System::Double tolerance);

    /// \brief
    /// Transform floating-point ordinates to integer ordinates on the grid.
    /// 
    /// \param coordinateX 
    /// Input Floating-point X ordinate.
    /// \param coordinateY 
    /// Input Floating-point Y ordinate.
    /// \param gridX 
    /// Output Integer X ordinate.
    /// \param gridY 
    /// Output Integer Y ordinate.
    /// 
	System::Void TransformToGrid(System::Double coordinateX, System::Double coordinateY, System::Int64* gridX, System::Int64* gridY);

    /// \brief
    /// Transform integer ordinates on the grid to floating-point ordinates.
    /// 
    /// \param gridX 
    /// Input Integer X ordinate.
    /// \param gridY 
    /// Input Integer Y ordinate.
    /// \param coordinateX 
    /// Output Floating-point X ordinate.
    /// \param coordinateY 
    /// Output Floating-point Y ordinate.
    /// 
	System::Void TransformFromGrid(System::Int64 gridX, System::Int64 gridY, System::Double* coordinateX, System::Double* coordinateY);

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public:
    /// \brief
    /// Constructs an SpatialGridTransform managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	SpatialGridTransform(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoSpatialGridTransform* GetImpObj();
};

END_NAMESPACE_OSGEO_SPATIAL


