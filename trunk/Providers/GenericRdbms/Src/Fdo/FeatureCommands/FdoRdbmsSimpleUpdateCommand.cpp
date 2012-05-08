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
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>
#include "FdoRdbmsSimpleUpdateCommand.h"
#include "FdoRdbmsUpdateCommand.h"
#include "Inc/Common/Io/ByteStreamReader.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#include "Fdo/Pvc/FdoRdbmsPropBindHelper.h"

#ifndef _WIN32
#define  wcsicmp wcscasecmp
#endif

static char* TRANSACTION_NAME_SUPD = "TrSUpdCmd";


FdoRdbmsSimpleUpdateCommand::FdoRdbmsSimpleUpdateCommand ()
{
    mPropertyValues = NULL;
    mFdoConnection = NULL;
    mClassName = NULL;
    mConnection = NULL;
    mContainsObjectProperties = false;
    mIsObjectObject = false;
    mBindHelper = NULL;
    mFilter = NULL;
    mHasRevisionNumber = false;
    m_qid = -1;
    mBackupCmd = NULL;
}

FdoRdbmsSimpleUpdateCommand::FdoRdbmsSimpleUpdateCommand (FdoIConnection *connection)
{
    mFdoConnection = static_cast<FdoRdbmsConnection*> (connection);
    FDO_SAFE_ADDREF(mFdoConnection);
    mPropertyValues = FdoRdbmsPropertyValueCollection::Create();
    mParmeterValues = FdoRdbmsFdoParameterValueCollection::Create();
    mClassName = NULL;
    mConnection = mFdoConnection->GetDbiConnection();
    mContainsObjectProperties = false;
    mIsObjectObject = false;
    mBindHelper = NULL;
    mFilter = NULL;
    mHasRevisionNumber = false;
    m_qid = -1;
    mBackupCmd = NULL;
}

FdoRdbmsSimpleUpdateCommand::~FdoRdbmsSimpleUpdateCommand()
{
    FlushUpdate();
    FDO_SAFE_RELEASE(mPropertyValues);
    FDO_SAFE_RELEASE(mParmeterValues);
    FDO_SAFE_RELEASE(mFdoConnection);
    FDO_SAFE_RELEASE(mClassName);
    FDO_SAFE_RELEASE(mFilter);
    FDO_SAFE_RELEASE(mBackupCmd);
    delete mBindHelper;
}

FdoRdbmsSimpleUpdateCommand* FdoRdbmsSimpleUpdateCommand::Create(FdoIConnection *connection)
{
    return new FdoRdbmsSimpleUpdateCommand(connection);
}

void FdoRdbmsSimpleUpdateCommand::FlushUpdate()
{
    if (mFdoConnection->GetConnectionState() != FdoConnectionState_Closed)
    {
        // if user closed the connection too earlier we cannot free the cursor
        if (m_qid != -1)
            mConnection->GetGdbiCommands()->free_cursor(m_qid);
    }
    m_qid = -1;
    mUpdateSql.resize(0);
    mContainsObjectProperties = false;
    mBindProps.clear();
    mParamMapping.clear();
    mHasRevisionNumber = false;
}

void FdoRdbmsSimpleUpdateCommand::UpdateCustomUpdate()
{
    if (mPropertyValues->GetCollectionChanged())
        mBackupCmd->SetFeatureClassName(mClassName);

    if (mBackupCmd->GetClassNameRef() == NULL)
        mBackupCmd->SetFeatureClassName(mClassName);

    mPropertyValues->SetCollectionChanged(false);
    FdoPtr<FdoPropertyValueCollection> pvals = mBackupCmd->GetPropertyValues();
    pvals->Clear();
    FdoInt32 cnt = mPropertyValues->GetCount();
    for (int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoPropertyValue> propVal = mPropertyValues->GetItem(i);
        pvals->Add(propVal);
    }
    
    mParmeterValues->SetCollectionChanged(false);
    FdoPtr<FdoParameterValueCollection> paramvals = mBackupCmd->GetParameterValues();
    paramvals->Clear();
    cnt = mParmeterValues->GetCount();
    for (int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoParameterValue> paramVal = mParmeterValues->GetItem(i);
        paramvals->Add(paramVal);
    }
    mBackupCmd->SetFilter(mFilter);
}

void FdoRdbmsSimpleUpdateCommand::PrepareUpdate(const FdoSmLpClassDefinition* classDefinition)
{
    FlushUpdate();

    FdoPtr<FdoStringCollection> propNames = FdoStringCollection::Create();
    // Make sure no unexpected properties are added to the property value collection
    if (mPropertyValues->GetCount())
        SanitizePropertyValues(classDefinition, mPropertyValues, propNames.p, mContainsObjectProperties);

    FdoPtr<FdoRdbmsSqlBuilder> sqlBuilder = mFdoConnection->GetSqlBuilder();
    if (mContainsObjectProperties || mIsObjectObject || sqlBuilder == NULL) // we do not support insert objects props
    {
        mBindProps.clear();
        return;
    }
    mPropertyValues->SetCollectionChanged(false);
    mParmeterValues->SetCollectionChanged(false);

    const FdoSmLpDataPropertyDefinition *lpSystemProp = FdoSmLpDataPropertyDefinition::Cast(classDefinition->RefSystemProperty(L"RevisionNumber"));
    if (lpSystemProp && lpSystemProp->RefColumn())
        mHasRevisionNumber = true;

    FdoInt32 cnt = mPropertyValues->GetCount();
    if (cnt == 0)
    {
        // we cannot insert a new row with this command type, try with complex one
        mContainsObjectProperties = true;
        return;
    }
    if (mBindHelper == NULL)
        mBindHelper = new FdoRdbmsPropBindHelper(mFdoConnection);
    else
        mBindHelper->Clear();

    mUpdateSql.append(L"UPDATE ");
    const FdoSmLpDbObject* table = classDefinition->RefDbObject();
    FdoStringP tableName = table->RefDbObject()->GetDbQName();
    mUpdateSql.append((FdoString*)tableName);
    if (mHasRevisionNumber)
        mUpdateSql.append(L" SET revisionnumber = revisionnumber+1,");
    else
        mUpdateSql.append(L" SET ");

    for (FdoInt32 i = 0; i < cnt; i++)
    {
        mUpdateSql.append(propNames->GetString(i));
        mUpdateSql.append(L"=?,");
    }
    mUpdateSql.resize(mUpdateSql.size()-1);

    if (mFilter != NULL)
    {
        sqlBuilder->SetParameterValues(mParmeterValues);
        FdoString* txtWhere = sqlBuilder->ToUpdateFilterSqlString(mClassName, mFilter);
        if (txtWhere == NULL)
        {
            FlushUpdate();
            // we cannot insert a new row with this command type, try with complex one
            mContainsObjectProperties = true;
            return;
        }

        std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >* paramsUsed = sqlBuilder->GetUsedParameterValues();
        if (paramsUsed != NULL && paramsUsed->size())
        {
            size_t stSz = mBindProps.size();
            cnt = mParmeterValues->GetCount();
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

        mUpdateSql.append(L" WHERE ");
        mUpdateSql.append(txtWhere);
    }
}

void FdoRdbmsSimpleUpdateCommand::SanitizePropertyValues( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection *propValCollection, FdoStringCollection* propNames, bool& containsObjectProperties)
{
    containsObjectProperties = false;
    const FdoSmLpPropertyDefinitionCollection *properties = classDefinition->RefProperties();
    for (int i = 0; i < propValCollection->GetCount(); i++)
    {
        FdoPtr<FdoPropertyValue>propVal = propValCollection->GetItem(i);
        FdoPtr<FdoIdentifier>proId = propVal->GetName();
        const FdoSmLpPropertyDefinition *prop = properties->RefItem( proId->GetText() );
        if( prop == NULL )
        {
            FdoInt32 length;
            FdoString** scopes = proId->GetScope(length);
            if (length == 0)
               throw FdoCommandException::Create( NlsMsgGet1( FDORDBMS_56, "Property '%1$ls' not found", proId->GetText() ) );

            containsObjectProperties = true;
            return;
        }
        switch(prop->GetPropertyType())
        {
        case FdoPropertyType_DataProperty:
            {
                const FdoSmLpDataPropertyDefinition *dataProp = (FdoSmLpDataPropertyDefinition *)prop;
                if (dataProp->GetIsSystem() || dataProp->GetIsAutoGenerated())
                   throw FdoCommandException::Create (NlsMsgGet1 (FDORDBMS_393, "Property %1$ls is not user modifiable", proId->GetText()));

                const FdoSmLpSimplePropertyDefinition* simpleProp = static_cast<const FdoSmLpSimplePropertyDefinition*>(dataProp);
                const FdoSmPhColumn* column = simpleProp->RefColumn();
                if (column == NULL)
                    throw FdoCommandException::Create( NlsMsgGet1( FDORDBMS_56, "Property '%1$ls' not found", proId->GetText() ) );
                propNames->Add(column->GetDbName());

                FdoPtr<FdoValueExpression> exp = propVal->GetValue();
                FdoExpressionItemType type = exp->GetExpressionType();
                if (type != FdoExpressionItemType_DataValue && type != FdoExpressionItemType_GeometryValue)
                    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

                mBindProps.push_back(std::make_pair(static_cast<FdoLiteralValue*>(exp.p), 0));
            }
            break;
        case FdoPropertyType_ObjectProperty:
        case FdoPropertyType_AssociationProperty:
            containsObjectProperties = true;
            return;
        case FdoPropertyType_GeometricProperty:
            {
                const FdoSmLpGeometricPropertyDefinition* geomProp = static_cast<const FdoSmLpGeometricPropertyDefinition*>(prop);
                const FdoSmPhColumn* geomColumn = geomProp ? geomProp->RefColumn() : (const FdoSmPhColumn*) NULL;
                FdoPtr<FdoValueExpression> exp = propVal->GetValue();
                FdoExpressionItemType type = exp->GetExpressionType();
                if (type != FdoExpressionItemType_DataValue && type != FdoExpressionItemType_GeometryValue)
                    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
                if (geomColumn == NULL)
                    throw FdoCommandException::Create( NlsMsgGet1( FDORDBMS_56, "Property '%1$ls' not found", proId->GetText() ) );
                propNames->Add(geomColumn->GetDbName());

                FdoSmPhColumnGeomP geomCol = ((FdoSmPhColumn*)geomColumn)->SmartCast<FdoSmPhColumnGeom>();
                mBindProps.push_back(std::make_pair(static_cast<FdoLiteralValue*>(exp.p), geomCol->GetSRID()));
            }
            break;
        }
    }
}

void FdoRdbmsSimpleUpdateCommand::RebindValues()
{
    FdoInt32 cnt = mPropertyValues->GetCount();
    for (FdoInt32 i = 0; i < cnt; i++)
    {
        FdoPtr<FdoPropertyValue> propVal = mPropertyValues->GetItem(i);
        FdoPtr<FdoValueExpression> exp = propVal->GetValue();
        mBindProps.at(i).first = static_cast<FdoLiteralValue*>(exp.p);
    }

    for (FdoInt32 i = 0; i < mParamMapping.size(); i++)
    {
        std::vector< std::pair< size_t, size_t > >::reference rf = mParamMapping.at(i);

        FdoPtr<FdoParameterValue> pval = mParmeterValues->GetItem((int)rf.second);
        FdoPtr<FdoLiteralValue> pparVal = pval->GetValue();
        mBindProps.at(rf.first).first = pparVal.p;
    }
}

FdoInt32 FdoRdbmsSimpleUpdateCommand::Execute ()
{
    if (!mConnection || !mFdoConnection || mFdoConnection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if (!mClassName)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_35, "Class is null"));

    // if we have a complex update just execute the sub command
    if (mBackupCmd != NULL)
    {
        UpdateCustomUpdate();
        return mBackupCmd->Execute();
    }

    const FdoSmLpClassDefinition* classDefinition = mConnection->GetSchemaUtil()->GetClass(mClassName->GetText());

    if (mUpdateSql.size() == 0 || mPropertyValues->GetCollectionChanged() || mParmeterValues->GetCollectionChanged())
        PrepareUpdate(classDefinition);
    else
        RebindValues();

    if (mContainsObjectProperties || mIsObjectObject || mUpdateSql.size() == 0)
    {
        if (mBackupCmd == NULL)
            mBackupCmd = FdoRdbmsUpdateCommand::Create(mFdoConnection);
        // call the custom update;
        UpdateCustomUpdate();
        return mBackupCmd->Execute();
    }
    GdbiConnection* gdbiConn = mConnection->GetGdbiConnection();

    bool bBeginTransaction = false;
    FdoInt32 retVal = 0;
    try
    {
        if (mFdoConnection->GetIsTransactionStarted() == false)
        {
            mConnection->GetGdbiCommands()->tran_begin(TRANSACTION_NAME_SUPD);
            bBeginTransaction = true;
        }
        if (m_qid == -1)
        {
            mConnection->GetGdbiCommands()->sql ((wchar_t *)(const wchar_t *)mUpdateSql.c_str(), &m_qid);
            mBindHelper->BindParameters(mConnection->GetGdbiCommands(), m_qid, &mBindProps);
        }
        else
            mBindHelper->BindValues(mConnection->GetGdbiCommands(), m_qid, &mBindProps);

        retVal = (RDBI_SUCCESS == mConnection->GetGdbiCommands()->execute(m_qid)) ? mConnection->GetGdbiCommands()->crsr_nrows(m_qid) : 0;

        if (bBeginTransaction)
        {
            bBeginTransaction = false;
            mConnection->GetGdbiCommands()->tran_end(TRANSACTION_NAME_SUPD);
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
