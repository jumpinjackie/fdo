#ifndef FDOSMPHDBOBJECTCOMPONENTLOADER_H
#define FDOSMPHDBOBJECTCOMPONENTLOADER_H		1
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

#ifdef _WIN32
#pragma once
#endif

//#include <Sm/Ph/DbObject.h>

// This class provides a template for lazy loading the components of a number of DbObjects.
// DbObject components include columns, keys, indexes, constraints and base object relationships.
// Most of these components are bulk fetched when the DbObjects are fetched. 
// However, indexes are fetched only when the indexes for a DbObject are asked for. It is anticipated
// that if the indexes for one DbObject are needed, the indexes for other DbObjects will also be 
// requested. Therefore, the requested indexes plus indexes for some of the other cached DbObjects are 
// loaded in the same select.
// This template handles lazy loading indexes but may be used for other components int the future.
//
// Template argument R must be set to the reader class for the type of component being loaded.
template <class R> class FdoSmPhDbObjectComponentLoader : public FdoSmDisposable
{
public:

    /// Constructs an instance of a component loader
    FdoSmPhDbObjectComponentLoader(
        FdoSmPhOwnerP owner,             // load components for DbObjects in this owner.
        FdoSmPhDbObjectsP dbObjects      // candidate dbObject list. Components will be loaded for requested
                                         // DbObject plus some of these candidates. 
    ) :
        mNextCandIdx(0)
    {
        mOwner = owner;
        mDbObjects = dbObjects;
        mCandidates = FdoDictionary::Create();   
    }

	~FdoSmPhDbObjectComponentLoader(void)
    {
    }

    FdoSmPhOwnerP GetOwner()
    {
        return mOwner;
    }

    // Main function that lazy loads the components
    void Load( 
        FdoStringP objectName,          // Requested DbObject (must be in mOwner)
        bool partial,                   // false: mDbObjects includes all objects in mOwner (none left to cache)
        FdoInt32 fetchSize              // Number of DbObjects to load components for.
    )
    {
        FdoSmPhDbObjectsP dbObjects = mDbObjects;
        // Can't bulk load if fetch size too small or no db objects
        // have been cached
        if ( (fetchSize < 2) || (dbObjects->GetCount() == 0) ) 
            return;

        // Check if requested dbObject is on the index candidates list
        FdoDictionaryP candidates = FdoDictionary::Create();
        FdoInt32 ix = mCandidates->IndexOf( objectName );
        FdoInt32 idx;

        if ( ix < 0 ) {
            // Put given DbObjects in candidates list.
            LoadCandidates( dbObjects, objectName, mNextCandIdx );
            ix = mCandidates->IndexOf( objectName );
        }

        // dbObject is still not a candidate, fall back to loading only the components
        // for that dbObject.
        if ( ix < 0 ) 
            return;

        FdoStringsP cands = FdoStringCollection::Create();
        FdoPtr<R> rdr;

        // The following block creates a component reader to fetch components for given object plus
        // a certain number of other candidates (bulk fetched for better performance).
        // This type of fetch is done by binding all of the candidates into a select statement.
        // When there are many candidates, it is more efficient to fetch compontes for all DbObjects for 
        // the owner. Therefore, the following is skipped if all dbObjects for this owner have been cached,
        // and at least half of them are candidates.
        //
        // TODO: check if 50% is the optimal threshold.
        if ( partial || (((double)(mCandidates->GetCount())) / ((double)(dbObjects->GetCount())) < 0.5) ) {
            // Fetch some of the other candidates. Get the ones in the neighbourhood of the given object.
            FdoInt32 start = ix - (fetchSize/2);
            if ( start < 0 ) 
                start = 0;

            // Build the candidates list.
            FdoInt32 end;

            for ( end = start; (end < mCandidates->GetCount()) && (cands->GetCount() < fetchSize); end++ ) {
                FdoDictionaryElementP elem = mCandidates->GetItem(end);
                cands->Add( elem->GetName() );
            }

            // Pad out list with empty object names. 
            // Candidate fetches are done by binding candidate names into selects. Selects can be re-used
            // if number of bind variables stays consistent
            while ( cands->GetCount() < fetchSize ) 
                cands->Add( L"" );

            // Remove candidates from candidate list.
            // Put in temporary list to track which ones were not found.
            for  ( idx = (end - 1); idx >= start; idx-- ) {
                FdoDictionaryElementP elem = mCandidates->GetItem( idx );
                candidates->Add( elem );
                mCandidates->RemoveAt(idx);
            }

            // Read the candidates.

            // Create reader for candidate components.
            rdr = CreateReader( cands );
        }

        if ( (!rdr) && (!partial) ) {
            // Reader for candidates not created. There were either too many candidates or
            // the provider does not support retrieving this type of component by binding multiple dbObjects.

            // If all dbObjects are cached then load all components for this owner. Otherwise,
            // fall back to loading one dbObject at a time.
            rdr = CreateReader();
            if ( rdr ) {
                // Candidates list no longer needed. All components will be loaded.
                mCandidates->Clear();

                // Reset the current candidates list to include all dbObjects for this owner.
                candidates->Clear();

                for ( idx = 0; idx < dbObjects->GetCount(); idx++ ) {
                    FdoSmPhDbObjectP dbObject = dbObjects->GetItem(idx);
                    FdoDictionaryElementP elem = FdoDictionaryElement::Create( dbObject->GetName(), L"" );
                    candidates->Add( elem );
                }
            }
        }

        if ( rdr && rdr->ReadNext() ) {
            // A component reader was created so cache the components that it fetches.

            while ( !rdr->IsEOF()) {
                // Get the dbObject name for the currently read row.
                FdoStringP nextObjectName = GetObjectName( rdr );
                FdoSmPhDbObjectP dbObject = FindDbObject( dbObjects, nextObjectName );
                
                if ( dbObject ) {
                    // Cache all components for this dbObject
                    if ( !CacheObjectComponents( dbObject, rdr ) )
                        // If the components were cached, the component reader is advanced to just 
                        // after the current dbObject. However, if they weren't, then must
                        // advance the reader here. This can happen if the current dbObject
                        // is not in the cache.
                        rdr->ReadNext();
                }
                else {
                    rdr->ReadNext();
                }

                FdoDictionaryElementP elem = candidates->FindItem( nextObjectName );
                if ( elem )
                    // Mark current candidate dbObject has having been read.
                    elem->SetValue(L"f");
            }

            // Do component caching for any candidates for which no components were found. 
            // This forces the component list for each candidate to be initialized so that
            // we won't retry loading its components. The retry would be done with one query
            // per candidate, which would be slow.
            // At this point, the component reader is at EOF, so no components are cached.
            for  ( ix = 0; ix < candidates->GetCount(); ix++ ) {
                FdoDictionaryElementP elem = candidates->GetItem( ix );
                if ( wcslen(elem->GetValue()) == 0 ) {
                    FdoSmPhDbObjectP dbObject = FindDbObject( dbObjects, elem->GetName() );
                    if (dbObject )
                        CacheObjectComponents( dbObject, rdr );
                }
            }
        }
    }

protected:
    // Add the given dbObjects to the candidates list.
    // Default implementation adds all dbObject.
    virtual void LoadCandidates( 
        FdoSmPhDbObjectsP dbObjects,   // the DbObjects.
        FdoStringP objectName,         // the object that was requested 
        int& nextCandIdx               // Tracks which DbObjects have already been processed.
    )
    {
        FdoInt32 idx;
        FdoInt32 start = nextCandIdx;

        // Update starting index now in case this function
        // gets called recursively. Prevents infinite recursion.
        nextCandIdx = dbObjects->GetCount();

        // Check each dbObject not yet checked
        for ( idx = start; idx < dbObjects->GetCount(); idx++ ) {
            FdoSmPhDbObjectP dbObject = dbObjects->GetItem( idx );
            // Skip DbObject if its components are already loaded
            if ( !ComponentsLoaded(dbObject) ) {
                // Skip DbObject if it is not a candidate
                if ( IsCandidate(dbObject) ) 
                    AddCandidate( dbObject->GetName() );
            }
        }
    }

    // Returns true if the components for the given DbObject are already cached.
    virtual bool ComponentsLoaded( FdoSmPhDbObjectP dbObject ) = 0;
    
    // Returns true if the given DbObject is a candidate for bulk loading.
    // By default, all DbObjects are candidates but this function can be overridden
    // to be more selective about which are candidates.
    virtual bool IsCandidate( FdoSmPhDbObjectP dbObject )
    {
        return true;
    }

    // Add the given DbObject name to the candidates list.
    void AddCandidate( FdoStringP objectName )
    {
        FdoDictionaryElementP elem = mCandidates->FindItem( objectName );
            
        if ( !elem ) {
            elem = FdoDictionaryElement::Create( objectName, L"" );
            mCandidates->Add( elem );
        }
    }

    // Find an object by name in the given dbObjects list
    virtual FdoSmPhDbObjectP FindDbObject( 
        FdoSmPhDbObjectsP dbObjects,   // the DbObjects.
        FdoStringP objectName          // name of the object
    )
    {
        return dbObjects->FindItem(objectName);
    }

    // Get the fetch candidates list.
    FdoDictionaryP GetCandidates()
    {
        return mCandidates;
    }

    // Create a component reader that reads all components for the given candidate DbObjects.
    virtual FdoPtr<R> CreateReader( FdoStringsP candidates ) = 0;
    // Create a component reader that reads components for all DbObjects in mOwner.
    virtual FdoPtr<R> CreateReader() = 0;
    // Returns the name of the DbObject that the given component reader is positioned at.
    virtual FdoStringP GetObjectName( FdoPtr<R> rdr ) = 0;
    // Cache all components for the given DbObject from the given component reader.
    virtual bool CacheObjectComponents( FdoSmPhDbObjectP dbObject, FdoPtr<R> rdr ) = 0;

private:
    FdoSmPhOwnerP mOwner;
    FdoSmPhDbObjectsP mDbObjects;
    FdoDictionaryP mCandidates;      // List of candidate objects for fetching components from RDBMS. 
    int mNextCandIdx;
};

#endif



