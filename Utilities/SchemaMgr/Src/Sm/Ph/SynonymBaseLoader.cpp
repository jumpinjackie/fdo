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
#include <Sm/Ph/SynonymBaseLoader.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Owner.h>
#include <Sm/Ph/Synonym.h>
#include <Sm/Ph/Rd/SynonymReader.h>

FdoSmPhSynonymBaseLoader::FdoSmPhSynonymBaseLoader(
    FdoSmPhOwnerP owner,
    FdoSmPhDbObjectsP dbObjects
) :
    FdoSmPhDbObjectComponentLoader<FdoSmPhRdSynonymReader>( owner, dbObjects ),
    mOwner(owner)
{
}

FdoSmPhSynonymBaseLoader::~FdoSmPhSynonymBaseLoader()
{
}

bool FdoSmPhSynonymBaseLoader::ComponentsLoaded( FdoSmPhDbObjectP dbObject )
{
    bool loaded = false;

    FdoSmPhSynonymP synonym = dbObject.p->SmartCast<FdoSmPhSynonym>();

    // A synonym's components are on its base object.
    if ( synonym ) {
        loaded = synonym->BaseLoaded();
    }
    else {
        loaded = true;
    }

    return loaded;
}
    
bool FdoSmPhSynonymBaseLoader::IsCandidate( FdoSmPhDbObjectP dbObject )
{
    // Candidate must be a synonym.
    FdoSmPhSynonymP synonym = dbObject.p->SmartCast<FdoSmPhSynonym>();

    return (synonym != NULL);
}

FdoPtr<FdoSmPhRdSynonymReader> FdoSmPhSynonymBaseLoader::CreateReader( FdoStringsP candidates )
{
    return mOwner->CreateSynonymReader(candidates);
}

FdoPtr<FdoSmPhRdSynonymReader> FdoSmPhSynonymBaseLoader::CreateReader()
{
    return mOwner->CreateSynonymReader();
}

FdoStringP FdoSmPhSynonymBaseLoader::GetObjectName( FdoSmPhRdSynonymReaderP rdr )
{
    return rdr->GetString( L"", L"name" );
}

bool FdoSmPhSynonymBaseLoader::CacheObjectComponents( FdoSmPhDbObjectP dbObject, FdoSmPhRdSynonymReaderP rdr )
{
    FdoSmPhSynonymP synonym = dbObject.p->SmartCast<FdoSmPhSynonym>();

    if ( synonym ) 
        return synonym->CacheSynonymBaseObjects( rdr );

    return false;
}

