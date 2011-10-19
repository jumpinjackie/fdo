#ifndef FDORDBMSSIMPLESELECTCOMMAND_H
#define FDORDBMSSIMPLESELECTCOMMAND_H 1
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

#include "FdoRdbms.h"

// FeatureCommand
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"
#include "Fdo/Other/FdoRdbmsSQLBuilder.h"

class FdoRdbmsException;
class FdoRdbmsPropBindHelper;
class FdoRdbmsSelectCommand;

class FdoRdbmsSimpleSelectCommand : public FdoISelect
{
private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsSimpleSelectCommand(const FdoRdbmsSimpleSelectCommand &right);

    FdoRdbmsSimpleSelectCommand ();

    FdoRdbmsSimpleSelectCommand (FdoIConnection *connection);

    // function called when we have prepared insert and the property value collection changes
    void FlushSelect();
    void PrepareSelect (const FdoSmLpClassDefinition* classDefinition);
    void RebindValues();
    void UpdateCustomSelect();
    bool HasObjectProperty (const FdoSmLpClassDefinition *classDefinition);

protected:
    virtual ~FdoRdbmsSimpleSelectCommand();
    virtual void Dispose() { delete this; }

public:

    static FdoRdbmsSimpleSelectCommand* Create(FdoIConnection *connection);
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsSimpleSelectCommand & operator=(const FdoRdbmsSimpleSelectCommand &right);

    virtual FdoIdentifierCollection*    GetPropertyNames()     { return FDO_SAFE_ADDREF(m_properties); }
        
    virtual FdoIdentifier* GetFeatureClassName() { return FDO_SAFE_ADDREF(mClassName); }

    // Sets the name of the class to be operated upon as an Identifier.
    virtual void SetFeatureClassName(FdoIdentifier* value)
    {
        FlushSelect();
        FDO_SAFE_RELEASE(mClassName);

        if( NULL == mConnection )
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

        if (value)
        {
            const FdoSmLpClassDefinition *classDefinition = mConnection->GetSchemaUtil()->GetClass(value->GetText());
            if ( classDefinition == NULL )
                throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_333, "Class '%1$ls' not found", value->GetText()));

            if (classDefinition->GetIsAbstract())
                throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_276, "Setting an abstract class is not currently supported for this command"));
            
            FdoString* name = value->GetText();
            while(*name != '\0' && *name != '.') name++;
            mIsObjectObject = (*name == L'.');

            mConnection->GetSchemaUtil()->CheckClass(value->GetText());

            mClassName = FDO_SAFE_ADDREF(value);
        }
    }

    // Sets the name of the class to be operated upon as an String
    virtual void SetFeatureClassName(FdoString* value)
    {
		FdoPtr<FdoIdentifier> idf = FdoIdentifier::Create(value);
		SetFeatureClassName(idf);
    }

    virtual FdoIFeatureReader* Execute();
               
    virtual FdoIdentifierCollection* GetOrdering()
    { 
        if (!m_orderingProps)
            m_orderingProps = FdoRdbmsIdentifierCollection::Create(); 

        return FDO_SAFE_ADDREF(m_orderingProps);
    }

    //irrelevant stuff
    virtual FdoLockType                 GetLockType()                   { return FdoLockType_Exclusive;};
    virtual void                        SetLockType(FdoLockType value)  { };
    virtual FdoLockStrategy             GetLockStrategy()               { return FdoLockStrategy_All;};
    virtual void                        SetLockStrategy(FdoLockStrategy value) { };
    virtual FdoIFeatureReader*          ExecuteWithLock()               { return NULL; }
    virtual FdoILockConflictReader*     GetLockConflicts()              { return NULL; }
    virtual void                        SetOrderingOption(FdoOrderingOption option) {m_option = option;}
    virtual FdoOrderingOption           GetOrderingOption()             { return m_option; }

    virtual FdoIConnection* GetConnection() { return FDO_SAFE_ADDREF(mFdoConnection); }
    virtual int GetCommandTimeout() { return 0; }
    virtual void SetCommandTimeout(int value) {}
    virtual FdoITransaction* GetTransaction() { return NULL; }
    virtual void SetTransaction(FdoITransaction* value) {}
    virtual void Prepare() {}
    virtual void Cancel() {}

    // join methods
    virtual FdoJoinCriteriaCollection* GetJoinCriteria()
    {
        if (m_joinCriteria == NULL)
            m_joinCriteria = FdoRdbmsJoinCriteriaCollection::Create();

        return FDO_SAFE_ADDREF(m_joinCriteria);
    }

    virtual FdoString* GetAlias()
    {
        return (m_alias == NULL) ? NULL : m_alias->GetName();
    }

    virtual void SetAlias(FdoString* alias)
    {
        FlushSelect();
        FDO_SAFE_RELEASE(m_alias);
        if (alias != NULL && *alias != '\0')
            m_alias = FdoIdentifier::Create(alias);
    }

    virtual FdoFilter* GetFilter() { return FDO_SAFE_ADDREF(mFilter); }
    virtual void SetFilter(FdoFilter* value)
    {
        FlushSelect();
        FDO_SAFE_RELEASE(mFilter);
        mFilter = FDO_SAFE_ADDREF(value);
    }
    virtual void SetFilter(FdoString* value)
    {
        FlushSelect();
        FDO_SAFE_RELEASE(mFilter);
        if (value != NULL)
            mFilter = FdoFilter::Parse(value);
    }

    virtual FdoParameterValueCollection* GetParameterValues()
    { 
        if( m_pParmeterValues == NULL )
            m_pParmeterValues = FdoRdbmsFdoParameterValueCollection::Create();

        return FDO_SAFE_ADDREF(m_pParmeterValues);
    }

protected:
    DbiConnection* mConnection;
    FdoRdbmsConnection* mFdoConnection;
private:
    std::wstring mSelectSql;
    FdoIdentifier*  mClassName;
    bool mContainsObjectProperties;
    bool mIsObjectObject;
    FdoRdbmsPropBindHelper* mBindHelper;
    std::vector< std::pair< FdoLiteralValue*, FdoInt64 > > mBindProps;
    std::vector< std::pair< size_t, size_t > > mParamMapping;
    //std::vector< GdbiQueryIdentifier* > mStatements;

    FdoRdbmsIdentifierCollection* m_orderingProps;
    FdoRdbmsIdentifierCollection* m_properties;
    std::map<std::wstring, FdoOrderingOption> m_orderingOptions;
    FdoOrderingOption m_option;
    FdoRdbmsJoinCriteriaCollection* m_joinCriteria;
    FdoIdentifier* m_alias;
    FdoFilter* mFilter;
    FdoRdbmsFdoParameterValueCollection* m_pParmeterValues;

    FdoRdbmsSelectCommand* mBackupCmd;
};

#endif // FDORDBMSSIMPLESELECTCOMMAND_H
