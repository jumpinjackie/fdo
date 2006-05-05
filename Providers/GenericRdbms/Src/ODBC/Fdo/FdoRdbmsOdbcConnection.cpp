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

#include <Inc/Rdbi/proto.h>
#include "../../ODBCDriver/context.h"
int odbcdr_rdbi_init( odbcdr_context_def **, rdbi_methods	methods );


wchar_t* getComDir (); // in Odbc.cpp
#define OD_MAX_RETRY_COUNT 10

FdoRdbmsOdbcConnection::FdoRdbmsOdbcConnection():
mFilterProcessor( NULL ),
mConnectionInfo(NULL)
{
}

FdoRdbmsOdbcConnection::~FdoRdbmsOdbcConnection ()
{
    if( mFilterProcessor )
        delete mFilterProcessor;

    FDO_SAFE_RELEASE(mConnectionInfo);
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

    return FDO_SAFE_ADDREF(mFilterProcessor);
}

FdoIConnectionInfo* FdoRdbmsOdbcConnection::GetConnectionInfo()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new FdoRdbmsOdbcConnectionInfo(this);
    FDO_SAFE_ADDREF(mConnectionInfo);
    return mConnectionInfo;
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

    if ((0 != when.hour) || (0 != when.minute) || (0 != when.seconds))
    {
        // "1979-11-30 00:00:00"
        sprintf (ret, "%4d-%02d-%02d %02d:%02d:%02d",
            when.year,
            when.month,
            when.day,
            when.hour,
            when.minute,
            when.seconds);
    }
    else
    {
        // "1979-11-30"
        sprintf (ret, "%4d-%02d-%02d",
            when.year,
            when.month,
            when.day);
    }

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
    return (FdoIDataStorePropertyDictionary *) new FdoRdbmsOdbcDataStorePropertyDictionary( this, action );
}

FdoRdbmsLockManager *FdoRdbmsOdbcConnection::CreateLockManager()
{
	return NULL;
}
