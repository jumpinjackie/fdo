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

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \ingroup (enums)
/// \brief
/// The OrderType enumeration list contains two values to indicate if the
/// object collection is to be ordered in ascending or descending fashion.
public __value enum OrderType
{
    /// Specifies the collection is to be ordered from the smallest value to the largest value.
	OrderType_Ascending = FdoOrderType_Ascending,

    /// Specifies the collection is to be ordered from the largest value to the smallest value.
	OrderType_Descending = FdoOrderType_Descending
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


