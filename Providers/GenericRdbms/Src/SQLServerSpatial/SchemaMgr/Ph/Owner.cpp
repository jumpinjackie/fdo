/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include "Rd/SpatialContextReader.h"
#include "Rd/CoordSysReader.h"
#include "Rd/DbSchemaReader.h"
#include "Rdbi/proto.h"
#include <FdoCommonStringUtil.h>

struct odbcdr_context_def;

extern "C" {
    int odbcdr_run_sql (
        odbcdr_context_def *context,
        char *sql
    );
}

FdoSmPhSqsOwner::FdoSmPhSqsOwner(
    FdoStringP name,
    bool hasMetaSchema,
    const FdoSmPhDatabase* pDatabase,
    FdoSchemaElementState elementState
) :
    FdoSmPhGrdOwner(name, hasMetaSchema, pDatabase, elementState)
{
}

FdoSmPhSqsOwner::~FdoSmPhSqsOwner(void)
{
}

FdoSmPhSqsSchemaP FdoSmPhSqsOwner::FindSchema( FdoStringP schemaName )
{
    FdoSmPhSqsSchemaP schema = GetSchemas()->FindItem( schemaName );

    return schema;
}

FdoSmPhSqsSchemasP FdoSmPhSqsOwner::GetSchemas()
{
    LoadSchemas();

    return mSchemas;
}

FdoSmPhCoordinateSystemP FdoSmPhSqsOwner::FindCoordinateSystem( FdoInt64 srid )
{
    // First check the RDBMS catalogue
    FdoSmPhCoordinateSystemP coordSys = FdoSmPhOwner::FindCoordinateSystem( srid );

    if ( !coordSys ) {
        // Not in catalogue, load extra coordinate systems from file
        LoadExtendedCoordinateSystems();
        // Check if it's an extended (extra) coordinate system
        coordSys = mExtendedCoordinateSystems->FindItemById( srid );
    }

    return coordSys;
}

FdoSmPhCoordinateSystemP FdoSmPhSqsOwner::FindCoordinateSystem( FdoStringP csName )
{
    // See comments in FdoSmPhSqsOwner::FindCoordinateSystem( FdoInt64 )
    FdoSmPhCoordinateSystemP coordSys = FdoSmPhOwner::FindCoordinateSystem( csName );

    if ( !coordSys ) {
        LoadExtendedCoordinateSystems();
        coordSys = mExtendedCoordinateSystems->FindItem( csName );

        if ( coordSys ) {
            // The coordinate system name is not in the RDBMS catalogue but is in
            // the extended list. However, it's EPSG number (SRID) might be in the catalogue.
            // If it is then retrieve the catalogued coordinate system instead.
            FdoSmPhCoordinateSystemP coordSys2 = FdoSmPhOwner::FindCoordinateSystem( coordSys->GetSrid() ) ;
            if ( coordSys2 ) 
                coordSys = coordSys2;
        }
    }

    return coordSys;
}

FdoSmPhCoordinateSystemP FdoSmPhSqsOwner::FindCoordinateSystemByWkt( FdoStringP wkt )
{
    // See comments in FdoSmPhSqsOwner::FindCoordinateSystem( FdoInt64 )
    FdoSmPhCoordinateSystemP coordSys = FdoSmPhOwner::FindCoordinateSystemByWkt( wkt );

    if ( !coordSys ) {
        LoadExtendedCoordinateSystems();
        coordSys = mExtendedCoordinateSystems->FindItemByWkt( wkt );

        if ( coordSys ) {
            // The coordinate system wkt is not in the RDBMS catalogue but is in
            // the extended list. However, its EPSG number (SRID) might be in the catalogue.
            // If it is then retrieve the catalogued coordinate system instead.
            FdoSmPhCoordinateSystemP coordSys2 = FdoSmPhOwner::FindCoordinateSystem( coordSys->GetSrid() ) ;
            if ( coordSys2 ) 
                coordSys = coordSys2;
        }
    }

    return coordSys;
}


void FdoSmPhSqsOwner::DiscardSchema( FdoSmPhSqsSchema* schema )
{
    if ( mSchemas )
        mSchemas->Remove( schema );
}

void FdoSmPhSqsOwner::CommitChildren( bool isBeforeParent )
{
    int i;

    if ( isBeforeParent ) 
        // This ensures that the tables to delete are deleted before 
        // their containing schemas.
        FdoSmPhOwner::CommitChildren( isBeforeParent );

    if ( mSchemas ) {

        // Commit the schemas.
        for ( i = (mSchemas->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhSqsSchemaP schema = mSchemas->GetItem(i);
            schema->Commit( true, isBeforeParent );
        }
    }

    if ( !isBeforeParent ) 
        // This ensures that tables to create are created 
        // after their containing schemas.
        FdoSmPhOwner::CommitChildren( isBeforeParent );
}

FdoSchemaExceptionP FdoSmPhSqsOwner::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmPhOwner::Errors2Exception(pFirstException);

    if ( mSchemas) {
    	// Add errors for the owner's database objects.
	    for ( int i = 0; i < mSchemas->GetCount(); i++ )
    		pException = mSchemas->RefItem(i)->Errors2Exception(pException);
    }

	return pException;
}



void FdoSmPhSqsOwner::SetCurrent()
{
    FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();

    if ( RDBI_SUCCESS != rdbi_set_schemaW( mgr->GetRdbiContext(), (wchar_t*)GetName() ))
    {
        rdbi_get_msg(mgr->GetRdbiContext()); 
        throw FdoSchemaException::Create( mgr->GetRdbiContext()->last_error_msg ); 
    }
	mgr->RemoveStaticReaders();

}

FdoInt64 FdoSmPhSqsOwner::SampleColumnSrid( FdoStringP dbObjectName, FdoStringP columnName )
{
	// SRID is -1 if table is empty or geometry column has not been populated yet
    FdoInt64 srid = -1;
    FdoStringP fmtObjectName = FdoStringP(L"\"") + dbObjectName.Replace(L".", L"\".\"") + L"\"";

    // Delimit column name with []. Can't use " when part of function.
	FdoStringP sqlStmt = FdoStringP::Format(
		L"select [%ls].STSrid as srid from %ls.%ls", 
        (FdoString*) columnName,
        (FdoString*) this->GetDbName(),
        (FdoString*) fmtObjectName
    );

	FdoSmPhSqsMgrP mgr = this->GetManager()->SmartCast<FdoSmPhSqsMgr>();
	GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
	GdbiQueryResult *gdbiResult = gdbiConn->ExecuteQuery((const char*)sqlStmt);

	if (gdbiResult->ReadNext())
	{
		if (!gdbiResult->GetIsNull(L"srid"))
			srid = gdbiResult->GetInt64(L"srid", NULL, NULL);
	}
	gdbiResult->End();
    delete gdbiResult;

    return srid;
}

FdoSmPhSqsSchemaP FdoSmPhSqsOwner::CreateSchema( FdoStringP schemaName )
{
    FdoSmPhSqsSchemaP schema = new FdoSmPhSqsSchema( schemaName, this );
    GetSchemas()->Add( schema );

    return schema;
}

FdoSmPhDbObjectP FdoSmPhSqsOwner::NewTable(
    FdoStringP tableName,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
)
{
    return new FdoSmPhSqsTable(tableName, this, elementState, L"", reader);
}

FdoSmPhDbObjectP FdoSmPhSqsOwner::NewView(
    FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
)
{
    return new FdoSmPhSqsView(viewName, rootDatabase, rootOwner, rootObjectName, this, elementState, reader);
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhSqsOwner::CreateDbObjectReader( FdoStringP dbObject) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsDbObjectReader( FDO_SAFE_ADDREF(pOwner), dbObject );
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhSqsOwner::CreateDbObjectReader( FdoStringsP objectNames ) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsDbObjectReader( FDO_SAFE_ADDREF(pOwner), objectNames );
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhSqsOwner::CreateDbObjectReader( FdoSmPhRdTableJoinP join ) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsDbObjectReader( FDO_SAFE_ADDREF(pOwner), join );
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhSqsOwner::CreateBaseObjectReader() const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsBaseObjectReader( FDO_SAFE_ADDREF(pOwner) );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhSqsOwner::CreateConstraintReader( FdoStringP constraintName) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsConstraintReader( FDO_SAFE_ADDREF(pOwner), constraintName );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhSqsOwner::CreateConstraintReader( FdoStringP tableName, FdoStringP constraintType) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsConstraintReader( FDO_SAFE_ADDREF(pOwner), tableName, constraintType );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhSqsOwner::CreateConstraintReader( FdoStringsP tableNames, FdoStringP constraintType) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsConstraintReader( FDO_SAFE_ADDREF(pOwner), tableNames, constraintType );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhSqsOwner::CreateConstraintReader( FdoSmPhRdTableJoinP join , FdoStringP constraintType) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsConstraintReader( FDO_SAFE_ADDREF(pOwner), join, constraintType );
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhSqsOwner::CreateFkeyReader() const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsFkeyReader( pOwner->GetManager(), FDO_SAFE_ADDREF(pOwner) );
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhSqsOwner::CreateIndexReader() const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsIndexReader( pOwner->GetManager(), FDO_SAFE_ADDREF(pOwner) );
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhSqsOwner::CreatePkeyReader() const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsPkeyReader( FDO_SAFE_ADDREF(pOwner) );
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhSqsOwner::CreatePkeyReader( FdoPtr<FdoSmPhRdTableJoin> join ) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsPkeyReader( FDO_SAFE_ADDREF(pOwner), join );
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhSqsOwner::CreateColumnReader() const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsColumnReader( FDO_SAFE_ADDREF(pOwner), (FdoSmPhRdTableJoin*)NULL );
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhSqsOwner::CreateColumnReader( FdoStringsP objectNames ) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsColumnReader( FDO_SAFE_ADDREF(pOwner), objectNames );
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhSqsOwner::CreateColumnReader( FdoSmPhRdTableJoinP join ) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsColumnReader( FDO_SAFE_ADDREF(pOwner), join );
}

FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhSqsOwner::CreateRdSpatialContextReader()
{
    return new FdoSmPhRdSqsSpatialContextReader(FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhSqsOwner::CreateRdSpatialContextReader( FdoStringP dbObjectName )
{
    return new FdoSmPhRdSqsSpatialContextReader(FDO_SAFE_ADDREF(this), dbObjectName );
}

FdoSmPhRdCoordSysReaderP FdoSmPhSqsOwner::CreateCoordSysReader( FdoInt64 srid ) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsCoordSysReader( FDO_SAFE_ADDREF(pOwner), srid );
}

FdoSmPhRdCoordSysReaderP FdoSmPhSqsOwner::CreateCoordSysReader( FdoStringP csName ) const
{
    FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;

    return new FdoSmPhRdSqsCoordSysReader( FDO_SAFE_ADDREF(pOwner), csName );
}

bool FdoSmPhSqsOwner::Add()
{
    FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
    GdbiCommands* gdbiCommands = gdbiConn->GetCommands();
	bool		  autoCmtChanged = false;

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create database %ls",
        GetDbName()
    );

    // Create the owner (datastore)
    // The SqlServer "create database must be in it's own transaction

    // Open and close a dummy transaction to force a commit before creating the database.
    gdbiCommands->tran_begin( "SmPreCreateDatabase" );
    gdbiCommands->tran_end( "SmPreCreateDatabase" );

	int autoCmtMode = gdbiCommands->autocommit_mode();
	if (autoCmtMode == 1) //SQL_AUTOCOMMIT_ON 
	{
		gdbiCommands->autocommit_off();
		autoCmtChanged = true;
	}
    // Wrap the database create in a transaction.
    gdbiCommands->tran_begin( "SmCreateDatabase" );
    try {
        gdbiConn->ExecuteNonQuery( (FdoString*) sqlStmt );
    }
    catch ( ... ) {
        try {
            gdbiCommands->tran_end( "SmCreateDatabase" );
			if (autoCmtChanged)
				gdbiCommands->autocommit_on();
        }
        catch ( ... ) {        
        }
    
        throw;
    }
    gdbiCommands->tran_end( "SmCreateDatabase" );

    if ( GetHasMetaSchema() ) {
        FdoStringsP keywords = FdoStringCollection::Create();
        keywords->Add( rdbi_vndr_name(mgr->GetRdbiContext()) );
        keywords->Add( L"SQLServer" );

        // The following keywords cause the long transaction and locking
        // system properties to be added and inherited by each class table.
        if ( GetLtMode() == FdoMode )
            keywords->Add( L"FdoLt" );

        if ( GetLckMode() == FdoMode )
            keywords->Add( L"FdoLock" );

        // Switch to newly created owner and add the MetaSchema

        SetCurrent();

        try {
            AddMetaSchema( keywords, GetIsSystem());
        }
        catch ( ... ) {
            try {
                FdoSmPhOwnerP prevOwner = mgr->FindOwner();
                
                if ( prevOwner )
                    prevOwner->SetCurrent();
            }
            catch (...) {
            }
            throw;
        }

		if (GetIsSystem() == false)
			SetOptions();

		if (autoCmtChanged)
			gdbiCommands->autocommit_on();

        // Switch back to default owner
        FdoSmPhOwnerP prevOwner = mgr->FindOwner();
                
        if ( prevOwner && FdoStringP(prevOwner->GetName()).GetLength() != 0)
            prevOwner->SetCurrent();

        //TODO: spatial handling?
    }
    
    return true;
}

bool FdoSmPhSqsOwner::Delete()
{
    FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
    GdbiCommands* gdbiCommands = gdbiConn->GetCommands();

    FdoSmPhOwnerP currOwner = mgr->FindOwner();

    // Do use master when the datastore to delete is the current one. 
    if ( currOwner && FdoStringP(currOwner->GetName()).ICompare(GetName()) == 0 ) 
        gdbiConn->ExecuteNonQuery( "USE master", TRUE );

    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop database %ls",
        GetDbName()
    );

    // The SqlServer "drop database must be in it's own transaction

    // Open and close a dummy transaction to force a commit before dropping the database.
    gdbiCommands->tran_begin( "SmPreDropDatabase" );
    gdbiCommands->tran_end( "SmPreDropDatabase" );

    // Wrap the database drop in a transaction.
    gdbiCommands->tran_begin( "SmDropDatabase" );
    try {
        gdbiConn->ExecuteNonQuery( (const wchar_t*) sqlStmt, TRUE);
    }
    catch ( ... ) {
        try {
            gdbiCommands->tran_end( "SmDropDatabase" );
        }
        catch ( ... ) {        
        }
    
        throw;
    }
    gdbiCommands->tran_end( "SmDropDatabase" );

    return true;
}

FdoInt32 FdoSmPhSqsOwner::GetCandFetchSize()
{
    // On SqlServer, there are 2 bind variables per bound table name so fetch size is half the default.
    // Fetch size of 50 tables gives marginal performance improvement but doubles the length of the
    // DbObjectReader's sql statement. 
    return 25;
}

void FdoSmPhSqsOwner::SetOptions()
{
	bool validOption;

	// validate LT method
	validOption = (GetLtMode() == NoLtLock || GetLtMode() == FdoMode);
	if (validOption == false)	
		SetLtMode(FdoMode);
	FdoStringP tempValue = FdoStringP::Format(L"%d", (int)GetLtMode());
	FdoStringP sql_stmt = FdoStringP::Format(L"update f_options set value = '%ls' where name = 'LT_MODE'",
							 (FdoString *) tempValue);

	FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
	gdbiConn->ExecuteNonQuery( (FdoString*) sql_stmt );

	// validate locking method
	validOption = (GetLckMode() == NoLtLock || GetLckMode() == FdoMode);
	if (validOption == false)	
		SetLckMode(NoLtLock);
	tempValue = FdoStringP::Format(L"%d", (int)GetLckMode());
	sql_stmt = FdoStringP::Format(L"update f_options set value = '%ls' where name ='LOCKING_MODE'",
							(FdoString *) tempValue);

	gdbiConn->ExecuteNonQuery( (FdoString*) sql_stmt );

}
FdoString* FdoSmPhSqsOwner::GetDbNameClause(bool isEqual)
{
	if (isEqual)
		return L"schemaname = db_name()";
	else
		return L"schemaname != db_name()";
}

FdoStringP FdoSmPhSqsOwner::GetBestSchemaName() const
{
    return L"dbo";
}

void FdoSmPhSqsOwner::CreateMetaClass()
{
	FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
	FdoStringP sql_stmt;

	sql_stmt = FdoStringP::Format(L"insert into f_classdefinition "
			L"(classname,schemaname,tablename,classtype,description,isabstract,parentclassname,istablecreator,isfixedtable,hasversion,haslock) "
			L"values ('Class', 'F_MetaClass', ' ', 1, %ls, 0, 'ClassDefinition', 0, 1, 0, 0)",
			(FdoString *) GetManager()->FormatSQLVal(NlsMsgGet(FDORDBMS_504, "Non-feature metaclass"), FdoSmPhColType_String));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt); 

	sql_stmt = FdoStringP::Format(L"insert into f_classdefinition "
			L"(classname, schemaname,tablename,classtype,description,isabstract,parentclassname,istablecreator,isfixedtable,hasversion,haslock) "
			L"values('ClassDefinition', 'F_MetaClass', 'f_classdefinition', 1, %ls, 1, null, 0, 1, 0, 0)",
			(FdoString *) GetManager()->FormatSQLVal(NlsMsgGet(FDORDBMS_496, "Base for all metaclasses"), FdoSmPhColType_String));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename, classid,columnname,attributename,columntype,columnsize,columnscale,attributetype,isnullable,"
			L"isfeatid, issystem, isreadonly,isautogenerated,isrevisionnumber,owner,description,iscolumncreator,isfixedcolumn) "
			L"values ('f_classdefinition', IDENT_CURRENT('f_classdefinition'), 'classname', 'ClassName', 'CHAR', 30, 0, 'string', 0, 0, 1, 1, 0, 0,"
			L"SYSTEM_USER, %ls, 0, 1)",
			(FdoString *) GetManager()->FormatSQLVal(NlsMsgGet(FDORDBMS_497, "FDO base property: object class name"), FdoSmPhColType_String));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename, classid, columnname, attributename, columntype, columnsize, columnscale, attributetype, isnullable,"
			L"isfeatid, issystem, isreadonly, isautogenerated, isrevisionnumber, owner, description, iscolumncreator, isfixedcolumn) "
			L"values ('f_classdefinition', IDENT_CURRENT('f_classdefinition'), 'schemaname', 'SchemaName', 'CHAR', 255, 0, 'string', 1, 0, 1, 1, 0, 0,"
			L"SYSTEM_USER, %ls, 0, 1)",
			(FdoString *) GetManager()->FormatSQLVal(NlsMsgGet(FDORDBMS_498, "FDO base property: object class schema name"), FdoSmPhColType_String));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_classdefinition "
			L"(classname,schemaname,tablename,classtype,description,isabstract,parentclassname,istablecreator,isfixedtable,hasversion,haslock) "
			L"values ('Feature','F_MetaClass','f_feature',1,%ls,0,'ClassDefinition',0,1,0,0)",
			(FdoString *) GetManager()->FormatSQLVal(NlsMsgGet(FDORDBMS_499, "Feature metaclass"), FdoSmPhColType_String));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition  "
			L"(tablename,classid,columnname,attributename,idposition,columntype,columnsize,columnscale,attributetype,"
			L"isnullable,isfeatid,issystem,isreadonly,isautogenerated,isrevisionnumber,owner,description,iscolumncreator,isfixedcolumn)  "
			L"values ('f_feature',IDENT_CURRENT('f_classdefinition'),'featid','FeatId',1,'NUMBER',20,0,'int64',0,1,1,1,1,0, "
			L"SYSTEM_USER,%ls,0,1)",
			(FdoString *) GetManager()->FormatSQLVal(NlsMsgGet(FDORDBMS_500, "Feature id number (uniquely identifies a feature)"), FdoSmPhColType_String));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename,classid,columnname,attributename,idposition,columntype,columnsize,columnscale,attributetype, "
			L"isnullable,isfeatid,issystem,isreadonly,isautogenerated,isrevisionnumber,owner,description,iscolumncreator,isfixedcolumn) "
			L"values ('f_feature',IDENT_CURRENT('f_classdefinition'),'classid','ClassId',1,'NUMBER',20,0,'int64',0,0,1,1,1,0, "
			L"SYSTEM_USER,%ls,0,1)",
			(FdoString *) GetManager()->FormatSQLVal(NlsMsgGet(FDORDBMS_501, "Class id number (uniquely identifies a class)"), FdoSmPhColType_String));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename,classid,columnname,attributename,columntype,columnsize,columnscale,attributetype, "
			L"isnullable,isfeatid,issystem,isreadonly,isautogenerated,isrevisionnumber,owner,description,iscolumncreator,isfixedcolumn) "
			L"values ('f_feature',IDENT_CURRENT('f_classdefinition'),'revisionnumber','RevisionNumber','NUMBER',0,0,'double',0,0,1,1,1,1, "
			L"SYSTEM_USER,%ls,0,1)", 
			(FdoString *) GetManager()->FormatSQLVal(NlsMsgGet(FDORDBMS_502, "Edit count for supporting optimistic locking.  This value is incremented everytime the object is updated."), FdoSmPhColType_String));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);

	sql_stmt = FdoStringP::Format(L"insert into f_attributedefinition "
			L"(tablename,classid,columnname,attributename,columntype,columnsize,columnscale,attributetype,geometrytype,isnullable, "
			L"isfeatid,issystem,isreadonly,isautogenerated,isrevisionnumber,owner,description,iscolumncreator,isfixedcolumn) "
			L"values ('n/a',IDENT_CURRENT('f_classdefinition'),'n/a','Bounds','GEOMETRY',0,0,'4','84224',1,0,1,1,0,0, "
			L"SYSTEM_USER,%ls,0,0)",
			(FdoString *) GetManager()->FormatSQLVal(NlsMsgGet(FDORDBMS_503, "Bounding box for the feature"), FdoSmPhColType_String));
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt);
}

void FdoSmPhSqsOwner::LoadSchemas()
{
    if ( !mSchemas ) {
        mSchemas = new FdoSmPhSqsSchemaCollection( this );

        FdoSmPhSqsOwner* pOwner = (FdoSmPhSqsOwner*) this;
        
        FdoSmPhRdSqsDbSchemaReaderP rdr = new FdoSmPhRdSqsDbSchemaReader(
            FDO_SAFE_ADDREF(pOwner)
        );

        while ( rdr->ReadNext() ) {
            FdoSmPhSqsSchemaP schema = new FdoSmPhSqsSchema(
                rdr->GetString( L"", L"schema_name"),
                this,
                rdr
            );

            mSchemas->Add( schema );
        }
    }
}

void FdoSmPhSqsOwner::LoadExtendedCoordinateSystems()
{
    // Nothing to do if already loaded
    if ( !mExtendedCoordinateSystems ) {
        mExtendedCoordinateSystems = new FdoSmPhCoordinateSystemCollection();
        FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();

        FdoStringP fileName = mgr->GetExtendedCsysFilename();
        FILE* fp = _wfopen( fileName, L"rt" );

        // Not an error if file not found. 
        // No extended coordinate systems in this case.
        if ( fp ) {
            FdoInt32 lineNum = 0;
            wchar_t buffer[5000];

            // Load each coordinate system from the file
            while ( fgetws(buffer, 4999, fp) ) {
                lineNum++;

                // Skip empty and comment lines.
                if ( (buffer[0] == 0) || (buffer[0] == '\n') || (wcsncmp(buffer, L"//", 2) == 0) )
                    continue;


                // Process current line
                // Some basic validation is done but it is not comprehensive.

                FdoInt32 bufLen = (FdoInt32) wcslen(buffer);

                FdoInt32 idx;
                wchar_t* sridPtr = NULL;
                wchar_t* namePtr = NULL;
                wchar_t* wktPtr = NULL;

                FdoInt64 srid = -1;
                FdoStringP csysName;

                // Skip leading blanks
                for ( idx = 0; (idx < bufLen) && (buffer[idx] == ' '); idx++ );

                if ( buffer[idx] ) 
                    // First word is the SRID (EPSG number)
                    sridPtr = &(buffer[idx]);

                // Skip to next blank.
                for ( ; (idx < bufLen) && (buffer[idx] != ' '); idx++ );
                
                // Chop out srid and parse it.
                buffer[idx] = 0;
                if ( idx < bufLen ) 
                    idx++;

                if ( sridPtr ) 
                    srid = FdoCommonStringUtil::StringToInt64( sridPtr );
               
                // Error if invalid srid.
                if ( srid <= 0 ) {
                    fclose(fp);

                    throw FdoSchemaException::Create( 
	   	                NlsMsgGet2(
			                FDORDBMS_538,
			                "First item must be numeric EPSG number; error at line %1$d in file '%2$ls'",
                            lineNum,
                            (FdoString*) fileName
    	                )
                    );
                }

                // Skip to next word.
                for ( ; (idx < bufLen) && (buffer[idx] == ' '); idx++ );

                if ( wcsncmp(&(buffer[idx]),L"PROJCS[", 7) == 0 ||
                     wcsncmp(&(buffer[idx]),L"GEOGCS[", 7) == 0
                ) {
                    // Next word is the start of WKT string.
                    // Note we currently only support geodetic or 
                    // projected coordinate systems in extended list.
                    wktPtr = &(buffer[idx]);
                }
                else {
                    // Next word not WKT, must be coordinate system name.
                    if ( buffer[idx] )
                        namePtr = &(buffer[idx]);

                    // Find the start of the WKT string.
                    for ( ; (idx < bufLen) &&(wcsncmp(&(buffer[idx]),L" PROJCS[", 8) != 0) && (wcsncmp(&(buffer[idx]),L" GEOGCS[", 8) != 0); idx++ );

                    // Split the name and the WKT.
                    if ( buffer[idx] ) {
                        buffer[idx++] = 0;
                        wktPtr = &(buffer[idx]);
                    }
                }

                if ( wktPtr ) {
                    // Trim any eol and trailing blanks from WKT.
                    for ( idx = (FdoInt32) wcslen(wktPtr) - 1; idx >= 0; idx-- ) {
                        if ( wktPtr[idx] == ']' ) { 
                            wktPtr[idx + 1] = 0;
                            break;
                        }
                    }

                    // The WKT string is not validated but we do a quick sanity check
                    // on the length. 
                    if ( wcslen(wktPtr) < 12 ) {
                        fclose(fp);

                        throw FdoSchemaException::Create( 
	   	                    NlsMsgGet2(
			                    FDORDBMS_539,
			                    "Invalid WKT string; error at line %1$d in file '%2$ls'",
                                lineNum,
                                (FdoString*) fileName
    	                    )
                        );
                    }
                }

                if ( namePtr ) {
                    // Trim trailing blanks from name
                    for ( idx = (FdoInt32) wcslen(namePtr) - 1; idx >= 0; idx-- ) {
                        if ( namePtr[idx] == ' ' ) 
                            namePtr[idx] = 0;
                        else
                            break;
                    }
                    csysName = namePtr;
                }
                else {
                    // Name not specified, extract it from the WKT.
                    if ( wktPtr ) {
                        csysName = &(wktPtr[9]);
                        csysName = csysName.Left(L"\"");
                    }
                }

                if ( csysName == L"" ) {
                    fclose(fp);

                    // Name still blank, meaning neither name no WKT were supplied.
                    // Report as an error.
                    throw FdoSchemaException::Create( 
	   	                NlsMsgGet2(
			                FDORDBMS_540,
			                "At least one of coordinate system name or WKT must be specified; error at line %1$d in file '%2$ls'",
                            lineNum,
                            (FdoString*) fileName
    	                )
                    );
                }

                // Create the coordinate system and add to the extended list
                FdoSmPhCoordinateSystemP coordSys = new FdoSmPhCoordinateSystem(
                    GetManager(),
                    csysName,
                    L"",
                    srid,
                    wktPtr
                );

                if ( mExtendedCoordinateSystems->IndexOf( coordSys->GetName() ) < 0 ) 
                    mExtendedCoordinateSystems->Add( coordSys );

            }

            fclose(fp);
        }
    }
}

