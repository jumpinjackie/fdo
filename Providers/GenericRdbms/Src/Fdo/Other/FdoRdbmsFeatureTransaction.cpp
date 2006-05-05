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


#include "stdafx.h"
#include <FdoRdbmsFeatureTransaction.h>

FdoRdbmsFeatureTransaction::FdoRdbmsFeatureTransaction( FdoIConnection *connection ) :
mConnection( NULL )
{
    FdoRdbmsConnection *conn = static_cast<FdoRdbmsConnection*>(connection);
    if( conn )
      mConnection = conn->GetDbiConnection();
    else
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    mFdoConnection = conn;
    FDO_SAFE_ADDREF(mFdoConnection);

    sprintf(mTransactionName, "transaction%d", mConnection->GetNextIndex());
    mConnection->GetGdbiCommands()->tran_begin(mTransactionName);
    mTransactionStarted = TRUE;
    mFdoConnection->SetIsTransactionStarted(true);
}


FdoRdbmsFeatureTransaction::~FdoRdbmsFeatureTransaction()
{
    if (mTransactionStarted)
    {
        mFdoConnection->SetIsTransactionStarted(false);
        if (FdoPtr<FdoIConnection>(GetConnection ())->GetConnectionState () == FdoConnectionState_Open)
        {
            mConnection->GetGdbiCommands()->tran_rolbk();
            SynchSchema();
        }
    }
    FDO_SAFE_RELEASE(mFdoConnection);
}

void FdoRdbmsFeatureTransaction::Commit()
{
    if (mTransactionStarted == TRUE)
    {
        mTransactionStarted = FALSE;
        mFdoConnection->SetIsTransactionStarted(false);
        mConnection->GetGdbiCommands()->tran_end(mTransactionName);
    }
}

void FdoRdbmsFeatureTransaction::Rollback()
{
    if (mTransactionStarted == TRUE)
    {
        mTransactionStarted = FALSE;
        mFdoConnection->SetIsTransactionStarted(false);
        mConnection->GetGdbiCommands()->tran_rolbk();
        SynchSchema();
    }
}

FdoIConnection* FdoRdbmsFeatureTransaction::GetConnection()
{
    FDO_SAFE_ADDREF(mFdoConnection);
    return mFdoConnection;
}

void FdoRdbmsFeatureTransaction::SynchSchema()
{
    try {
        FdoSchemaManagerP schemaMgr = mFdoConnection->GetSchemaManager();

        // Metaschema changes are rolled back but not all RDBMS's roll back
        // the Table and Column changes
        // (Oracle does not roll back DDL changes ).
        // The following "rolls back" the Tables and Column changes by 
        // re-synchronizing them with the Metaschema.
        if ( schemaMgr )
            schemaMgr->SynchPhysical();
    }
    // Throw away synchronization errors. Rollback likely happened because of 
    // an error so don't overshadow original error with this one.
    catch ( FdoException* ex ) {
        ex->Release();
    }
    catch (...) 
    {
    }
}

void FdoRdbmsFeatureTransaction::Dispose()
{
    delete this;
}
