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
#include "FDO\Commands\SpatialContext\mgIGetSpatialContexts.h"

class FdoIGetSpatialContexts;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT
public __gc __interface ISpatialContextReader;

/// \brief
/// The FdoIGetSpatialContexts interface defines the GetSpatialContexts command,
/// which enumerates the existing spatial contexts.
private __gc class IGetSpatialContextsImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IGetSpatialContexts
{
public:
    /// \brief
    ///  Gets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.
    /// 
    /// \return
    /// Returns Boolean value
    /// 
	__property System::Boolean get_ActiveOnly();

    /// \brief
    ///  Sets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.
    /// 
    /// \param value 
    /// Input the Boolean flag
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ActiveOnly(System::Boolean value);

    /// \brief
    /// Executes the GetSpatialContexts command returning an FdoISpatialContextReader.
    /// 
    /// \return
    /// Returns nothing
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReader* Execute();

public private:
	IGetSpatialContextsImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetSpatialContexts* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT


