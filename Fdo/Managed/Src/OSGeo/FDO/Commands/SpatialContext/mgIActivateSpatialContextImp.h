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
#include "FDO\Commands\SpatialContext\mgIActivateSpatialContext.h"

class FdoIActivateSpatialContext;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT

/// \brief
/// The IActivateSpatialContextImp class is a concrete implementation of IActivateSpatialContext.
/// The IActivateSpatialContext interface defines the ActivateSpatialContext
/// command, which activates a specified spatial context. Input to the command
/// is the name of the context to activate.
private __gc class IActivateSpatialContextImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                                public NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IActivateSpatialContext
{
public:
    /// \brief
    /// Gets the name of the context to activate as a string.
    /// 
    /// \return
    /// Returns the name of the spatial context
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Sets the name of the context to activate as a string.
    /// 
    /// \param value 
    /// Input the name of the spatial context
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Executes the ActivateSpatialContext command. An exception is thrown
    /// if the spatial context does not exist.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();

public private:
	IActivateSpatialContextImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIActivateSpatialContext* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT
