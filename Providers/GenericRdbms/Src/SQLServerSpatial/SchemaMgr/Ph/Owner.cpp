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

    // Note, column not quote-delimited since this does not work with column member extraction.
	FdoStringP sqlStmt = FdoStringP::Format(
		L"select %ls.STSrid as srid from %ls.%ls", 
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
    return new FdoSmPhRdSqsSpatialContextReader(FDO_SAFE_ADDREF(this) );
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
