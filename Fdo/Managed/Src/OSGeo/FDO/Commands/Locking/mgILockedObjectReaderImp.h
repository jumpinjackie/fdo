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

#include "FDO\Commands\Locking\mgLockType.h"
#include "FDO\Commands\Locking\mgILockedObjectReader.h"

class FdoILockedObjectReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
ref class PropertyValueCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING

/// \ingroup (OSGeoFDOCommandsLocking)
/// \brief
/// The ILockedObjectReaderImp class is a concrete implementation of ILockedObjectReader.
/// The ILockedObjectReader interface provides forward-only, read-only
/// functionality to identify lock information for a set of objects.
/// \remarks
/// A reference to an ILockedObjectReader interface is returned from the GetLockedObjects
/// and GetLockInfo commands. The initial position of the ILockInfoReader is
/// prior to the first item. Thus, you must call ReadNext to begin accessing
/// any data.
private ref class ILockedObjectReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                            public NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReader
{
public:
    /// \brief
    /// Gets the class name of the feature currently being read.
    /// 
    /// \remarks
    /// The value of the class name will be null in the event that the
    /// class name could not be resolved. This can happen if the table has
    /// no class id field and multiple classes have been written to the table.
    /// 
    /// \return
    /// Returns the name of the class.
    /// 
	virtual System::String^ GetFeatureClassName();

    /// \brief
    /// Gets an PropertyValueCollection containing the property values
    /// that uniquely identify the feature currently being read.
    /// 
    /// \return
    /// Returns the property collection identifying the feature.
    /// 
	virtual NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ GetIdentity();

    /// \brief
    /// Gets the name of user who holds the lock on the feature currently being
    /// read.
    /// 
    /// \return
    /// Returns the lock owner
    /// 
	virtual System::String^ GetLockOwner();

    /// \brief
    /// Gets the type of the lock held on the feature currently being read.
    /// 
    /// \return
    /// Returns the lock type
    /// 
	virtual NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::LockType GetLockType();

    /// \brief
    /// Gets the name of the long transaction in which the feature 
    /// currently being read is locked.
    /// 
    /// \return
    /// Returns the name of the long transaction in which the feature
    /// currently being read is locked.
    /// 
	virtual System::String^ GetLongTransaction();

    /// \brief
    /// Advances the reader to the next item. The default position of the 
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	virtual System::Boolean ReadNext();

    /// \brief
    /// Closes the ILockInfoReader object, freeing any resources it may
    /// be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Close();

internal:
	ILockedObjectReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoILockedObjectReader* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING


