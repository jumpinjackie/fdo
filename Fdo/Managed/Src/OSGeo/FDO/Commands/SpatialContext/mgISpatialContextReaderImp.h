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

#include "FDO\Commands\SpatialContext\mgISpatialContextReader.h"
#include "FDO\Commands\SpatialContext\mgSpatialContextExtentType.h"

enum SpatialContextExtentType;
class ISpatialContextReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT

/// \brief
/// The FdoISpatialContextReader interface provides forward-only, read-only
/// functionality for enumerating spatial contexts. A reference to an
/// FdoISpatialContextReader is returned from the GetSpatialContexts command.
/// The initial position of the FdoISpatialContextReader interface is prior to the
/// first item. Thus, you must call ReadNext to begin accessing any data.
private __gc class ISpatialContextReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReader
{
public:
    /// \brief
    /// Gets the name of the spatial context currently being read.
    /// 
    /// \return
    /// Returns the name of the spatial context.
    /// 
	System::String* GetName();

    /// \brief
    /// Gets the description of the spatial context currently being read.
    /// 
    /// \return
    /// Returns the description of the spatial context.
    /// 
	System::String* GetDescription();

    /// \brief
    /// Gets the name of the coordinate system of the spatial context currently
    /// being read. If the result is a null or empty string, the coordinate system is
    /// unknown to the spatial context.
    /// 
    /// \return
    /// Returns the coordinate system name of the spatial context.
    /// 
	System::String* GetCoordinateSystem();

    /// \brief
    /// Gets the name of the coordinate system in OpenGIS SRS WKT format of the spatial context currently
    /// being read. If the result is an empty string, the coordinate system WKT description is
    /// not available.
    /// 
    /// \return
    /// Returns the coordinate system description in WKT of of the spatial context.
    /// 
	System::String* GetCoordinateSystemWkt();

    /// \brief
    /// Gets the extent type of the spatial context currently being read.
    /// 
    /// \return
    /// Returns the extent type.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType GetExtentType();

    /// \brief
    /// Gets the extent of the spatial context currently being read as a byte
    /// array in FGF format.
    /// 
    /// \param length 
    /// Output the length of the returned byte array.
    /// 
    /// \return
    /// Returns the extent as a byte array in FGF format.
    /// 
	System::Byte GetExtent() [];

    /// \brief
    /// Gets the tolerance value for XY ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs frequently in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
	System::Double GetXYTolerance();

    /// \brief
    /// Gets the tolerance value for Z ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs a frequently in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
	System::Double GetZTolerance();

    /// \brief
    /// Returns true if the spatial context currently being read is the active
    /// spatial context.
    /// 
    /// \return
    /// Returns true if the current spatial context is the active one.
    /// 
	System::Boolean IsActive();

    /// \brief
    /// Advances the reader to the next item. The default position of the reader
    /// is prior to the first item. Thus, you must call ReadNext to begin
    /// accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	System::Boolean ReadNext();

protected:
	__sealed System::Void ReleaseUnmanagedObject();

public private:
	ISpatialContextReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoISpatialContextReader* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT


