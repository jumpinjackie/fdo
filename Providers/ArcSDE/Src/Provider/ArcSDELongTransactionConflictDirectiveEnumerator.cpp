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

ArcSDELongTransactionConflictDirectiveEnumerator::ArcSDELongTransactionConflictDirectiveEnumerator (ArcSDEConnection* connection) :
    mConnection (connection),
    mConflicts (ConflictCollection::Create ()),
    mListIndex (-1),
    mIdIndex (-1)
{
    FDO_SAFE_ADDREF (mConnection.p);
}

ArcSDELongTransactionConflictDirectiveEnumerator::~ArcSDELongTransactionConflictDirectiveEnumerator (void)
{
    for (int i = 0; i < mConflicts->GetCount (); i++)
        delete (*mConflicts)[i];
    mConflicts->Release ();
}

void ArcSDELongTransactionConflictDirectiveEnumerator::AddConflictList (ConflictList* list)
{
    int index;
    ConflictList* old_list;
    Conflict* conflict;

    index = -1;
    for (int i = 0; ((i < mConflicts->GetCount ()) && (-1 == index)); i++)
        if (0 == wcscmp ((*mConflicts)[i]->mClassName, list->mClassName))
            index = i;
    if (-1 != index)
    {
        list->Sort ();
        old_list = (*mConflicts)[index];
        for (int i = 0; i < old_list->GetCount (); i++)
        {
            conflict = (*(old_list->mIds))[i];
            list->setResolution (conflict);
        }
        (*mConflicts)[index] = list;
        delete old_list;
    }
    else
        mConflicts = ConflictCollection::Append (mConflicts, list);
}

ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList* ArcSDELongTransactionConflictDirectiveEnumerator::GetConflictList (FdoString* cls)
{
    ConflictList* ret;

    ret = NULL;

    for (int i = 0; ((i < mConflicts->GetCount ()) && (NULL == ret)); i++)
        if (0 == wcscmp ((*mConflicts)[i]->mClassName, cls))
            ret = (*mConflicts)[i];

    return (ret);
}

bool ArcSDELongTransactionConflictDirectiveEnumerator::HasConflicts ()
{
    bool ret;

    ret = (0 != GetCount ());
    if (ret)
    {
        // check dispositions
        ret = false;
        Reset ();
        while (ReadNext () && !ret)
            if (FdoLongTransactionConflictResolution_Unresolved == GetResolution ())
                ret = true;
        Reset ();
    }

    return (ret);
}

void ArcSDELongTransactionConflictDirectiveEnumerator::ValidateIndices ()
{
    if (-1 == mListIndex)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_READER_NOT_READY, "Must %1$ls prior to accessing reader.", L"ReadNext"));

    if ((mListIndex >= mConflicts->GetCount ()) || (mIdIndex >= (*mConflicts)[mListIndex]->mIds->GetCount ()))
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_EXHAUSTED, "Reader is exhausted."));
}

void ArcSDELongTransactionConflictDirectiveEnumerator::empty ()
{
    for (int i = 0; i < mConflicts->GetCount (); i++)
        delete (*mConflicts)[i];
    ArcSDELongTransactionConflictDirectiveEnumerator::ConflictCollection::SetSize (mConflicts, 0);
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void ArcSDELongTransactionConflictDirectiveEnumerator::Dispose ()
{
    delete this;
}

/// <summary>Gets the class name of the current feature in conflict.</summary>
/// <returns>Returns the class name</returns> 
FdoString* ArcSDELongTransactionConflictDirectiveEnumerator::GetFeatureClassName ()
{
    ValidateIndices ();

    return ((*mConflicts)[mListIndex]->mClassName);
}

/// <summary>Checks if we're the only one hanging onto the GetIdentity objects.</summary>
// <returns>Returns true if we should allocate another set of identity objects.</returns> 
bool ArcSDELongTransactionConflictDirectiveEnumerator::Referenced ()
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

/// <summary>Returns FdoPropertyValueCollection containing the property values that
/// uniquely identify the current feature in conflict.</summary>
/// <returns>Returns FdoPropertyValueCollection</returns> 
FdoPropertyValueCollection* ArcSDELongTransactionConflictDirectiveEnumerator::GetIdentity ()
{
    FdoPropertyValueCollection* ret;

    ValidateIndices ();

    if (Referenced ())
    {   // new objects required
        mIdentity = FdoPropertyValueCollection::Create ();
        FdoPtr<FdoInt32Value> value = FdoInt32Value::Create ((*((*mConflicts)[mListIndex]->mIds))[mIdIndex]->mId);
        FdoPtr<FdoPropertyValue> property = FdoPropertyValue::Create ((*mConflicts)[mListIndex]->mRowIdProperty, value);
        mIdentity->Add (property);
    }
    else
    {  // the user isn't holding on to the id, so reuse it
       FdoPtr<FdoPropertyValue> property = mIdentity->GetItem (0);
       property->SetName ((*mConflicts)[mListIndex]->mRowIdProperty);
       FdoPtr<FdoInt32Value> value = (FdoInt32Value*)property->GetValue ();
       value->SetInt32 ((*((*mConflicts)[mListIndex]->mIds))[mIdIndex]->mId);
    }
    ret = mIdentity.p;
    ret->AddRef ();

    return (ret);
}

/// <summary>Gets the FdoLongTransactionConflictResolution value to use for the current
/// feature. The default value is FdoLongTransactionConflictResolution_Child.</summary>
/// <returns>Returns FdoLongTransactionConflictResolution value</returns> 
FdoLongTransactionConflictResolution ArcSDELongTransactionConflictDirectiveEnumerator::GetResolution ()
{
    ValidateIndices ();

    return ((*(((*mConflicts)[mListIndex])->mIds))[mIdIndex]->mResolution);
}

/// <summary>Sets the FdoLongTransactionConflictResolution value to use for the current
/// feature. The default value is FdoLongTransactionConflictResolution_Child.</summary>
/// <param name="value">Input the FdoLongTransactionConflictResolution value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDELongTransactionConflictDirectiveEnumerator::SetResolution (FdoLongTransactionConflictResolution value)
{
    ValidateIndices ();

    (*(((*mConflicts)[mListIndex])->mIds))[mIdIndex]->mResolution = value;
}

/// <summary>Returns the number of conflicts reported by this enumerator.</summary>
/// <returns>Returns the number of conflicts</returns> 
FdoInt32 ArcSDELongTransactionConflictDirectiveEnumerator::GetCount ()
{
    FdoInt32 ret;

    ret = 0;

    for (int i = 0; i < mConflicts->GetCount (); i++)
        ret += (*mConflicts)[i]->mIds->GetCount ();

    return (ret);
}

/// <summary>Advances the enumerator to the conflict. The default position of the
/// enumerator is prior to the first item. Thus, you must call ReadNext
/// to begin accessing any data. ReadNext returns true until the end of the
/// directives is reached. After the end of the directives is passed,
/// subsequent calls to ReadNext return false until Reset is called.</summary>
/// <returns>Returns True until the end of the directives is reached</returns> 
bool ArcSDELongTransactionConflictDirectiveEnumerator::ReadNext ()
{
    bool ret;

    ret = false;

    if (-1 == mListIndex)
    {
        mListIndex = 0;
        mIdIndex = 0;
    }
    else
        mIdIndex++;
    ret = ((mListIndex < mConflicts->GetCount ()) && (mIdIndex < (*mConflicts)[mListIndex]->mIds->GetCount ()));
    if (!ret)
    {
        mListIndex++;
        mIdIndex = 0;
        ret = ((mListIndex < mConflicts->GetCount ()) && (mIdIndex < (*mConflicts)[mListIndex]->mIds->GetCount ()));
    }

    return (ret);
}

/// <summary>Sets the enumerator to its initial position, which is before the first
/// conflict.</summary>
/// <returns>Returns nothing</returns> 
void ArcSDELongTransactionConflictDirectiveEnumerator::Reset ()
{
    mListIndex = -1;
    mIdIndex = -1;
}

