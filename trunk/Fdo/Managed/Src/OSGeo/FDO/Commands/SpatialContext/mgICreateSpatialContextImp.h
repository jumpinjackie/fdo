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

#include "FDO\Commands\mgICommandImp.h"
#include "FDO\Commands\SpatialContext\mgICreateSpatialContext.h"
#include "FDO\Commands\SpatialContext\mgSpatialContextExtentType.h"

class FdoICreateSpatialContext;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT

/// \ingroup (OSGeoFDOCommandsSpatialContext)
/// \brief
/// The ICreateSpatialContextImp class is a concrete implementation of ICreateSpatialContext.
/// The ICreateSpatialContext interface defines the CreateSpatialContext
/// command, which creates a new spatial context. Input to the command includes
/// the name, description, coordinate system, extent type, and extent for the
/// new context. Command execution will fail if the context already exists, or
/// if the coordinate system or extent type is not supported by the provider.
/// <p><b>Note:</b> The geographic extent of a coordinate system should generally be
/// defined as an envelope (or bounding box). However, any geometry type that
/// falls into the surface geometric types can be used (e.g., polygons). If a
/// non-rectangular area is passed in and the provider supports only a
/// rectangular extent, the provider will compute the minimum box that
/// encloses the given geometry and use that for the extent value.
private ref class ICreateSpatialContextImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                              public NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContext
{
public:
    /// \brief
    /// Gets the name of the context to create as a string.
    /// 
    /// \return
    /// Returns the name of the spatial context
    /// 
    /// \brief
    /// Sets the name of the context to create as a string.
    /// 
    /// \param value 
    /// Input the name of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::String^ Name
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the description of the context to create as a string.
    /// 
    /// \return
    /// Returns the description of the spatial context
    /// 
    /// \brief
    /// Sets the description of the context to create as a string.
    /// 
    /// \param value 
    /// Input the description of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::String^ Description
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the coordinate system of the context to create as a string
    /// in OpenGIS SRS WKT format.
    /// 
    /// \return
    /// Returns the coordinate system
    /// 
    /// \brief
    /// Sets the coordinate system of the context to create as a string in
    /// OpenGIS SRS WKT format.
    /// 
    /// \param value 
    /// Input the coordinate system
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::String^ CoordinateSystem
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the coordinate system definition in OGC SRS WKT format for the 
    /// spatial context currently being created. If the result is an empty string, 
    /// the coordinate system WKT description has not been set.
    /// 
    /// \return
    /// Returns the coordinate system description in WKT of of the spatial context.
    /// 
    /// \brief
    /// Sets the coordinate system definition in OGC SRS WKT 
    /// format for the spatial context currently being created.
    /// 
    /// \param value 
    /// Input the coordinate system WKT
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::String^ CoordinateSystemWkt
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the desired extent type of the context to create, either static or dynamic.
    /// 
    /// \return
    /// Returns the extent type
    /// 
    /// \brief
    /// Sets the desired extent type of the context to create, either static or dynamic.
    /// 
    /// \param value 
    /// Input the extent type
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType ExtentType
    {
        NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType get();
        System::Void set(NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::SpatialContextExtentType value);
    }

    /// \brief
    /// Gets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic, any value specified via this function is ignored.
    /// 
    /// \return
    /// Returns the extent of the spatial context
    /// 
    /// \brief
    /// Sets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic this is optional and any value specified is ignored.
    /// 
    /// \param buffer 
    /// Input the extent of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property array<System::Byte>^ Extent
    {
        array<System::Byte>^ get();
        System::Void set(array<System::Byte>^ value);
    }

    /// \brief
    /// Gets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
    /// \brief
    /// Sets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \param value 
    /// Input the tolerance
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::Double XYTolerance
    {
        System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \return
    /// Returns the tolerance
    /// 
    /// \brief
    /// Sets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.
    /// 
    /// \param value 
    /// Input the tolerance
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::Double ZTolerance
    {
        System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Gets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists, command execution
    /// will fail.
    /// 
    /// \return
    /// Returns true if command should update existing contexts
    /// 
    /// \brief
    /// Sets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists, command execution
    /// will fail.
    /// 
    /// \param value 
    /// Input true if existing contexts should be updated
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::Boolean UpdateExisting
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    /// Executes the CreateSpatialContext command. An exception is thrown if
    /// the spatial context already exists, or if the coordinate system or
    /// extent type is not supported by the provider.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Execute();

internal:
	ICreateSpatialContextImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoICreateSpatialContext* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT


