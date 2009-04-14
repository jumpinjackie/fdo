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

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL

/// <summary>Enumerated type representing the possible storage engine types for MySQL tables</summary>
/// <param name="OvStorageEngineType_MyISAM">The binary portable storage engine that is the improved replacement for ISAM</param>
/// <param name="OvStorageEngineType_ISAM">The original MySQL storage engine</param>
/// <param name="OvStorageEngineType_InnoDB">Transaction-safe tables with row locking and foreign keys</param>
/// <param name="OvStorageEngineType_BDB">BerkeleyDB; Transaction-safe tables with page locking</param>
/// <param name="OvStorageEngineType_Merge">A collection of MyISAM tables used as one table</param>
/// <param name="OvStorageEngineType_Memory">The data for this table is stored only in memory.  Also known as the Heap storage engine</param>
/// <param name="OvStorageEngineType_Federated">Storage engine that accesses remote tables</param>
/// <param name="OvStorageEngineType_Archive">The archiving storage engine</param>
/// <param name="OvStorageEngineType_CSV">Tables that store rows in comma-separated values format</param>
/// <param name="OvStorageEngineType_Example">An example engine</param>
/// <param name="OvStorageEngineType_NDBClustered">Clustered, fault-tolerant, memory-based tables</param>
/// <param name="OvStorageEngineType_Unknown">The storage engine is unknown</param>
/// <param name="OvStorageEngineType_Default">The storage engine override is inherited from somewhere else.</param>
public __value enum OvStorageEngineType
{
    OvStorageEngineType_MyISAM = MySQLOvStorageEngineType_MyISAM,
    OvStorageEngineType_ISAM = MySQLOvStorageEngineType_ISAM,
    OvStorageEngineType_InnoDB = MySQLOvStorageEngineType_InnoDB,
    OvStorageEngineType_BDB = MySQLOvStorageEngineType_BDB,
    OvStorageEngineType_Merge = MySQLOvStorageEngineType_Merge,
    OvStorageEngineType_Memory = MySQLOvStorageEngineType_Memory,
    OvStorageEngineType_Federated = MySQLOvStorageEngineType_Federated,
    OvStorageEngineType_Archive = MySQLOvStorageEngineType_Archive,
    OvStorageEngineType_CSV = MySQLOvStorageEngineType_CSV,
    OvStorageEngineType_Example = MySQLOvStorageEngineType_Example,
    OvStorageEngineType_NDBClustered = MySQLOvStorageEngineType_NDBClustered,
    OvStorageEngineType_Unknown = MySQLOvStorageEngineType_Unknown,
    OvStorageEngineType_Default = MySQLOvStorageEngineType_Default
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS_OVERRIDE_MYSQL
