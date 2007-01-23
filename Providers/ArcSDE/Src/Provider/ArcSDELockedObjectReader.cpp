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

ArcSDELockedObjectReader::ArcSDELockedObjectReader (ArcSDEConnection* connection) :
    mConnection (connection),
    mLocks (LockCollection::Create ()),
    mListIndex (-1),
    mIndex (-1),
    mNumLocks (0),
    mRowIds (NULL),
    mUserNames (NULL),
    mClassName (),
    mUser ()
{
    FDO_SAFE_ADDREF (mConnection.p);
}

ArcSDELockedObjectReader::~ArcSDELockedObjectReader (void)
{
    for (int i = 0; i < mLocks->GetCount (); i++)
        delete (*mLocks)[i];
    if (0 != mNumLocks)
        SE_table_free_rowlocks_list (mNumLocks, mRowIds, mUserNames);
}

/// <summary>Add a row id to the reader's list.</summary>
/// <param name="table">The table name of the locked object.</param>
/// <param name="id">The row id of the locked object.</param>
void ArcSDELockedObjectReader::AddIdentity (CHAR* table, LONG id)
{
    int index;
    LockList* list;

    index = -1;
    for (int i = 0; ((i < mLocks->GetCount ()) && (-1 == index)); i++)
        if (0 == strcmp ((*mLocks)[i]->mTableName, table))
            index = i;
    if (-1 != index)
    {
        list = (*mLocks)[index];
        list->mIds = list->mIds->Append (list->mIds, id);
    }
    else
    {
        list = new LockList (table);
        list->mIds = list->mIds->Append (list->mIds, id);
        mLocks.p = mLocks->Append (mLocks, list);
    }
}

/// <summary>Checks that the reader is in a valid state.</summary>
void ArcSDELockedObjectReader::ValidateIndices ()
{
    if ((-1 == mIndex) || (-1 == mListIndex))
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_READER_NOT_READY, "Must %1$ls prior to accessing reader.", L"ReadNext"));
    if ((-2 == mIndex) || (-2 == mListIndex))
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_CLOSED, "Reader is closed."));
    if ((mListIndex >= mLocks->GetCount ()) || (mIndex >= (*mLocks)[mListIndex]->mIds->GetCount ()))
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_EXHAUSTED, "Reader is exhausted."));
}

FdoClassDefinition* ArcSDELockedObjectReader::GetFeatureClass ()
{
    CHAR* table;
    wchar_t* wtable;
    FdoPtr<FdoClassDefinition> ret;

    table = (*mLocks)[mListIndex]->mTableName;
    multibyte_to_wide (wtable, table);
    ret = mConnection->TableToClass (wtable);

    return (FDO_SAFE_ADDREF(ret.p));
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void ArcSDELockedObjectReader::Dispose ()
{
    delete this;
}

/// <summary>Gets the class name of the feature currently being read.</summary>
/// <returns>Returns the name of the class.</returns> 
FdoString* ArcSDELockedObjectReader::GetFeatureClassName ()
{
    ValidateIndices ();

    if (mClassName == L"")
        mClassName = FdoPtr<FdoClassDefinition>(GetFeatureClass ())->GetQualifiedName ();

    return (mClassName);
}

/// <summary>Checks if we're the only one hanging onto the GetIdentity objects.</summary>
// <returns>Returns true if we should allocate another set of identity objects.</returns> 
bool ArcSDELockedObjectReader::Referenced ()
{
    FdoInt32 i;
    FdoInt32 j;
    bool ret;

    ret = true;

    if (mIdentity != NULL)
    {
        mIdentity.p->AddRef ();
        i = mIdentity.p->Release ();
        if (2 > i)
        {
            FdoPropertyValue* property = mIdentity->GetItem (0);
            FdoInt32Value* value = (FdoInt32Value*)property->GetValue ();
            j = value->Release ();
            i = property->Release ();
            if ((2 > i) && (2 > j))
                ret = false;
        }
    }

    return (ret);
}

/// <summary>Returns an FdoPropertyValueCollection containing the property values
/// that uniquely identify the feature currently being read.</summary>
/// <returns>Returns the property collection identifying the feature.</returns> 
FdoPropertyValueCollection* ArcSDELockedObjectReader::GetIdentity ()
{
    LONG result;
    SE_REGINFO  registration;
    LONG type;
    CHAR column[SE_MAX_COLUMN_LEN];
    wchar_t* column_name;
    FdoPtr<FdoClassDefinition> definition;
    FdoString* property_name;
    FdoPtr<FdoInt32Value> value;
    FdoPtr<FdoPropertyValue> property;

    ValidateIndices ();

    if (Referenced ())
    {   // new objects required
        result = SE_reginfo_create (&registration);
        handle_sde_err<FdoCommandException> (mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_CREATE, "Table registration info could not be created.");
        result = SE_registration_get_info (mConnection->GetConnection (), (*mLocks)[mListIndex]->mTableName, registration);
        handle_sde_err<FdoCommandException> (mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");
        result = SE_reginfo_get_rowid_column (registration, column, &type);
        handle_sde_err<FdoCommandException> (mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"rowid column");
        SE_reginfo_free (registration);
        multibyte_to_wide (column_name, column);
        definition = GetFeatureClass ();
        property_name = mConnection->ColumnToProperty (definition, column_name);
        mIdentity = FdoPropertyValueCollection::Create ();
        value = FdoInt32Value::Create ((*(((*mLocks)[mListIndex])->mIds))[mIndex]);
        property = FdoPropertyValue::Create (property_name, value);
        mIdentity->Add (property);
    }
    else
    {  // the user isn't holding on to the id, and they're initialized, so reuse it
       property = mIdentity->GetItem (0);
       value = (FdoInt32Value*)property->GetValue ();
       value->SetInt32 ((*(((*mLocks)[mListIndex])->mIds))[mIndex]);
    }

    return (FDO_SAFE_ADDREF (mIdentity.p));
}

/// <summary>Gets the name of user who holds the lock on the feature currently being
/// read.</summary>
/// <returns>Returns the lock owner.</returns> 
FdoString* ArcSDELockedObjectReader::GetLockOwner ()
{
    LockList* list;
    CHAR* table;
    LONG result;
    LONG id;
    wchar_t* user;

    ValidateIndices ();

    if (mUser == L"")
    {
        list = (*mLocks)[mListIndex];
        if (0 == mNumLocks)
        {
            table = list->mTableName;
            result =  SE_table_get_rowlocks (mConnection->GetConnection (), table, &mNumLocks, &mRowIds, &mUserNames);
            handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_GET_ROW_LOCK_LIST_FAILED, "Failed to get the row lock list.");
            // TODO: create a data structure of these row ids (and names) and sort it so a binary search can be used
            // linear search for now... could be costly
        }
        id = (*(list->mIds))[mIndex];
        user = NULL;
        for (int i = 0; i < mNumLocks; i++)
            if (id == mRowIds[i])
            {
                multibyte_to_wide (user, mUserNames[i]);
                mUser = user;
                break; // TODO: only first lock? what about multiple read locks?
            }
        if (NULL == user)
            // TODO: uh-oh, not found... what now?
            mUser = NlsMsgGet(ARCSDE_LOCK_OWNER_UNKNOWN, "<Unknown lock owner>");
    }

    return (mUser);
}

/// <summary>Gets the name of the long transaction for the feature currently being read.</summary>
/// <returns>Returns the name of the long transaction.</returns> 
FdoString* ArcSDELockedObjectReader::GetLongTransaction ()
{
    ValidateIndices ();
    return (SDE_DEFAULT_LT_NAME);
}

/// <summary>Gets the type of the lock held on the feature currently being read.</summary>
/// <returns>Returns the lock type.</returns> 
const FdoLockType ArcSDELockedObjectReader::GetLockType ()
{
    return (FdoLockType_Exclusive); // always for ArcSDE
}

/// <summary>Advances the reader to the next item. The default position of the 
/// reader is prior to the first item. Thus, you must call ReadNext
/// to begin accessing any data.</summary>
/// <returns>Returns true if there is a next item.</returns> 
bool ArcSDELockedObjectReader::ReadNext ()
{
    bool ret;

    ret = false;

    if ((-1 == mIndex) || (-1 == mListIndex))
    {
        mListIndex = 0;
        mIndex = 0;
    }
    else if ((-2 == mIndex) || (-2 == mListIndex))
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_CLOSED, "Reader is closed."));
    else
    {
        mIndex++;
        IdArray* ids = (*mLocks)[mListIndex]->mIds;
        if (mIndex >= ids->GetCount ())
        {
            mListIndex++;
            mIndex = 0;
            if (0 != mNumLocks)
            {   // reset the cached rowlock info when we change table
                SE_table_free_rowlocks_list (mNumLocks, mRowIds, mUserNames);
                mNumLocks = 0;
                mRowIds = NULL;
                mUserNames = NULL;
                mIdentity = NULL;
            }
        }
    }
    ret = ((mListIndex < mLocks->GetCount ()) && (mIndex < (*mLocks)[mListIndex]->mIds->GetCount ()));
    mClassName = L""; // reset cached class name
    mUser = L""; // reset cached user name

    return (ret);
}

/// <summary>Closes the FdoILockConflictReader object, freeing any resources it may
/// be holding.</summary>
/// <returns>Returns nothing.</returns> 
void ArcSDELockedObjectReader::Close ()
{
    mIndex = -2;
    mListIndex = -2;
}

