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

#include <malloc.h>
#include <time.h>
#include <Inc/ut.h>
#include "FdoRdbmsSqlServerConnectionInfo.h"
#include "FdoRdbmsSqlServerConnection.h"
#include "FdoRdbmsSqlServerCreateDataStore.h"
#include "FdoRdbmsSqlServerDeleteDataStore.h"
#include "FdoRdbmsSqlServerFilterProcessor.h"
#include "FdoRdbmsSqlServerDeleteCommand.h"
#include "FdoRdbmsSqlServerCommandCapabilities.h"
#include "FdoRdbmsSqlServerFilterCapabilities.h"
#include "FdoRdbmsSqlServerExpressionCapabilities.h"

#include "../SchemaMgr/SchemaManager.h"
#include "../SchemaMgr/Ph/Mgr.h"
#include "../../Fdo/Capability/FdoRdbmsGeometryCapabilities.h"

#include "FdoRdbmsSqlServerSchemaCapabilities.h"
#include "FdoRdbmsSqlServerConnectionCapabilities.h"
#include "FdoRdbmsSqlServerGeometryCapabilities.h"
#include "FdoRdbmsSqlServerOptimizedAggregateReader.h"

#include "DbiConnection.h"
#include "Rdbms/FdoRdbmsCommandType.h"
#include "FdoRdbmsSqlServerProcessors.h"

#include <Inc/Rdbi/proto.h>
#include "../ODBCDriver/context.h"
int odbcdr_rdbi_init( odbcdr_context_def **, rdbi_methods	methods );


wchar_t* getComDir (); // in SqlServer.cpp
#define OD_MAX_RETRY_COUNT 10

FdoRdbmsSqlServerConnection::FdoRdbmsSqlServerConnection():
mFilterProcessor( NULL ),
mConnectionInfo(NULL),
mGeomVersion(1)
{
}

FdoRdbmsSqlServerConnection::~FdoRdbmsSqlServerConnection ()
{
    if( mFilterProcessor )
        delete mFilterProcessor;

    FDO_SAFE_RELEASE(mConnectionInfo);
}

FdoRdbmsSqlServerConnection* FdoRdbmsSqlServerConnection::Create()
{
    FdoRdbmsSqlServerConnection   *conn = new FdoRdbmsSqlServerConnection();
    conn->GetDbiConnection()->InitRdbi( (initializer*)odbcdr_rdbi_init );
	return conn;
}

FdoICommand *FdoRdbmsSqlServerConnection::CreateCommand (FdoInt32 commandType)
{
    FdoICommand* ret = NULL;
    switch (commandType)
    {
        case FdoCommandType_CreateDataStore:
            ret = new FdoRdbmsSqlServerCreateDataStore (this);
            break;

        case FdoCommandType_DestroyDataStore:
             ret = new FdoRdbmsSqlServerDeleteDataStore (this);
             break;

		case FdoCommandType_Delete:
             ret = new FdoRdbmsSqlServerDeleteCommand (this);
             break;

        case FdoCommandType_Select:
        case FdoCommandType_SelectAggregates:
        case FdoCommandType_Insert:
        case FdoCommandType_Update:
        case FdoCommandType_DescribeSchema:
        case FdoCommandType_ApplySchema:
        case FdoCommandType_DestroySchema:
        case FdoCommandType_DescribeSchemaMapping:
        case FdoCommandType_ActivateSpatialContext:
        case FdoCommandType_CreateSpatialContext:
        case FdoCommandType_DestroySpatialContext:
        case FdoCommandType_GetSpatialContexts:
        case FdoCommandType_ListDataStores:
        case FdoCommandType_SQLCommand:
        case FdoCommandType_GetSchemaNames:
        case FdoCommandType_GetClassNames:
            ret = FdoRdbmsConnection::CreateCommand(commandType);
            break;
        case FdoRdbmsCommandType_CreateSpatialIndex:
        case FdoRdbmsCommandType_DestroySpatialIndex:
        case FdoRdbmsCommandType_GetSpatialIndexes:
        default:
            throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_10, "Command not supported"));
            break;
    }
    return (ret);
}

FdoRdbmsFilterProcessor* FdoRdbmsSqlServerConnection::GetFilterProcessor()
{
    if( mFilterProcessor == NULL )
        mFilterProcessor = new FdoRdbmsSqlServerFilterProcessor( this );

    mFilterProcessor->Reset();
    return FDO_SAFE_ADDREF(mFilterProcessor);
}

FdoIConnectionInfo* FdoRdbmsSqlServerConnection::GetConnectionInfo()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new FdoRdbmsSqlServerConnectionInfo(this);
    FDO_SAFE_ADDREF(mConnectionInfo);
    return mConnectionInfo;
}

/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* FdoRdbmsSqlServerConnection::GetGeometryCapabilities()
{
    if( mGeometryCapabilities == NULL )
        mGeometryCapabilities = new FdoRdbmsSqlServerGeometryCapabilities();

    FdoRdbmsSqlServerGeometryCapabilities* sc = (FdoRdbmsSqlServerGeometryCapabilities*)mGeometryCapabilities;
    sc->SetGeomVersion(this->mGeomVersion);

    return FDO_SAFE_ADDREF(mGeometryCapabilities);
}

/// Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.
FdoISchemaCapabilities* FdoRdbmsSqlServerConnection::GetSchemaCapabilities()
{
    if (mSchemaCapabilities == NULL)
        mSchemaCapabilities = new FdoRdbmsSqlServerSchemaCapabilities();
    FDO_SAFE_ADDREF(mSchemaCapabilities);
    return mSchemaCapabilities;
}

/// Gets an FdoIConnectionCapabilities interface describing the provider's support for misc connection-related items
FdoIConnectionCapabilities* FdoRdbmsSqlServerConnection::GetConnectionCapabilities()
{
    if (mConnectionCapabilities == NULL)
        mConnectionCapabilities = new FdoRdbmsSqlServerConnectionCapabilities();
    FDO_SAFE_ADDREF(mConnectionCapabilities);
    return mConnectionCapabilities;
}


FdoSchemaManagerP FdoRdbmsSqlServerConnection::NewSchemaManager(
    GdbiConnection* gdbiConnection, 
    FdoStringP currUser,
    FdoStringP currSchema
)
{
    FdoSchemaManagerP schMgr = new FdoSqsSchemaManager(
        GetDbiConnection()->GetGdbiConnection(),
        currSchema
    );

    FdoSmPhSqsMgrP physMgr = schMgr->GetPhysicalSchema()->SmartCast<FdoSmPhSqsMgr>();

    // Pass down the location of the COM directory for MetaSchema creation scripts.
    physMgr->SetHomeDir( FdoStringP(getComDir()) );

    return schMgr;
}

 //
// Converts a SqlServer string date of a specific format to a FdoDateTime (time_t) format.
FdoDateTime  FdoRdbmsSqlServerConnection::DbiToFdoTime( const char* timeStr )
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
// Converts a SqlServer string date of a specific format to a FdoDateTime (time_t) format.
FdoDateTime  FdoRdbmsSqlServerConnection::DbiToFdoTime( const wchar_t* timeStr )
{
    FdoDateTime fdoTime;
    int year, month, day, hour, minute, seconds;
    year = month = day = hour = minute = seconds = 0;

    if( timeStr != NULL && *timeStr != '\0' )
    {
        int count = swscanf(timeStr, L"%4d-%02d-%02d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &seconds);     
        if( count != 6 )
            count = swscanf(timeStr, L"%4d-%02d-%02d",&year, &month, &day);
    }
    fdoTime.year = (FdoInt16)year;
    fdoTime.month = (FdoByte)month;
    fdoTime.day = (FdoByte)day;
    fdoTime.hour = (FdoByte)hour;
    fdoTime.minute = (FdoByte)minute;
    fdoTime.seconds = (float)seconds;
    return fdoTime;
}

const wchar_t* FdoRdbmsSqlServerConnection::FdoToDbiTime( FdoDateTime time, wchar_t* dest, size_t size )
{
    bool isDateSupplied = ((time.year != -1) || (time.month != -1) || (time.day != -1));
    bool isValidDate    = isDateSupplied && ((time.year != -1) && (time.month != -1) && (time.day != -1));

    bool isTimeSupplied = ((time.hour != -1) || (time.minute != -1));
    bool isValidTime    = isTimeSupplied && ((time.hour != -1) && (time.minute != -1));

    if ((isDateSupplied  && !isValidDate)    ||
        (isTimeSupplied  && !isValidTime)    ||
        (isTimeSupplied  && !isDateSupplied) ||
        (!isDateSupplied && !isTimeSupplied)    )
		 throw FdoException::Create(NlsMsgGet(FDORDBMS_480,
                                              "Incomplete date/time setting."));

    if (time.year >= 0 && time.year < 50)
        time.year += 2000;
    else if (time.year >= 50 && time.year < 99)
        time.year += 1900;

    if ((isDateSupplied) && (!isTimeSupplied))
        swprintf (dest, size, L"%04d-%02d-%02d", time.year, time.month, time.day);
    else
    {
        float sec = ((int)(time.seconds*1000.0f))/1000.0f;
        if (sec >= 60.0f)
            sec = 59.999f;

        swprintf (dest, size, L"%04d-%02d-%02d %02d:%02d:%02.3f",
                 time.year,
                 time.month,
                 time.day,
                 time.hour,
                 time.minute,
                 sec);
    }
    return (dest);
}

//
// Convert time_t( FdoDateTime ) to a SqlServer string date of the form.
// It returns a statically allocated storage that can be overwritten by
// subsequent call to this or other methods.
// For the convertion, the following basic rules apply:
//
//  DATE                TIME                RESULT
//  -------------------------------------------------
//  n/a                 n/a                 exception
//  completely set      n/a                 date
//  n/a                 completely set      exception
//  completely set      completely set      date&time
//
//  partially set       n/a                 exception
//  n/a                 partially set       exception
//  partially set       partially set       exception
//  partially set       completely set      exception
//  completely set      partially set       exception
//

const char* FdoRdbmsSqlServerConnection::FdoToDbiTime( FdoDateTime  when )
{
    char *ret           = GetDbiConnection()->GetUtility()->newCharP();

    bool isDateSupplied = ((when.year != -1) || (when.month != -1) || (when.day != -1));
    bool isValidDate    = isDateSupplied && ((when.year != -1) && (when.month != -1) && (when.day != -1));

    bool isTimeSupplied = ((when.hour != -1) || (when.minute != -1));
    bool isValidTime    = isTimeSupplied && ((when.hour != -1) && (when.minute != -1));

    if ((isDateSupplied  && !isValidDate)    ||
        (isTimeSupplied  && !isValidTime)    ||
        (isTimeSupplied  && !isDateSupplied) ||
        (!isDateSupplied && !isTimeSupplied)    )
		 throw FdoException::Create(NlsMsgGet(FDORDBMS_480,
                                              "Incomplete date/time setting."));

    if (when.year >= 0 && when.year < 50)
        when.year += 2000;
    else if (when.year >= 50 && when.year < 99)
        when.year += 1900;

    if ((isDateSupplied) && (!isTimeSupplied))
        sprintf (ret, "%04d-%02d-%02d", when.year, when.month, when.day);
    else
    {
        float sec = ((int)(when.seconds*1000.0f))/1000.0f;
        if (sec >= 60.0f)
            sec = 59.999f;

        sprintf (ret, "%04d-%02d-%02d %02d:%02d:%02.3f",
                 when.year,
                 when.month,
                 when.day,
                 when.hour,
                 when.minute,
                 sec);
    }

    return (ret);
}

// Converts an FDO FdoDateTime value to an ODBC datetime value;
// NOTE: ODBC datetime values in a filter use a different syntax than datetime values in insert/select/update values:
//      {d 'yyyy-mm-dd'}                   // date only
//      {t 'hh:mm:ss[.f...]'}              // time only
//      {ts 'yyyy-mm-dd hh:mm:ss[.f...]'}  // date and time
const char* FdoRdbmsSqlServerConnection::FdoToDbiTimeFilter( FdoDateTime  when )
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
    return ret;
}


FdoConnectionState FdoRdbmsSqlServerConnection::Open()
{
    mIsGeogLatLongSet = false;

    FdoConnectionState  state = GetConnectionState();
	if( state != FdoConnectionState_Open )
	{
  	    state = FdoRdbmsConnection::Open();
        try
        {
            CheckForUnsupportedVersion();
        }
        catch (FdoException *ex)
        {
	        try
	        {
		        Close();
	        }
	        catch(...)
	        {
	        }
	        throw ex;
        }

	    if( state == FdoConnectionState_Open )
	    {
	        try
	        {
                CheckForFdoGeometries();
	        }
	        catch (FdoException *ex)
	        {
		        try
		        {
			        Close();
		        }
		        catch(...)
		        {
		        }
		        throw ex;
	        }
	    }
    }
    return state;
}

void FdoRdbmsSqlServerConnection::Close()
{
	if( GetConnectionState() == FdoConnectionState_Open )
    {
        FdoPtr<FdoRdbmsLongTransactionManager> ltManager = FdoRdbmsConnection::GetLongTransactionManager();
        if (ltManager)
            ltManager->Deactivate();
    }
		
	FdoRdbmsConnection::Close();
}

void FdoRdbmsSqlServerConnection::CheckForUnsupportedVersion()
{
    FdoSmPhSqsMgrP phMgr = GetSchemaManager()->GetPhysicalSchema()->SmartCast<FdoSmPhSqsMgr>();

    FdoVectorP verTokens = FdoVector::Create( phMgr->GetDbVersion(), L"." );
    FdoVectorP minSupported = FdoVector::Create();
    minSupported->Add( 9 );
    minSupported->Add( 0 );
    minSupported->Add( 0 );

    if (verTokens->GetValue(0) >= 11)
        mGeomVersion = 2;

    if ( verTokens < minSupported )
    {
        throw FdoConnectionException::Create(
            NlsMsgGet(
                FDORDBMS_545, 
                "Cannot connect OSGeo.SQLServerSpatial provider to server with version older than 9.0."
            )
        );
    }
}

void FdoRdbmsSqlServerConnection::CheckForFdoGeometries()
{
    FdoSmPhSqsMgrP phMgr = GetSchemaManager()->GetPhysicalSchema()->SmartCast<FdoSmPhSqsMgr>();
    FdoSmPhOwnerP owner = phMgr->FindOwner();

    // non-FDO datastores not supported by Autodesk.SqlServer provider so 
    // assume these can be opened by this provider. 
    // Also skip check if the Schema Manager can't find the datastore (owner == NULL).
    // This case will be trapped later on and an exception thrown.
    if ( !owner || !owner->GetHasAttrMetaSchema() ) 
        return;

    // Geometric properties have numeric attributetype. Following query find
    // geometric properties with image type columns. If any exist then
    // this datastore was created by the Autodesk.SqlServer provider.
	FdoStringP sqlStmt = L"select top 1 tablename from f_attributedefinition where lower(columntype) = 'image' and isnumeric(attributetype) = 1";

	GdbiConnection* gdbiConn = phMgr->GetGdbiConnection();
  	GdbiQueryResult *gdbiResult = gdbiConn->ExecuteQuery((const wchar_t*)sqlStmt);

	if (gdbiResult->ReadNext())
	{
        // Image type geometry found, can't use this datastore from this provider.
        gdbiResult->End();
        delete gdbiResult;

        throw FdoConnectionException::Create(
            NlsMsgGet1(
                FDORDBMS_38, 
                "Cannot open datastore '%1$ls' with the OSGeo.SQLServerSpatial provider; it was created by the Autodesk.SqlServer provider. You will need to migrate this datastore first.",
                owner->GetName()
            )
        );
    }

    gdbiResult->End();
    delete gdbiResult;

}

FdoStringP FdoRdbmsSqlServerConnection::GenConnectionStringParm( FdoStringP connectionString )
{
    FdoStringP newCs;

    FdoConnectionState  state = GetConnectionState();
    if ( (connectionString.GetLength() == 0) && (state != FdoConnectionState_Open) )
	{
        FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo();
        FdoPtr<FdoCommonConnPropDictionary> dict = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());
        
        // Supported parameters identify datastore, userid and password.
		// We'll generate an odbc connection string in this format:
		//  "DRIVER={SQL Server Native Client 10.0};MARS_Connection=yes;SERVER=seconds;UID=username;PWD=passwd;" 
		// If the UID and PWD parameters are not specified, the trusted connection 
		// (windows authentication) is assumed.
        FdoStringP dataSource = dict->GetProperty(FDO_RDBMS_CONNECTION_SERVICE);
        if (dataSource != NULL && dataSource.GetLength() > 0)
        {
            newCs = L"DRIVER={SQL Server Native Client 10.0};MARS_Connection=yes;SERVER=";
            newCs += dataSource;
			FdoStringP user = dict->GetProperty(FDO_RDBMS_CONNECTION_USERNAME);
			if (user.GetLength() > 0)
			{
				newCs += L";UID="; 
				newCs += user;
				FdoStringP passwd = dict->GetProperty(FDO_RDBMS_CONNECTION_PASSWORD);
				if (passwd.GetLength() > 0)
				{
					newCs += L";PWD=";
					newCs += passwd;
				}
			}
            else
                newCs += L";Trusted_Connection=yes";
        }
    }

    return newCs;
}

// Creates a Long Transaction Manager and its corresponding Long Transaction
// Manager Service.
FdoRdbmsLongTransactionManager *FdoRdbmsSqlServerConnection::CreateLongTransactionManager()
{
    return NULL;
}


FdoIDataStorePropertyDictionary*  FdoRdbmsSqlServerConnection::CreateDataStoreProperties( int action )
{
    FdoCommonDataStorePropDictionary* mDataStorePropertyDictionary = new FdoCommonDataStorePropDictionary(this);
	
    FdoPtr<ConnectionProperty> newProp;
	if ( action == FDO_RDBMS_DATASTORE_FOR_READ )
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", false, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
        
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_DESCRIPTION, NlsMsgGet(FDORDBMS_448, "Description"), L"", false, false, false, false, false, false, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);	
	}
	else if ( action == FDO_RDBMS_DATASTORE_FOR_CREATE )
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", true, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);

        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_DESCRIPTION, NlsMsgGet(FDORDBMS_448, "Description"), L"", false, false, false, false, false, false, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);

        wchar_t** enabledValues = new wchar_t*[2];
        enabledValues[0] = new wchar_t[10];
        wcscpy( enabledValues[0], L"false" );
        enabledValues[1] = new wchar_t[10];
        wcscpy( enabledValues[1], L"true" );

        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_FDO_ENABLED, NlsMsgGet(FDORDBMS_9, "IsFdoEnabled"), L"false", false, false, true, false, false, false, false, 2, (const wchar_t**) enabledValues);
        mDataStorePropertyDictionary->AddProperty(newProp);
	}
	else if ( action == FDO_RDBMS_DATASTORE_FOR_DELETE )
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", true, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
        
//      newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_DESCRIPTION, NlsMsgGet(FDORDBMS_119, "Password"), L"", true, true, false, false, false, false, false, 0, NULL);
//      mDataStorePropertyDictionary->AddProperty(newProp);
	}

    return mDataStorePropertyDictionary;
}
FdoRdbmsLockManager *FdoRdbmsSqlServerConnection::CreateLockManager()
{
	return NULL;
}

FdoICommandCapabilities* FdoRdbmsSqlServerConnection::GetCommandCapabilities()
{
    if (mCommandCapabilities == NULL)
        mCommandCapabilities = new FdoRdbmsSqlServerCommandCapabilities();
    FDO_SAFE_ADDREF(mCommandCapabilities);
    return mCommandCapabilities;
}

FdoIFilterCapabilities *FdoRdbmsSqlServerConnection::GetFilterCapabilities()
{
    if (mFilterCapabilities == NULL)
        mFilterCapabilities = new FdoRdbmsSqlServerFilterCapabilities();
    FDO_SAFE_ADDREF(mFilterCapabilities);
    return mFilterCapabilities;	
}

FdoIExpressionCapabilities* FdoRdbmsSqlServerConnection::GetExpressionCapabilities()
{
	if (mExpressionCapabilities == NULL)
		mExpressionCapabilities = new FdoRdbmsSqlServerExpressionCapabilities();
	FDO_SAFE_ADDREF(mExpressionCapabilities);
	return mExpressionCapabilities;
}

FdoRdbmsFeatureReader *FdoRdbmsSqlServerConnection::GetOptimizedAggregateReader(const FdoSmLpClassDefinition* classDef, aggr_list *selAggrList, FdoFilter* filter)
{ 
    bool optimize = false;

    // Optimization needed only when SpatialExtents function present.
    for ( size_t i = 0; i < selAggrList->size(); i++ )
	{ 
		AggregateElement	*id = selAggrList->at(i);
		if ( id->type == FdoPropertyType_GeometricProperty )
			optimize = true;
    }
        
    FdoRdbmsSqlServerOptimizedAggregateReader* rdr = NULL;
    if ( optimize ) 
        rdr = new FdoRdbmsSqlServerOptimizedAggregateReader(this, classDef, selAggrList, filter); 

    return rdr;
}

FdoStringP FdoRdbmsSqlServerConnection::GetBindString( int n, const FdoSmLpPropertyDefinition* prop )
{ 
    FdoStringP bindStr(L"?", true);
    return bindStr; 
}

bool  FdoRdbmsSqlServerConnection::BindGeometriesLast() 
{ 
    return true; 
}

bool FdoRdbmsSqlServerConnection::IsGeogLatLong()
{
    if ( !mIsGeogLatLongSet ) 
    {
        FdoSmPhSqsMgrP phMgr = GetSchemaManager()->GetPhysicalSchema()->SmartCast<FdoSmPhSqsMgr>();
        mIsGeogLatLong = phMgr->IsGeogLatLong();
        mIsGeogLatLongSet = true;
    }

    return mIsGeogLatLong;
}

FdoInt32 FdoRdbmsSqlServerConnection::ExecuteDdlNonQuery(FdoString* sql)
{
    GdbiConnection* gdbiConn = GetDbiConnection()->GetGdbiConnection();
    GdbiCommands* gdbiCommands = gdbiConn->GetCommands();
    bool autoCmtChanged = false;
    FdoInt32 retVal = 0;

    // Open and close a dummy transaction to force a commit before creating the database.
    gdbiCommands->tran_begin ("SmPreChangeDatabase");
    gdbiCommands->tran_end ("SmPreChangeDatabase");

    int autoCmtMode = gdbiCommands->autocommit_mode();
    if (autoCmtMode == 0) //SQL_AUTOCOMMIT_OFF
    {
        // we need it SQL_AUTOCOMMIT_ON with the new driver
        gdbiCommands->autocommit_on();
        autoCmtChanged = true;
    }
    // Wrap the database create in a transaction.
    gdbiCommands->tran_begin ("SmPreChangeDatabase");
    try
    {
        retVal = gdbiConn->ExecuteNonQuery(sql, true);
    }
    catch ( ... )
    {
        try
        {
            gdbiCommands->tran_end ("SmPreChangeDatabase");
	        if (autoCmtChanged)
		        gdbiCommands->autocommit_off();
        }
        catch (FdoException *ex) { ex->Release(); }
        catch ( ... ) {}            
        throw;
    }
    gdbiCommands->tran_end ("SmPreChangeDatabase");
    if (autoCmtChanged)
        gdbiCommands->autocommit_off();
    return retVal;
}

void FdoRdbmsSqlServerConnection::Flush()
{
    if (mEnforceClearSchAtFlush) // clear cached schema
    {
        FdoSchemaManagerP pschemaManager = GetSchemaManager();
        pschemaManager->Clear();
    }
}

FdoRdbmsSqlBuilder* FdoRdbmsSqlServerConnection::GetSqlBuilder()
{
    FdoSmPhSqsMgrP mrg = GetSchemaManager()->GetPhysicalSchema()->SmartCast<FdoSmPhSqsMgr>();
    FdoSmPhOwnerP owner = mrg->GetOwner();
    if (owner && !owner->GetHasAssocMetaSchema())
        return new FdoRdbmsSqlServerSqlBuilder (this);
    return NULL;    
}
