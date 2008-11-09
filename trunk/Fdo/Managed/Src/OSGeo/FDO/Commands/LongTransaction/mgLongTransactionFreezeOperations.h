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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
/// \endcond

/// \ingroup (OSGeoFDOCommandsLongTransaction)
/// \brief
/// The LongTransactionFreezeOperations enumeration defines the 
/// list of available operations that can be performed by the FreezeLongTransaction command.
[System::FlagsAttribute]
public __value enum LongTransactionFreezeOperations
{
    /// Freeze the long transaction for exclusive update.
	LongTransactionFreezeOperations_Freeze = FdoLongTransactionFreezeOperations_Freeze,

    /// Thaw the long transaction.
	LongTransactionFreezeOperations_Thaw = FdoLongTransactionFreezeOperations_Thaw
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
/// \endcond
