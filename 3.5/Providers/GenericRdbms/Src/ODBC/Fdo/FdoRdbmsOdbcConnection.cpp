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
#include <time.h>

#ifdef _WIN32
#include <tchar.h>
#include<odbcinst.h>
#else
#include<odbcinst.h>
#endif

#include <Inc/ut.h>
#include "FdoRdbmsOdbcConnectionInfo.h"
#include "FdoRdbmsOdbcConnection.h"
#include "FdoRdbmsOdbcCreateDataStore.h"
#include "FdoRdbmsOdbcDeleteDataStore.h"
#include "FdoRdbmsOdbcFilterProcessor.h"

#include "FdoRdbmsOdbcConnectionCapabilities.h"
#include "FdoRdbmsOdbcCommandCapabilities.h"
#include "FdoRdbmsOdbcExpressionCapabilities.h"
#include "FdoRdbmsOdbcFilterCapabilities.h"
#include "FdoRdbmsOdbcSchemaCapabilities.h"
#include "FdoRdbmsOdbcGeometryCapabilities.h"

#include "../SchemaMgr/SchemaManager.h"
#include "../SchemaMgr/Ph/Mgr.h"
#include "../../Fdo/Capability/FdoRdbmsGeometryCapabilities.h"

#include "FdoRdbmsOdbcSpatialManager.h"

#include "DbiConnection.h"
#include <FdoCommonConnStringParser.h>

#include <Inc/Rdbi/proto.h>
#include "../../ODBCDriver/context.h"
int odbcdr_rdbi_init( odbcdr_context_def **, rdbi_methods	methods );


wchar_t* getComDir (); // in Odbc.cpp
#define OD_MAX_RETRY_COUNT 10

FdoRdbmsOdbcConnection::FdoRdbmsOdbcConnection()
{
}

FdoRdbmsOdbcConnection::~FdoRdbmsOdbcConnection ()
{
}

FdoRdbmsOdbcConnection* FdoRdbmsOdbcConnection::Create()
{
    FdoRdbmsOdbcConnection   *conn = new FdoRdbmsOdbcConnection();
    conn->GetDbiConnection()->InitRdbi( (initializer*)odbcdr_rdbi_init );
	return conn;
}

FdoICommand *FdoRdbmsOdbcConnection::CreateCommand (FdoInt32 commandType)
{
    FdoICommand* ret;

    switch (commandType)
    {
        case FdoCommandType_CreateDataStore:
            ret = new FdoRdbmsOdbcCreateDataStore (this);
            break;

        case FdoCommandType_DestroyDataStore:
             ret = new FdoRdbmsOdbcDeleteDataStore (this);
             break;

		 default:
             return FdoRdbmsConnection::CreateCommand( commandType );
    }
    return (ret);
}

FdoRdbmsFilterProcessor* FdoRdbmsOdbcConnection::GetFilterProcessor()
{
    if( mFilterProcessor == NULL )
        mFilterProcessor = new FdoRdbmsOdbcFilterProcessor( this );

    return FDO_SAFE_ADDREF(mFilterProcessor.p);
}

FdoIConnectionInfo* FdoRdbmsOdbcConnection::GetConnectionInfo()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new FdoRdbmsOdbcConnectionInfo(this);
    return FDO_SAFE_ADDREF(mConnectionInfo.p);
}

FdoIConnectionCapabilities* FdoRdbmsOdbcConnection::GetConnectionCapabilities()
{
    if (mConnectionCapabilities == NULL)
        mConnectionCapabilities = new FdoRdbmsOdbcConnectionCapabilities();
    FDO_SAFE_ADDREF(mConnectionCapabilities);
    return mConnectionCapabilities;
}

FdoICommandCapabilities* FdoRdbmsOdbcConnection::GetCommandCapabilities()
{
    if (mCommandCapabilities == NULL)
        mCommandCapabilities = new FdoRdbmsOdbcCommandCapabilities();
    FDO_SAFE_ADDREF(mCommandCapabilities);
    return mCommandCapabilities;
}

FdoISchemaCapabilities* FdoRdbmsOdbcConnection::GetSchemaCapabilities()
{
    if (mSchemaCapabilities == NULL)
        mSchemaCapabilities = new FdoRdbmsOdbcSchemaCapabilities();
    FDO_SAFE_ADDREF(mSchemaCapabilities);
    return mSchemaCapabilities;
}

FdoIFilterCapabilities* FdoRdbmsOdbcConnection::GetFilterCapabilities()
{
    if (mFilterCapabilities == NULL)
        mFilterCapabilities = new FdoRdbmsOdbcFilterCapabilities();
    FDO_SAFE_ADDREF(mFilterCapabilities);
    return mFilterCapabilities;
}

FdoIExpressionCapabilities* FdoRdbmsOdbcConnection::GetExpressionCapabilities()
{
    if (mExpressionCapabilities == NULL)
        mExpressionCapabilities = new FdoRdbmsOdbcExpressionCapabilities();
    FDO_SAFE_ADDREF(mExpressionCapabilities);
    return mExpressionCapabilities;
}

/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* FdoRdbmsOdbcConnection::GetGeometryCapabilities()
{
    if( mGeometryCapabilities == NULL )
        mGeometryCapabilities = new FdoRdbmsOdbcGeometryCapabilities();

    return FDO_SAFE_ADDREF(mGeometryCapabilities);
}

FdoSchemaManagerP FdoRdbmsOdbcConnection::NewSchemaManager(
    GdbiConnection* gdbiConnection, 
    FdoStringP currUser,
    FdoStringP currSchema
)
{
    FdoSchemaManagerP schMgr = new FdoOdbcSchemaManager(
        GetDbiConnection()->GetGdbiConnection(),
        currSchema
    );

    FdoSmPhOdbcMgrP physMgr = schMgr->GetPhysicalSchema()->SmartCast<FdoSmPhOdbcMgr>();

    // Pass down the location of the COM directory for MetaSchema creation scripts.
    physMgr->SetHomeDir( FdoStringP(getComDir()) );

    return schMgr;
}

FdoSchemaManagerP FdoRdbmsOdbcConnection::CreateSchemaManager()
 {
    DbiConnection* pConn = GetDbiConnection();
    FdoStringP userName = pConn->GetUser();
    FdoStringP schemaName = pConn->GetDbSchemaName();
    if (schemaName.GetLength() == 0)
    {
        FdoStringP connectionStringProperty = pConn->GetConnectionString();
        if (connectionStringProperty.GetLength() != 0 && pConn->GetDbVersion() == RDBI_DBVERSION_ODBC_ORACLE)
        {
            FdoCommonConnStringParser parser (NULL, connectionStringProperty);
            if (parser.IsConnStringValid())
                schemaName = parser.GetPropertyValueW(L"XSM");
        }
        if (schemaName.GetLength() == 0)
            schemaName = GetSchemaNameFromDsn();

        if (schemaName.GetLength() > 0)
        {
            // If the DSN contained a schema name, take that as the only one that
            // we want to see, and which will be specified in all future requests
            // from this provider.
            // If this is not set, all schemas will be visible (occasionally more
            // useful, but also can be slow).
            pConn->SetDbSchemaName(schemaName);
        }
    }

#if 0
    // Use the login user name as a default for the active schema.
    if (userName.GetLength() > 0 && schemaName.GetLength() <= 0)
        schemaName = userName;
#endif

    if (schemaName.GetLength() > 0)
    {
        // Set sane default for the RDBMS' schema (where things go for cases where
        // we do not specify a schema in this provider).
        pConn->SetActiveSchema(schemaName);
    }

    // Call base class' method.
    FdoSchemaManagerP schMgr = FdoRdbmsConnection::CreateSchemaManager();

    return schMgr;
}


 //
// Converts a Odbc string date of a specific format to a FdoDateTime (time_t) format.
FdoDateTime  FdoRdbmsOdbcConnection::DbiToFdoTime( const char* timeStr )
{
    FdoDateTime fdoTime;
    int year, month, day, hour, minute, seconds;
    year = month = day = hour = minute = seconds = 0;

    if( timeStr != NULL && *timeStr != '\0' )
    {
        int count = sscanf(timeStr,"%4d-%02d-%02d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &seconds);     
        if( count != 6 )
            count = sscanf(timeStr,"%4d-%02d-%02d",&year, &month, &day);
    }
    fdoTime.year = (FdoInt16)year;
    fdoTime.month = (FdoByte)month;
    fdoTime.day = (FdoByte)day;
    fdoTime.hour = (FdoByte)hour;
    fdoTime.minute = (FdoByte)minute;
    fdoTime.seconds = (float)seconds;
    return fdoTime;
}

//
// Convert time_t( FdoDateTime ) to a Odbc string date of the form.
// It return a statically allocated storage that can be overwritten by subsequent call to this or other methods.
const char* FdoRdbmsOdbcConnection::FdoToDbiTime( FdoDateTime  when )
{
    char *ret = GetDbiConnection()->GetUtility()->newCharP();

    if ( when.IsDateTime())
    {
        // "1979-11-30 10:32:12"
        sprintf (ret, "%4d-%02d-%02d %02d:%02d:%02d",
            when.year,
            when.month,
            when.day,
            when.hour,
            when.minute,
            (int)when.seconds);
    }
	else if (when.IsTime())
	{
        // "10:32:12"
        sprintf (ret, "%02d:%02d:%02d",
            when.hour,
            when.minute,
            (int)when.seconds);
	}
    else if (when.IsDate())
    {
        // "1979-11-30"
        sprintf (ret, "%4d-%02d-%02d",
            when.year,
            when.month,
            when.day);
    }
	else
		 throw FdoException::Create(NlsMsgGet(FDORDBMS_480, "Incomplete date/time setting."));

    return (ret);
}

// Converts an FDO FdoDateTime value to an ODBC datetime value;
// NOTE: ODBC datetime values in a filter use a different syntax than datetime values in insert/select/update values:
//      {d 'yyyy-mm-dd'}                   // date only
//      {t 'hh:mm:ss[.f...]'}              // time only
//      {ts 'yyyy-mm-dd hh:mm:ss[.f...]'}  // date and time
const char* FdoRdbmsOdbcConnection::FdoToDbiTimeFilter( FdoDateTime  when )
{
    char *ret = GetDbiConnection()->GetUtility()->newCharP();

    // Get current time, in case we need to supply default values:
    time_t    currentTime;
    struct tm* ptms;
    time( &currentTime );
    ptms = localtime( &currentTime );

    // Append prefix:
    FdoStringP strTime;
    if (when.IsDate())
        strTime += FDORDBMSODBCFILTER_DATE_PREFIX;
    else if (when.IsDateTime())
        strTime += FDORDBMSODBCFILTER_TIMESTAMP_PREFIX ;
    else
        strTime += FDORDBMSODBCFILTER_TIME_PREFIX;

    // Append date portion:
    if (when.IsDate() || when.IsDateTime())
    {
        strTime += FdoStringP::Format(
                FDORDBMSODBCFILTER_DATE_FORMAT,
                when.year == -1 ? ptms->tm_year+1900 :(int)when.year,
                when.month == -1 ? ptms->tm_mon+1 : (int)when.month,
                when.day == -1 ? 1 : (int)when.day);
    }

    // Append separator (if both date and time present):
    if (when.IsDateTime())
        strTime += FDORDBMSODBCFILTER_DATETIME_SEPARATOR;

    // Append time portion:
    if (when.IsDateTime() || when.IsTime())
    {
        strTime += FdoStringP::Format(
                FDORDBMSODBCFILTER_TIME_FORMAT,
                when.hour == -1 ? 0 : (int)when.hour,
                when.minute == -1 ? 0 : (int)when.minute,
                when.seconds == -1 ? 0: (int)when.seconds );
    }

    // Append suffix:
    strTime += FDORDBMSODBCFILTER_DATETIME_SUFFIX;

    // Return the string:
    sprintf(ret, "%ls", (FdoString*)strTime);
    return (ret);
}


FdoConnectionState FdoRdbmsOdbcConnection::Open()
{
    FdoConnectionState  state = GetConnectionState();
	if( state != FdoConnectionState_Open )
	{
  	    state = FdoRdbmsConnection::Open(true /* skipPending*/);

        // Override stubbed spatial manager with one that supports relational spatial indexing.
        FdoPtr<FdoRdbmsSpatialManager> spatialManager =
            FdoRdbmsOdbcSpatialManager::Create();
        SetSpatialManager(spatialManager);
    }
    return state;
}

void FdoRdbmsOdbcConnection::Close()
{
	FdoRdbmsConnection::Close();
}


// Creates a Long Transaction Manager and its corresponding Long Transaction
// Manager Service.
FdoRdbmsLongTransactionManager *FdoRdbmsOdbcConnection::CreateLongTransactionManager()
{
    return NULL;
}

FdoIDataStorePropertyDictionary*  FdoRdbmsOdbcConnection::CreateDataStoreProperties( int action )
{
    FdoCommonDataStorePropDictionary* mDataStorePropertyDictionary = new FdoCommonDataStorePropDictionary(this);
	
    FdoPtr<ConnectionProperty> newProp;
	if ( action == FDO_RDBMS_DATASTORE_FOR_READ )
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", false, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
        
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_DESCRIPTION, NlsMsgGet(FDORDBMS_448, "Description"), L"", false, false, false, false, false, false, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
		
    	wchar_t **LtModeValues = new wchar_t*[2];
        LtModeValues[0] = new wchar_t[4];
        LtModeValues[1] = new wchar_t[5];
        wcscpy(LtModeValues[0], L"FDO");
        wcscpy(LtModeValues[1], L"NONE");
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_LTMODE, NlsMsgGet(FDORDBMS_449, "LtMode"), L"FDO", false, false, true, false, false, false, false, 2, (const wchar_t**)LtModeValues);
        mDataStorePropertyDictionary->AddProperty(newProp);
		
    	wchar_t **LockModeValues = new wchar_t*[2];
        LockModeValues[0] = new wchar_t[4];
        LockModeValues[1] = new wchar_t[5];
        wcscpy(LockModeValues[0], L"FDO");
        wcscpy(LockModeValues[1], L"NONE");
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_LOCKMODE, NlsMsgGet(FDORDBMS_450, "LockMode"), L"FDO", false, false, true, false, false, false, false, 2, (const wchar_t**)LockModeValues);
        mDataStorePropertyDictionary->AddProperty(newProp);
	}
	else if ( action == FDO_RDBMS_DATASTORE_FOR_CREATE )
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", true, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);

        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_DESCRIPTION, NlsMsgGet(FDORDBMS_448, "Description"), L"", false, false, false, false, false, false, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
        
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_PASSWORD, NlsMsgGet(FDORDBMS_119, "Password"), L"", true, true, false, false, false, false, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
    	
        wchar_t **LtModeValues = new wchar_t*[2];
        LtModeValues[0] = new wchar_t[4];
        LtModeValues[1] = new wchar_t[5];
        wcscpy(LtModeValues[0], L"FDO");
        wcscpy(LtModeValues[1], L"NONE");
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_LTMODE, NlsMsgGet(FDORDBMS_449, "LtMode"), L"FDO", false, false, true, false, false, false, false, 2, (const wchar_t**)LtModeValues);
        mDataStorePropertyDictionary->AddProperty(newProp);
		
    	wchar_t **LockModeValues = new wchar_t*[2];
        LockModeValues[0] = new wchar_t[4];
        LockModeValues[1] = new wchar_t[5];
        wcscpy(LockModeValues[0], L"FDO");
        wcscpy(LockModeValues[1], L"NONE");
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_LOCKMODE, NlsMsgGet(FDORDBMS_450, "LockMode"), L"FDO", false, false, true, false, false, false, false, 2, (const wchar_t**)LockModeValues);
        mDataStorePropertyDictionary->AddProperty(newProp);
	}
	else if ( action == FDO_RDBMS_DATASTORE_FOR_DELETE )
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", true, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
        
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_PASSWORD, NlsMsgGet(FDORDBMS_119, "Password"), L"", true, true, false, false, false, false, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
	}

    return mDataStorePropertyDictionary;
}

FdoRdbmsLockManager *FdoRdbmsOdbcConnection::CreateLockManager()
{
	return NULL;
}

#ifdef _WIN32
#define ODBC_FODBC_INI        L"ODBC.INI"
FdoStringP FdoRdbmsOdbcConnection::GetSchemaNameFromDsn()
{
    FdoStringP schemaName;

    // For Oracle on Windows, get the UserId field.
    // This field is normally just a default for the username, so this use does overload
    // it.  However, Oracle's driver does not offer a separate "schema" field as other
    // drivers do.
    DbiConnection* pConn = GetDbiConnection();
    FdoStringP dsn = pConn->GetDataSource();
    wchar_t buffValue[ODBCDR_CONNECTION_SIZE];
    if (pConn->GetDbVersion() == RDBI_DBVERSION_ODBC_ORACLE)
    {
        if(0 != ::SQLGetPrivateProfileStringW( dsn, L"UserID", L"", buffValue, ODBCDR_CONNECTION_SIZE, ODBC_FODBC_INI ))
	        schemaName = buffValue;
    }
    return schemaName;
}
#else
#define ODBC_FODBC_INI        "ODBC.INI"
FdoStringP FdoRdbmsOdbcConnection::GetSchemaNameFromDsn()
{
    FdoStringP schemaName;
    DbiConnection* pConn = GetDbiConnection();
    FdoStringP dsn = pConn->GetDataSource();
    char buffValue[ODBCDR_CONNECTION_SIZE];
    if (pConn->GetDbVersion() == RDBI_DBVERSION_ODBC_ORACLE &&
        dsn.GetLength() > 0)
    {
        if(0 != ::SQLGetPrivateProfileString( dsn, "UserID", "", buffValue, ODBCDR_CONNECTION_SIZE, ODBC_FODBC_INI ))
	        schemaName = buffValue;
    }
    return schemaName;
}
#endif
