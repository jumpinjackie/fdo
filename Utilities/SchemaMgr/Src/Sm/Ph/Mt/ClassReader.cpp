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
#include <Sm/Ph/DependencyReader.h>
#include <Sm/Ph/Mt/ClassReader.h>
#include <Sm/Ph/Rd/ColumnReader.h>
#include <Sm/Ph/Rd/PkeyReader.h>

FdoSmPhMtClassReader::FdoSmPhMtClassReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoStringP className, FdoSmPhMgrP mgr ) : 
	FdoSmPhReader( MakeReader(froms, schemaName, mgr, className) )
{
    // Bulk load physical objects for better performance.
    if( className == NULL )
        CachePhysical( schemaName, mgr );
}

FdoSmPhMtClassReader::~FdoSmPhMtClassReader(void)
{
}

FdoSmPhReaderP FdoSmPhMtClassReader::MakeReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr, FdoString* className )
{
    // Generate the where clause
    FdoStringP where;
    if( className == NULL || className[0] == '\0' )
        where = FdoStringP::Format( 
            L"where schemaname = %ls and f_classdefinition.classtype = f_classtype.classtype order by f_classdefinition.classname", 
    	    (FdoString*) mgr->FormatSQLVal(schemaName, FdoSmPhColType_String)
    );
    else
        where = FdoStringP::Format( 
            L"where schemaname = %ls and f_classdefinition.classname = %ls and  f_classdefinition.classtype = f_classtype.classtype order by f_classdefinition.classname", 
    	    (FdoString*) mgr->FormatSQLVal(schemaName, FdoSmPhColType_String),
            (FdoString*) mgr->FormatSQLVal(className, FdoSmPhColType_String)
    );

    // Create a query reader to wrap around
    FdoSmPhRdQueryReaderP pSubReader = mgr->CreateQueryReader( froms, where );

    return FDO_SAFE_ADDREF( (FdoSmPhRdQueryReader*)pSubReader );
}

void FdoSmPhMtClassReader::CachePhysical( FdoStringP schemaName, FdoSmPhMgrP mgr )
{
    FdoSmPhOwnerP owner = mgr->GetOwner();
    FdoSmPhRdDbObjectReaderP objReader;
    FdoSmPhRdPkeyReaderP pkeyReader;
    FdoSmPhRdConstraintReaderP ukeyReader;
    FdoSmPhRdConstraintReaderP ckeyReader;
    FdoSmPhRdColumnReaderP columnReader;
    FdoSmPhDependencyReaderP depReader;

    // The tables referenced by the MetaClass schema are loaded by FdoSmPhOwner::CacheCandDbObjects
    // so no need to bulk load them a second time.
    if ( schemaName != FdoSmPhMgr::mMetaClassSchemaName ) {

        // Create a join object for only reading physical objects needed by this feature schema.
        FdoSmPhRdTableJoinP join = new FdoSmPhMtClassTableJoin( owner, schemaName );

        // Create reader for owner's db objects. Use the join to retrieve only tables for 
        // classes in this schema.
        objReader = owner->CreateDbObjectReader( join );

        // Caching db object components so create readers for components.
        // This function does interleaved fetches from each reader so all readers
        // (including dbObject reader) must return rows ordered by dbObject name.
        //
        // For datastores with MetaSchema, only constraints need to be bulk fetched.
        // Primary and Foreign keys, and indexes are never fetched.
        //
        // Doing a single query per owner for each component is more efficient than
        // a query per dbObject.
        // The join is used to limit results to those needed for this schema.
        pkeyReader = owner->CreatePkeyReader( join );
        ukeyReader = owner->CreateConstraintReader( join, L"U" );
        ckeyReader = owner->CreateConstraintReader( join, L"C" );

        columnReader = owner->CreateColumnReader( join );

        depReader = new FdoSmPhDependencyReader( join, mgr );

        while ( objReader && objReader->ReadNext() ) {
            // Cache the current dbObject
            FdoSmPhDbObjectP dbObject = owner->CacheDbObject( objReader );

            if ( dbObject ) {
                // Load the components into the db object.
                FdoSmPhTableP table = dbObject->SmartCast<FdoSmPhTable>();

                if ( columnReader ) 
                    dbObject->CacheColumns( columnReader );

                if ( pkeyReader ) 
                    dbObject->CachePkeys( pkeyReader );

                if ( depReader ) 
                    dbObject->CacheDependenciesUp( depReader );

                if ( table ) {
                    if ( ukeyReader ) 
                        table->CacheUkeys( ukeyReader );

                    if ( ckeyReader ) 
                        table->CacheCkeys( ckeyReader );

                }
            }
        }
    }
}

