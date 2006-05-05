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

#include <sdeerno.h>

ArcSDELockConflictReader::ArcSDELockConflictReader (ArcSDEConnection* connection, FdoString* cls, CHAR* table, FdoString* property) :
    mConnection (connection),
    mClassName (cls),
#ifdef _WIN32
    mTable (_strdup (table)),
#else
    mTable (strdup (table)),
#endif
    mIdProperty (property),
    mIds (IdArray::Create ()),
    mIndex (-1),
    mNumLocks (0),
    mRowIds (NULL),
    mUserNames (NULL),
    mUser (),
    mSelect (NULL),
    mReader (NULL)
{
    FDO_SAFE_ADDREF (mConnection.p);
}

ArcSDELockConflictReader::~ArcSDELockConflictReader (void)
{
    free (mTable);
    mIds->Release ();
    if (0 != mNumLocks)
        SE_table_free_rowlocks_list (mNumLocks, mRowIds, mUserNames);
    if (NULL != GetSelectCommand ())
    {
        GetSelectCommand ()->SetLockConflictReader (NULL);
        GetSelectCommand ()->Release ();
    }
}

/// <summary>Add a row id to the reader's list.</summary>
/// <param name="id">The row id of the conflict.</param>
void ArcSDELockConflictReader::AddIdentity (LONG id)
{
    mIds = IdArray::Append (mIds, id);
}

/// <summary>Checks that the reader is in a valid state.</summary>
void ArcSDELockConflictReader::ValidateIndices ()
{
    if (-1 == mIndex)
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_READER_NOT_READY, "Must %1$ls prior to accessing reader.", L"ReadNext"));
    if (-2 == mIndex)
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_CLOSED, "Reader is closed."));
    if (mIndex >= mIds->GetCount ())
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_EXHAUSTED, "Reader is exhausted."));
}

void ArcSDELockConflictReader::SetSelectCommand (ArcSDESelectCommand* select)
{
    mSelect = select;
    if (NULL != GetSelectCommand ())
    {
        // the conflict reader holds a soft reference to the select command
        // the select command holds a hard reference to the conflict reader
        GetSelectCommand ()->AddRef ();
        GetSelectCommand ()->SetLockConflictReader (this);
    }
}

ArcSDESelectCommand* ArcSDELockConflictReader::GetSelectCommand ()
{
    return (mSelect);
}

void ArcSDELockConflictReader::SetFeatureReader (ArcSDEFeatureReader* reader)
{
    // this code and the over-ridden Dispose() maintain the refcount on this object
    // artificially low by one, which is the refcount that would have been placed
    // on this object by the feature reader
    ArcSDEFeatureReader* buddy = GetFeatureReader ();
    mReader = reader;
    if (NULL == reader)
    {
        // our feature reader wants to die, he'll soon Release() us
        AddRef ();
        // we're no longer interested in him
        if (NULL != buddy)
            buddy->FdoIFeatureReader::Release ();
    }
    else
    {
        Release (); // our feature reader is letting us know he's interested, he's already done AddRef()
        GetFeatureReader ()->FdoIFeatureReader::AddRef (); // we're interested in him
    }
}

ArcSDEFeatureReader* ArcSDELockConflictReader::GetFeatureReader ()
{
    return (mReader);
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void ArcSDELockConflictReader::Dispose ()
{
    if (NULL == GetFeatureReader ())
        delete this; // we're stand-alone, OK to go bye-bye
    else
        // our feature reader still needs us, but we don't need it so Release()
        // this may lead to a recursive call through SetFeatureReader()
        // to this->Dispose() if his refcount goes to zero
        // because we're the only one referencing him,
        // but at that time the feature reader will be NULL
        GetFeatureReader ()->FdoIFeatureReader::Release ();
}

/// <summary>Gets the class name of the feature currently being read.</summary>
/// <returns>Returns the class name.</returns> 
FdoString* ArcSDELockConflictReader::GetFeatureClassName ()
{
    ValidateIndices ();

    return (mClassName); // always the same one for a conflict reader
}

/// <summary>Checks if we're the only one hanging onto the GetIdentity objects.</summary>
// <returns>Returns true if we should allocate another set of identity objects.</returns> 
bool ArcSDELockConflictReader::Referenced ()
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
/// that uniquely identify feature currently being read.</summary>
/// <returns>Returns the property collection identifying the feature.</returns> 
FdoPropertyValueCollection* ArcSDELockConflictReader::GetIdentity ()
{
    ValidateIndices ();

    if (Referenced ())
    {   // new objects required
        mIdentity = FdoPropertyValueCollection::Create ();
        FdoPtr<FdoInt32Value> value = FdoInt32Value::Create ((*mIds)[mIndex]);
        FdoPtr<FdoPropertyValue> property = FdoPropertyValue::Create (mIdProperty, value);
        mIdentity->Add (property);
    }
    else
    {  // the user isn't holding on to the id, so reuse it
       FdoPtr<FdoPropertyValue> property = mIdentity->GetItem (0);
       FdoPtr<FdoInt32Value> value = (FdoInt32Value*)property->GetValue ();
       value->SetInt32 ((*mIds)[mIndex]);
    }

    return (FDO_SAFE_ADDREF (mIdentity.p));
}

/// <summary>Gets the name of the owner that holds a lock on the feature 
/// currently being read.</summary>
/// <returns>Returns the name of the owner.</returns> 
FdoString* ArcSDELockConflictReader::GetLockOwner ()
{
    LONG result;
    LONG id;
    wchar_t* user;

    ValidateIndices ();

    if (mUser == L"")
    {
        if (0 == mNumLocks)
        {
            result =  SE_table_get_rowlocks (mConnection->GetConnection (), mTable, &mNumLocks, &mRowIds, &mUserNames);
            handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_GET_ROW_LOCK_LIST_FAILED, "Failed to get the row lock list.");
        }
        // TODO: create a data structure of these row ids (and names) and sort it so a binary search can be used
        // linear search for now... could be costly
        id = (*mIds)[mIndex];
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
FdoString* ArcSDELockConflictReader::GetLongTransaction ()
{
    ValidateIndices ();
    return (SDE_DEFAULT_LT_NAME);
}

/// <summary>Gets the conflict type for the conflict currently being read.</summary>
/// <returns>Returns the conflict type for the conflict currently being read.</returns> 
FdoConflictType ArcSDELockConflictReader::GetConflictType ()
{
    ValidateIndices ();
    return (FdoConflictType_LockConflict);
}

/// <summary>Advances the reader to the next item. The default position of the 
/// reader is prior to the first item. Thus, you must call ReadNext
/// to begin accessing any data.</summary>
/// <returns>Returns true if there is a next item.</returns> 
bool ArcSDELockConflictReader::ReadNext ()
{
    bool ret;

    ret = false;

    // in the case of a user accessing the conflicts before the reader (FdoISelect)
    // make sure that the conflicts are populated
    if (NULL != GetFeatureReader ())
        GetFeatureReader ()->PrepareStream ();

    if (-1 == mIndex)
        mIndex = 0;
    else if (-2 == mIndex)
        throw FdoException::Create (NlsMsgGet(ARCSDE_READER_CLOSED, "Reader is closed."));
    else
        mIndex++;
    ret = mIndex < mIds->GetCount ();
    mUser = (FdoString*)NULL; // reset cached user name

    return (ret);
}

/// <summary>Closes the FdoILockConflictReader object, freeing any resources it may
/// be holding.</summary>
/// <returns>Returns nothing.</returns> 
void ArcSDELockConflictReader::Close ()
{
    mIndex = -2;
}

