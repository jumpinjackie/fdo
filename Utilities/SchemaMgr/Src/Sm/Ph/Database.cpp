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

#include "stdafx.h"
#include <Sm/Ph/Database.h>
#include <Sm/Error.h>
#include <Sm/Ph/Rd/OwnerReader.h>
#include <Sm/Ph/Rd/CharacterSetReader.h>
#include <Sm/Ph/Rd/CollationReader.h>

FdoSmPhDatabase::FdoSmPhDatabase(
    FdoStringP name, 
    FdoPtr<FdoSmPhMgr> mgr,
	FdoSchemaElementState elementState
) : 
    FdoSmPhDbElement(name, mgr, NULL, elementState )
{
}

FdoSmPhDatabase::~FdoSmPhDatabase(void)
{
}

const FdoSmPhOwner* FdoSmPhDatabase::RefOwner(FdoStringP owner) const
{
    return (FdoSmPhOwner*) ((FdoSmPhDatabase*) this)->FindOwner(owner);
}

FdoSmPhOwnerP FdoSmPhDatabase::FindOwner(FdoStringP owner)
{
    FdoSmPhOwnersP owners = GetOwners();

    // Look up the owner in the cache. 
    FdoSmPhOwnerP pOwner = owners->FindItem(owner);

    if ( !pOwner ) {
        // Not in cache so read it in.
        FdoPtr<FdoSmPhRdOwnerReader> reader = CreateOwnerReader( owner );

        while (!pOwner && reader->ReadNext())
		{
			if (reader->GetName() == owner)
				pOwner = NewOwner(reader->GetName(), reader->GetHasMetaSchema(), FdoSchemaElementState_Unchanged);
        }

        if ( pOwner )
            // Owner found, add it the the cache.
            owners->Add( pOwner );
    }

    return(pOwner);
}

FdoSmPhOwnerP FdoSmPhDatabase::GetOwner(FdoStringP owner)
{
    FdoSmPhOwnerP pOwner = FindOwner(owner);

    if ( !pOwner ) {
        bool defaultDb = ( wcslen(GetName()) == 0 );
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_4),
                defaultDb ? L"" : GetName() ,
                defaultDb ? L"" : L".",
				(FdoString*) owner
            )
        );
    }

    return(pOwner);
}

const FdoSmPhCharacterSet* FdoSmPhDatabase::RefCharacterSet(FdoStringP characterSetName) const
{
    return (FdoSmPhCharacterSet*) ((FdoSmPhDatabase*) this)->FindCharacterSet(characterSetName);
}

FdoSmPhCharacterSetP FdoSmPhDatabase::FindCharacterSet(FdoStringP characterSetName)
{
    FdoSmPhCharacterSetsP characterSets = GetCharacterSets();

    // Look up the character set in the cache. 
    FdoSmPhCharacterSetP characterSet = characterSets->FindItem(characterSetName);

    if ( !characterSet ) {
        // Not in cache so read it in.
        FdoPtr<FdoSmPhRdCharacterSetReader> reader = CreateCharacterSetReader( characterSetName );

        if ( reader && reader->ReadNext() )
		{
			characterSet = NewCharacterSet(reader->GetString(L"",L"character_set_name"), reader);
        }

        if ( characterSet )
            // Character set found, add it the the cache.
            characterSets->Add( characterSet );
    }

    return( characterSet );
}

FdoSmPhCharacterSetP FdoSmPhDatabase::GetCharacterSet(FdoStringP characterSetName)
{
    FdoSmPhCharacterSetP characterSet = FindCharacterSet( characterSetName);

    if ( !characterSet ) {
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_21),
				(FdoString*) characterSetName
            )
        );
    }

    return( characterSet );
}

const FdoSmPhCollation* FdoSmPhDatabase::RefCollation(FdoStringP collationName) const
{
    return (FdoSmPhCollation*) ((FdoSmPhDatabase*) this)->FindCollation(collationName);
}

FdoSmPhCollationP FdoSmPhDatabase::FindCollation(FdoStringP collationName)
{
    FdoSmPhCollationsP collations = GetCollations();

    // Look up the collation in the cache. 
    FdoSmPhCollationP collation = collations->FindItem(collationName);

    if ( !collation ) {
        // Not in cache so read it in.
        FdoPtr<FdoSmPhRdCollationReader> reader = CreateCollationReader( collationName );

        if ( reader && reader->ReadNext() )
		{
			collation = NewCollation(reader->GetString(L"",L"collation_name"), reader);
        }

        if ( collation )
            // Collation found, add it the the cache.
            collations->Add( collation );
    }

    return( collation );
}

FdoSmPhCollationP FdoSmPhDatabase::GetCollation(FdoStringP collationName)
{
    FdoSmPhCollationP collation = FindCollation( collationName);

    if ( !collation ) {
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_28),
				(FdoString*) collationName
            )
        );
    }

    return( collation );
}

FdoSmPhOwnerP FdoSmPhDatabase::CreateOwner(FdoStringP owner, bool hasMetaSchema)
{
    FdoStringP name = GetName();

    if ( FindOwner( owner ) )
		throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_17),
                (FdoString*) name,
                (name.GetLength() == 0) ? L"" : L".",
   				(FdoString*) owner
			)
		);

    FdoSmPhOwnerP pOwner = NewOwner(owner, hasMetaSchema);

    GetOwners()->Add(pOwner);

    return pOwner;
}

void FdoSmPhDatabase::UnsetCurrentOwner()
{
}

void FdoSmPhDatabase::DiscardOwner( FdoSmPhOwner* owner )
{
    if ( mOwners )
        mOwners->Remove( owner );
}

void FdoSmPhDatabase::OnAfterCommit()
{
    FdoInt32 idx;

    if ( mOwners ) {
        for ( idx = 0; idx < mOwners->GetCount(); idx++ )
            FdoSmPhOwnerP(mOwners->GetItem(idx))->OnAfterCommit();
    }
}

FdoPtr<FdoSmPhRdCharacterSetReader> FdoSmPhDatabase::CreateCharacterSetReader( FdoStringP characterSetName ) const
{
    return (FdoSmPhRdCharacterSetReader*) NULL;
}

FdoPtr<FdoSmPhRdCollationReader> FdoSmPhDatabase::CreateCollationReader( FdoStringP collationName ) const
{
    return (FdoSmPhRdCollationReader*) NULL;
}

FdoSchemaExceptionP FdoSmPhDatabase::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmPhDbElement::Errors2Exception(pFirstException);

    if ( mOwners ) {
    	for ( int i = 0; i < mOwners->GetCount(); i++ )
	    	pException = mOwners->RefItem(i)->Errors2Exception(pException);
    }

	return pException;
}

void FdoSmPhDatabase::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<database name=\"%ls\">\n",
			GetName()
	);

	if ( mOwners && ref == 0 ) {
		for ( int i = 0; i < mOwners->GetCount(); i++ ) 
			mOwners->RefItem(i)->XMLSerialize(xmlFp, ref);
	}

	fprintf( xmlFp, "</database>\n" );

}

FdoSmPhOwnersP FdoSmPhDatabase::GetOwners()
{
    if ( !mOwners ) {
        // Owner cache not initialized so initialize it.
        mOwners = new FdoSmPhOwnerCollection(this);
    }

    return mOwners;
}

FdoSmPhCharacterSetsP FdoSmPhDatabase::GetCharacterSets()
{
    if ( !mCharacterSets ) {
        // Character Set cache not initialized so initialize it.
        mCharacterSets = new FdoSmPhCharacterSetCollection();
    }

    return mCharacterSets;
}

FdoSmPhCollationsP FdoSmPhDatabase::GetCollations()
{
    if ( !mCollations ) {
        // Collation cache not initialized so initialize it.
        mCollations = new FdoSmPhCollationCollection();
    }

    return mCollations;
}

FdoSmPhCharacterSetP FdoSmPhDatabase::NewCharacterSet(
    FdoStringP characterSetName,
    FdoSmPhRdCharacterSetReader* reader
)
{
    return (FdoSmPhCharacterSet*) NULL;
}

FdoSmPhCollationP FdoSmPhDatabase::NewCollation(
    FdoStringP collationName,
    FdoSmPhRdCollationReader* reader
)
{
    return (FdoSmPhCollation*) NULL;
}

void FdoSmPhDatabase::CommitChildren( bool isBeforeParent )
{
    int i; 

    if ( mOwners ) {
        for ( i = 0; i < mOwners->GetCount(); i++ ) {
            FdoSmPhOwnerP(mOwners->GetItem(i))->Commit( true, isBeforeParent );
        }
    }
}
