//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "Owner.h"
#include "Table.h"
#include "View.h"
#include "Mgr.h"
#include "Rd/DbObjectReader.h"
#include "Rd/BaseObjectReader.h"
#include "Rd/ColumnReader.h"
#include "Rd/ConstraintReader.h"
#include "Rd/FkeyReader.h"
#include "Rd/IndexReader.h"
#include "Rd/PkeyReader.h"
#include "Rd/DbSchemaReader.h"
#include "Rd/SpatialContextReader.h"
#include <Sm/Ph/Rd/QueryReader.h>
#include "Inc/Rdbi/proto.h"

//
// FdoSmPhPostGisOwner implementation.
//

FdoSmPhPostGisOwner::FdoSmPhPostGisOwner()
{
    Init();
}

FdoSmPhPostGisOwner::FdoSmPhPostGisOwner(FdoStringP name,
    bool hasMetaSchema,
    const FdoSmPhDatabase* pDatabase,
    FdoSchemaElementState state,
    FdoSmPhRdDbObjectReader* reader)
    : FdoSmPhGrdOwner(name, hasMetaSchema, pDatabase, state)
{
    Init();
}

FdoSmPhPostGisOwner::~FdoSmPhPostGisOwner()
{
    // idle
}

void FdoSmPhPostGisOwner::Init()
{
    mTablesTable = L"information_schema.tables";
    mColumnsTable = L"information_schema.columns";

    mTableConstraintsTable = L"information_schema.table_constraints";
    mKeyColumnUsageTable = L"information_schema.key_column_usage";
}

FdoStringP FdoSmPhPostGisOwner::GetTablesTable() const
{
    return mTablesTable;
}

FdoStringP FdoSmPhPostGisOwner::GetColumnsTable() const
{
    return mColumnsTable;
}

FdoStringP FdoSmPhPostGisOwner::GetTableConstraintsTable() const
{
    return mTableConstraintsTable;
}

FdoStringP FdoSmPhPostGisOwner::GetKeyColumnUsageTable() const
{
    return mKeyColumnUsageTable;
}

bool FdoSmPhPostGisOwner::IsDbObjectNameReserved( FdoStringP objectName )
{
    bool isReserved = FdoSmPhGrdOwner::IsDbObjectNameReserved( objectName );

    // The PostGIS DbObjectReader does not currently pick up indexes so
    // they are not cached in the owner's DbObject list and are thus not picked
    // up by base IsDbObjectNameReserved.

    // Do extra check against pg_class table to ensure objectName not currently
    // used by an index.

    if ( !isReserved ) {
        FdoStringP sqlString = FdoStringP::Format(
            L"select 1 from pg_catalog.pg_class C, pg_catalog.pg_namespace N "
            L"where C.relnamespace = N.oid and upper(N.nspname || '.' || C.relname) = %ls", 
            (FdoString*) GetManager()->FormatSQLVal(objectName.Upper(), FdoSmPhColType_String)
        );

        FdoSmPhRowP row = new FdoSmPhRow( GetManager(), L"GetObjectExists" );

        FdoSmPhRdQueryReaderP reader = GetManager()->CreateQueryReader( row, sqlString );

        isReserved = reader->ReadNext();
    }

    return isReserved;
}


void FdoSmPhPostGisOwner::SetCurrent()
{
    FdoStringP name(GetName());
    FdoSmPhPostGisMgrP mgr(GetManager()->SmartCast<FdoSmPhPostGisMgr>());

    rdbi_set_schema(mgr->GetRdbiContext(),
        static_cast<const char*>(name));
}

void FdoSmPhPostGisOwner::DiscardSchema( FdoSmPhPostGisSchema* schema )
{
    if ( mSchemas )
        mSchemas->Remove( schema );
}

FdoSmPhDbObjectP FdoSmPhPostGisOwner::NewTable(FdoStringP name,
    FdoSchemaElementState state,
    FdoSmPhRdDbObjectReader* reader)
{
    FdoSmPhPostGisTable* pgTable = NULL;
    pgTable = new FdoSmPhPostGisTable(name, this, state, L"", reader);
    return pgTable;
}

FdoSmPhDbObjectP FdoSmPhPostGisOwner::NewView(FdoStringP name,
    FdoStringP rootDb,
    FdoStringP rootOwner,
    FdoStringP rootObjectName,
    FdoSchemaElementState state,
    FdoSmPhRdDbObjectReader* reader)
{
    FdoSmPhPostGisView* pgView = NULL;
    pgView = new FdoSmPhPostGisView(name, rootDb, rootOwner,
        rootObjectName, this, state, reader);

    return pgView;
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhPostGisOwner::CreateDbObjectReader(
    FdoStringP dbObject) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisDbObjectReader* reader = NULL;
    reader = new FdoSmPhRdPostGisDbObjectReader(thisOwner, dbObject);
    
    return reader;
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhPostGisOwner::CreateDbObjectReader(
    FdoStringsP objectNames) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisDbObjectReader* reader = NULL;
    reader = new FdoSmPhRdPostGisDbObjectReader(thisOwner, objectNames);
    
    return reader;
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhPostGisOwner::CreateDbObjectReader(
    FdoSmPhRdTableJoinP join) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisDbObjectReader* reader = NULL;
    reader = new FdoSmPhRdPostGisDbObjectReader(thisOwner, join);
    
    return reader;
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhPostGisOwner::CreateBaseObjectReader() const
{
    FdoSmPhPostGisOwner* pOwner = (FdoSmPhPostGisOwner*) this;

    return new FdoSmPhRdPostGisBaseObjectReader( FDO_SAFE_ADDREF(pOwner) );
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhPostGisOwner::CreateBaseObjectReader(FdoStringsP objectNames) const
{
    FdoSmPhPostGisOwner* pOwner = (FdoSmPhPostGisOwner*) this;

    return new FdoSmPhRdPostGisBaseObjectReader( FDO_SAFE_ADDREF(pOwner), objectNames );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhPostGisOwner::CreateConstraintReader(
    FdoStringP constraintName) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisConstraintReader* reader = NULL;
    reader = new FdoSmPhRdPostGisConstraintReader(thisOwner, constraintName);

    return reader;
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhPostGisOwner::CreateConstraintReader(
    FdoStringP tableName,
    FdoStringP constraintType) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisConstraintReader* reader = NULL;
    reader = new FdoSmPhRdPostGisConstraintReader(thisOwner,
        tableName, constraintType);

    return reader;
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhPostGisOwner::CreateConstraintReader( FdoStringsP objectNames, FdoStringP constraintType ) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisConstraintReader* reader = NULL;
    reader = new FdoSmPhRdPostGisConstraintReader(thisOwner,
        objectNames, constraintType);

    return reader;
}


FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhPostGisOwner::CreateConstraintReader(
    FdoSmPhRdTableJoinP join,
    FdoStringP constraintType) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisConstraintReader* reader = NULL;
    reader = new FdoSmPhRdPostGisConstraintReader(thisOwner,
        join, constraintType);

    return reader;
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhPostGisOwner::CreateFkeyReader() const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisFkeyReader* reader = NULL;
    reader = new FdoSmPhRdPostGisFkeyReader(thisOwner, (FdoSmPhDbObject*) NULL);

    return reader;
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhPostGisOwner::CreateFkeyReader(  FdoStringsP objectNames ) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisFkeyReader* reader = NULL;
    reader = new FdoSmPhRdPostGisFkeyReader(
        thisOwner,
        objectNames
        );

    return reader;
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhPostGisOwner::CreateIndexReader() const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisIndexReader* reader = NULL;
    reader = new FdoSmPhRdPostGisIndexReader(thisOwner, (FdoSmPhDbObject*) NULL);

    return reader;
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhPostGisOwner::CreateIndexReader( FdoStringsP objectNames ) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisIndexReader* reader = NULL;
    reader = new FdoSmPhRdPostGisIndexReader(thisOwner, objectNames);

    return reader;
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhPostGisOwner::CreatePkeyReader() const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisPkeyReader* reader = NULL;
    reader = new FdoSmPhRdPostGisPkeyReader(thisOwner, (FdoSmPhDbObject*) NULL);

    return reader;
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhPostGisOwner::CreatePkeyReader(  FdoStringsP objectNames ) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisPkeyReader* reader = NULL;
    reader = new FdoSmPhRdPostGisPkeyReader(
        thisOwner,
        objectNames
        );

    return reader;
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhPostGisOwner::CreateColumnReader() const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisColumnReader* reader = NULL;
    reader = new FdoSmPhRdPostGisColumnReader(thisOwner, (FdoSmPhDbObject*) NULL);

    return reader;
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhPostGisOwner::CreateColumnReader(FdoStringsP objectNames) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisColumnReader* reader = NULL;
    reader = new FdoSmPhRdPostGisColumnReader(thisOwner, objectNames);

    return reader;
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhPostGisOwner::CreateColumnReader(
    FdoSmPhRdTableJoinP join) const
{
    FdoSmPhPostGisOwner* thisOwner = NULL;
    thisOwner = const_cast<FdoSmPhPostGisOwner*>(this);
    FDO_SAFE_ADDREF(thisOwner);

    FdoSmPhRdPostGisColumnReader* reader = NULL;
    reader = new FdoSmPhRdPostGisColumnReader(thisOwner, join);

    return reader;
}

FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhPostGisOwner::CreateRdSpatialContextReader( )
{
    return new FdoSmPhRdPostGisSpatialContextReader( FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhPostGisOwner::CreateRdSpatialContextReader( FdoStringP dbObjectName )
{
    return new FdoSmPhRdPostGisSpatialContextReader(FDO_SAFE_ADDREF(this), dbObjectName );
}

void FdoSmPhPostGisOwner::CommitChildren( bool isBeforeParent )
{
    int i;

    if ( isBeforeParent ) 
        // This ensures that the tables to delete are deleted before 
        // their containing schemas.
        FdoSmPhOwner::CommitChildren( isBeforeParent );

    if ( mSchemas ) {

        // Commit the schemas.
        for ( i = (mSchemas->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhPostGisSchemaP schema = mSchemas->GetItem(i);
            schema->Commit( true, isBeforeParent );
        }
    }

    if ( !isBeforeParent ) 
        // This ensures that tables to create are created 
        // after their containing schemas.
        FdoSmPhOwner::CommitChildren( isBeforeParent );
}



bool FdoSmPhPostGisOwner::Add()
{
    FdoSmPhPostGisMgrP mgr(GetManager()->SmartCast<FdoSmPhPostGisMgr>());

    GdbiConnection* gdbiConn = NULL;
    gdbiConn = mgr->GetGdbiConnection();

     FdoStringP sqlStmt = FdoStringP::Format(
     L"CREATE DATABASE %ls TEMPLATE template_postgis ENCODING 'UTF8'", static_cast<FdoString*>(GetDbName()));
   
    gdbiConn->ExecuteNonQuery(static_cast<const char*>(sqlStmt), true);

    // Put datastore description into the database comments. 
    // While listing datastores, it is not easy to look into each 
    // database to find out if it has metaschema, and to extract the
    // description. Putting description in the database comments makes
    // it more readily available in this case.
    //
    // Another benefit is that descriptions, for datastores without metaschema,
    // can be persisted.

    FdoStringP description;

    // Encode whether the datastore has metaschema into the description.
    if ( GetHasMetaSchema() )
        description = FdoStringP(L"FDO Enabled: ") + GetDescription();
    else
        description = GetDescription();

    if ( description != L"" ) 
    {
        sqlStmt = FdoStringP::Format(
            L"comment on database \"%ls\" is %ls",
            GetName(),
            (FdoString*) mgr->FormatSQLVal(description, FdoSmPhColType_String)
        );
      
        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );
    }

    if (GetHasMetaSchema())
    {
        FdoStringsP keywords(FdoStringCollection::Create());
        keywords->Add(rdbi_vndr_name(mgr->GetRdbiContext()));
        keywords->Add(L"PostGIS" );

        // Switch to newly created owner and add the MetaSchema
        SetCurrent();

        try
        {
            AddMetaSchema(keywords, GetIsSystem());
        }
        catch (...)
        {
            try
            {
                // On error, switch back to default owner
                FdoSmPhOwnerP prevOwner(mgr->FindOwner());                
                if (prevOwner)
                    prevOwner->SetCurrent();
            }
            catch (...)
            {
                // Skip forward
            }
            throw;
        }
		
		if (!GetIsSystem())
        {
			SetOptions();
        }

        // Switch back to default owner
        FdoSmPhOwnerP prevOwner(mgr->FindOwner());
        if (prevOwner)
            prevOwner->SetCurrent();
    }
    
    return true;
}

bool FdoSmPhPostGisOwner::Modify()
{
    return true;
}

bool FdoSmPhPostGisOwner::Delete()
{
    FdoSmPhPostGisMgrP mgr(GetManager()->SmartCast<FdoSmPhPostGisMgr>());
    
    GdbiConnection* gdbiConn = NULL;
    gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"DROP DATABASE %ls",
        static_cast<FdoString*>(GetDbName()));

    gdbiConn->ExecuteNonQuery(static_cast<const char*>(sqlStmt), true);

    return true;
}

void FdoSmPhPostGisOwner::AddMetaSchema( FdoStringsP keywords, bool IsSystem)
{
	FdoSmPhGrdMgrP mgr = GetManager()->SmartCast<FdoSmPhGrdMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
	if (IsSystem)
		mgr->ExecSchemaFile( L"fdosys_sys.sql", keywords );
	else	{
		mgr->ExecSchemaFile( L"postgis_fdo_sys.sql", keywords );
		mgr->ExecSchemaFile( L"postgis_fdo_sys_idx.sql", keywords );
		CreateMetaClass();

        FdoStringP sqlStmt = FdoStringP::Format(
            L"update f_schemainfo set description = %ls where schemaname = '%ls'",
            (FdoString*) mgr->FormatSQLVal(GetDescription(), FdoSmPhColType_String),
            (FdoString*) FdoStringP(GetName()).Upper()
        );
  
        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );
	}
}

FdoSmPhPostGisSchemaP FdoSmPhPostGisOwner::CreateSchema( FdoStringP schemaName )
{
    FdoSmPhPostGisSchemaP schema = new FdoSmPhPostGisSchema( schemaName, this );
    GetSchemas()->Add( schema );

    return schema;
}

FdoSmPhPostGisSchemaP FdoSmPhPostGisOwner::FindSchema( FdoStringP schemaName )
{
    FdoSmPhPostGisSchemaP schema = GetSchemas()->FindItem( schemaName );

    return schema;
}

FdoSmPhPostGisSchemasP FdoSmPhPostGisOwner::GetSchemas()
{
    if ( !mSchemas ) {
        mSchemas = new FdoSmPhPostGisSchemaCollection( this );

        FdoSmPhPostGisOwner* pOwner = (FdoSmPhPostGisOwner*) this;
        
        FdoPtr<FdoSmPhRdPostGisDbSchemaReader> rdr = new FdoSmPhRdPostGisDbSchemaReader(
            FDO_SAFE_ADDREF(pOwner)
        );

        while ( rdr->ReadNext() ) {
            FdoSmPhPostGisSchemaP schema = new FdoSmPhPostGisSchema(
                rdr->GetString( L"", L"schema_name"),
                this,
                rdr
            );

            mSchemas->Add( schema );
        }
    }

    return mSchemas;
}

void FdoSmPhPostGisOwner::SetOptions()
{
	bool validOption = false;

	// validate LT method
	validOption = (GetLtMode() == NoLtLock || GetLtMode() == FdoMode);

	if (false == validOption)
    {
		SetLtMode(NoLtLock);
    }

	FdoStringP tempValue = FdoStringP::Format(L"%d",
        static_cast<int>(GetLtMode()));

	FdoStringP sqlStmt = FdoStringP::Format(
        L"UPDATE f_options SET value = '%ls'"
        L"WHERE name = 'LT_MODE'",
        static_cast<FdoString*>(tempValue));

	FdoSmPhPostGisMgrP mgr(GetManager()->SmartCast<FdoSmPhPostGisMgr>());
    
    GdbiConnection* gdbiConn = NULL;
    gdbiConn = mgr->GetGdbiConnection();

	gdbiConn->ExecuteNonQuery(static_cast<const char*>(sqlStmt));

	// validate locking method
	validOption = (NoLtLock == GetLckMode() || FdoMode == GetLckMode());
    if (false == validOption)
    {
		SetLckMode(NoLtLock);
    }

	tempValue = FdoStringP::Format(L"%d", static_cast<int>(GetLckMode()));

	sqlStmt = FdoStringP::Format(
        L"UPDATE f_options SET value = '%ls'"
        L"WHERE name ='LOCKING_MODE'",
		static_cast<FdoString*>(tempValue));

	gdbiConn->ExecuteNonQuery(static_cast<const char*>(sqlStmt));
}

FdoString* FdoSmPhPostGisOwner::GetDbNameClause(bool isEqual)
{

    if (isEqual)
        mDbNameClause = FdoStringP::Format( L"schemaname = '%ls'", GetName() );
	else
        mDbNameClause = FdoStringP::Format( L"schemaname != '%ls'", GetName() );

    return mDbNameClause;
}

void FdoSmPhPostGisOwner::CreateMetaClass()
{
	FdoSmPhPostGisMgrP mgr = GetManager()->SmartCast<FdoSmPhPostGisMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
	FdoStringP sql_stmt;

//BR - TODO - hardcoded classid and owning user.

	sql_stmt = FdoStringP::Format(L"insert into f_classdefinition "
			L"(classid,classname,schemaname,tablename,classtype,description,isabstract,parentclassname,istablecreator, isfixedtable, hasversion, haslock) "
			L"values (1,'Class', 'F_MetaClass', ' ', 1, '%ls', 0, 'ClassDefinition', 0, 1, 0, 0)",
			NlsMsgGet(FDORDBMS_504, "Non-feature metaclass"));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_classdefinition "
			L"(classid,classname, schemaname, tablename,classtype, description, isabstract, parentclassname, istablecreator, isfixedtable, hasversion, haslock) "
			L"values (2,'ClassDefinition', 'F_MetaClass', 'f_classdefinition', 1, '%ls', 1, null, 0, 1, 0, 0) ",
			NlsMsgGet(FDORDBMS_496, "Base for all metaclasses")); 
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename, classid, columnname, attributename, columntype, columnsize, columnscale, attributetype, isnullable, "
			L"isfeatid, issystem, isreadonly, isautogenerated, isrevisionnumber, owner, description, iscolumncreator, isfixedcolumn) "
			L"values('f_classdefinition', 2, 'classname', 'ClassName', 'CHAR', 30, 0, 'string', 0, 0, 1, 1, 0, 0,"
			L"'todo', '%ls', 0, 1)",
			NlsMsgGet(FDORDBMS_497, "FDO base property: object class name"));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename, classid, columnname, attributename, columntype, columnsize, columnscale, attributetype, isnullable, "
			L"isfeatid, issystem, isreadonly, isautogenerated, isrevisionnumber, owner, description, iscolumncreator, isfixedcolumn) "
			L"values('f_classdefinition', 2, 'schemaname', 'SchemaName', 'CHAR', 255, 0, 'string', 1, 0, 1, 1, 0, 0,"
			L"'todo','%ls', 0, 1)",
			NlsMsgGet(FDORDBMS_498, "FDO base property: object class schema name"));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_classdefinition "
			L"(classid,classname,schemaname,tablename,classtype,description,isabstract,parentclassname,istablecreator,isfixedtable,hasversion,haslock) "
			L"values (3,'Feature','F_MetaClass','f_feature',1,'%ls',0,'ClassDefinition',0,1,0,0)",
			NlsMsgGet(FDORDBMS_499, "Feature metaclass"));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename,classid,columnname,attributename,idposition,columntype,columnsize,columnscale,attributetype, "
			L"isnullable,isfeatid,issystem,isreadonly,isautogenerated,isrevisionnumber,owner,description,iscolumncreator,isfixedcolumn) "
			L"values ('f_feature',3,'classid','ClassId',1,'BIGINT',0,0,'int64',0,0,1,1,1,0, "
			L"'todo','%ls',0,1)",
			NlsMsgGet(FDORDBMS_501, "Class id number (uniquely identifies a class)"));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename,classid,columnname,attributename,columntype,columnsize,columnscale,attributetype, "
			L"isnullable,isfeatid,issystem,isreadonly,isautogenerated,isrevisionnumber,owner,description,iscolumncreator,isfixedcolumn) "
			L"values ('f_feature',3,'revisionnumber','RevisionNumber','BIGINT',0,0,'double',0,0,1,1,1,1, "
			L"'todo','%ls',0,1)",
			NlsMsgGet(FDORDBMS_502, "Edit count for supporting optimistic locking.  This value is incremented everytime the object is updated."));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename,classid,columnname,attributename,columntype,columnsize,columnscale,attributetype,geometrytype,isnullable, "
			L"isfeatid,issystem,isreadonly,isautogenerated,isrevisionnumber,owner,description,iscolumncreator,isfixedcolumn) "
			L"values ('n/a',3,'n/a','Bounds','GEOMETRY',0,0,'4','84224',1,0,1,1,0,0, "
			L"'todo','%ls', 0, 0)",
			NlsMsgGet(FDORDBMS_503, "Bounding box for the feature"));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);
}
