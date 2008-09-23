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
#include <malloc.h>
#include <Sm/Ph/Mgr.h>

#include <Sm/Error.h>
#include <Sm/Ph/SchemaReader.h>
#include <Sm/Ph/ClassReader.h>
#include <Sm/Ph/AssociationReader.h>
#include <Sm/Ph/OptionsReader.h>
#include <Sm/Ph/DependencyWriter.h>
#include <Sm/Ph/SpatialContextReader.h>
#include <Sm/Ph/SpatialContextGroupReader.h>
#include <Sm/Ph/SpatialContextGeomReader.h>
#include <Sm/Ph/ClassWriter.h>
#include <Sm/Ph/PropertyWriter.h>
#include <Sm/Ph/SchemaWriter.h>
#include <Sm/Ph/AssociationWriter.h>
#include <Sm/Ph/SADWriter.h>
#include <Sm/Ph/SpatialContextWriter.h>
#include <Sm/Ph/SpatialContextReader.h>
#include <Sm/Ph/SpatialContextGroupWriter.h>
#include <Sm/Ph/SpatialContextGeomWriter.h>
#include <Sm/Ph/Rd/SchemaReader.h>
#include <Sm/Ph/Rd/ClassReader.h>
#include <Sm/Ph/Rd/PropertyReader.h>
#include <Sm/Ph/Reader.h>

/*
#include <Sm/Ph/IndexWriter.h>
#include <Sm/Ph/IndexPropertyWriter.h>

#include <wctype.h>
*/
FdoString* FdoSmPhMgr::mMetaClassSchemaName = L"F_MetaClass";

const FdoStringP FdoSmPhMgr::SchemaType(L"schema", true);
const FdoStringP FdoSmPhMgr::ClassType(L"class", true);
const FdoStringP FdoSmPhMgr::PropertyType(L"property", true);

const FdoStringP FdoSmPhMgr::ScInfoNoMetaTable(L"f_scinfo", true);
const FdoStringP FdoSmPhMgr::ScInfoNoMetaPKey(L"f_scinfo_id", true);


const FdoStringP FdoSmPhMgr::ObjPrefix( L"A", true);
const FdoStringP FdoSmPhMgr::RdSchemaPrefix(L"Fdo", true);

FdoSmPhMgr::FdoSmPhMgr()
{
    Clear();
}

FdoSmPhMgr::~FdoSmPhMgr(void)
{
}


FdoPtr<FdoSmPhSchemaReader> FdoSmPhMgr::CreateSchemaReader( FdoSmPhOwnerP owner)
{
    return( new FdoSmPhSchemaReader(owner ? owner : GetOwner()) );
}

FdoPtr<FdoSmPhClassReader> FdoSmPhMgr::CreateClassReader(FdoStringP schemaName)
{
	return( new FdoSmPhClassReader(schemaName, FDO_SAFE_ADDREF(this)) );
}

FdoPtr<FdoSmPhClassReader> FdoSmPhMgr::CreateClassReader(FdoStringP schemaName, FdoStringP className)
{
	return( new FdoSmPhClassReader(schemaName, className, FDO_SAFE_ADDREF(this)) );
}

FdoPtr<FdoSmPhAssociationReader> FdoSmPhMgr::CreateAssociationReader(
    FdoStringP pkTableName, 
    FdoStringP fkTableName, 
    bool bAnd
)
{
	return( new FdoSmPhAssociationReader(pkTableName, fkTableName, bAnd, FDO_SAFE_ADDREF(this)) );
}

FdoPtr<FdoSmPhOptionsReader> FdoSmPhMgr::CreateOptionsReader(FdoStringP owner)
{
	return( new FdoSmPhOptionsReader(FDO_SAFE_ADDREF(this), owner ));
}

FdoPtr<FdoSmPhSpatialContextReader> FdoSmPhMgr::CreateSpatialContextReader()
{
	return( new FdoSmPhSpatialContextReader(FDO_SAFE_ADDREF(this)) );
}

FdoPtr<FdoSmPhSpatialContextGroupReader> FdoSmPhMgr::CreateSpatialContextGroupReader()
{
	return( new FdoSmPhSpatialContextGroupReader(FDO_SAFE_ADDREF(this)) );
}

FdoInt64    FdoSmPhMgr::FindScIdFromName(FdoString * scName)
{
    FdoInt64 scId = -1;

    FdoSmPhSpatialContextReaderP scReader = CreateSpatialContextReader();

    while (scId < 0 && scReader->ReadNext())
    {
        if (scReader->GetName() == scName)
        {
            scId = scReader->GetId();
        }
    }

    return scId;
}

FdoPtr<FdoSmPhSpatialContextGeomReader> FdoSmPhMgr::CreateSpatialContextGeomReader()
{
	return( new FdoSmPhSpatialContextGeomReader(FDO_SAFE_ADDREF(this)) );
}

FdoPtr<FdoSmPhRdSchemaReader> FdoSmPhMgr::CreateRdSchemaReader( FdoSmPhRowsP rows, FdoSmPhOwnerP owner, bool dsInfo )
{
    return new FdoSmPhRdSchemaReader( rows, owner, dsInfo );
}

FdoPtr<FdoSmPhRdClassReader> FdoSmPhMgr::CreateRdClassReader( 
    FdoPtr<FdoSmPhRowCollection> rows, 
    FdoStringP schemaName, 
    FdoStringP className,
    FdoBoolean keyedOnly,
    FdoStringP database,
    FdoStringP owner
)
{
    return new FdoSmPhRdClassReader( rows, schemaName, className, FDO_SAFE_ADDREF(this), keyedOnly, database, owner );
}

FdoPtr<FdoSmPhRdPropertyReader> FdoSmPhMgr::CreateRdPropertyReader( FdoSmPhDbObjectP dbObject )
{
    return new FdoSmPhRdPropertyReader( dbObject, FDO_SAFE_ADDREF(this) );
}

const FdoSmPhDatabase* FdoSmPhMgr::RefDatabase(FdoStringP database) const
{
    return (FdoSmPhDatabase*) ((FdoSmPhMgr*) this)->FindDatabase(database);
}

FdoSmPhDatabaseP FdoSmPhMgr::FindDatabase(FdoStringP database, bool caseSensitive)
{
    if ( !mDatabases ) {
        // Cache doesn't exist, create it.
        mDatabases = new FdoSmPhDatabaseCollection();
        // The following creates the default database as the first item in the collection.
        GetDatabase();
    }

    // Find the database in the cache
    FdoSmPhDatabaseP pDatabase = mDatabases->FindItem(database);

    if ( !pDatabase ) {
        // Not in the cache, create it
        pDatabase = CreateDatabase(database);

        if ( pDatabase && (database == pDatabase->GetName()) ) {
            // Database exists, add it to the cache
            mDatabases->Add(pDatabase);
        }
        else {
            // Database doesn't exist
            pDatabase = NULL;
            if ( !caseSensitive) {
                // Try match to database with default case name if 
                // not doing case-sensitive match.
                FdoStringP CiName = GetDcDatabaseName( database );
                if ( CiName != database ) 
                    pDatabase = FindDatabase( CiName );
            }
        }
    }

    return(pDatabase);
}

FdoSmPhDatabaseP FdoSmPhMgr::GetDatabase(FdoStringP database)
{
    FdoSmPhDatabaseP pDatabase = FindDatabase(database);

    if ( !pDatabase ) 
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_1), 
				(FdoString*) database
            )
        );


    return(pDatabase);
}

FdoSmPhDatabaseP FdoSmPhMgr::GetCachedDatabase(FdoInt32 idx)
{
    FdoSmPhDatabaseP database;

    if ( mDatabases && (idx >= 0) && (idx < mDatabases->GetCount()) ) 
        database = mDatabases->GetItem( idx );

    return database;
}

FdoSmPhOwnerP FdoSmPhMgr::FindOwner(FdoStringP ownerName, FdoStringP database, bool caseSensitive)
{
    FdoSmPhDatabaseP pDatabase = FindDatabase(database, caseSensitive);
    FdoSmPhOwnerP pOwner;
    
    FdoStringP dfltdOwner;
    bool ownerDefaulted = false;
    
    if ( pDatabase ) {
        // If this is the default database and owner is blank then retrieve the 
        // default owner.
        if ( (wcslen(pDatabase->GetName()) == 0) && (ownerName.GetLength() == 0) ) {
            dfltdOwner = GetDefaultOwnerName();
            ownerDefaulted = true;
        }
        else {
            dfltdOwner = ownerName;
            ownerDefaulted = false;
        }

        // Try case-sensitive match first
        pOwner = pDatabase->FindOwner( dfltdOwner );

        if ( (!pOwner) && ((!caseSensitive) || ownerDefaulted) ) {
            // No match and not asked for case sensitive match, so 
            // try to match to owner with default case name
            FdoStringP CiName = GetDcOwnerName( dfltdOwner );
            if ( CiName != dfltdOwner ) {
                pOwner = pDatabase->FindOwner(CiName);
            }
        }

        if ( pOwner && ownerDefaulted )
            // Update default owner name to exact name.
            // This prevents doing subsequent database lookups
            // if matched to default case name.
            SetDefaultOwnerName( pOwner->GetName() );
    }


	return( pOwner );
}

FdoSmPhOwnerP FdoSmPhMgr::GetOwner(FdoStringP ownerName, FdoStringP database, bool caseSensitive)
{
    FdoSmPhOwnerP pOwner = FindOwner(ownerName, database, caseSensitive);

    if ( !pOwner && (ownerName != L"" || database != L"" || mDefaultOwnerName != L"")) {
        bool defaultDb = ( database == L"" );
		bool defaultOwner = ( ownerName == L"" );
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_415),
				(FdoString*) database,
				defaultDb ? L"" : L".",
                defaultOwner ? (FdoString*) mDefaultOwnerName : (FdoString*) ownerName
             )
        );
    }

    return(pOwner);
}

const FdoSmPhDbObject* FdoSmPhMgr::RefDbObject(FdoStringP objectName, FdoStringP owner, FdoStringP database ) const
{
    return (FdoSmPhDbObject*) ((FdoSmPhMgr*) this)->FindDbObject(objectName, owner, database );
}

FdoSmPhDbObjectP FdoSmPhMgr::FindDbObject(FdoStringP objectName, FdoStringP owner, FdoStringP database, bool caseSensitive)
{
    FdoSmPhDbObjectP dbObject;
    
    FdoSmPhOwnerP pOwner = FindOwner(owner, database, caseSensitive);
    if ( pOwner ) {
        // Try case-sensitive match first
        dbObject = pOwner->FindDbObject(objectName);

        if ( (!dbObject) && (!caseSensitive) ) {
            // No match and not asked for case sensitive match, so 
            // try to match to db object with default case name
            FdoStringP CiName = GetDcDbObjectName( objectName );
            if ( CiName != objectName ) 
                dbObject = pOwner->FindDbObject(CiName);
        }
    }

	return( dbObject );
}

void FdoSmPhMgr::SetConfiguration( 
    FdoStringP providerName,
    FdoIoStreamP configDoc,
    FdoFeatureSchemasP configSchemas,
    FdoSchemaMappingsP configMappings 
)
{
    // For now, overriding MetaSchema with config document is not allowed.
    if ( (configSchemas || configMappings) && FindDbObject( GetDcDbObjectName(L"f_schemainfo"), GetDefaultOwnerName() ) ) {
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_18_CONFIG_W_METASCHEMA),
                GetOwner()->GetName()
            )
        );
    }

    mProviderName = providerName;
    mConfigDoc = configDoc;
    mConfigSchemas = configSchemas;
    mConfigMappings = configMappings;
}

FdoStringP FdoSmPhMgr::GetProviderName()
{
    return mProviderName;
}

FdoIoStreamP FdoSmPhMgr::GetConfigDoc()
{
    return mConfigDoc;
}

FdoFeatureSchemasP FdoSmPhMgr::GetConfigSchemas()
{
    return mConfigSchemas;
}

FdoSchemaMappingsP FdoSmPhMgr::GetConfigMappings()
{
    return mConfigMappings;
}

FdoStringP FdoSmPhMgr::GetDbObjectClassification( FdoStringP dbObjectName )
{
    FdoStringP              className;
    FdoDictionaryElementP   elem = mDbObjectClassification->FindItem( dbObjectName );

    if ( elem )
        className = elem->GetValue();

    return className;
}

void FdoSmPhMgr::SetDbObjectClassification( FdoStringP dbObjectName, FdoStringP className )
{
    FdoDictionaryElementP   elem = mDbObjectClassification->FindItem( dbObjectName );

    if ( !elem ) {
        elem = FdoDictionaryElement::Create( dbObjectName, className );
        mDbObjectClassification->Add( elem );
    }
    else {
        elem->SetValue( className );
    }
}

bool FdoSmPhMgr::GetBulkLoadConstraints()
{
    return mBulkLoadConstraints;
}

void FdoSmPhMgr::SetBulkLoadConstraints( bool bulkLoad )
{
    mBulkLoadConstraints = bulkLoad;
}

bool FdoSmPhMgr::GetBulkLoadSpatialContexts()
{
    return mBulkLoadSpatialContexts;
}

void FdoSmPhMgr::SetBulkLoadSpatialContexts( bool bulkLoad )
{
    mBulkLoadSpatialContexts = bulkLoad;
}

/*
bool FdoSmPhMgr::IsObjectNameValid( FdoStringP objectName )
{
	// Censor out characters not allowed by the current provider,
	FdoStringP  workName = CensorDbObjName(objectName);

	// Adjust the name if it is too long, is a reserved name or the name of an existing
	// table.
	//
	// Do length check against char* version since this is what goes into the database.

	if ( (tableName != workName) ||
         (strlen(Name) > MaxObjectLen()) ||
		 IsObjectNameValid() 
	)
        return false;

    return true;
}
*/

void FdoSmPhMgr::AddRollbackTable(FdoString* name, FdoSchemaElementState elementState)
{
    if ( mRollbackCache ) 
        mRollbackCache->AddTable( name, elementState );
}
	
void FdoSmPhMgr::AddRollbackColumn(FdoString* tableName, FdoString* columnName, FdoSchemaElementState elementState)
{
    if ( mRollbackCache ) 
        mRollbackCache->AddColumn( tableName, columnName, elementState );
}
	
void FdoSmPhMgr::ClearRollback()
{
    if ( mRollbackCache ) 
        mRollbackCache->Clear();
}

bool FdoSmPhMgr::HasRollbackEntries()
{
    return mRollbackCache ? mRollbackCache->HasEntries() : false;
}

const FdoSmPhRbTable* FdoSmPhMgr::RefRollbackTable(FdoString* name)
{
    return mRollbackCache ? mRollbackCache->RefTable( name ) : NULL;
}
 
const FdoSmPhRbColumn* FdoSmPhMgr::RefRollbackColumn(FdoString* tableName, FdoString* columnName)
{
    return mRollbackCache ? mRollbackCache->RefColumn( tableName, columnName ) : NULL;
}

FdoSmPhSchemaWriterP FdoSmPhMgr::GetSchemaWriter()
{
    if ( !mSchemaWriter ) 
        mSchemaWriter = NewSchemaWriter();

    mSchemaWriter->Clear();

    return mSchemaWriter;
}

FdoSmPhClassWriterP FdoSmPhMgr::GetClassWriter()
{
    if ( !mClassWriter ) 
        mClassWriter = NewClassWriter();

    mClassWriter->Clear();

    return mClassWriter;
}

FdoSmPhPropertyWriterP FdoSmPhMgr::GetPropertyWriter()
{
    if ( !mPropertyWriter ) 
        mPropertyWriter = NewPropertyWriter();

    mPropertyWriter->Clear();

    return mPropertyWriter;
}

FdoSmPhDependencyWriterP FdoSmPhMgr::GetDependencyWriter()
{
    if ( !mDependencyWriter ) 
        mDependencyWriter = NewDependencyWriter();

    mDependencyWriter->Clear();

    return mDependencyWriter;
}

FdoSmPhAssociationWriterP FdoSmPhMgr::GetAssociationWriter()
{
    if ( !mAssociationWriter ) 
        mAssociationWriter = NewAssociationWriter();

    mAssociationWriter->Clear();

    return mAssociationWriter;
}

FdoSmPhSpatialContextWriterP FdoSmPhMgr::GetSpatialContextWriter()
{
    if ( !mSpatialContextWriter ) 
        mSpatialContextWriter = NewSpatialContextWriter();

    mSpatialContextWriter->Clear();

    return mSpatialContextWriter;
}

FdoSmPhSpatialContextGroupWriterP FdoSmPhMgr::GetSpatialContextGroupWriter()
{
    if ( !mSpatialContextGroupWriter ) 
        mSpatialContextGroupWriter = NewSpatialContextGroupWriter();

    mSpatialContextGroupWriter->Clear();

    return mSpatialContextGroupWriter;
}

FdoSmPhSpatialContextGeomWriterP FdoSmPhMgr::GetSpatialContextGeomWriter()
{
    if ( !mSpatialContextGeomWriter ) 
        mSpatialContextGeomWriter = new FdoSmPhSpatialContextGeomWriter(FDO_SAFE_ADDREF(this));

    mSpatialContextGeomWriter->Clear();

    return mSpatialContextGeomWriter;
}

/*
FdoSmPhIndexWriter* FdoSmPhMgr::GetIndexWriter()
{
    if ( !mpIndexWriter ) 
        mpIndexWriter = new FdoSmPhIndexWriter(this);

    mpIndexWriter->Clear();

    return mpIndexWriter;
}

FdoSmPhIndexPropertyWriter* FdoSmPhMgr::GetIndexPropertyWriter()
{
    if ( !mpIndexPropertyWriter ) 
        mpIndexPropertyWriter = new FdoSmPhIndexPropertyWriter(this);

    mpIndexPropertyWriter->Clear();

    return mpIndexPropertyWriter;
}
*/
FdoSmPhSADWriterP FdoSmPhMgr::GetSADWriter()
{
    if ( !mpSADWriter ) 
        mpSADWriter = NewSADWriter();

    mpSADWriter->Clear();

    return mpSADWriter;
}

/*
void FdoSmPhMgr::ExecuteSelect( FdoSmPhFieldCollection* fields, FdoStringP whereClause )
{
	FdoSmPhQueryReader rdr( fields, whereClause, this );
	while ( rdr.ReadNext() );
}
*/
void FdoSmPhMgr::Commit()
{
	for ( int i = 0; i < mDatabases->GetCount(); i++ ) 
		FdoSmPhDatabaseP(mDatabases->GetItem(i))->Commit();
}

FdoSmPhMgr::CoordinateSystemMatchLevel FdoSmPhMgr::GetCoordinateSystemMatchLevel()
{
    return CoordinateSystemMatchLevel_Lax;
}

bool FdoSmPhMgr::SupportsAnsiQuotes()
{
    return true;
}

bool FdoSmPhMgr::IsRdbObjNameAscii7()
{
    return false;
}

FdoSize FdoSmPhMgr::TableNameMaxLen()
{
    return DbObjectNameMaxLen();
}

bool FdoSmPhMgr::SupportsMixedCase()
{
    return false;
}

FdoStringP FdoSmPhMgr::GetDcDatabaseName( FdoStringP databaseName )
{
    return GetDcRdbmsObjectName( databaseName );
}

FdoStringP FdoSmPhMgr::GetDcOwnerName( FdoStringP ownerName )
{
    return GetDcRdbmsObjectName( ownerName );
}

FdoStringP FdoSmPhMgr::GetDcDbObjectName( FdoStringP objectName )
{
    return GetDcRdbmsObjectName( objectName );
}

FdoStringP FdoSmPhMgr::GetDcColumnName( FdoStringP columnName )
{
    return GetDcRdbmsObjectName( columnName );
}

FdoStringP FdoSmPhMgr::GetDcRdbmsObjectName( FdoStringP objectName )
{
    return objectName.Lower();
}

FdoStringP FdoSmPhMgr::GetRealDbObjectName( FdoStringP objectName)
{
    return objectName;
}

FdoStringP FdoSmPhMgr::DbObject2MetaSchemaName( FdoStringP objectName )
{
    return objectName;
}

FdoStringP FdoSmPhMgr::CensorDbObjectName( FdoStringP objName, bool forceAscii7, bool compress )
{
    if ( (!forceAscii7) && (!IsRdbObjNameAscii7()) ) 
        // not forcing to ASCII&, and provider supports non-ASCII7 element
        // names in RDBMS. Leave object name as is.
        return objName;

    wchar_t* workString = (wchar_t*) alloca((objName.GetLength()+1) * sizeof(wchar_t));
    wcscpy(workString, (const wchar_t*)objName);

    size_t iDest = 0;
    int censor = 0;

    for ( size_t i = 0; i < wcslen(workString); i++ ) {

        // Get the Utf8 version of each unicode character in the name.
        FdoStringP midString = objName.Mid( i, 1 );
        unsigned char* workChar = (unsigned char*)((const char *)midString);

        bool charOk = true;
        // Check each utf8 char to see if it is acceptable to the RDBMS.
        for ( size_t j = 0; j < strlen((const char*)workChar); j++ ) {
            // Only alphanumerics, '_' or '$' are valid in a database object name.
            // 8-bit characters are also risky so censor them out even if they pass
            // the isalnum test. Due to the unicode-utf8 conversion, the 8-bit characters
            // in the utf8 string won't look anything like the unicode characters anyway.

            if ( (!isalnum(workChar[j])) || (workChar[j] & 0x80) ) {
                if ( workChar[j] != '_' && workChar[j] != '$' && workChar[j] != '.' ) {
                        // Not all chars ok so change the current unicode character.
                        charOk = false;
                        break;
                }
            }
        }

        if ( charOk )
            censor = 0;
        else
            censor++;

        switch (censor) {
        case 0:
            // In State 0, the character is ok so leave it as is.
            if ( i != iDest ) 
                // Name has been compressed so shift character to new position.
                workString[iDest] = workString[i];
            iDest++;
            break;

        case 1:
            // Current character not ok, previous character ok.
            // Censor current character.
            workString[iDest++] = '_';
            break;
        
        default:
            // Current and previous characters not ok.
            // Suppress writing censor character if compressing.
            // Representing consecutive censored characters with a
            // single '_' makes the object name slightly more readable.
            if ( !compress ) 
                workString[iDest++] = '_';
            break;
        }
    }

    workString[iDest] = 0;

    FdoStringP outName(workString);

    // First character in a database object name must be alphabetic.
    // If not then prepend an "A" ( for application ).
    if ( !iswalpha(workString[0]) ) 
        outName = ObjPrefix + outName;

    return(outName);
}


FdoStringP FdoSmPhMgr::FormatSQLVal( FdoStringP value, FdoSmPhColType valueType )
{
	FdoStringP sqlString;
    
    if ( value.GetLength() > 0 ) {
        if ( valueType == FdoSmPhColType_String || valueType == FdoSmPhColType_Date )
            sqlString = FdoStringP(L"'") + FdoStringP(value).Replace( L"'", L"''" ) + FdoStringP(L"'");
        else
            sqlString = value;
    }
    else {
        sqlString = L"null";
    }

	return sqlString;
}

FdoStringP FdoSmPhMgr::FormatOrderCol( FdoStringP colName, FdoSmPhColType colType )
{
	return colName;
}

void FdoSmPhMgr::ValidateStringLength( 
    FdoStringP pString, 
    size_t limit, 
    FdoInt32   elementNlsNum,
    const char* dfltElementName,
    FdoInt32   itemNlsNum,
    const char* dfltItemName
) const
{
    if ( strlen((const char*) FdoStringP(pString)) > limit ) {
        FdoStringP elementName = FdoSmError::NLSGetMessage( elementNlsNum, (char*) dfltElementName );     
        FdoStringP itemName = FdoSmError::NLSGetMessage( itemNlsNum, (char*) dfltItemName );

		//UTF8 representation of %1$ls %2$ls exceeds %3$d characters, value is '%4$ls'",
        throw FdoSchemaException::Create(
			FdoSmError::NLSGetMessage(
				FDO_NLSID( FDOSM_272 ),
				(FdoString*) elementName,
                (FdoString*) itemName,
                limit,
                (FdoString*) pString
			)
		);
    }
}

void FdoSmPhMgr::SetRollbackCache( FdoSmPhRbCacheP rollbackCache )
{
    mRollbackCache = rollbackCache;
}

void FdoSmPhMgr::Clear()
{
    mDbObjectClassification = FdoDictionary::Create();

    if ( mDatabases ) 
        mDatabases->Clear();

    mBulkLoadConstraints = false;
    mBulkLoadSpatialContexts = false;
}

void FdoSmPhMgr::OnAfterCommit()
{
    FdoInt32 idx;

    if ( mDatabases ) {
        for ( idx = 0; idx < mDatabases->GetCount(); idx++ )
            FdoSmPhDatabaseP(mDatabases->GetItem(idx))->OnAfterCommit();
    }
}
 
void FdoSmPhMgr::XMLSerialize( FdoString* sFileName ) const
{
#ifdef _WIN32
	FILE* xmlFp = _wfopen( sFileName, L"w" );
#else
	FILE* xmlFp = fopen( (const char*) FdoStringP(sFileName), "w" );
#endif

	fprintf( xmlFp, "<?xml version=\"1.0\" standalone=\"yes\"?>\n" );
	fprintf( xmlFp, "<physical xmlns:xsi=\"http://www.w3.org/2001/XMLSchema\" >\n" );
	
    if ( mDatabases ) {
    	for ( int i = 0; i < mDatabases->GetCount(); i++ ) {
	    	mDatabases->RefItem(i)->XMLSerialize(xmlFp, 0);
	    }
    }
	
	fprintf( xmlFp, "</physical>\n" );

	fclose(xmlFp);

}
