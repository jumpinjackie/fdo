#ifndef FDORDBMSSIMPLEUPDATECOMMAND_H
#define FDORDBMSSIMPLEUPDATECOMMAND_H 1
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
#include "FdoRdbmsFeatureCommand.h"
#include "Fdo/Other/FdoRdbmsSQLBuilder.h"

class DbiConnection;
class FdoRdbmsPropBindHelper;
class FdoRdbmsUpdateCommand;

// this update command do not handle lock, update association/object properties
class FdoRdbmsSimpleUpdateCommand : public FdoIUpdate
{
private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsSimpleUpdateCommand (const FdoRdbmsSimpleUpdateCommand& right);

    FdoRdbmsSimpleUpdateCommand ();
    FdoRdbmsSimpleUpdateCommand (FdoIConnection *connection);

protected:
    virtual ~FdoRdbmsSimpleUpdateCommand();

    virtual void Dispose() { delete this; }

private:
    // function called when we have prepared insert and the property value collection changes
    void FlushUpdate();
    void RebindValues();
    void SanitizePropertyValues (const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection *propValCollection, FdoStringCollection* propNames, bool& containsObjectProperties);
    void PrepareUpdate(const FdoSmLpClassDefinition* classDefinition);
    void UpdateCustomUpdate();

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsSimpleUpdateCommand& operator=(const FdoRdbmsSimpleUpdateCommand &right);

    static FdoRdbmsSimpleUpdateCommand* Create(FdoIConnection *connection);

    //    Executes the update command and returns the number of
    //    modified instances.
    virtual FdoInt32 Execute ();

    FdoILockConflictReader* GetLockConflicts() { return NULL; }

    virtual FdoPropertyValueCollection* GetPropertyValues() { return FDO_SAFE_ADDREF(mPropertyValues); }

    virtual FdoIdentifier* GetClassNameRef() { return mClassName; }

    virtual FdoIdentifier* GetFeatureClassName() { return FDO_SAFE_ADDREF(mClassName); }

    // Sets the name of the class to be operated upon as an Identifier.
    virtual void SetFeatureClassName(FdoIdentifier* value)
    {
        FlushUpdate();
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
            // should we do that or caller should do it?
            mPropertyValues->Clear();
        }
    }

    // Sets the name of the class to be operated upon as an String
    virtual void SetFeatureClassName(FdoString* value)
    {
		FdoPtr<FdoIdentifier> idf = FdoIdentifier::Create(value);
		SetFeatureClassName(idf);
    }

    virtual FdoIConnection* GetConnection() { return FDO_SAFE_ADDREF(mFdoConnection); }
    virtual FdoParameterValueCollection* GetParameterValues() { return FDO_SAFE_ADDREF(mParmeterValues); }

    virtual FdoITransaction* GetTransaction()           { return NULL; }
    virtual void SetTransaction(FdoITransaction* value) { }
    virtual FdoInt32 GetCommandTimeout()                { return 0;}
    virtual void SetCommandTimeout(FdoInt32 value)      { }
    virtual void Prepare()                              { }
    virtual void Cancel()                               { }
    
    virtual FdoFilter* GetFilter() { return FDO_SAFE_ADDREF(mFilter); }
    virtual void SetFilter(FdoFilter* value)
    {
        FlushUpdate();
        FDO_SAFE_RELEASE(mFilter);
        mFilter = FDO_SAFE_ADDREF(value);
    }
    virtual void SetFilter(FdoString* value)
    {
        FDO_SAFE_RELEASE(mFilter);
        if (value != NULL)
            mFilter = FdoFilter::Parse(value);
    }

protected:
    DbiConnection* mConnection;
    FdoRdbmsConnection* mFdoConnection;

private:
    FdoRdbmsPropertyValueCollection* mPropertyValues;
    FdoRdbmsFdoParameterValueCollection* mParmeterValues;
    FdoIdentifier*  mClassName;
    FdoFilter*      mFilter;

    std::wstring mUpdateSql;
    bool mContainsObjectProperties;
    bool mIsObjectObject;
    FdoRdbmsPropBindHelper* mBindHelper;
    std::vector< std::pair< FdoLiteralValue*, FdoInt64 > > mBindProps;
    std::vector< std::pair< size_t, size_t > > mParamMapping;
    bool mHasRevisionNumber;
    int m_qid;

    FdoRdbmsUpdateCommand* mBackupCmd;
};

#endif // FDORDBMSSIMPLEUPDATECOMMAND_H
