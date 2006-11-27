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
#include "FDO\Commands\UnitOfMeasure\mgIDestroyMeasureUnit.h"

class FdoIDestroyMeasureUnit;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE

/// \brief
/// The FdoIDestroyMeasureUnit interface defines the DestroyMeasureUnit command,
/// which deletes a measurement unit. Input to the command is simply the
/// abbreviation of the measure unit to be destroyed.
private __gc class IDestroyMeasureUnitImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, public NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IDestroyMeasureUnit
{
public:
    /// \brief
    /// Gets the abbreviation of the measure unit to destroy as a string
    /// 
    /// \return
    /// Returns the abbreviation of the measure unit to be deleted.
    /// 
	__property System::String* get_Abbreviation();

    /// \brief
    /// Sets the abbreviation of the measure unit to destroy as a string
    /// 
    /// \param value 
    /// Input the abbreviation of the measure unit to be deleted.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Abbreviation(System::String* value);

    /// \brief
    /// Executes the DestroyMeasureUnit command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();

public private:
	IDestroyMeasureUnitImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDestroyMeasureUnit* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE


