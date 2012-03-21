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
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>
#include "FdoRdbmsSimpleDeleteCommand.h"
#include "FdoRdbmsDeleteCommand.h"
#include "Inc/Common/Io/ByteStreamReader.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#include "Fdo/Pvc/FdoRdbmsPropBindHelper.h"

#ifndef _WIN32
#define  wcsicmp wcscasecmp
#endif

static char* TRANSACTION_NAME_SDEL = "TrSDelCmd";


FdoRdbmsSimpleDeleteCommand::FdoRdbmsSimpleDeleteCommand ()
{
    mFdoConnection = NULL;
    mClassName = NULL;
    mConnection = NULL;
    mContainsObjectProperties = false;
    mIsObjectObject = false;
    mBindHelper = NULL;
    mFilter = NULL;
    m_qid = -1;
    mBackupCmd = NULL;
}

FdoRdbmsSimpleDeleteCommand::FdoRdbmsSimpleDeleteCommand (FdoIConnection *connection)
{
    mFdoConnection = static_cast<FdoRdbmsConnection*> (connection);
    FDO_SAFE_ADDREF(mFdoConnection);
    mParmeterValues = FdoRdbmsFdoParameterValueCollection::Create();
    mClassName = NULL;
    mConnection = mFdoConnection->GetDbiConnection();
    mContainsObjectProperties = false;
    mIsObjectObject = false;
    mBindHelper = NULL;
    mFilter = NULL;
    m_qid = -1;
    mBackupCmd = NULL;
}

FdoRdbmsSimpleDeleteCommand::~FdoRdbmsSimpleDeleteCommand()
{
    FlushDelete();
    FDO_SAFE_RELEASE(mParmeterValues);
    FDO_SAFE_RELEASE(mFdoConnection);
    FDO_SAFE_RELEASE(mClassName);
    FDO_SAFE_RELEASE(mFilter);
    FDO_SAFE_RELEASE(mBackupCmd);
    delete mBindHelper;
}

FdoRdbmsSimpleDeleteCommand* FdoRdbmsSimpleDeleteCommand::Create(FdoIConnection *connection)
{
    return new FdoRdbmsSimpleDeleteCommand(connection);
}

void FdoRdbmsSimpleDeleteCommand::SetFeatureClassName(FdoIdentifier* value)
{
    FlushDelete();
    FDO_SAFE_RELEASE(mClassName);

    if( NULL == mConnection )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if (value)
    {
        const FdoSmLpClassDefinition *classDefinition = mConnection->GetSchemaUtil()->GetClass(value->GetText());
        if ( classDefinition == NULL )
            throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_333, "Class '%1$ls' not found", value->GetText()));

        if (classDefinition->GetIsAbstract())
            throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_266, "Creating/Updating a standalone instance for class '%1$ls' is not allowed", value->GetText()));
        
        FdoString* name = value->GetText();
        while(*name != '\0' && *name != '.') name++;
        mIsObjectObject = (*name == L'.');

        mConnection->GetSchemaUtil()->CheckClass(value->GetText());

        mClassName = FDO_SAFE_ADDREF(value);
    }
}

void FdoRdbmsSimpleDeleteCommand::FlushDelete()
{
    if (mFdoConnection->GetConnectionState() != FdoConnectionState_Closed)
    {
        // if user closed the connection too earlier we cannot free the cursor
        if (m_qid != -1)
            mConnection->GetGdbiCommands()->free_cursor(m_qid);
    }
    m_qid = -1;
    mDeleteSql.resize(0);
    mContainsObjectProperties = false;
    mBindProps.clear();
    mParamMapping.clear();
}

void FdoRdbmsSimpleDeleteCommand::UpdateCustomDelete()
{
    if (mBackupCmd->GetClassNameRef() == NULL)
        mBackupCmd->SetFeatureClassName(mClassName);

    mParmeterValues->SetCollectionChanged(false);
    FdoPtr<FdoParameterValueCollection> paramvals = mBackupCmd->GetParameterValues();
    paramvals->Clear();
    int cnt = mParmeterValues->GetCount();
    for (int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoParameterValue> paramVal = mParmeterValues->GetItem(i);
        paramvals->Add(paramVal);
    }
    mBackupCmd->SetFilter(mFilter);
}

void FdoRdbmsSimpleDeleteCommand::PrepareDelete(const FdoSmLpClassDefinition* classDefinition)
{
    FlushDelete();

    FdoPtr<FdoRdbmsSqlBuilder> sqlBuilder = mFdoConnection->GetSqlBuilder();
    if (mContainsObjectProperties || mIsObjectObject || sqlBuilder == NULL) // we do not support delete objects props
    {
        mBindProps.clear();
        return;
    }
    mParmeterValues->SetCollectionChanged(false);

    if (mBindHelper == NULL)
        mBindHelper = new FdoRdbmsPropBindHelper(mFdoConnection);
    else
        mBindHelper->Clear();

    mDeleteSql.append(L"DELETE FROM ");
    const FdoSmLpDbObject* table = classDefinition->RefDbObject();
    FdoStringP tableName = table->RefDbObject()->GetDbQName();
    mDeleteSql.append((FdoString*)tableName);

    if (mFilter != NULL)
    {
        sqlBuilder->SetParameterValues(mParmeterValues);
        FdoString* txtWhere = sqlBuilder->ToUpdateFilterSqlString(mClassName, mFilter);
        if (txtWhere == NULL)
        {
            FlushDelete();
            // we cannot delete a row with this command type, try with complex one
            mContainsObjectProperties = true;
            return;
        }

        std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >* paramsUsed = sqlBuilder->GetUsedParameterValues();
        if (paramsUsed != NULL && paramsUsed->size())
        {
            size_t stSz = mBindProps.size();
            int cnt = mParmeterValues->GetCount();
            for (size_t idx = 0; idx < paramsUsed->size(); idx++)
            {
                std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >::reference rf = paramsUsed->at(idx);
                FdoLiteralValue* lval = rf.first;
                for (FdoInt32 i = 0; i < cnt; i++)
                {
                    FdoPtr<FdoParameterValue> pval = mParmeterValues->GetItem(i);
                    FdoPtr<FdoLiteralValue> pparVal = pval->GetValue();
                    if (lval == pparVal.p)
                    {
                        mParamMapping.push_back(std::make_pair((idx+stSz), (size_t)i));
                        mBindProps.push_back(std::make_pair(lval, rf.second));
                        break;
                    }
                }
            }
        }

        mDeleteSql.append(L" WHERE ");
        mDeleteSql.append(txtWhere);
    }
}

void FdoRdbmsSimpleDeleteCommand::RebindValues()
{
    for (size_t i = 0; i < mParamMapping.size(); i++)
    {
        std::vector< std::pair< size_t, size_t > >::reference rf = mParamMapping.at(i);

        FdoPtr<FdoParameterValue> pval = mParmeterValues->GetItem((int)rf.second);
        FdoPtr<FdoLiteralValue> pparVal = pval->GetValue();
        mBindProps.at(rf.first).first = pparVal.p;
    }
}

FdoInt32 FdoRdbmsSimpleDeleteCommand::Execute ()
{
    if (!mConnection || !mFdoConnection || mFdoConnection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if (!mClassName)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_35, "Class is null"));

    // if we have a complex update just execute the sub command
    if (mBackupCmd != NULL)
    {
        UpdateCustomDelete();
        return mBackupCmd->Execute();
    }

    const FdoSmLpClassDefinition* classDefinition = mConnection->GetSchemaUtil()->GetClass(mClassName->GetText());

    if (mDeleteSql.size() == 0 || mParmeterValues->GetCollectionChanged())
        PrepareDelete(classDefinition);
    else
        RebindValues();

    if (mContainsObjectProperties || mIsObjectObject || mDeleteSql.size() == 0)
    {
        if (mBackupCmd == NULL)
            mBackupCmd = FdoRdbmsDeleteCommand::Create(mFdoConnection);
        // call the custom update;
        UpdateCustomDelete();
        return mBackupCmd->Execute();
    }
    GdbiConnection* gdbiConn = mConnection->GetGdbiConnection();

    bool bBeginTransaction = false;
    FdoInt32 retVal = 0;
    try
    {
        if (mFdoConnection->GetIsTransactionStarted() == false)
        {
            mConnection->GetGdbiCommands()->tran_begin(TRANSACTION_NAME_SDEL);
            bBeginTransaction = true;
        }
        if (m_qid == -1)
        {
            mConnection->GetGdbiCommands()->sql ((wchar_t *)(const wchar_t *)mDeleteSql.c_str(), &m_qid);
            mBindHelper->BindParameters(mConnection->GetGdbiCommands(), m_qid, &mBindProps);
        }
        else
            mBindHelper->BindValues(mConnection->GetGdbiCommands(), m_qid, &mBindProps);

        retVal = (RDBI_SUCCESS == mConnection->GetGdbiCommands()->execute(m_qid)) ? mConnection->GetGdbiCommands()->crsr_nrows(m_qid) : 0;

        if (bBeginTransaction)
        {
            bBeginTransaction = false;
            mConnection->GetGdbiCommands()->tran_end(TRANSACTION_NAME_SDEL);
        }
    }
    catch(...)
    {
        try
        {
            if (bBeginTransaction)
                mConnection->GetGdbiCommands()->tran_rolbk();
        }
        catch (FdoException* pExc) { pExc->Release(); }
        catch ( ... ) { } // Do not mask the original exception

        throw;
    }
    return retVal;
}
