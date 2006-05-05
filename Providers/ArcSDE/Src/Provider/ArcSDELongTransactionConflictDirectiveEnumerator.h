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

#ifndef ArcSDELongTransactionConflictDirectiveEnumerator_H
#define ArcSDELongTransactionConflictDirectiveEnumerator_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <stdlib.h>
#include <search.h>


class ArcSDELongTransactionConflictDirectiveEnumerator :
    public FdoILongTransactionConflictDirectiveEnumerator
{
    friend class ArcSDECommitLongTransactionCommand;


    class Conflict
    {
    public:
        FdoLongTransactionConflictResolution mResolution;
        FdoInt32 mId;

        Conflict (FdoInt32 id) :
            mResolution (FdoLongTransactionConflictResolution_Unresolved),
            mId (id)
        {
        }

    };

    static int OS__cdecl compare (const void* left, const void* right)
    {
        return ((*((Conflict**)left))->mId - (*((Conflict**)right))->mId);
    }


    typedef FdoArray<Conflict*> ConflictArray;

    class ConflictList
    {
    public:
        FdoStringP mClassName;
        FdoStringP mRowIdProperty;
        ConflictArray* mIds;
        bool mMarked; // used to remove unmodified lists
        
        ConflictList (FdoString* class_name, FdoString* row_id_property) :
            mClassName (class_name),
            mRowIdProperty (row_id_property)
        {
            mIds = ConflictArray::Create ();
            mMarked = true;
        }

        ~ConflictList ()
        {
            for (int i = 0; i < mIds->GetCount (); i++)
                delete (*mIds)[i];
            FDO_SAFE_RELEASE (mIds);
        }

        void AddConflict (Conflict* conflict)
        {
            mIds = ConflictArray::Append (mIds, conflict);
        }

        FdoInt32 GetCount ()
        {
            return (mIds->GetCount ());
        }

        void Sort ()
        {
            qsort (mIds->GetData (), mIds->GetCount (), sizeof (Conflict*), compare);
        }

        // assumes the ids have been sorted
        Conflict** Find (FdoInt32 id)
        {
            Conflict key(id);
            Conflict* test = &key;

            return ((Conflict**)bsearch (&test, mIds->GetData (), mIds->GetCount (), sizeof (Conflict*), compare));
        }

        // assumes the ids have been sorted
        // assigns the old resolution to the element in this list with the same id as the old one
        void setResolution (Conflict* old)
        {
            Conflict** element;

            element = (Conflict**)bsearch (&old, mIds->GetData (), mIds->GetCount (), sizeof (Conflict*), compare);
            if (NULL != element)
                (*element)->mResolution = old->mResolution;
            // note: it's not an error not to find the id,
            // it could have been resolved elsewise
        }
    };
    typedef FdoArray<ConflictList*> ConflictCollection;

    FdoPtr<ArcSDEConnection> mConnection;
    ConflictCollection* mConflicts;
    int mListIndex;
    int mIdIndex;

    // GetIdentity objects, reused if the refcount allows
    FdoPtr<FdoPropertyValueCollection> mIdentity;

protected:
    // 0-argument constructor to please FdoPtr::operator->() :
    ArcSDELongTransactionConflictDirectiveEnumerator() { ArcSDELongTransactionConflictDirectiveEnumerator(NULL); }

    ArcSDELongTransactionConflictDirectiveEnumerator (ArcSDEConnection* connection);
    virtual ~ArcSDELongTransactionConflictDirectiveEnumerator (void);

    // internal methods
    virtual void AddConflictList (ConflictList* list);
    virtual ConflictList* GetConflictList (FdoString* cls);
    virtual bool HasConflicts ();
    virtual void ValidateIndices ();
    virtual void empty ();
    virtual bool Referenced ();

public:
    // FdoIDisposable interface

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    void Dispose ();

    // FdoILongTransactionConflictDirectiveEnumerator overrides

    /// <summary>Gets the class name of the current feature in conflict.</summary>
    /// <returns>Returns the class name</returns> 
    FdoString* GetFeatureClassName ();

    /// <summary>Returns FdoPropertyValueCollection containing the property values that
    /// uniquely identify the current feature in conflict.</summary>
    /// <returns>Returns FdoPropertyValueCollection</returns> 
    FdoPropertyValueCollection* GetIdentity ();

    /// <summary>Gets the FdoLongTransactionConflictResolution value to use for the current
    /// feature. The default value is FdoLongTransactionConflictResolution_Child.</summary>
    /// <returns>Returns FdoLongTransactionConflictResolution value</returns> 
    FdoLongTransactionConflictResolution GetResolution ();

    /// <summary>Sets the FdoLongTransactionConflictResolution value to use for the current
    /// feature. The default value is FdoLongTransactionConflictResolution_Child.</summary>
    /// <param name="value">Input the FdoLongTransactionConflictResolution value</param> 
    /// <returns>Returns nothing</returns> 
    void SetResolution (FdoLongTransactionConflictResolution value);

    /// <summary>Returns the number of conflicts reported by this enumerator.</summary>
    /// <returns>Returns the number of conflicts</returns> 
    FdoInt32 GetCount ();

    /// <summary>Advances the enumerator to the conflict. The default position of the
    /// enumerator is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data. ReadNext returns true until the end of the
    /// directives is reached. After the end of the directives is passed,
    /// subsequent calls to ReadNext return false until Reset is called.</summary>
    /// <returns>Returns True until the end of the directives is reached</returns> 
    bool ReadNext ();

    /// <summary>Sets the enumerator to its initial position, which is before the first
    /// conflict.</summary>
    /// <returns>Returns nothing</returns> 
    void Reset ();
};

#endif // ArcSDELongTransactionConflictDirectiveEnumerator_H

