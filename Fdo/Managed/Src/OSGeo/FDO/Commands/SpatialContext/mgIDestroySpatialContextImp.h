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
#include "FDO\Commands\SpatialContext\mgIDestroySpatialContext.h"

class FdoIDestroySpatialContext;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT

/// \ingroup (OSGeoFDOCommandsSpatialContext)
/// \brief
/// The IDestroySpatialContextImp class is a concrete implementation of IDestroySpatialContext.
/// The IDestroySpatialContext interface defines the DestroySpatialContext
/// command, which destroys an existing spatial context. Input to the command is
/// the name of the context to destroy. Destroying a spatial context destroys
/// all data stored in that context.
private __gc class IDestroySpatialContextImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                               public NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IDestroySpatialContext
{
public:
    /// \brief
    /// Gets the name of the context to destroy as a string.
    /// 
    /// \return
    /// Returns the name of the spatial context to be deleted
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Sets the name of the context to destroy as a string.
    /// 
    /// \param value 
    /// Input the name of the spatial context to be deleted
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Executes the DestroySpatialContext command, removing all data stored
    /// in that context. An exception is thrown if the spatial context does not exist.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();

public private:
	IDestroySpatialContextImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDestroySpatialContext* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT


