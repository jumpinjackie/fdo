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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
/// \endcond

/// \ingroup (OSGeoFDOCommandsLocking)
/// \brief
/// The LockStrategy enumeration defines how lock acquisition should proceed.
public __value enum LockStrategy
{
    /// Declares that either all of the locks must be acquired or none should be acquired.
	LockStrategy_All = FdoLockStrategy_All,

    /// Declares that all of the locks that can be acquired should be acquired.
	LockStrategy_Partial = FdoLockStrategy_Partial
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
/// \endcond
