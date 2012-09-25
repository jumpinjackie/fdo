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
#ifdef _WIN32
#include <CRTDBG.H>
#endif

// FdoRdbmsSelectCommand
#include "FdoRdbmsSelectCommand.h"
#include "FdoRdbmsFeatureReader.h"
#include "FdoRdbmsSimpleFeatureReader.h"
#include "Fdo/Pvc/FdoRdbmsPropBindHelper.h"
#include "FdoRdbmsSimpleSelectCommand.h"
#include "FdoCommonOSUtil.h"

FdoRdbmsSimpleSelectCommand::FdoRdbmsSimpleSelectCommand ()
{
    mFdoConnection = NULL;
    mClassName = NULL;
    mConnection = NULL;
    mContainsObjectProperties = false;
    mIsObjectObject = false;
    mBindHelper = NULL;
    mSqlBuilder = NULL;
    mBackupCmd = NULL;
    mFilter = NULL;
    m_pParmeterValues = NULL;
    m_orderingProps = NULL;
    m_properties = NULL;
    m_joinCriteria = NULL;
    m_alias = NULL;
}

FdoRdbmsSimpleSelectCommand::FdoRdbmsSimpleSelectCommand (FdoIConnection *connection)
{
    mFdoConnection = static_cast<FdoRdbmsConnection*> (connection);
    FDO_SAFE_ADDREF(mFdoConnection);
    mConnection = mFdoConnection->GetDbiConnection();
    m_properties = FdoRdbmsIdentifierCollection::Create();
    mContainsObjectProperties = false;
    mIsObjectObject = false;
    mBindHelper = NULL;
    mSqlBuilder = NULL;
    mClassName = NULL;
    mBackupCmd = NULL;
    mFilter = NULL;
    m_pParmeterValues = NULL;
    m_orderingProps = NULL;
    m_joinCriteria = NULL;
    m_alias = NULL;
}

FdoRdbmsSimpleSelectCommand::~FdoRdbmsSimpleSelectCommand()
{
    FlushSelect();
    FDO_SAFE_RELEASE(mFdoConnection);
    FDO_SAFE_RELEASE(mClassName);
    FDO_SAFE_RELEASE(mBackupCmd);
    delete mBindHelper;
    FDO_SAFE_RELEASE(mSqlBuilder);

    FDO_SAFE_RELEASE(m_pParmeterValues);
    FDO_SAFE_RELEASE(mFilter);
    FDO_SAFE_RELEASE(m_properties);
    FDO_SAFE_RELEASE(m_orderingProps);
    FDO_SAFE_RELEASE(m_joinCriteria);
    FDO_SAFE_RELEASE(m_alias);
}

FdoRdbmsSimpleSelectCommand* FdoRdbmsSimpleSelectCommand::Create(FdoIConnection *connection)
{
    return new FdoRdbmsSimpleSelectCommand(connection);
}

void FdoRdbmsSimpleSelectCommand::FlushSelect()
{
    //bool connClose = (mFdoConnection->GetConnectionState() != FdoConnectionState_Open);
    //for(size_t i = 0; i < mStatements.size(); i++)
    //{
    //    GdbiQueryIdentifier* statement = mStatements[i];
    //    // if user closed the connection too earlier we cannot free the cursor
    //    if (connClose)
    //        statement->EnforceEmptyQueryId();
    //    FDO_SAFE_RELEASE(statement);
    //}
    //mStatements.clear();
    mSelectSql.resize(0);
    mContainsObjectProperties = false;
    mBindProps.clear();
    mParamMapping.clear();
}

void FdoRdbmsSimpleSelectCommand::PrepareSelect (const FdoSmLpClassDefinition* classDefinition)
{
    FlushSelect();

    mContainsObjectProperties = HasObjectProperty (classDefinition);
    if (mContainsObjectProperties)
        return;

    if (mSqlBuilder == NULL)
        mSqlBuilder = mFdoConnection->GetSqlBuilder();

    if (mSqlBuilder)
    {
        mSqlBuilder->Reset();
        std::vector<NameOrderingPair> ordering;
        mSqlBuilder->SetParameterValues(m_pParmeterValues);
        if (m_orderingProps && m_orderingProps->GetCount())
        {
            for (int i=0; i<m_orderingProps->GetCount(); i++)
            {
                FdoPtr<FdoIdentifier> id = m_orderingProps->GetItem(i);
                ordering.push_back(NameOrderingPair(id.p, m_option)); 
            }
        }
        
        if (m_properties != NULL)
            m_properties->SetCollectionChanged(false);
        if (m_orderingProps != NULL)
            m_orderingProps->SetCollectionChanged(false);
        if (m_joinCriteria != NULL)
            m_joinCriteria->SetCollectionChanged(false);
        if (m_pParmeterValues != NULL)
            m_pParmeterValues->SetCollectionChanged(false);

        FdoString* sqlString = mSqlBuilder->ToSelectSqlString(mClassName, m_alias, mFilter, m_properties, ordering, m_joinCriteria);
        if (sqlString != NULL && *sqlString != '\0')
        {
            mSelectSql = sqlString;

            if (mBindHelper == NULL)
                mBindHelper = new FdoRdbmsPropBindHelper(mFdoConnection);

            FdoInt32 cnt = (m_pParmeterValues != NULL) ? m_pParmeterValues->GetCount() : 0;
            std::vector< std::pair< FdoLiteralValue*, FdoInt64 > >* paramsUsed = mSqlBuilder->GetUsedParameterValues();
            if (paramsUsed != NULL && paramsUsed->size())
            {
                for(size_t i = 0; i < paramsUsed->size(); i++)
                {
                    mBindProps.push_back(paramsUsed->at(i));
                    FdoLiteralValue* lval = mBindProps.back().first;
                    for (FdoInt32 idx = 0; idx < cnt; idx++)
                    {
                        FdoPtr<FdoParameterValue> pval = m_pParmeterValues->GetItem(idx);
                        FdoPtr<FdoLiteralValue> pparVal = pval->GetValue();
                        if (lval == pparVal.p)
                        {
                            mParamMapping.push_back(std::make_pair(i, (size_t)idx));
                            break;
                        }
                    }
                }
            }
        }
    }
}

void FdoRdbmsSimpleSelectCommand::RebindValues()
{
    for(size_t i = 0; i < mParamMapping.size(); i++)
    {
        std::vector< std::pair< size_t, size_t > >::reference rf = mParamMapping.at(i);
        FdoPtr<FdoParameterValue> pval = m_pParmeterValues->GetItem((int)rf.second);
        FdoPtr<FdoLiteralValue> pparVal = pval->GetValue();
        mBindProps.at(rf.first).first = pparVal.p;
    }
}

void FdoRdbmsSimpleSelectCommand::UpdateCustomSelect()
{
    if (m_properties->GetCollectionChanged())
        mBackupCmd->SetFeatureClassName(mClassName);

    if (mBackupCmd->GetClassNameRef() == NULL)
        mBackupCmd->SetFeatureClassName(mClassName);

    mBackupCmd->SetFilter(mFilter);
    mBackupCmd->SetOrderingOption(m_option);
    if (m_alias != NULL)
        mBackupCmd->SetAlias(m_alias->GetName());

    if (m_properties != NULL)
    {
        m_properties->SetCollectionChanged(false);
        FdoPtr<FdoIdentifierCollection> coll = mBackupCmd->GetPropertyNames();
        coll->Clear();
        FdoInt32 cnt = m_properties->GetCount();
        for (int i = 0; i < cnt; i++)
        {
            FdoPtr<FdoIdentifier> op = m_properties->GetItem(i);
            coll->Add(op);
        }
    }

    if (m_orderingProps != NULL)
    {
        m_orderingProps->SetCollectionChanged(false);
        FdoPtr<FdoIdentifierCollection> coll = mBackupCmd->GetOrdering();
        coll->Clear();
        FdoInt32 cnt = m_orderingProps->GetCount();
        for (int i = 0; i < cnt; i++)
        {
            FdoPtr<FdoIdentifier> op = m_orderingProps->GetItem(i);
            coll->Add(op);
        }
    }

    if (m_pParmeterValues != NULL)
    {
        m_pParmeterValues->SetCollectionChanged(false);
        FdoPtr<FdoParameterValueCollection> coll = mBackupCmd->GetParameterValues();
        coll->Clear();
        FdoInt32 cnt = m_pParmeterValues->GetCount();
        for (int i = 0; i < cnt; i++)
        {
            FdoPtr<FdoParameterValue> pv = m_pParmeterValues->GetItem(i);
            coll->Add(pv);
        }
    }

    if (m_joinCriteria != NULL)
    {
        m_joinCriteria->SetCollectionChanged(false);
        FdoPtr<FdoJoinCriteriaCollection> coll = mBackupCmd->GetJoinCriteria();
        coll->Clear();
        FdoInt32 cnt = m_joinCriteria->GetCount();
        for (int i = 0; i < cnt; i++)
        {
            FdoPtr<FdoJoinCriteria> jc = m_joinCriteria->GetItem(i);
            coll->Add(jc);
        }
    }
}

bool FdoRdbmsSimpleSelectCommand::HasObjectProperty (const FdoSmLpClassDefinition *classDefinition)
{
    bool retVal = false;
    const   FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();
    
    for (int i = 0; i < propertyDefinitions->GetCount(); i++)
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitions->RefItem(i);
        if (FdoPropertyType_AssociationProperty == propertyDefinition->GetPropertyType() || FdoPropertyType_ObjectProperty == propertyDefinition->GetPropertyType())
        {
            retVal = true;
            break;
        }
    }
    return retVal;
}

FdoIFeatureReader* FdoRdbmsSimpleSelectCommand::Execute()
{
    if (!mConnection || !mFdoConnection || mFdoConnection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if (!mClassName)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_35, "Class is null"));

    // if we have a complex update just execute the sub command
    if (mBackupCmd != NULL)
    {
        UpdateCustomSelect();
        return mBackupCmd->Execute();
    }

    const FdoSmLpClassDefinition* classDefinition = mConnection->GetSchemaUtil()->GetClass(mClassName->GetText());

    if (mSelectSql.size() == 0 || (m_properties != NULL && m_properties->GetCollectionChanged()) || 
        (m_orderingProps != NULL && m_orderingProps->GetCollectionChanged()) || 
        (m_joinCriteria != NULL && m_joinCriteria->GetCollectionChanged()) || 
        (m_pParmeterValues != NULL && m_pParmeterValues->GetCollectionChanged()))
        PrepareSelect(classDefinition);

    if (mContainsObjectProperties || mIsObjectObject || mSelectSql.size() == 0)
    {
        if (mBackupCmd == NULL)
            mBackupCmd = FdoRdbmsSelectCommand::Create(mFdoConnection);
        // call the custom select;
        UpdateCustomSelect();
        return mBackupCmd->Execute();
    }
    // let's look for a free statement
    FdoPtr<GdbiQueryIdentifier> statement;
#if 0
    //GdbiQueryIdentifier* statement = NULL;
    //for(size_t i = 0; i < mStatements.size(); i++)
    //{
    //    // free statement?
    //    if (mStatements[i]->GetRefCount() == 1)
    //    {
    //        statement = mStatements[i];
    //        break;
    //    }
    //}
#endif
    GdbiConnection* gdbiConn = mConnection->GetGdbiConnection();

    int qid;
#if 0
    //if (statement == NULL)
#endif
    {
        mConnection->GetGdbiCommands()->sql ((wchar_t *)(const wchar_t *)mSelectSql.c_str(), &qid);

        std::vector< std::pair< int, char > >* types = mSqlBuilder != NULL ? mSqlBuilder->GetSelectGeometryTypes() : NULL;
        if (types != NULL)
        {
            char temp[32];
            for (std::vector< std::pair< int, char > >::iterator it = types->begin(); it < types->end(); it++)
            {
                FdoCommonOSUtil::ltoa(it->first+1, temp);
                mConnection->GetGdbiCommands()->geom_type_set(qid, temp, it->second);
            }
        }
        statement = new GdbiQueryIdentifier(gdbiConn->GetCommands(), qid);
#if 0
        //mStatements.push_back(statement);
#endif
        mBindHelper->BindParameters(mConnection->GetGdbiCommands(), qid, &mBindProps);
    }
#if 0
    //else
    //{
    //    RebindValues();
    //    qid = statement->GetQueryId();
    //    mBindHelper->BindValues(mConnection->GetGdbiCommands(), qid, &mBindProps);
    //}
#endif

    GdbiQueryResult* queryRslt = GdbiQueryResult::Create (mConnection->GetGdbiCommands(), statement);
    bool isFeatureClass = ( classDefinition != NULL &&  classDefinition->GetClassType() == FdoClassType_FeatureClass );

    return FdoRdbmsSimpleFeatureReader::Create (mFdoConnection, queryRslt, isFeatureClass, classDefinition, NULL, m_properties);
}
