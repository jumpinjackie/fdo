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

#include "FDO\Commands\UnitOfMeasure\mgBaseUnit.h"
#include "FDO\Commands\UnitOfMeasure\mgIMeasureUnitReader.h"

class FdoIMeasureUnitReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE

/// \ingroup (OSGeoFDOCommandsUnitOfMeasure)
/// \brief
/// The IMeasureUnitReaderImp class is a concrete implementation of IMeasureUnitReader.
/// The IMeasureUnitReader interface provides forward-only, read-only
/// functionality for enumerating measurement units.  A reference to an
/// IMeasureUnitReader interface is returned from the GetMeasureUnits command.
/// The initial position of the IMeasureUnitReader is prior to the
/// first item. Thus, you must call ReadNext to begin accessing any data.
private __gc class IMeasureUnitReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                           public NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReader
{
public:
    /// \brief
    /// Gets the abbreviation of the measure unit currently being read.
    /// 
    /// \return
    /// Returns the abbreviation of the measure unit
    /// 
	System::String* GetAbbreviation();

    /// \brief
    /// Gets the name of the measure unit currently being read.
    /// 
    /// \return
    /// Returns the name of the measure unit
    /// 
	System::String* GetName();

    /// \brief
    /// Gets the description of the measure unit currently being read.
    /// 
    /// \return
    /// Returns the description of the measure unit
    /// 
	System::String* GetDescription();

    /// \brief
    /// Gets the base unit of the measure unit currently being read.
    /// 
    /// \return
    /// Returns the base unit
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::BaseUnit GetBaseUnit();

    /// \brief
    /// Gets the scale factor of the measure unit currently being read.
    /// 
    /// \return
    /// Returns the scale factor of the measure unit
    /// 
	System::Double GetScaleFactor();

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item
    /// 
	System::Boolean ReadNext();

    /// \brief
    /// Closes the IMeasureUnitReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Close();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	IMeasureUnitReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoIMeasureUnitReader* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE


