/*
 * (C) Copyright 2011 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 * 
 */

#include "stdafx.h"
#include <Sm/Ph/Synonym.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Owner.h>
#include <Sm/Ph/Rd/SynonymReader.h>
#include <Sm/Ph/SynonymBaseLoader.h>

FdoSmPhSynonym::FdoSmPhSynonym(
    FdoStringP name,
    FdoSmPhDbObjectP rootObject,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhDbObject( name, pOwner, elementState ),
    mSynonymLoaded(false)
{
    if ( elementState == FdoSchemaElementState_Added ) {
        // Dangling synonyms not allowed
        if ( !rootObject ) 
            throw FdoSchemaException::Create( 
			    FdoException::NLSGetMessage(
				    FDO_NLSID(FDOSM_426),
                    (FdoString*) GetQName()
                )
    		);

        SetRootObject( rootObject );
        mSynonymLoaded = true;
    }
    else {
        if ( rootObject )
            // Modifying object reference not allowed
            throw FdoSchemaException::Create( 
			    FdoException::NLSGetMessage(
				    FDO_NLSID(FDOSM_426),
                    (FdoString*) GetQName()
                )
    		);
    }
}

FdoSmPhSynonym::~FdoSmPhSynonym()
{
}

FdoSmPhDbObjectP FdoSmPhSynonym::GetRootObject()
{
    LoadSynonym();

    return FdoSmPhDbObject::GetRootObject();
}

FdoSmPhColumnsP FdoSmPhSynonym::GetColumns()
{
    FdoSmPhDbObjectP rootObject = GetRootObject();

    if ( rootObject ) 
        return rootObject->GetColumns();

    return FdoSmPhDbObject::GetColumns();
}

FdoSmPhColumnsP FdoSmPhSynonym::GetPkeyColumns()
{
    FdoSmPhDbObjectP rootObject = GetRootObject();

    if ( rootObject ) 
        return rootObject->GetPkeyColumns();

    return FdoSmPhDbObject::GetPkeyColumns();
}


FdoPtr<FdoSmPhIndexCollection> FdoSmPhSynonym::GetIndexes()
{
    FdoSmPhDbObjectP rootObject = GetRootObject();

    if ( rootObject ) 
        return rootObject->GetIndexes();

    return FdoSmPhDbObject::GetIndexes();
}


FdoSmPhFkeysP FdoSmPhSynonym::GetFkeysUp()
{
    FdoSmPhDbObjectP rootObject = GetRootObject();

    if ( rootObject ) 
        return rootObject->GetFkeysUp();

    return FdoSmPhDbObject::GetFkeysUp();
}

FdoLtLockModeType FdoSmPhSynonym::GetLtMode() const
{
    FdoLtLockModeType ltMode;

    FdoSmPhDbObjectP dbObject = ((FdoSmPhSynonym*) this)->GetRootObject();

    if ( dbObject ) 
        ltMode = dbObject->GetLtMode();
    else
        ltMode = FdoSmPhDbObject::GetLtMode();

    return ltMode;
}

FdoLtLockModeType FdoSmPhSynonym::GetLockingMode() const
{
    FdoLtLockModeType lockingMode;

    FdoSmPhDbObjectP dbObject = ((FdoSmPhSynonym*) this)->GetRootObject();

    if ( dbObject ) 
        lockingMode = dbObject->GetLockingMode();
    else
        lockingMode = FdoSmPhDbObject::GetLockingMode();

    return lockingMode;
}

const FdoLockType* FdoSmPhSynonym::GetLockTypes(FdoInt32& size) const
{
    const FdoLockType* lockTypes;

    FdoSmPhDbObjectP dbObject = ((FdoSmPhSynonym*) this)->GetRootObject();

    if ( dbObject ) 
        lockTypes = dbObject->GetLockTypes(size);
    else
        lockTypes = FdoSmPhDbObject::GetLockTypes(size);

    return lockTypes;
}

bool FdoSmPhSynonym::ClassifyObjectType(FdoBoolean classifyDefaultTypes )
{
    // Leave it to specific schema managers to decide whether to expose
    // synonym as FDO class.
    return false;
}

void FdoSmPhSynonym::LoadSynonym()
{
    if ( !BaseLoaded() ) {
        FdoSmPhOwner* pOwner = (FdoSmPhOwner*) GetParent();

        // Base not yet loaded, bulk load it along with some other
        // candidates
        pOwner->CacheSynonymBases( GetName() );

        if ( !BaseLoaded() ) {
            // Bulk load did not find base object.
            // Fall back to retrieving only base for this synonym
            FdoSmPhRdSynonymReaderP rdr = pOwner->CreateSynonymReader( GetName() );

            CacheSynonymBaseObjects( rdr );
        }
    }
}

bool FdoSmPhSynonym::BaseLoaded()
{
    if ( !mSynonymLoaded ) {
        // Synonym base has not been loaded but base might have
        // been loaded by the generic base object loader.
        FdoSmPhBaseObjectsP baseObjects = Get_BaseObjects();

        if ( baseObjects ) {
            if ( baseObjects->GetCount() > 0 ) 
                // Generic loader did load base object.
                mSynonymLoaded = true;
            else
                // Generic loader tried but did not find base object.
                // This can happen if base is in a different datastore.
                // In this case we need to use the FdoSmPhSynonymBaseLoader
                // to get the base object. 
                // First we must destroy this synonym's base object 
                // collection to force loading via the FdoSmPhSynonymBaseLoader
                DiscardBaseObjects();
        }
    }

    return mSynonymLoaded;
}

bool FdoSmPhSynonym::IndexesLoaded()
{
    // Skip trying to load indexes for synonym. They never have
    // direct indexes but "inherit" them from their base objects.
    return true;
}

bool FdoSmPhSynonym::CacheSynonymBaseObjects( FdoSmPhRdSynonymReaderP rdr )
{
    mSynonymLoaded = true;

    CacheBaseObjects( rdr->SmartCast<FdoSmPhRdBaseObjectReader>() );

    return true;
}

void FdoSmPhSynonym::LoadFkeyRefCands()
{
    // Skip trying to load foreign keys for synonym. They never have
    // direct foreign keys but "inherit" them from their base objects.
}

