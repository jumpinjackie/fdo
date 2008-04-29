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
#include "Mgr.h"
#include "Database.h"
#include "TempObject.h"
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPhysicalSchemaMapping.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvTextInRowOption.h>
#include <Sm/Ph/Rd/QueryReader.h>

FdoSmPhSqsMgr::SqsStringMap FdoSmPhSqsMgr::mSqsReservedDbObjectNames;

FdoSmPhSqsMgr::FdoSmPhSqsMgr(GdbiConnection* connection, FdoStringP SqsSchemaName) :
    FdoSmPhGrdMgr(connection)
{
    SetDefaultOwnerName(SqsSchemaName);
}

FdoSmPhSqsMgr::~FdoSmPhSqsMgr(void)
{
}

FdoStringP FdoSmPhSqsMgr::GetDbVersion()
{

    if ( mDbVersion == L"" ) {
        GdbiConnection* gdbiConn = GetGdbiConnection();
        GdbiCommands* gdbiCommands = gdbiConn->GetCommands();
	    bool		  autoCmtChanged = false;

        // Open and close a dummy transaction to force a commit
        gdbiCommands->tran_begin( "SmPreGetDbVersion" );
        gdbiCommands->tran_end( "SmPreGetDbVersion" );

	    int autoCmtMode = gdbiCommands->autocommit_mode();
	    if (autoCmtMode == 1) //SQL_AUTOCOMMIT_ON 
	    {
		    gdbiCommands->autocommit_off();
		    autoCmtChanged = true;
	    }

        mDbVersion = L"Unknown - 0.0";
        try {
            FdoSmPhRowP row = new FdoSmPhRow( FDO_SAFE_ADDREF(this), L"sp_ver" );
            FdoSmPhFieldP field = new FdoSmPhField( row, L"attribute_value", row->CreateColumnChar(L"attribute_value", false, 50) );
            FdoPtr<FdoSmPhRdQueryReader> rdr = CreateQueryReader( row, L"exec sp_server_info @attribute_id=2" );

            // TODO: ReadNext() generates a "function sequence error" on Sql Server 2000. Need to 
            // investigate why this happens. It might be due to another select being open.
            // When this happens, version 0.0 (unknown) is returned. 
            if ( rdr->ReadNext() ) {
                mDbVersion = rdr->GetString( L"", L"attribute_value" );
            }
        }
        catch ( FdoException* ex ) {
            ex->Release();
        }
        catch ( ... ) {
        }

	    if (autoCmtChanged)
		    gdbiCommands->autocommit_on();

    }
 
    FdoStringP retVersion = mDbVersion.Right( L"- " ).Left( L" " );

    return retVersion;
}

FdoSmPhDatabaseP FdoSmPhSqsMgr::CreateDatabase(FdoStringP database)
{
    //TODO: when database not L"", verify that it exists.
    return new FdoSmPhSqsDatabase(database, FDO_SAFE_ADDREF(this) );
}

FdoSmPhDbObjectP FdoSmPhSqsMgr::CreateTempObject()
{
    return new FdoSmPhSqsTempObject( FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhCfgSchemaReader> FdoSmPhSqsMgr::CreateCfgSchemaReader( FdoSmPhRowsP rows )
{
    return new FdoSmPhCfgGrdSchemaReader( rows, FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhCfgClassReader> FdoSmPhSqsMgr::CreateCfgClassReader( FdoSmPhRowsP rows, FdoStringP schemaName )
{
    return new FdoSmPhCfgGrdClassReader( rows, schemaName, FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhCfgPropertyReader> FdoSmPhSqsMgr::CreateCfgPropertyReader( FdoStringP schemaName, FdoStringP className, FdoSmPhDbObjectP dbObject )
{
    return new FdoSmPhCfgGrdPropertyReader( schemaName, className, dbObject, FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhRdSchemaReader> FdoSmPhSqsMgr::CreateRdSchemaReader( FdoSmPhRowsP rows, FdoSmPhOwnerP owner, bool dsInfo )
{
    return new FdoSmPhRdSqsSchemaReader( rows, owner, dsInfo );
}

FdoStringP FdoSmPhSqsMgr::GetOverrideDatabase(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoSqlServerOvPhysicalSchemaMapping* sqsMapping = (FdoSqlServerOvPhysicalSchemaMapping*) mapping.p;

    return sqsMapping ? sqsMapping->GetDatabase() : L"";
}

FdoStringP FdoSmPhSqsMgr::GetOverrideOwner(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoSqlServerOvPhysicalSchemaMapping* sqsMapping = (FdoSqlServerOvPhysicalSchemaMapping*) mapping.p;

    return sqsMapping ? sqsMapping->GetOwner() : L"";
}

FdoStringP FdoSmPhSqsMgr::GetOverrideTableStorage(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoSqlServerOvPhysicalSchemaMapping* sqsMapping = (FdoSqlServerOvPhysicalSchemaMapping*) mapping.p;

    return sqsMapping ? sqsMapping->GetTableFilegroup() : L"";
}

FdoStringP FdoSmPhSqsMgr::GetOverrideIndexStorage(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoSqlServerOvPhysicalSchemaMapping* sqsMapping = (FdoSqlServerOvPhysicalSchemaMapping*) mapping.p;

    return sqsMapping ? sqsMapping->GetIndexFilegroup() : L"";
}

FdoStringP FdoSmPhSqsMgr::GetOverrideTextStorage(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoSqlServerOvPhysicalSchemaMapping* sqsMapping = (FdoSqlServerOvPhysicalSchemaMapping*) mapping.p;

    return sqsMapping ? sqsMapping->GetTextFilegroup() : L"";
}

FdoStringP FdoSmPhSqsMgr::GetOverrideTextInRow(FdoRdbmsOvSchemaMappingP mapping)
{
    FdoSqlServerOvPhysicalSchemaMapping* sqsMapping = (FdoSqlServerOvPhysicalSchemaMapping*) mapping.p;

    return sqsMapping ? TextInRow_EnumToString(sqsMapping->GetTextInRow()) : L"";
}


/* TODO: This might move down to the generic levle
FdoSmPhReaderP FdoSmPhSqsMgr::GetDbObjectsReader( bool databaseSet )
{
    if ( databaseSet )
        return mDbObjectsReaderD;
    else
        return mDbObjectsReader;
}

void FdoSmPhSqsMgr::SetDbObjectsReader( FdoSmPhReaderP reader, bool databaseSet )
{
    if ( databaseSet )
        mDbObjectsReaderD = reader;
    else
        mDbObjectsReader = reader;
}

FdoSmPhReaderP FdoSmPhSqsMgr::GetDbObjectReader( bool databaseSet )
{
    if ( databaseSet )
        return mDbObjectReaderD;
    else
        return mDbObjectReader;
}

void FdoSmPhSqsMgr::SetDbObjectReader( FdoSmPhReaderP reader, bool databaseSet )
{
    if ( databaseSet )
        mDbObjectReaderD = reader;
    else
        mDbObjectReader = reader;
}
*/
FdoStringP FdoSmPhSqsMgr::FormatDefaultedField( FdoStringP fieldName, FdoStringP colName, FdoStringP defaultValue, FdoSmPhColType fieldType )
{
    return FdoStringP::Format(
        L"isnull(%ls,%ls) %ls",
        (FdoString*) colName,
        (FdoString*) FormatSQLVal( defaultValue, fieldType ),
        (FdoString*) fieldName
    );
}

FdoStringP FdoSmPhSqsMgr::FormatSQLVal( FdoStringP value, FdoSmPhColType valueType )
{
	FdoStringP sqlString;
    
    if ( value.GetLength() > 0 ) {
        if ( valueType == FdoSmPhColType_String || valueType == FdoSmPhColType_Date )
            sqlString = FdoStringP(L"N'") + FdoStringP(value).Replace( L"'", L"''" ) + FdoStringP(L"'");
        else
            sqlString = value;
    }
    else {
        sqlString = L"null";
    }

	return sqlString;
}

FdoStringP FdoSmPhSqsMgr::FormatOrderCol( FdoStringP colName, FdoSmPhColType colType )
{
    FdoStringP fmtName = colName;

    if ( (colType == FdoSmPhColType_String) && (!IsRdbObjNameAscii7()) )
        // for string columns, force simple binary ordering. 
        // As long as the column is nvarchar any binary collation
        // will do, regardless of the database character set.
        //
        // Columns are varchar when IsRdbObjNameAscii7() is true. In this
        // case do not add collation since it makes Japanese characters
        // lower than latin characters (we want the opposite).
        fmtName += " collate latin1_general_bin";

	return fmtName;
}



FdoStringP FdoSmPhSqsMgr::FormatCurrentDateField()
{
    return L"getdate()";
}

FdoStringP FdoSmPhSqsMgr::FormatBindField( int pos)
{
    return L"?";
}

FdoBoolean FdoSmPhSqsMgr::IsDbObjectNameReserved( FdoStringP objName )
{
    // This provider double-quote delimits all db object names in SQL statements so
    // no need to reserve names.
    return false;
}

bool FdoSmPhSqsMgr::IsRdbObjNameAscii7()
{
    // The SqlServer provider can create database elements, with non-ASCII7 names, if the 
    // MetaSchema stores strings in nvarchar columns. This is true for datastores created
    // by FDO 3.3 and after.

    FdoSmPhOwnerP owner = this->GetOwner();
    if ( (!owner) || (!owner->GetHasMetaSchema()) ) 
        return false;

    FdoSmPhDbObjectP dbObject = owner->FindDbObject( L"dbo.f_classdefinition" );
    if ( !dbObject ) 
        return false;

    FdoSmPhColumnP column = dbObject->GetColumns()->FindItem( L"classname" );

    if ( column && column->GetTypeName().ICompare(L"varchar") == 0 )
        // Most Schema Manager queries are ordered on string columns. When these
        // are varchar, it is difficult to pick a collation that returns rows
        // in a predictable order, when the columns contain non-ASCII7 data.
        // Therefore, stick to ASCII7 database element names for these datastores.
        return true;

    return false;
}

FdoSize FdoSmPhSqsMgr::DbObjectNameMaxLen()
{
    return 128;
}

FdoSize FdoSmPhSqsMgr::ColNameMaxLen()
{
    return 128;
}

bool FdoSmPhSqsMgr::SupportsMixedCase()
{
    return true;
}

FdoStringP FdoSmPhSqsMgr::GetDcRdbmsObjectName( FdoStringP objectName )
{
    // TODO: remove lower case conversion when forward compatibility with FDO 3.2
    // no longer needs to be preserved. Ordered INFORMATION_SCHEMA queries done by FDO 3.2 do not
    // return rows in predictable order when database element names are in 
    // mixed case.
    return objectName.Lower();
}

FdoStringP FdoSmPhSqsMgr::GetDcDbObjectName( FdoStringP objectName )
{
    FdoStringP fullObjectName = GetRealDbObjectName( objectName );

    return GetDcRdbmsObjectName( fullObjectName );
}

FdoStringP FdoSmPhSqsMgr::GetRealDbObjectName( FdoStringP objectName )
{
    FdoStringP fullObjectName = ((objectName == L"") || (objectName.Contains(L"."))) ?
        objectName :
        // user defaults to dbo
        FdoStringP(L"dbo.") + objectName;

    return fullObjectName;
}

FdoStringP FdoSmPhSqsMgr::DbObject2MetaSchemaName( FdoStringP objectName )
{
    if ( objectName.Mid(0, 4) == L"dbo." )
        return objectName.Mid(4,99999);

    return objectName;
}

FdoSmPhRowP FdoSmPhSqsMgr::MakeByDbObjectBinds (FdoStringP object_owner, FdoStringP object_name)
{
    FdoSmPhRowP row = new FdoSmPhRow( FDO_SAFE_ADDREF(this), L"Binds" );
    FdoSmPhDbObjectP rowObj = row->GetDbObject();

    FdoSmPhFieldP field = new FdoSmPhField(
        row,
        L"object_catalog",
        rowObj->CreateColumnDbObject(L"object_catalog",false)
    );

    if ( object_name != L"" ) {
        field = new FdoSmPhField(
            row,
            L"object_schema",
            rowObj->CreateColumnDbObject(L"object_schema",false)
        );

        field = new FdoSmPhField(
            row,
            L"object_name",
            rowObj->CreateColumnDbObject(L"object_name",false)
        );
    }

    SetByDbObjectBinds( row, object_owner, object_name );

    return( row );
}

void FdoSmPhSqsMgr::SetByDbObjectBinds (FdoSmPhRowP binds, FdoStringP object_owner, FdoStringP object_name)
{
	FdoSmPhFieldsP	fields = binds->GetFields ();	

	FdoSmPhFieldP(fields->GetItem (L"object_catalog"))->SetFieldValue (object_owner);

	FdoSmPhFieldP	field = fields->FindItem(L"object_schema");

    if ( field ) {
        if ( object_name.Contains(L".") ) {
            // Object name contains user name, set object_schema to user name.

            FdoSmPhFieldP(fields->GetItem (L"object_schema"))->SetFieldValue (object_name.Left(L"."));
            FdoSmPhFieldP(fields->GetItem (L"object_name"))->SetFieldValue (object_name.Right(L"."));
        }
        else {
            // Object name not qualified by user name, default the user name to dbo.
            FdoSmPhFieldP(fields->GetItem (L"object_schema"))->SetFieldValue (L"dbo");
            FdoSmPhFieldP(fields->GetItem (L"object_name"))->SetFieldValue (object_name);
        }
    }
}

FdoSmPhSqsMgr::SqsStringMap::SqsStringMap()
{
    // No reserved words for SqlServer
};


