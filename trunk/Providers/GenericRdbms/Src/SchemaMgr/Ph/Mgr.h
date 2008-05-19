#ifndef FDOSMPHGRDMGR_H
#define FDOSMPHGRDMGR_H     1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Reader.h>
#include <Sm/Ph/ClassWriter.h>
#include <Sm/Ph/SpatialContextReader.h>
#include <Sm/Ph/SpatialContextWriter.h>
#include <Sm/Ph/SpatialContextGroupReader.h>
#include <Sm/Ph/SpatialContextGroupWriter.h>
#include <Sm/Ph/PropertyWriter.h>
#include <Sm/Ph/DependencyWriter.h>
#include <Sm/Ph/AssociationWriter.h>
#include <Sm/Ph/SchemaWriter.h>
#include <Sm/Ph/SADWriter.h>
#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>
#ifdef USE_NONRDBMS_HEADER
#include <inc/rdbi/context.h>
#else
#include <Inc/Rdbi/context.h>
#endif
#include "../../Gdbi/GdbiTypes.h"
#include "../../Gdbi/GdbiQueryResult.h"
#include "../../Gdbi/GdbiStatement.h"
#include "../../Gdbi/GdbiConnection.h"
#include "../../Gdbi/GdbiCommands.h"

class FdoSmPhStaticReader : public FdoSmPhSchemaElement
{
public:
	FdoSmPhStaticReader( FdoStringP readerName, FdoSmPhReaderP reader ):
		FdoSmPhSchemaElement( readerName, L"", (FdoSmPhMgr*)NULL, NULL)
	{
		mReader = FDO_SAFE_ADDREF( reader.p );
	}
	FdoSmPhReaderP GetReader()
	{
		return mReader;
	}

protected:
	FdoSmPhStaticReader();
	~FdoSmPhStaticReader(void){}

private:
	FdoSmPhReaderP mReader;
};

typedef FdoPtr<FdoSmPhStaticReader> FdoSmPhStaticReaderP;

class FdoSmPhStaticReaderCollection : public FdoSmNamedCollection<FdoSmPhStaticReader>
{
	public:
	FdoSmPhStaticReaderCollection(void) :
		FdoSmNamedCollection<FdoSmPhStaticReader>()
	{}
	~FdoSmPhStaticReaderCollection(void) {}
};

typedef FdoPtr<FdoSmPhStaticReaderCollection> FdoSmPhStaticReadersP;

// This is the base class for RDBMS type provider Physical Schema Managers
class FdoSmPhGrdMgr : public FdoSmPhMgr
{
public:

    // Create a reader for retrieving rows for an RDBMS query.
    // Query is specified by table list (rows) and optional where clause.
    virtual FdoPtr<FdoSmPhRdQueryReader> CreateQueryReader(
        FdoPtr<FdoSmPhRowCollection> rows,
        FdoStringP where,
        FdoPtr<FdoSmPhRow> binds = (FdoSmPhRow*) NULL
    );

    // Execute a query and get a reader for its results set
    // Query is specified by full SQL statement.
    virtual FdoPtr<FdoSmPhRdQueryReader> CreateQueryReader(
        FdoPtr<FdoSmPhRow> fields,  // query's select column list. Must match fields 
                                    // in sStatement.
        FdoStringP sStatement,      // query statement. Must reference each column in the fields
                                    // collection and each field in the binds collection.
        FdoPtr<FdoSmPhRow> binds = (FdoSmPhRow*) NULL    // Fields to bind into query (Query parameters).
                                    // If NULL, then this is not a bound query.
    );

	// Create generic writer for writing to a database table.
    virtual FdoPtr<FdoSmPhCommandWriter> CreateCommandWriter( FdoPtr<FdoSmPhRow> row );

    // Returns the RDBMS connection being managed.
    GdbiConnection* GetGdbiConnection()
    {
        return mGdbiConnection;
    }

    rdbi_context_def* GetRdbiContext()
    {
        return mRdbiContext;
    }

    void SetRdbiContext( rdbi_context_def* context )
    {
        mRdbiContext = context;
    }

    // Get the home directory for MetaSchema creation scripts.
    FdoStringP GetHomeDir()
    {
        return (mHomeDir == L"") ? FdoStringP(L".") : mHomeDir;
    }

    void SetHomeDir( FdoStringP homeDir )
    {
        mHomeDir = homeDir;
    }

    // Execute a file to create the MetaSchema
    // Parameters
    //   file_name: input
    //      File name of file to be processed. Must be under HomeDir.
    //
    //  keywords: input
    //      list of keywords that will  be  defined.
    //      They can be detected within the executed file with #ifdef
    //      <keyword>.  This  parameter  may be NULL, meaning that no
    //      keywords are defined.
    void ExecSchemaFile(
        FdoStringP fileName,
        FdoStringsP keywords
    );

    // Checks for general reserved works. Each provider can override
    // this function to check for extra RDBMS vendor-specific words.
    // See FdoSmPhMqlMgr or FdoSmPhOraMgr for example.
    virtual FdoBoolean IsDbObjectNameReserved( FdoStringP objName );

    // Reverse-engineer the given table or view into a class, based on the autogeneration
    // directives in the given schema override set.
    // Returns L"" if the table's class is not part of the schema for the 
    // given override set.
    FdoStringP ClassifyDbObject( 
        FdoStringP tableName, 
        FdoPtr<FdoRdbmsOvPhysicalSchemaMapping> mapping, 
        FdoBoolean hasKey  // true if the table or view has identity, false otherwise.
                           // Objects without identity are only classified if they are 
                           // explicitly referenced by the table list in the mapping's
                           // autogeneration directives.
    );

    // Given a schema override set, extract the database instance setting
    virtual FdoStringP GetOverrideDatabase(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the owner (datastore) setting
    virtual FdoStringP GetOverrideOwner(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the table storage (Oracle tablespace, SQL Server filegroup, etc) setting
    virtual FdoStringP GetOverrideTableStorage(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the index storage setting
    virtual FdoStringP GetOverrideIndexStorage(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the text storage setting
    virtual FdoStringP GetOverrideTextStorage(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the text-in-row setting
    virtual FdoStringP GetOverrideTextInRow(FdoRdbmsOvSchemaMappingP mapping);
    // Given a schema override set, extract the table storage engine setting
    virtual FdoStringP GetOverrideTableStorageEngine(FdoRdbmsOvSchemaMappingP mapping);

    virtual bool IsRdbUnicode();

	void SetStaticReader( FdoStringP readerName, FdoPtr<FdoSmPhReader> reader );
	FdoPtr<FdoSmPhReader> GetStaticReader( FdoStringP readerName );
	void RemoveStaticReaders();

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhGrdMgr() {}

    // Create Physical Schema Manager..
    FdoSmPhGrdMgr(
        GdbiConnection* connection // provides access to the RDBMS
    );

    virtual ~FdoSmPhGrdMgr(void);

    	// Get writer for writing feature schemas to the current connection.
    virtual FdoSmPhSchemaWriterP NewSchemaWriter();

	// Get writer for writing classes to the current connection.
    virtual FdoSmPhClassWriterP NewClassWriter();

	// Get writer for writing properties to the current connection.
    virtual FdoSmPhPropertyWriterP NewPropertyWriter();

	// Get writer for writing attribute dependencies to the current connection.
    virtual FdoSmPhDependencyWriterP NewDependencyWriter();

	// Get writer for writing associations to the current connection.
    virtual FdoSmPhAssociationWriterP NewAssociationWriter();

	// Get writer for writing Schema Attribute Dictionary entries
    // to the current connection.
    virtual FdoSmPhSADWriterP NewSADWriter();

	// Get writer for writing spatial contexts to the current connection.
    virtual FdoSmPhSpatialContextWriterP NewSpatialContextWriter();

	// Get writer for writing spatial contextgroups to the current connection.
    virtual FdoSmPhSpatialContextGroupWriterP NewSpatialContextGroupWriter();    // General reserved word list base class.
    // Providers can extend to add specific reserved words.
    class StringMap : public std::map<FdoStringP,void*>
    {
    public:
        StringMap();
        virtual ~StringMap() {}

        void Insert( FdoString* resString );

        bool IsReserved( FdoStringP checkString );
    };

private:
    // Removes C-style comments
    void RemoveFileComments(char* line, int* inside_comment, char* quote_char);

    GdbiConnection*     mGdbiConnection;
    rdbi_context_def*   mRdbiContext;
    FdoStringP          mHomeDir;

    // General reserved word list (words reserved by most RDBMS's).
    StringMap mReservedDbObjectNames;
	FdoSmPhStaticReadersP mStaticReaders;
};

typedef FdoPtr<FdoSmPhGrdMgr> FdoSmPhGrdMgrP;

#endif

