//
// Copyright (C) 2006 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include "FdoRdbmsPostGisConnectionInfo.h"
#include "FdoRdbmsPostGisConnection.h"
#include "FdoRdbmsPostGisCreateDataStore.h"
#include "FdoRdbmsPostGisDeleteDataStore.h"
#include "FdoRdbmsPostGisGetDataStores.h"
#include "FdoRdbmsPostGisFilterProcessor.h"
#include "../SchemaMgr/SchemaManager.h"
#include "../SchemaMgr/Ph/Mgr.h"
#include "FdoRdbmsPostGisGeometryCapabilities.h"
#include "FdoRdbmsPostGisSchemaCapabilities.h"
#include "FdoRdbmsPostGisConnectionCapabilities.h"
#include "FdoRdbmsPostGisCommandCapabilities.h"
#include "FdoRdbmsPostGisFilterCapabilities.h"
#include <Rdbms/Override/PostGis/PostGisOvPhysicalSchemaMapping.h>
#include <DbiConnection.h>
#include <Inc/Rdbi/proto.h>
#include <cassert>

// Initializes RDBI driver for PostGIS
extern initializer postgis_rdbi_init;

// Implemented in PostGis.cpp
wchar_t* getComDir ();

#define OD_MAX_RETRY_COUNT 10

FdoRdbmsPostGisConnection::FdoRdbmsPostGisConnection()
    : mFilterProcessor(NULL), mConnectionInfo(NULL)
{
    // idle
}

FdoRdbmsPostGisConnection::~FdoRdbmsPostGisConnection()
{
    delete mFilterProcessor;

    FDO_SAFE_RELEASE(mConnectionInfo);
}

FdoRdbmsPostGisConnection* FdoRdbmsPostGisConnection::Create()
{
    FdoRdbmsPostGisConnection* conn = NULL;
    conn = new FdoRdbmsPostGisConnection();

    conn->GetDbiConnection()->InitRdbi(postgis_rdbi_init);

	return conn;
}

FdoICommand *FdoRdbmsPostGisConnection::CreateCommand(FdoInt32 commandType)
{
    FdoICommand* ret = NULL;

    switch (commandType)
    {
    case FdoCommandType_CreateDataStore:
        ret = new FdoRdbmsPostGisCreateDataStore(this);
        break;

    case FdoCommandType_DestroyDataStore:
        ret = new FdoRdbmsPostGisDeleteDataStore(this);
        break;

    case FdoCommandType_ListDataStores:
        ret = new FdoRdbmsPostGisGetDataStores(this);
        break;

    case FdoCommandType_ActivateSpatialContext:
    case FdoCommandType_AcquireLock:
    case FdoCommandType_GetLockInfo:
    case FdoCommandType_GetLockedObjects:
    case FdoCommandType_GetLockOwners:
    case FdoCommandType_ReleaseLock:
    case FdoCommandType_ActivateLongTransaction:
    case FdoCommandType_DeactivateLongTransaction:
    case FdoCommandType_CommitLongTransaction:
    case FdoCommandType_CreateLongTransaction:
    case FdoCommandType_GetLongTransactions:
    case FdoCommandType_RollbackLongTransaction:
        {
            throw FdoConnectionException::Create(
                NlsMsgGet(FDORDBMS_10, "Command not supported"));
        }

    default:
        return FdoRdbmsConnection::CreateCommand(commandType);
    }

    return (ret);
}

FdoRdbmsFilterProcessor* FdoRdbmsPostGisConnection::GetFilterProcessor()
{
    if (NULL == mFilterProcessor)
    {
        mFilterProcessor = new FdoRdbmsPostGisFilterProcessor(this);
    }

    return FDO_SAFE_ADDREF(mFilterProcessor);
}

FdoIConnectionInfo* FdoRdbmsPostGisConnection::GetConnectionInfo()
{
    if (NULL == mConnectionInfo)
    {
        mConnectionInfo = new FdoRdbmsPostGisConnectionInfo(this);
    }

    FDO_SAFE_ADDREF(mConnectionInfo);
    return mConnectionInfo;
}

FdoIConnectionCapabilities* FdoRdbmsPostGisConnection::GetConnectionCapabilities()
{
    if (NULL == mConnectionCapabilities)
    {
        mConnectionCapabilities = new FdoRdbmsPostGisConnectionCapabilities();
    }

    FDO_SAFE_ADDREF(mConnectionCapabilities);
    return mConnectionCapabilities;
}

FdoIGeometryCapabilities* FdoRdbmsPostGisConnection::GetGeometryCapabilities()
{
    if (NULL == mGeometryCapabilities)
    {
        mGeometryCapabilities = new FdoRdbmsPostGisGeometryCapabilities();
    }

    FDO_SAFE_ADDREF(mGeometryCapabilities);
    return mGeometryCapabilities;
}

FdoISchemaCapabilities* FdoRdbmsPostGisConnection::GetSchemaCapabilities()
{
    if (NULL == mSchemaCapabilities)
    {
        mSchemaCapabilities = new FdoRdbmsPostGisSchemaCapabilities();
    }

    FDO_SAFE_ADDREF(mSchemaCapabilities);
    return mSchemaCapabilities;
}

FdoSchemaManagerP FdoRdbmsPostGisConnection::NewSchemaManager(
    GdbiConnection* gdbiConnection, 
    FdoStringP currUser,
    FdoStringP currSchema)
{
    FdoSchemaManagerP schMgr(new FdoPostGisSchemaManager(gdbiConnection, currSchema));

    FdoSmPhPostGisMgrP physMgr(
        schMgr->GetPhysicalSchema()->SmartCast<FdoSmPhPostGisMgr>());

    // Pass down the location of the COM directory for MetaSchema creation scripts.
	physMgr->SetHomeDir(FdoStringP(getComDir()));

    return schMgr;
}

FdoPhysicalSchemaMapping* FdoRdbmsPostGisConnection::CreateSchemaMapping()
{
    FdoPhysicalSchemaMapping* smMapping = NULL;
    smMapping = FdoPostGISOvPhysicalSchemaMapping::Create();
                
    assert(NULL != smMapping);

    return smMapping;
}

FdoDateTime FdoRdbmsPostGisConnection::DbiToFdoTime(const char* timeStr)
{
    FdoDateTime fdoTime;
    const int datePartsCount = 6;
    int year, month, day, hour, minute, seconds;
    year = month = day = hour = minute = seconds = 0;

    if (NULL != timeStr && '\0' != (*timeStr))
    {
        int count = sscanf(timeStr,"%4d-%02d-%02d %02d:%02d:%02d",
                    &year, &month, &day, &hour, &minute, &seconds);     
        if (datePartsCount != count)
        {
            count = sscanf(timeStr,"%4d-%02d-%02d",&year, &month, &day);
        }
    }

    fdoTime.year = static_cast<FdoInt16>(year);
    fdoTime.month = static_cast<FdoByte>(month);
    fdoTime.day = static_cast<FdoByte>(day);
    fdoTime.hour = static_cast<FdoByte>(hour);
    fdoTime.minute = static_cast<FdoByte>(minute);
    fdoTime.seconds = static_cast<float>(seconds);

    return fdoTime;
}

const char* FdoRdbmsPostGisConnection::FdoToDbiTime( FdoDateTime  when )
{

    char *ret = GetDbiConnection()->GetUtility()->newCharP();
    assert(NULL != ret);

	bool isDateSupplied = ((when.year != -1) || (when.month != -1) || (when.day != -1));
    bool isValidDate = isDateSupplied
                        && ((when.year != -1) && (when.month != -1) && (when.day != -1));

	bool isTimeSupplied = ((when.hour != -1) || (when.minute != -1));
    bool isValidTime = isTimeSupplied
                        && ((when.hour != -1) && (when.minute != -1));

	if ((isDateSupplied  && !isValidDate)
        || (isTimeSupplied  && !isValidTime)
        || (!isDateSupplied && !isTimeSupplied))
    {
		 throw FdoException::Create(NlsMsgGet(FDORDBMS_480,
                     "Incomplete date/time setting."));
    }

    if (isDateSupplied) 
	{
		if (!isTimeSupplied)
		{
			// "1979-11-30"
			sprintf (ret, "%4d-%02d-%02d",
				when.year,
				when.month,
				when.day);
		}
		else
        {
            // "1979-11-30 00:00:00"
            sprintf (ret, "%4d-%02d-%02d %02d:%02d:%02.2f",
                when.year,
                when.month,
                when.day,
                when.hour,
                when.minute,
                when.seconds);
        }
    }
    else
    {
        // TODO: mloskot - check this comment against PostGIS capabilities.
        //
		// PostGIS (MySQL) accepts time only dates, but it needs the date fields too,
		// otherwise time could be interpreted as date '2010-45-45' which is 
		// illegal and therefore changed to '0000-00-00'.

		// "0000-00-00 10:45:05"
		sprintf (ret, "0000-00-00 %02d:%02d:%02.2f",
				when.hour,
				when.minute,
				when.seconds);
    }

    return (ret);
}

FdoStringP FdoRdbmsPostGisConnection::GetBindString( int n, const FdoSmLpPropertyDefinition* prop )
{
    FdoStringP str = FdoStringP::Format(L"$%d",n);
    return str;
}

FdoConnectionState FdoRdbmsPostGisConnection::Open()
{
    FdoConnectionState state = GetConnectionState();
 	if (FdoConnectionState_Open == state )
        return (state);

    if( FdoConnectionState_Pending == state ) // Close it and re-open it with the datastore context set.
        this->Close();

    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo();
    FdoPtr<FdoCommonConnPropDictionary> dict = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());      
    FdoPtr<ConnectionProperty> datastoreProp = dict->FindProperty(FDO_RDBMS_CONNECTION_DATASTORE);
    if( datastoreProp != NULL && wcslen(datastoreProp->GetValue())!=0 )
    {
        // Append the datastore name to the service string; the driver expects a <dbname>@<host>:<port> service format.
        FdoStringP service = datastoreProp->GetValue();

        service += "@";
        FdoPtr<ConnectionProperty> servprop = dict->FindProperty(FDO_RDBMS_CONNECTION_SERVICE);
        if (servprop == NULL ) 
        {
            // it should not happen but just in case.
            servprop = new ConnectionProperty (FDO_RDBMS_CONNECTION_SERVICE, 
                NlsMsgGet(FDORDBMS_120, "Service"), L"", 
                true, false, false, false, false, false, false, 0, NULL); 
            dict->AddProperty(servprop);
        }

        FdoStringP servpropstr = servprop->GetValue();
        // Remove any current datastore in service name.
        // If re-opening, datastore would have been added by first open.
        if ( servpropstr.Contains(L"@") ) 
            servpropstr = servpropstr.Right(L"@");

        if( servpropstr.GetLength() != 0 )
            service += servpropstr;
        else
            service += L"localhost"; // defaults to localhost

        servprop->SetValue((const wchar_t*)service );
    }       

    return FdoRdbmsConnection::Open();
}

void FdoRdbmsPostGisConnection::Close()
{
	FdoRdbmsConnection::Close();
}

FdoRdbmsLongTransactionManager* FdoRdbmsPostGisConnection::CreateLongTransactionManager()
{
    return NULL;
}

FdoIDataStorePropertyDictionary* FdoRdbmsPostGisConnection::CreateDataStoreProperties(
    int action)
{
    FdoCommonDataStorePropDictionary* dsPropDict = NULL;
    dsPropDict = new FdoCommonDataStorePropDictionary(this);
	
    FdoPtr<ConnectionProperty> newProp;
	if (FDO_RDBMS_DATASTORE_FOR_READ == action)
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE,
                NlsMsgGet(FDORDBMS_117, "DataStore"), L"",
                false, false, false, false, false, true, false, 0, NULL);
        dsPropDict->AddProperty(newProp);
        
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_DESCRIPTION,
                NlsMsgGet(FDORDBMS_448, "Description"), L"",
                false, false, false, false, false, false, false, 0, NULL);
        dsPropDict->AddProperty(newProp);
		
    	wchar_t **LtModeValues = new wchar_t*[2];
        LtModeValues[0] = new wchar_t[4];
        LtModeValues[1] = new wchar_t[5];
        wcscpy(LtModeValues[0], L"FDO");
        wcscpy(LtModeValues[1], L"NONE");
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_LTMODE,
                NlsMsgGet(FDORDBMS_449, "LtMode"), L"FDO",
                false, false, true, false, false, false, false, 2,
                const_cast<const wchar_t**>(LtModeValues));
        dsPropDict->AddProperty(newProp);
		
    	wchar_t **LockModeValues = new wchar_t*[2];
        LockModeValues[0] = new wchar_t[4];
        LockModeValues[1] = new wchar_t[5];
        wcscpy(LockModeValues[0], L"FDO");
        wcscpy(LockModeValues[1], L"NONE");
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_LOCKMODE,
                NlsMsgGet(FDORDBMS_450, "LockMode"), L"FDO",
                false, false, true, false, false, false, false, 2,
                const_cast<const wchar_t**>(LockModeValues));
        dsPropDict->AddProperty(newProp);
	}
	else if (FDO_RDBMS_DATASTORE_FOR_CREATE == action)
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE,
                NlsMsgGet(FDORDBMS_117, "DataStore"), L"",
                true, false, false, false, false, true, false, 0, NULL);
        dsPropDict->AddProperty(newProp);

        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_DESCRIPTION,
                NlsMsgGet(FDORDBMS_448, "Description"), L"",
                false, false, false, false, false, false, false, 0, NULL);
        dsPropDict->AddProperty(newProp);

        wchar_t** enabledValues = new wchar_t*[2];
        enabledValues[0] = new wchar_t[10];
        wcscpy( enabledValues[0], L"false" );
        enabledValues[1] = new wchar_t[10];
        wcscpy( enabledValues[1], L"true" );

        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_FDO_ENABLED, NlsMsgGet(FDORDBMS_9, "IsFdoEnabled"), L"false", false, false, true, false, false, false, false, 2, (const wchar_t**) enabledValues);
        dsPropDict->AddProperty(newProp);
	}
	else if (FDO_RDBMS_DATASTORE_FOR_DELETE == action)
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE,
                NlsMsgGet(FDORDBMS_117, "DataStore"), L"",
                true, false, false, false, false, true, false, 0, NULL);
        dsPropDict->AddProperty(newProp);
	}

    return dsPropDict;
}

FdoRdbmsLockManager *FdoRdbmsPostGisConnection::CreateLockManager()
{
    // TODO: mloskot - consider locking for PostGIS
	return NULL;
}

FdoICommandCapabilities* FdoRdbmsPostGisConnection::GetCommandCapabilities()
{
    if (NULL== mCommandCapabilities)
    {
        mCommandCapabilities = new FdoRdbmsPostGisCommandCapabilities();
    }

    FDO_SAFE_ADDREF(mCommandCapabilities);
    return mCommandCapabilities;
}

FdoIFilterCapabilities *FdoRdbmsPostGisConnection::GetFilterCapabilities()
{
    if (NULL == mFilterCapabilities)
    {
        mFilterCapabilities = new FdoRdbmsPostGisFilterCapabilities();
    }

    FDO_SAFE_ADDREF(mFilterCapabilities);
    return mFilterCapabilities;	
}
