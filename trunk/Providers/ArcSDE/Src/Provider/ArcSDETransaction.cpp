/*
 * 
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

#include "stdafx.h"
#include "ArcSDEUtils.h"

ArcSDETransaction::ArcSDETransaction () :
    mConnection (NULL),
    mSpent (false)
{
}

ArcSDETransaction::~ArcSDETransaction (void)
{
    if (!mSpent)
        Commit ();
}

/// <summary> Gets the FdoIConnection object associated with the transaction.</summary>
/// <returns>Returns FdoIConnection object</returns>
FdoIConnection* ArcSDETransaction::GetConnection ()
{
    return (FDO_SAFE_ADDREF(mConnection));
}

/// <summary> Commits the transaction.</summary>
/// <returns>Returns nothing</returns>
void ArcSDETransaction::Commit ()
{
    SE_CONNECTION connection;
    LONG result;

    if (mSpent)
        throw FdoException::Create (NlsMsgGet(ARCSDE_TRANSACTION_SPENT, "This transaction has already been committed or rolled back."));

    if (mConnection != NULL)
    {
        // finish edit session
        mConnection->Flush ();

        // commit
        connection = mConnection->GetConnection ();
        result = SE_connection_commit_transaction (connection);
        mSpent = true;
        mConnection->SetTransaction (NULL);
        handle_sde_err<FdoException>(connection, result, __FILE__, __LINE__, ARCSDE_TRANSACTION_COMMIT_FAILED, "Failed to commit this transaction.");
    }
}

/// <summary> Rolls back a transaction from a pending state.</summary>
/// <returns>Returns nothing</returns>
void ArcSDETransaction::Rollback ()
{
    SE_CONNECTION connection;
    LONG result;

    if (mSpent)
        throw FdoException::Create (NlsMsgGet(ARCSDE_TRANSACTION_SPENT, "This transaction has already been committed or rolled back."));

    if (mConnection != NULL)
    {
        // finish edit session
        mConnection->Flush ();

        // rollback
        connection = mConnection->GetConnection ();
        result = SE_connection_rollback_transaction (connection);
        mSpent = true;
        mConnection->SetTransaction (NULL);
        handle_sde_err<FdoException>(connection, result, __FILE__, __LINE__, ARCSDE_TRANSACTION_ROLLBACK_FAILED, "Failed to rollback this transaction.");
    }
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void ArcSDETransaction::Dispose ()
{
    if (NULL == mConnection)
        delete this; // we're stand-alone, OK to go bye-bye
    else
    {
        // our connection still needs us, but we don't need it so Release()
        // this may lead to a recursive call through SetConnection()
        // to this->Dispose() if his refcount goes to zero
        // because we're the only one referencing him,
        // but at that time the connection will be NULL
        mSpent = true;
        mConnection->FdoIConnection::Release ();
        mConnection = NULL;
    }
}

void ArcSDETransaction::SetConnection (ArcSDEConnection* connection)
{
    LONG result;

    // this code and the over-ridden Dispose() maintain the refcount on this object
    // artificially low by one, which is the refcount that would have been placed
    // on this object by the connection
    FDO_SAFE_RELEASE (mConnection);
    mConnection = connection;
    if (NULL == mConnection)
        // our connection wants to die, he'll soon Release() us
        AddRef ();
    else
    {
        Release (); // our connection is letting us know he's interested, he's already done AddRef()
        mConnection->AddRef ();
        // begin the underlying transaction
        result = SE_connection_start_transaction (mConnection->GetConnection ());
        handle_sde_err<FdoException>(mConnection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_TRANSACTION_START_FAILED, "Failed to start transaction.");
    }
}

bool ArcSDETransaction::IsSpent ()
{
    return (mSpent);
}

