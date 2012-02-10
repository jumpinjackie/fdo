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
#include <Sm/Ph/ClassReader.h>
#include <Sm/Ph/ClassWriter.h>
#include <Sm/Ph/Mt/ClassReader.h>
#include <Sm/Ph/Rd/ClassReader.h>
#include <Sm/Ph/Rd/PropertyReader.h>
#include <Sm/Ph/Cfg/ClassReader.h>
#include <Sm/Ph/Cfg/PropertyReader.h>
#include <Sm/Ph/ClassSOWriter.h>


FdoSmPhClassReader::FdoSmPhClassReader(FdoStringP schemaName, FdoSmPhMgrP physicalSchema) : 
	FdoSmPhReader( MakeReader(schemaName, physicalSchema) ),
	mSchemaName(schemaName),
    mbMultiClassReder(true)
{
    mpSOReader = new FdoSmPhSOReader(FdoSmPhMgr::ClassType, physicalSchema->GetOwner());
}

FdoSmPhClassReader::FdoSmPhClassReader(FdoStringP schemaName, FdoStringP className, FdoSmPhMgrP physicalSchema) : 
	FdoSmPhReader( MakeReader(schemaName, physicalSchema, className) ),
	mSchemaName(schemaName),
    mbMultiClassReder(true)
{
    mpSOReader = new FdoSmPhSOReader(FdoSmPhMgr::ClassType, physicalSchema->GetOwner());
}

FdoSmPhClassReader::FdoSmPhClassReader(FdoStringP schemaName, FdoSmPhMgrP physicalSchema, bool fullLoad) : 
	FdoSmPhReader( MakeReader(schemaName, physicalSchema, NULL, fullLoad) ),
	mSchemaName(schemaName),
    mbMultiClassReder(true)
{
    mpSOReader = new FdoSmPhSOReader(FdoSmPhMgr::ClassType, physicalSchema->GetOwner());
}

FdoSmPhClassReader::~FdoSmPhClassReader(void)
{
}

FdoInt64 FdoSmPhClassReader::GetId()
{
	return( GetInt64(L"", L"classid") );
}

FdoStringP FdoSmPhClassReader::GetName()
{
	return(GetString(L"", L"classname"));
}

FdoStringP FdoSmPhClassReader::GetDescription()
{
	return(GetString(L"", L"description"));
}

FdoStringP FdoSmPhClassReader::GetBaseName()
{
	return(GetString(L"", L"parentclassname"));
}

FdoStringP FdoSmPhClassReader::GetClassType()
{
	return(GetString(L"", L"classtypename"));
}

bool FdoSmPhClassReader::GetIsAbstract()
{
	return(GetBoolean(L"", L"isabstract"));
}

FdoStringP FdoSmPhClassReader::GetDatabase()
{
   	return(GetString(L"", L"tablelinkname"));
}

FdoStringP FdoSmPhClassReader::GetOwner()
{
   	return(GetString(L"", L"tableowner"));
}


FdoStringP FdoSmPhClassReader::GetGeometryProperty()
{
	return(GetString(L"", L"geometryproperty"));
}

bool FdoSmPhClassReader::GetIsFixedTable()
{
	return(GetBoolean(L"", L"isfixedtable"));
}

bool FdoSmPhClassReader::GetIsTableCreator()
{
// TODO:   if ( mbTableCreatorDefined ) {
        return(GetBoolean(L"", L"istablecreator"));
/*
    }
    else {
        return( Get_Table() != NULL );
    }
*/
}

FdoStringP FdoSmPhClassReader::GetTableName()
{
	return(GetManager()->GetRealDbObjectName(GetString(L"", L"tablename")));
}

FdoStringP FdoSmPhClassReader::GetRootTableName()
{
	return(GetManager()->GetRealDbObjectName(GetString(L"", L"roottablename")));
}

FdoStringP FdoSmPhClassReader::GetTableMapping()
{
	return(GetString(L"", L"tablemapping"));
}

FdoStringP FdoSmPhClassReader::GetTableStorage()
{
	if (mbSchemaOptionsTableDefined && mbReadFromMetadata)
		return mpClassSOReader->GetOption(L"tablestorage");  // read from F_SCHEMAOPTIONS instead of F_SCHEMAINFO
	else
    	return(GetString(L"", L"tablespacename"));
}

FdoStringP FdoSmPhClassReader::GetOption( FdoStringP optionName )
{
	if (mbSchemaOptionsTableDefined && mbReadFromMetadata)
		return mpClassSOReader->GetOption(optionName);
	else
		return L"";
}

FdoSmPhClassPropertyReaderP FdoSmPhClassReader::CreateClassPropertyReader()
{
    FdoSmPhPropertyReaderP propReader;
    FdoSmPhOwnerP owner = GetManager()->GetOwner();
    FdoStringP className = GetName();

    if (owner && owner->GetHasAttrMetaSchema() ) {
        // for now we will ask only for one class.
        FdoPtr<FdoStringCollection> classNames;
        // do not use single class read when we expect to read all classes (or a subset)
        if (!IsMultiClassReder())
        {
            classNames = FdoStringCollection::Create();
            classNames->Add(className);
        }
        // Read properties from metaschema
	    if ( !mPropReader ) 
		    mPropReader = new FdoSmPhPropertyReader(mSchemaName, GetManager(), classNames);
    
        propReader = mPropReader;
    }
    else {
        FdoSmPhDbObjectP dbObject = GetManager()->FindDbObject(
            GetTableName(),
            GetOwner(),
            GetDatabase()
        );

        if ( GetManager()->GetConfigMappings() || GetManager()->GetConfigSchemas() ) {
            // Config document is set so read properties from it.
            propReader = new FdoSmPhPropertyReader(
                GetManager()->CreateCfgPropertyReader( mSchemaName, GetName(), dbObject ).p->SmartCast<FdoSmPhReader>(),
                GetManager()
            );
        }
        else {

            // Properties not in metaschema or config document so read them 
            // from the native physical schema.
            propReader = new FdoSmPhPropertyReader( 
                            GetManager()->CreateRdPropertyReader( dbObject).p->SmartCast<FdoSmPhReader>(),
                            GetManager()
            );

            if ( dbObject )
                className = dbObject->GetBestClassName();
        }
    }


	// Create the Property Schema Attribute Dictionary (SAD) reader if not 
	// already done.
	if ( !mPropSADReader ) 
	    mPropSADReader = new FdoSmPhSADReader(FdoSmPhMgr::PropertyType, GetManager(), mSchemaName );

	// Note that the class property reader only works properly if the 
	// class reader and property reader both retrieve their rows ordered
	// by class name.

	return new FdoSmPhClassPropertyReader(mSchemaName, className, propReader, mPropSADReader);
}

/*TODO
FdoSmPhClassIndexReaderP FdoSmPhClassReader::GetClassIndexReader()
{
	// Create the index reader if not already done.
	if ( !mpIndexReader ) 
		mpIndexReader = new FdoSmPhIndexReader(mSchemaName, GetPhysicalSchema());

	// Note that the class index reader only works properly if the 
	// index reader and indexproperty reader both retrieve their rows ordered
	// by class name and index name.
	return new FdoSmPhClassIndexReader(mSchemaName, GetName(), mpIndexReader );
}
*/
FdoSmPhClassSADReaderP FdoSmPhClassReader::GetClassSADReader()
{
	// Create the Class SAD reader if not already done.
	if ( !mClassSADReader ) 
		mClassSADReader = new FdoSmPhSADReader(FdoSmPhMgr::ClassType, GetManager(), mSchemaName );

	// Note that the class SAD reader only works properly if the 
	// Class reader and the underlying Class SAD reader both retrieve their 
	// rows ordered by class name.
	return new FdoSmPhClassSADReader(mSchemaName, GetName(), mClassSADReader );
}

FdoSmPhReaderP FdoSmPhClassReader::MakeReader( FdoStringP schemaName, FdoSmPhMgrP mgr, FdoString* className, bool fullLoad)
{
    mbTableCreatorDefined = false;
    mbSchemaOptionsTableDefined = false;
    mbReadFromMetadata = false;
    FdoSmPhReaderP pSubReader;

    // Create the row collection describing class attributes.
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    FdoSmPhRowP classRow = FdoSmPhClassWriter::MakeRow(mgr);
    rows->Add( classRow );

    FdoSmPhRowP typeRow = FdoSmPhClassWriter::MakeClassTypeRow(mgr);
    rows->Add( typeRow );


	// Determine which table/field names to use, depending on if F_SCHEMAOPTIONS exists:
    FdoSmPhOwnerP owner = mgr->GetOwner();
    mbSchemaOptionsTableDefined = owner->GetHasSCOptionMetaSchema();

	// Create the appropriate class reader:
    FdoSchemaMappingsP mappings = mgr->GetConfigMappings();
    FdoFeatureSchemasP schemas = mgr->GetConfigSchemas();
    if ( mappings || schemas ) {
        // Config document is set so read classes from it.
        pSubReader = mgr->CreateCfgClassReader( rows, schemaName ).p->SmartCast<FdoSmPhReader>();
    }
    else {
        if (owner->GetHasClassMetaSchema()) {
            mbReadFromMetadata = true;
            // F_CLASSDEFINITION exists, read from MetaSchema
            pSubReader = MakeMtReader( rows, schemaName, mgr, className, fullLoad);
        }
        else {
            // F_CLASSDEFINITION does not exist, read from native physical schema.
            pSubReader = MakeRdReader( rows, schemaName, mgr, className );
        }
    }

    return pSubReader;
}

FdoSmPhReaderP FdoSmPhClassReader::MakeMtReader( FdoSmPhRowsP rows, FdoStringP schemaName, FdoSmPhMgrP mgr, FdoString* className, bool fullLoad )
{
    return new FdoSmPhMtClassReader( rows, schemaName, className, mgr, fullLoad);
}

FdoSmPhReaderP FdoSmPhClassReader::MakeRdReader( FdoSmPhRowsP rows, FdoStringP schemaName, FdoSmPhMgrP mgr, FdoString* className  )
{
    return mgr->CreateRdClassReader( rows, schemaName, className )->SmartCast<FdoSmPhReader>();
}


bool FdoSmPhClassReader::ReadNext()
{
	bool ret = FdoSmPhReader::ReadNext();

	// Create new schema options reader for this class (wrapped around mpSOReader):
	if (ret)
	{
        mpClassSOReader = new FdoSmPhClassSOReader(mSchemaName + L":" + GetName(), mpSOReader);
		mpClassSOReader->ReadNext();
	}
	else
		mpClassSOReader = NULL;

	return ret;
}
