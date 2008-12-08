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

#ifndef CPP_UNIT_UNITTESTUTIL_H
#define CPP_UNIT_UNITTESTUTIL_H

// Various utility functions to help with unit testing.
#include <Sm/SchemaManager.h>
#include "SchemaOverrideUtil.h"

enum StringConnTypeRequest {
    Connection_WithDatastore,
    Connection_NoDatastore,
    Connection_WithDSN,
    Connection_OraSetup
};

class ConnectionUtil;
class StaticConnection;

class UnitTestUtil
{
    static char *CommandFilesDirectory;
    static FdoString *ActiveProviderName;
public:
	static ConnectionUtil* InfoUtilConnection;

	static char* pSortLpSheet;
public:
    UnitTestUtil(void);
    ~UnitTestUtil(void);

    // Executes a set of SQL commands against the given connection.
    // A single command can span multiple elements in the
    // sCommands array. All commands must be separated by ';'
    // sCommands can have any number of elements as long as the
    // last element is NULL.
    static void Sql2Db(const wchar_t** sCommands, FdoIConnection* connection);
    // Executes a single SQL command against the given connection.
    static void Sql2Db(const wchar_t* sCommand, FdoIConnection* connection);
    static void CreateAcadSchema(FdoIConnection* connection, bool useBaseMapping=false );

	static void CreateLandSchema(FdoIConnection* connection );
	static void CreateNonUniqueSchema(FdoIConnection* connection );
	static void CreateCityTable(FdoIConnection* connection );
    static void CreateTable1(FdoIConnection* connection );

	static void GetRealUserName(char * user_name, unsigned size );
	static char *w2a(const wchar_t* wString);

    // Fail the current Unit Test, showing the given exception's message.
    static void FailOnException( FdoException* e );

    // Print an exception, and all it's cause exceptions to a file
    // or stdout by default.
    static void PrintException( FdoException* e, FILE* fp = stdout, FdoBoolean stripLineNo = false );

    // Print an exception, and all it's cause exceptions to the given file.
    static void PrintException( FdoException* e, const char* fileName, FdoBoolean stripLineNo = false );

    // Given an exception, return a string with the messages for the
    // first and last exceptions in the exception chain.
    static void Exception2String( FdoException* e, char* buffer );

    // Get the value of the environment variable specified by pVar.
    // If pVar is not set the pDefault is returned.
    static const char* GetEnv( const char* pVar, const char* pDefault );

	static FdoIConnection* GetProviderConnectionObject();
	static wchar_t *GetConnectionString(StringConnTypeRequest pTypeReq = Connection_WithDatastore, FdoString *suffix = L"", bool bAddExtraneousSpaces = false);
    static void CreateDB(bool addSchema = true, bool useBaseMapping=false, FdoString *suffix = L"", int lt_method = 0, bool lt_method_fixed = false );
	static void CreateDB( FdoIConnection* connection, FdoString *datastore, FdoString *description, FdoString *password, char *schemaType, int local_lt_method );   
    static FdoSmPhOwnerP CreateDBNoMeta( FdoSchemaManagerP sm, FdoString *datastore, FdoString *password = L"test" );
    static FdoStringP GetEnviron(const char *name, FdoString *suffix = L"");
	static void SetProvider( const char *providerName );

    // Set up and open a connection to a Database.
    //
    // The default* parameters provide default service, user, password,
    // and datastore values.
    //
    // The override* parameters specify environment variables that override
    // the above default values ( if the variables are set ).
    //
    // bPredelete: if true, delete the datastore if it exists.
    // bCreate: if true, create the datastore if it doesn't exist or was
    //      predeleted.
    // bCreated: indicates whether datastore was created. This parameter is
    //      optional.
    // lt_method:  determines whether database is created or opened in
    //      OWM mode. can be one of:
    //
    //      DBI_WORKSPACE_MANAGER_LT_METHOD - OWM enabled datastore
    //      DBI_NO_LT_METHOD - datastore without versioning. Any tables
    //          that are created are not OWM enabled. Other tables
    //          remain as they are.
    //      -1 - defaults to DBI_WORKSPACE_MANAGER_LT_METHOD
    //
    // lt_method_fixed:
    //      false: lt_method can be overridden by the "ltmethod"
    //          command line argument.
    //      true: lt_method cannot be overridden.
    static FdoIConnection*  CreateConnection(
        bool bPredelete,
        bool bCreate,
        FdoString *suffix,
        const char *schemaType = NULL,
        bool* bCreated = NULL,
        int  lt_method = -1,
        bool lt_method_fixed = false
    );

	// Close a connection to a Database.
    //
    // The default* parameters provide default service, user, password,
    // and datastore values.
    //
    // The override* parameters specify environment variables that override
    // the above default values ( if the variables are set ).
    //
    // bDelete: if true, delete the datastore after closing connection.
    static void CloseConnection(
        FdoIConnection* connection,
        bool bDelete,
        FdoString *suffix
    );
	static void DropDb(FdoString *suffix = L"" );
	static void DropDb( FdoIConnection *connection, FdoString* pDatastore, FdoString* pPassword, FdoString* pService );
    
	// Check if the given connect contains the given datastore.
    //
    // Returns:
    //      true - datastore exists.
    //      false - otherwise.
    static bool DatastoreExists(FdoString *suffix = L"");
    
    static void CreateSpatialContext( 
        FdoIConnection* connection,
        FdoStringP scName, 
        FdoStringP csysName, 
        double xmin, double ymin, double xmax, double ymax, 
        FdoStringP csysWKT = L"", 
        double xyTol = 0.001, double zTol = 0.001 
    );

    static void DeleteSpatialContext( 
        FdoIConnection* connection,
        FdoStringP scName
    );

	static FdoIConnection* GetConnection(FdoString *suffix = L"", bool bCreate = false, bool bRecreateData = false, StringConnTypeRequest pTypeReq = Connection_WithDatastore, int lt_method = 0, bool lt_method_fixed = false );
    static FdoPtr<FdoIConnection> UnitTestUtil::GetDirectConnection (FdoIConnection *currentConnection);
    static StaticConnection* NewStaticConnection();
    static SchemaOverrideUtilP NewSchemaOverrideUtil();

    // compares two doubles for *approximate* equality:
    static bool fuzzyEqual (const double d1, const double d2);

	// Compare an output file against its master (expected) copy.
    // Fail if they are different.
    static void CheckOutput( const char* masterFileName, const char* outFileName );

    // Checks if a read only property collection contains the named property
    static bool ContainsRdOnlyProp( FdoReadOnlyPropertyDefinitionCollection* props, FdoString* propName );

    // Finds a named property in a read only property collection
    static FdoPropertyP FindRdOnlyProp( FdoReadOnlyPropertyDefinitionCollection* props, FdoString* propName );

	// Compare to files.
    //
    // Returns:
    //      0 - they are the same
    //      -1 - they are different.
    static int CompareFiles( const char* file1Name, const char* file2Name );
 
	static void GrantDatastore( FdoIConnection* connection, FdoSchemaManagerP mgr, FdoStringP privileges, FdoStringP grantee );

	static void WriteSortedLp( const FdoSmLpSchemaCollection* lp, FdoString* fileName, char* styleSheetString = NULL );

	static void ExportDb( 
        FdoIConnection* connection, 
        FdoIoStream* stream, 
        FdoXmlSpatialContextFlags* flags = NULL, 
        bool includeDefaultMappings = false, 
        // When the following two variables are set, any schema named
        // oldDsSchemaName is renamed to newDsSchemaName. This is usefull
        // for handling autogenerated schemas that will go into master files.
        // These schemas often have a user name in their names, which must be
        // removed.
        FdoString* oldDsSchemaName = NULL,
        FdoString* newDsSchemaName = L"AutoGen",
        bool includeFeatures = false
    );

    static void Config2SortedFile( FdoIoStream* stream, FdoString* fileName, char* styleSheetString = NULL );

    static void Stream2File( FdoIoStream* stream, FdoString* fileName );

	static void LogicalPhysicalFormat(FdoIoStream* stream1, FdoIoStream* stream2, FdoStringP providerName );

    static void LogicalPhysicalBend(FdoIoStream* stream1, FdoIoStream* stream2, FdoStringP providerName );

    static void LogicalPhysicalSort( FdoIoStream* stream1, FdoIoStream* stream2 );

	static void SortLpSchemas( FdoIoStream* inStream, FdoIoStream* outStream, char* styleSheetString = NULL );

	static void SortXml( FdoIoStream* inStream, char* styleSheetString, FdoIoStream* outStream );

	static void OverrideBend( FdoIoStream* stream1, FdoIoStream* stream2, FdoStringP providerName, 
		FdoStringP providerUri, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix, FdoStringP tablespace = L"" );
    static bool DoThisTest( char *testGroupName );

	// Accesses the data on the current feature.
    static void ProcessFeature(FdoIFeatureReader* featureReader);

	static FdoStringP GetOutputFileName (FdoString* pBaseFileName);

	// Combines the input strings into one string.  You must call
    // "delete" on the returned string when done.
    static wchar_t* MakeString(FdoString* str1,
                        FdoString* str2 = NULL,
                        FdoString* str3 = NULL,
                        FdoString* str4 = NULL,
                        FdoString* str5 = NULL,
                        FdoString* str6 = NULL,
                        FdoString* str7 = NULL,
                        FdoString* str8 = NULL,
                        FdoString* str9 = NULL,
                        FdoString* str10 = NULL);

    // Accesses the data on the current row.
    static void ProcessData(FdoIDataReader* reader);

    static wchar_t GetNlsChar( int index );
    static FdoStringP GetNlsValue( int index );
    static FdoStringP GetNlsObjectName( FdoStringP inName );

    static FdoSmPhScInfoP CreateScInfo( FdoInt64 srid, double minx, double miny, double maxx, double maxy, double xtol, double ztol );

private:
	static char* pSortConfigSheet;
};

#endif // CPP_UNIT_UNITTESTUTIL_H
