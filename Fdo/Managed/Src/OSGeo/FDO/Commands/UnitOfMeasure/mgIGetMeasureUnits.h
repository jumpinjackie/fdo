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

#include "FDO\Commands\mgICommand.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE
public __gc __interface IMeasureUnitReader;

/// \ingroup (OSGeoFDOCommandsUnitOfMeasure)
/// \interface OSGeo::FDO::Commands::UnitOfMeasure::IGetMeasureUnits
/// \brief
/// The IGetMeasureUnits interface defines the GetMeasureUnits command, which
/// enumerates the existing measurement units.
public __gc __interface IGetMeasureUnits : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Executes the GetMeasureUnits command and returns an IMeasureUnitReader.
    /// 
    /// \return
    /// Returns the measure unit reader
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReader* Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE


