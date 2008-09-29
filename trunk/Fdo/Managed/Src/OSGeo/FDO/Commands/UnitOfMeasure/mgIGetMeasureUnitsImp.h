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
#include "FDO\Commands\UnitOfMeasure\mgIGetMeasureUnits.h"

class FdoIGetMeasureUnits;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE
public __gc __interface IMeasureUnitReader;

/// \ingroup (OSGeoFDOCommandsUnitOfMeasure)
/// \brief
/// The IGetMeasureUnitsImp class is a concrete implementation of IGetMeasureUnits.
/// The IGetMeasureUnits interface defines the GetMeasureUnits command, which
/// enumerates the existing measurement units.
private __gc class IGetMeasureUnitsImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                         public NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IGetMeasureUnits
{
public:
    /// \brief
    /// Executes the GetMeasureUnits command and returns an IMeasureUnitReader.
    /// 
    /// \return
    /// Returns the measure unit reader
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReader* Execute();

public:
    /// \brief
    /// Constructs an IGetMeasureUnitsImp object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	IGetMeasureUnitsImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetMeasureUnits* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE


