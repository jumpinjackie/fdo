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
    m_savepoints = FdoStringCollection::Create();
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
    m_savepoints->Clear();
    FDO_SAFE_RELEASE(mFdoConnection);
}

void FdoRdbmsFeatureTransaction::Commit()
{
    if (mTransactionStarted == TRUE)
    {
        mTransactionStarted = FALSE;
        mFdoConnection->SetIsTransactionStarted(false);
        mConnection->GetGdbiCommands()->tran_end(mTransactionName);
        m_savepoints->Clear();
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
        m_savepoints->Clear();
    }
}

FdoString* FdoRdbmsFeatureTransaction::AddSavePoint(FdoString* suggestName)
{
    FdoPtr<FdoIConnectionCapabilities> ccp = mFdoConnection->GetConnectionCapabilities();
    if(!ccp->SupportsSavePoint())
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_186_SAVEPOINT_NOT_SUPPORTED)));
    
    if(suggestName == NULL || wcslen(suggestName) == 0)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_14_NULLSTRING)));
    }
    
    FdoStringP realName;
    try
    {
        realName = suggestName;
        int postfixCount = 0;
        while(true)
        {
            postfixCount++;
            if(!mConnection->GetGdbiCommands()->sp_exists(realName))
            {
                break;
            }
            realName = FdoStringP::Format(L"%ls%d", (FdoString*)suggestName, postfixCount);
        }
        mConnection->GetGdbiCommands()->sp_add(realName);
    }
    catch(GdbiException* ex)
    {
        throw FdoException::Create(ex->GetExceptionMessage(), ex->GetCause(), ex->GetNativeErrorCode());
    }
    
    return  m_savepoints->GetString(m_savepoints->Add(realName));
}

void FdoRdbmsFeatureTransaction::ReleaseSavePoint(FdoString* savePointName)
{
    FdoPtr<FdoIConnectionCapabilities> ccp = mFdoConnection->GetConnectionCapabilities();
    if(!ccp->SupportsSavePoint())
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_186_SAVEPOINT_NOT_SUPPORTED)));
 
    if(savePointName == NULL || wcslen(savePointName) == 0)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_14_NULLSTRING)));
    }
    
    if(!mConnection->GetGdbiCommands()->sp_exists(savePointName))
    {	
        FdoStringP errorMessage = FdoException::NLSGetMessage(FDO_NLSID(FDO_187_SAVEPOINT_NOT_EXIST), savePointName);
        throw  FdoException::Create(errorMessage);
    }

    try
    {
        mConnection->GetGdbiCommands()->sp_release(savePointName);
    }
    catch(GdbiException* ex)
    {
        throw FdoException::Create(ex->GetExceptionMessage(), ex->GetCause(), ex->GetNativeErrorCode());
    }

    int index = m_savepoints->IndexOf(savePointName);
    if(index != -1)
        m_savepoints->RemoveAt(index);
}

void FdoRdbmsFeatureTransaction::Rollback(FdoString* savePointName)
{
    FdoPtr<FdoIConnectionCapabilities> ccp = mFdoConnection->GetConnectionCapabilities();
    if(!ccp->SupportsSavePoint())
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_186_SAVEPOINT_NOT_SUPPORTED)));

    if(savePointName == NULL || wcslen(savePointName) == 0)
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_14_NULLSTRING)));
    }
    
    if(!mConnection->GetGdbiCommands()->sp_exists(savePointName))
    {	
        FdoStringP errorMessage = FdoException::NLSGetMessage(FDO_NLSID(FDO_187_SAVEPOINT_NOT_EXIST), savePointName);
        throw  FdoException::Create(errorMessage);
    }
    
    try
    {
        FdoStringP name = savePointName;
        mConnection->GetGdbiCommands()->sp_rollback(name);
    }
    catch(GdbiException* ex)
    {
        throw FdoException::Create(ex->GetExceptionMessage(), ex->GetCause(), ex->GetNativeErrorCode());
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
