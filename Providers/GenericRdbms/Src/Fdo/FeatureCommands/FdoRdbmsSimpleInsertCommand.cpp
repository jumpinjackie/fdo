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
 */

#include "stdafx.h"
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>
#include <time.h>

// FdoRdbmsSimpleInsertCommand
#include "FdoRdbmsException.h"
#include "FdoRdbmsSimpleInsertCommand.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFeatureInfoReader.h"
#include "FdoRdbmsInsertCommand.h"
#include "Inc/ut.h"
#include "Fdo/Pvc/FdoRdbmsPropBindHelper.h"

static char* TRANSACTION_NAME_SINS = "TrSInsCmd";

FdoRdbmsSimpleInsertCommand::FdoRdbmsSimpleInsertCommand ()
{
    mPropertyValues = NULL;
    mAutoGenPropertyValues = NULL;
    mClassName = NULL;
    mConnection = NULL;
    mContainsObjectProperties = false;
    mBindHelper = NULL;
    mHasRevisionNumber = false;
    mBackupCmd = NULL;
    m_qid = -1;
}

FdoRdbmsSimpleInsertCommand::FdoRdbmsSimpleInsertCommand (FdoIConnection *connection) : FdoRdbmsCommand<FdoIInsert>(connection)
{
    mPropertyValues = FdoRdbmsPropertyValueCollection::Create();
    mLocalPropertyValues = FdoPropertyValueCollection::Create();
    mAutoGenPropertyValues = FdoPropertyValueCollection::Create();
    mClassName = NULL;
    mConnection = mFdoConnection->GetDbiConnection();
    mContainsObjectProperties = false;
    mBindHelper = NULL;
    mHasRevisionNumber = false;
    mBackupCmd = NULL;
    mClassName = FdoIdentifier::Create();
    m_qid = -1;
}

FdoRdbmsSimpleInsertCommand::~FdoRdbmsSimpleInsertCommand()
{
    FlushInsert();
    FDO_SAFE_RELEASE(mPropertyValues);
    FDO_SAFE_RELEASE(mAutoGenPropertyValues);
    FDO_SAFE_RELEASE(mClassName);
    FDO_SAFE_RELEASE(mLocalPropertyValues);
    FDO_SAFE_RELEASE(mBackupCmd);
    delete mBindHelper;
}

FdoRdbmsSimpleInsertCommand* FdoRdbmsSimpleInsertCommand::Create(FdoIConnection *connection)
{
    return new FdoRdbmsSimpleInsertCommand(connection);
}

void FdoRdbmsSimpleInsertCommand::FlushInsert()
{
    if (m_qid != -1)
        mConnection->GetGdbiCommands()->free_cursor(m_qid);
    m_qid = -1;
    mInsertSql.resize(0);
    mLocalPropertyValues->Clear();
    mContainsObjectProperties = false;
    mBindProps.clear();
    mAutoGenPropertyType.clear();
    mAutoGenPropertyValues->Clear();
    mHasRevisionNumber = false;
}

void FdoRdbmsSimpleInsertCommand::PrepareInsert(const FdoSmLpClassDefinition* classDefinition)
{
    FlushInsert();

    FdoPtr<FdoStringCollection> propNames = FdoStringCollection::Create();
    // Make sure no unexpected properties are added to the property value collection
    if (mPropertyValues->GetCount())
        SanitizePropertyValues(classDefinition, mPropertyValues, propNames.p, mContainsObjectProperties);

    if (mContainsObjectProperties) // we do not support insert objects props
    {
        mBindProps.clear();
        return;
    }
    mPropertyValues->SetCollectionChanged(false);

    FdoInt32 cnt = mPropertyValues->GetCount();
    for (int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoPropertyValue> itm = mPropertyValues->GetItem(i);
        mLocalPropertyValues->Add(itm);        
    }

    const FdoSmLpDataPropertyDefinition *lpSystemProp = FdoSmLpDataPropertyDefinition::Cast(classDefinition->RefSystemProperty(L"ClassId"));
    if (lpSystemProp && lpSystemProp->RefColumn())
    {
        FdoPtr<FdoPropertyValue>classId = FdoPropertyValue::Create();
        FdoPtr<FdoDataValue>clidValue = FdoDataValue::Create(classDefinition->GetId());
        classId->SetValue( clidValue );
        classId->SetName( lpSystemProp->GetName() );
        mLocalPropertyValues->Add( classId );
        propNames->Add(L"\"ClassId\"");
        mBindProps.push_back(std::make_pair(clidValue.p, 0));
    }
    lpSystemProp = FdoSmLpDataPropertyDefinition::Cast(classDefinition->RefSystemProperty(L"RevisionNumber"));
    if (lpSystemProp && lpSystemProp->RefColumn())
    {
        FdoPtr<FdoPropertyValue>revNum = FdoPropertyValue::Create();
        FdoPtr<FdoDataValue>revValue = FdoDataValue::Create((FdoInt64) 0 );
        revNum->SetValue( revValue );
        revNum->SetName( lpSystemProp->GetName() );
        mLocalPropertyValues->Add( revNum );
        propNames->Add(L"\"RevisionNumber\"");
        mBindProps.push_back(std::make_pair(revValue.p, 0));
        mHasRevisionNumber = true;
    }

    const FdoSmLpDataPropertyDefinitionCollection * idPropDefs = classDefinition->RefIdentityProperties();
    cnt = idPropDefs->GetCount();
    for (FdoInt32 i = 0; i < cnt; i++)
    {
        const FdoSmLpDataPropertyDefinition * idPropDef = idPropDefs->RefItem(i);
        if (idPropDef->GetIsAutoGenerated())
        {
            FdoPtr<FdoPropertyValue> idPropValue = FdoPropertyValue::Create();
            FdoString* idPropName = idPropDef->GetName();
            idPropValue->SetName (idPropName);
            FdoPtr<FdoInt64Value> agv = FdoInt64Value::Create ((FdoInt64)0);
            idPropValue->SetValue(agv);
            mAutoGenPropertyValues->Add(idPropValue);
            const FdoSmPhColumn *columnDef = NULL;
            const FdoSmLpSimplePropertyDefinition* simplePropDef = static_cast<const FdoSmLpSimplePropertyDefinition*>(idPropDef);
            if (simplePropDef != NULL)
                columnDef = simplePropDef->RefColumn();
            // in case we have AutoGenerated but not Autoincrement add the value to the insert statement
            std::wstring sequenceName = SetAutoGeneratedValue(idPropDef, idPropValue);
            if (columnDef != NULL && !columnDef->GetAutoincrement())
            {
                mBindProps.push_back(std::make_pair(agv.p, 0));
                mLocalPropertyValues->Add(idPropValue);
                propNames->Add(columnDef->GetDbName());
                mAutoGenPropertyType.push_back(std::make_pair(sequenceName, 10 + mLocalPropertyValues->GetCount()));
            }
            else
                mAutoGenPropertyType.push_back(std::make_pair(sequenceName, 0));
        }
        else
        {
            FdoInt32 cnt2 = mPropertyValues->GetCount();
            for (FdoInt32 y = 0; y < cnt2; y++)
            {
                FdoPtr<FdoPropertyValue> itm = mPropertyValues->GetItem(y);
                FdoString* idPropName = idPropDef->GetName();
                FdoPtr<FdoIdentifier> idf = itm->GetName();
                if (wcscmp(idf->GetName(), idPropName) == 0)
                {
                    mAutoGenPropertyType.push_back(std::make_pair(L"", 10 + y));
                    break;
                }
            }
        }
    }
    cnt = mLocalPropertyValues->GetCount();
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

    mInsertSql.append(L"INSERT INTO ");
    const FdoSmLpDbObject* table = classDefinition->RefDbObject();
    FdoStringP tableName = table->RefDbObject()->GetDbQName();
    mInsertSql.append((FdoString*)tableName);
    mInsertSql.append(L" (");

    std::wstring valuesQm;
    for (FdoInt32 i = 0; i < cnt; i++)
    {
        mInsertSql.append(propNames->GetString(i));
        mInsertSql.append(L",");
        valuesQm.append(L"?,");
    }
    valuesQm.resize(valuesQm.size()-1);
    valuesQm.append(L")");

    mInsertSql.resize(mInsertSql.size()-1);
    mInsertSql.append(L") VALUES (");
    mInsertSql.append(valuesQm);
}

FdoString* FdoRdbmsSimpleInsertCommand::SetAutoGeneratedValue(const FdoSmLpDataPropertyDefinition* dataProp, FdoPropertyValue* propValue)
{
    FdoInt64 nextSeq = 0;
    FdoString* sequenceName = dataProp->GetSequenceName();

    if (!sequenceName || *sequenceName == L'\0')
        sequenceName = DBI_FEATURE_SEQUENCEW;

    nextSeq = mConnection->GetGdbiCommands()->NextSequenceNumber(sequenceName);

    FdoPtr<FdoInt64Value> autoGenValue = static_cast<FdoInt64Value*>(propValue->GetValue());
    autoGenValue->SetInt64 (nextSeq);
    return sequenceName;
}

void FdoRdbmsSimpleInsertCommand::SanitizePropertyValues( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection *propValCollection, FdoStringCollection* propNames, bool& containsObjectProperties)
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

void FdoRdbmsSimpleInsertCommand::RebindValues()
{
    FdoInt32 cnt = mPropertyValues->GetCount();
    for (int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoPropertyValue> propVal = mPropertyValues->GetItem(i);
        FdoPtr<FdoValueExpression> exp = propVal->GetValue();
        mBindProps.at(i).first = static_cast<FdoLiteralValue*>(exp.p);
    }

    cnt = mAutoGenPropertyValues->GetCount();
    for (int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoPropertyValue> propVal = mAutoGenPropertyValues->GetItem(i);
        std::vector< std::pair< std::wstring, short > >::reference cref = mAutoGenPropertyType.at(i);
        if (cref.second >= 10 && cref.first.size() != 0) // autogenerated and not autoincrement?
        {
            FdoInt64 nextSeq = mConnection->GetGdbiCommands()->NextSequenceNumber(cref.first.c_str());
            FdoPtr<FdoInt64Value> autoGenValue = static_cast<FdoInt64Value*>(propVal->GetValue());
            autoGenValue->SetInt64 (nextSeq);
        }
    }
}

void FdoRdbmsSimpleInsertCommand::FetchAutoincrementedIdValues()
{
    FdoInt32 cnt = mAutoGenPropertyValues->GetCount();
    for (int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoPropertyValue> propVal = mAutoGenPropertyValues->GetItem(i);
        std::vector< std::pair< std::wstring, short > >::reference cref = mAutoGenPropertyType.at(i);
        if (cref.second == 0 && cref.first.size() != 0) // autogenerated and autoincrement?
        {
            FdoInt64 nextSeq = mConnection->GetGdbiCommands()->NextSequenceNumber(cref.first.c_str());
            FdoPtr<FdoInt64Value> autoGenValue = static_cast<FdoInt64Value*>(propVal->GetValue());
            autoGenValue->SetInt64 (nextSeq);
        }
    }
}

void FdoRdbmsSimpleInsertCommand::UpdateCustomInsert()
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
}

FdoIFeatureReader* FdoRdbmsSimpleInsertCommand::Execute ()
{
    if (!mConnection)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if (!mClassName)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_35, "Class is null"));

    // if we have a complex insert just execute the sub command
    if (mBackupCmd != NULL)
    {
        UpdateCustomInsert();
        return mBackupCmd->Execute();
    }

    const FdoSmLpClassDefinition* classDefinition = mConnection->GetSchemaUtil()->GetClass(mClassName->GetText());

    if (mInsertSql.size() == 0 || mPropertyValues->GetCollectionChanged())
        PrepareInsert(classDefinition);
    else
        RebindValues();

    if (mContainsObjectProperties || mInsertSql.size() == 0)
    {
        if (mBackupCmd == NULL)
            mBackupCmd = FdoRdbmsInsertCommand::Create(mFdoConnection);
        // call the custom insert;
        UpdateCustomInsert();
        return mBackupCmd->Execute();
    }
    GdbiConnection* gdbiConn = mConnection->GetGdbiConnection();

    bool bBeginTransaction = false;
    try
    {
        if (mFdoConnection->GetIsTransactionStarted() == false)
        {
            mConnection->GetGdbiCommands()->tran_begin(TRANSACTION_NAME_SINS);
            bBeginTransaction = true;
        }
        if (m_qid == -1)
        {
            mConnection->GetGdbiCommands()->sql ((wchar_t *)(const wchar_t *) mInsertSql.c_str(), &m_qid);
            mBindHelper->BindParameters(mConnection->GetGdbiCommands(), m_qid, &mBindProps);
        }
        else
            mBindHelper->BindValues(mConnection->GetGdbiCommands(), m_qid, &mBindProps);

        mConnection->GetGdbiCommands()->execute(m_qid);
        FetchAutoincrementedIdValues();
        
        if (bBeginTransaction)
        {
            bBeginTransaction = false;
            mConnection->GetGdbiCommands()->tran_end(TRANSACTION_NAME_SINS);
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

    // make a copy of return values
    FdoPtr<FdoPropertyValueCollection> retPropertyValues = FdoPropertyValueCollection::Create();
    int cnt = mAutoGenPropertyValues->GetCount();
    for (int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoPropertyValue> propVal = mAutoGenPropertyValues->GetItem(i);
        FdoPtr<FdoPropertyValue> copyPval = ClonePropertyValue(propVal);
        retPropertyValues->Add(copyPval);
    }

    cnt = mAutoGenPropertyType.size();
    for (int i = 0; i < cnt; i++)
    {
        std::vector< std::pair< std::wstring, short > >::reference cref = mAutoGenPropertyType.at(i);
        if (cref.second >= 10 && cref.first.size() == 0) // PK's
        {
            FdoPtr<FdoPropertyValue> propVal = mPropertyValues->GetItem(cref.second-10);
            FdoPtr<FdoPropertyValue> copyPval = ClonePropertyValue(propVal);
            retPropertyValues->Add(copyPval);
        }
    }
    
    if (mHasRevisionNumber)
    {
        FdoPtr<FdoPropertyValue> revNum = FdoPropertyValue::Create();
        FdoPtr<FdoDataValue> revValue = FdoDataValue::Create ((FdoInt64)0);
        revNum->SetValue(revValue );
        revNum->SetName(L"RevisionNumber");
        retPropertyValues->Add( revNum );
    }
    return FdoRdbmsFeatureInfoReader::Create (retPropertyValues, classDefinition );
}

FdoPropertyValue* FdoRdbmsSimpleInsertCommand::ClonePropertyValue(FdoPropertyValue* prop)
{
    FdoPtr<FdoValueExpression> propVal = prop->GetValue();
    FdoExpressionItemType expType = propVal->GetExpressionType();
    if (expType != FdoExpressionItemType_DataValue)
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_54, "Unhandled type: %1$d", 10+(int)expType));
    
    FdoPropertyValue* retVal = NULL;
    FdoPtr<FdoIdentifier> propName = prop->GetName();
    FdoPtr<FdoIdentifier> copyName = FdoIdentifier::Create(propName->GetText());

    FdoDataValue* pDataVal = static_cast<FdoDataValue*>(propVal.p);
    FdoDataType dtType = pDataVal->GetDataType();
    switch(dtType)
    {
    case FdoDataType_Boolean:
        {
            FdoPtr<FdoDataValue> dtv = FdoBooleanValue::Create(((FdoBooleanValue*)pDataVal)->GetBoolean());
            retVal = FdoPropertyValue::Create(copyName, dtv);
        }
        break;
    case FdoDataType_Byte:
        {
            FdoPtr<FdoDataValue> dtv = FdoByteValue::Create(((FdoByteValue*)pDataVal)->GetByte());
            retVal = FdoPropertyValue::Create(copyName, dtv);
        }
        break;
    case FdoDataType_DateTime:
        {
            FdoPtr<FdoDataValue> dtv = FdoDateTimeValue::Create(((FdoDateTimeValue*)pDataVal)->GetDateTime());
            retVal = FdoPropertyValue::Create(copyName, dtv);
        }
        break;
    case FdoDataType_Decimal:
    case FdoDataType_Double:
        {
            FdoPtr<FdoDataValue> dtv = FdoDoubleValue::Create(((FdoDoubleValue*)pDataVal)->GetDouble());
            retVal = FdoPropertyValue::Create(copyName, dtv);
        }
        break;
    case FdoDataType_Int16:
        {
            FdoPtr<FdoDataValue> dtv = FdoInt16Value::Create(((FdoInt16Value*)pDataVal)->GetInt16());
            retVal = FdoPropertyValue::Create(copyName, dtv);
        }
        break;
    case FdoDataType_Int32:
        {
            FdoPtr<FdoDataValue> dtv = FdoInt32Value::Create(((FdoInt32Value*)pDataVal)->GetInt32());
            retVal = FdoPropertyValue::Create(copyName, dtv);
        }
        break;
    case FdoDataType_Int64:
        {
            FdoPtr<FdoDataValue> dtv = FdoInt64Value::Create(((FdoInt64Value*)pDataVal)->GetInt64());
            retVal = FdoPropertyValue::Create(copyName, dtv);
        }
        break;
    case FdoDataType_Single:
        {
            FdoPtr<FdoDataValue> dtv = FdoSingleValue::Create(((FdoSingleValue*)pDataVal)->GetSingle());
            retVal = FdoPropertyValue::Create(copyName, dtv);
        }
        break;
    case FdoDataType_String:
        {
            FdoPtr<FdoDataValue> dtv = FdoStringValue::Create(((FdoStringValue*)pDataVal)->GetString());
            retVal = FdoPropertyValue::Create(copyName, dtv);
        }
        break;
    default:
        throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_54, "Unhandled type: %1$d", dtType));
    }
    return retVal;
}