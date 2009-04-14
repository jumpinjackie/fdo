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

#pragma  once

#include <Fdo\Commands\UnitOfMeasure\BaseUnit.h>

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE
/// \endcond

/// \ingroup (OSGeoFDOCommandsUnitOfMeasure)
/// \brief
/// The BaseUnit enumeration defines the well known base unit used to
/// establish a measure unit.
public __value enum BaseUnit
{
    /// Represents an unspecified base unit, it can be used to create an
    /// unknown or unspecified unit of measure.
	BaseUnit_None = FdoBaseUnit_None,

    /// The base unit for linear, area, and volume measurement units
    /// represented by meters, meters squared, or meters cubed respectively.
	BaseUnit_Meters = FdoBaseUnit_Meters,

    /// The base unit for the angular measurement units represented by radians.
	BaseUnit_Radians = FdoBaseUnit_Radians
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE
/// \endcond
