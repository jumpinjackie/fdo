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

#include "FDO\Commands\Locking\mgIGetLockInfo.h"
#include "FDO\Commands\mgIFeatureCommandImp.h"

class FdoIGetLockInfo;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
public __gc __interface ILockedObjectReader;

/// \brief
/// The IGetLockInfoImp class is a concrete implementation of IGetLockedObjects.
/// The IGetLockInfo interface defines the GetLockInfo command, which gets a
/// lock information for the feature instances of a given class that match the
/// specified filter. If the filter is empty, all feature instances of the given class are selected.
private __gc class IGetLockInfoImp : public NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp, 
                                     public NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockInfo
{
public:
    /// \brief
    /// Executes the GetLockInfo command, returning an ILockedObjectReader.
    /// 
    /// \return
    /// Returns a locked object reader.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReader* Execute();

public:
    /// \brief
    /// Constructs an IGetLockInfoImp managed object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	IGetLockInfoImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIGetLockInfo* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING


