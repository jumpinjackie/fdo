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

#include "FDO\Commands\mgIFeatureCommandImp.h"
#include "FDO\Commands\LongTransaction\mgICommitLongTransaction.h"

class FdoICommitLongTransaction;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
interface class ILongTransactionConflictDirectiveEnumerator;

/// \ingroup (OSGeoFDOCommandsLongTransaction)
/// \brief
/// The ICommitLongTransactionImp class is a 
/// concrete implementation of ICommitLongTransaction.
/// The ICommitLongTransaction interface defines the CommitLongTransaction
/// command, which allows a user to execute commit operations on a long
/// transaction. Two different commit operations are distinguished: full and partial.
///
/// <b>Full.</b> A full commit operation is executed on all data within a long transaction. The data is committed to the parent long transaction and the long transaction itself is removed.
///
/// <b>Partial.</b> A partial commit operation is executed on a subset of the data within a long transaction (this could be all data). The data is committed to the parent long transaction. However, the long transaction, itself, is not removed. The subset of the data to be committed is defined by a class name and a set of filter criteria.
///
/// To perform a commit operation the user requires the access and commit
/// privileges for the long transaction to be committed (in case of a
/// full commit the user also requires the remove privilege for the long
/// transaction) and the access privilege for the parent long transaction.
/// In addition, a full commit can be performed on a leaf long transaction
/// only. A long transaction is a leaf long transaction if it does not
/// have descendent versions.
private ref class ICommitLongTransactionImp : public NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp, 
                                               public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICommitLongTransaction
{
public:
    /// \brief
    /// Gets the name of the long transaction to commit as a string.
    /// 
    /// \return
    /// Returns the name of the long transaction
    /// 
    /// \brief
    /// Sets the name of the long transaction to commit as a string.
    /// 
    /// \param value 
    /// Input the name of the long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::String^ Name
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// Executes the commit long transaction command and returns an
    /// ILongTransactionConflictDirectiveEnumerator.
    /// If the returned enumerator's count is 0 then
    /// no conflicts were detected and the command was executed successfully.
    /// Otherwise conflicts were detected and the user must specify how to
    /// resolve them using the enumerator and call Execute again.
	virtual ILongTransactionConflictDirectiveEnumerator^ Execute();

internal:
	ICommitLongTransactionImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::IFeatureCommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoICommitLongTransaction* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


