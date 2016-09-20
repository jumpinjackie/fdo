#ifndef FDORDBMSSIMPLEDELETECOMMAND_H
#define FDORDBMSSIMPLEDELETECOMMAND_H     1
/*
 * Copyright (C) 2004-2012  Autodesk, Inc.
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

#include "FdoRdbms.h"
#include "FdoRdbmsFeatureCommand.h"
#include "Fdo/Other/FdoRdbmsSQLBuilder.h"

class DbiConnection;
class FdoRdbmsPropBindHelper;
class FdoRdbmsDeleteCommand;

// this update command do not handle lock, update association/object properties
class FdoRdbmsSimpleDeleteCommand : public FdoIDelete
{
private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsSimpleDeleteCommand (const FdoRdbmsSimpleDeleteCommand& right);

    FdoRdbmsSimpleDeleteCommand ();
    FdoRdbmsSimpleDeleteCommand (FdoIConnection *connection);

protected:
    virtual ~FdoRdbmsSimpleDeleteCommand();

    virtual void Dispose() { delete this; }

private:
    // function called when we have prepared delete and the property value collection changes
    void FlushDelete();
    void RebindValues();
    void PrepareDelete(const FdoSmLpClassDefinition* classDefinition);
    void UpdateCustomDelete();

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsSimpleDeleteCommand& operator=(const FdoRdbmsSimpleDeleteCommand &right);

    static FdoRdbmsSimpleDeleteCommand* Create(FdoIConnection *connection);

    //    Executes the update command and returns the number of
    //    modified instances.
    virtual FdoInt32 Execute ();

    FdoILockConflictReader* GetLockConflicts() { return NULL; }

    virtual FdoIdentifier* GetClassNameRef() { return mClassName; }

    virtual FdoIdentifier* GetFeatureClassName() { return FDO_SAFE_ADDREF(mClassName); }

    // Sets the name of the class to be operated upon as an Identifier.
    virtual void SetFeatureClassName(FdoIdentifier* value);

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
        FlushDelete();
        FDO_SAFE_RELEASE(mFilter);
        mFilter = FDO_SAFE_ADDREF(value);
    }
    virtual void SetFilter(FdoString* value)
    {
        FDO_SAFE_RELEASE(mFilter);
        FlushDelete();
        if (value != NULL)
            mFilter = FdoFilter::Parse(value);
    }

protected:
    DbiConnection* mConnection;
    FdoRdbmsConnection* mFdoConnection;

private:
    FdoRdbmsFdoParameterValueCollection* mParmeterValues;
    FdoIdentifier*  mClassName;
    FdoFilter*      mFilter;

    std::wstring mDeleteSql;
    bool mContainsObjectProperties;
    bool mIsObjectObject;
    FdoRdbmsPropBindHelper* mBindHelper;
    std::vector< std::pair< FdoLiteralValue*, FdoInt64 > > mBindProps;
    std::vector< std::pair< size_t, size_t > > mParamMapping;
    int m_qid;

    FdoRdbmsDeleteCommand* mBackupCmd;
};

#endif // FDORDBMSSIMPLEDELETECOMMAND_H

