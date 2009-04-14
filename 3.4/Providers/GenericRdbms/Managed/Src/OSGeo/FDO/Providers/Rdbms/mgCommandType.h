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

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS

/// <summary>The CommandType enumeration defines the list of Rdbms commands.</summary>
/// <param name="CommandType_CreateSpatialIndex">Represents the CreateSpatialIndex command.</param>
/// <param name="CommandType_DestroySpatialIndex">Represents the DestroySpatialIndex command.</param>
/// <param name="CommandType_GetSpatialIndexes">Represents the GetSpatialIndices command.</param>

public __value enum CommandType
{
    // Represents the CreateSpatialIndex command.
    CommandType_CreateSpatialIndex =  FdoRdbmsCommandType_CreateSpatialIndex,

    // Represents the DestroySpatialIndex command.
    CommandType_DestroySpatialIndex = FdoRdbmsCommandType_DestroySpatialIndex,

    // Represents the GetSpatialIndices command.
    CommandType_GetSpatialIndexes = FdoRdbmsCommandType_GetSpatialIndexes
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS

