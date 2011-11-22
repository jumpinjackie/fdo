#ifndef FDORDBMSPREPINSERTCOMMAND_H
#define FDORDBMSPREPINSERTCOMMAND_H 1
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
class FdoRdbmsInsertCommand;

class FdoRdbmsSimpleInsertCommand : public FdoIInsert
{
private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsSimpleInsertCommand(const FdoRdbmsSimpleInsertCommand &right);

    FdoRdbmsSimpleInsertCommand ();

    FdoRdbmsSimpleInsertCommand (FdoIConnection *connection);

    // function called when we have prepared insert and the property value collection changes
    void FlushInsert();
    void PrepareInsert (const FdoSmLpClassDefinition* classDefinition);
    void SanitizePropertyValues (const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection *propValCollection, FdoStringCollection* propNames, bool& containsObjectProperties);
    void RebindValues();
    FdoPropertyValue* ClonePropertyValue(FdoPropertyValue* prop);
    void UpdateCustomInsert();

protected:
    virtual ~FdoRdbmsSimpleInsertCommand();
    virtual void Dispose() { delete this; }

public:

    static FdoRdbmsSimpleInsertCommand* Create(FdoIConnection *connection);
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsSimpleInsertCommand & operator=(const FdoRdbmsSimpleInsertCommand &right);

    virtual FdoIConnection* GetConnection() { return FDO_SAFE_ADDREF(mFdoConnection); }
    virtual int GetCommandTimeout() { return 0; }
    virtual void SetCommandTimeout(int value) {}
    virtual FdoITransaction* GetTransaction() { return NULL; }
    virtual void SetTransaction(FdoITransaction* value) {}
    virtual void Prepare() {}
    virtual void Cancel() {}

    virtual FdoParameterValueCollection* GetParameterValues() { return NULL; }

    //    Executes the insert command.
    virtual FdoIFeatureReader* Execute ();

    virtual FdoPropertyValueCollection* GetPropertyValues() { return FDO_SAFE_ADDREF(mPropertyValues); }

    virtual FdoBatchParameterValueCollection* GetBatchParameterValues() { return NULL; }

    virtual FdoIdentifier* GetClassNameRef() { return mClassName; }

    virtual FdoIdentifier* GetFeatureClassName() { return FDO_SAFE_ADDREF(mClassName); }

    // Sets the name of the class to be operated upon as an Identifier.
    virtual void SetFeatureClassName(FdoIdentifier* value)
    {
        FlushInsert();
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

protected:
    DbiConnection* mConnection;
    FdoRdbmsConnection* mFdoConnection;

private:
    FdoRdbmsPropertyValueCollection* mPropertyValues;
    FdoIdentifier*  mClassName;
    FdoPropertyValueCollection *mAutoGenPropertyValues;
    std::vector< std::pair< std::wstring, short > > mAutoGenPropertyType; // sequence Name & short: 0 = autoincrement, 10 + idx = autogenerated

    // use for prepared insert
    std::wstring mInsertSql;
    FdoPropertyValueCollection *mLocalPropertyValues;
    bool mContainsObjectProperties;
    bool mIsObjectObject;
    bool mContainsAutoGenerated;
    FdoRdbmsPropBindHelper* mBindHelper;
    std::vector< std::pair< FdoLiteralValue*, FdoInt64 > > mBindProps;
    bool mHasRevisionNumber;
    int m_qid;

    FdoRdbmsInsertCommand* mBackupCmd;
};

#endif // FDORDBMSPREPINSERTCOMMAND_H
