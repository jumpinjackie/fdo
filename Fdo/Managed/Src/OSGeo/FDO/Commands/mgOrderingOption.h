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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS

/// \brief
/// The FdoOrderingOption enumeration defines the list acceptable values that 
/// can be specified as order by clause to the select and select aggregate commands.
/// <param name="FdoOrderingOption_Ascending">
/// Used for ascending option
/// </param>
/// <param name="FdoOrderingOption_Descending">
/// Used for descending option
/// </param>
public __value enum OrderingOption{
    /// Used for ascending option.
	OrderingOption_Ascending = FdoOrderingOption_Ascending,

    /// Used for descending option.
	OrderingOption_Descending = FdoOrderingOption_Descending
};

END_NAMESPACE_OSGEO_FDO_COMMANDS


