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
#include "FdoRdbmsSqlServerSpatialGeometryConverter.h"

#include "DbiConnection.h"
#include "Rdbms/FdoRdbmsCommandType.h"

#include <Inc/Rdbi/proto.h>
#include "../ODBCDriver/context.h"
int odbcdr_rdbi_init( odbcdr_context_def **, rdbi_methods	methods );


wchar_t* getComDir (); // in SqlServer.cpp
#define OD_MAX_RETRY_COUNT 10

FdoRdbmsSqlServerConnection::FdoRdbmsSqlServerConnection():
mFilterProcessor( NULL ),
mConnectionInfo(NULL),
mGeographyConverter(NULL)
{
}

FdoRdbmsSqlServerConnection::~FdoRdbmsSqlServerConnection ()
{
    if( mFilterProcessor )
        delete mFilterProcessor;

    if( mGeographyConverter )
        delete mGeographyConverter;

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
    FdoICommand* ret;

	FdoPtr<FdoICommandCapabilities> cmdCapabilities = GetCommandCapabilities();

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

        case FdoRdbmsCommandType_CreateSpatialIndex:
        case FdoRdbmsCommandType_DestroySpatialIndex:
        case FdoRdbmsCommandType_GetSpatialIndexes:
			throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_10, "Command not supported"));
			break;

		 default:
			 int size;
			 FdoInt32 *cmds = cmdCapabilities->GetCommands( size );
			 for(int i=0; i<size; i++ )
			 {
				 if( cmds[i] == commandType )
					 return FdoRdbmsConnection::CreateCommand( commandType );
			 }
             return FdoRdbmsConnection::CreateCommand( -1/*undefined*/ );
    }
    return (ret);
}

FdoRdbmsFilterProcessor* FdoRdbmsSqlServerConnection::GetFilterProcessor()
{
    if( mFilterProcessor == NULL )
        mFilterProcessor = new FdoRdbmsSqlServerFilterProcessor( this );

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

    if ((isDateSupplied) && (!isTimeSupplied))
        sprintf (ret, "%4d-%02d-%02d", when.year, when.month, when.day);
    else
        sprintf (ret, "%4d-%02d-%02d %02d:%02d:%02.2f",
                 when.year,
                 when.month,
                 when.day,
                 when.hour,
                 when.minute,
                 when.seconds);

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
    if ( !owner || !owner->GetHasMetaSchema() ) 
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
		//  "DRIVER={SQL Server};SERVER=seconds;UID=username;PWD=passwd;" 
		// If the UID and PWD parameters are not specified, the trusted connection 
		// (windows authentication) is assumed.
        FdoStringP dataSource = dict->GetProperty(FDO_RDBMS_CONNECTION_SERVICE);
        if (dataSource != NULL && dataSource.GetLength() > 0)
        {
            newCs = L"DRIVER={SQL Server}; SERVER=";
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
    bool isGeom = false;
    FdoInt64 srid = 0;
    FdoStringP bindStr(L"?", true);

    const FdoSmLpGeometricPropertyDefinition* geomProp =
        FdoSmLpGeometricPropertyDefinition::Cast(prop);

    if ( geomProp ) 
    {
        FdoStringP geomType(L"geometry", true);

        // For geometric properties, convert from WKB and add SRID.

        FdoStringP scName = geomProp->GetSpatialContextAssociation();

        FdoSchemaManagerP schemaMgr = this->GetSchemaManager();

        // First, get SRID. Try column first.

        FdoSmPhColumnP column = ((FdoSmLpGeometricPropertyDefinition*) geomProp)->GetColumn();
    
        if ( column ) 
        {
            FdoSmPhColumnGeomP geomColumn = column->SmartCast<FdoSmPhColumnGeom>();

            if ( geomColumn ) 
            {
                srid = geomColumn->GetSRID();
                // Also find out if geometry or geography column
                geomType = geomColumn->GetTypeName();
            }
        }

        // If Associated Spatial Context can be reached, get SRID from it
        // instead

        FdoSmLpSpatialContextMgrP scMgr = schemaMgr->GetLpSpatialContextMgr();
        FdoSmLpSpatialContextP sc = scMgr->FindSpatialContext(scName);

        if ( sc )
        {
            srid = sc->GetSrid();
        }

        bindStr = FdoStringP::Format( L"%ls::STGeomFromWKB(?, %ls)", (FdoString*) geomType, (FdoString*) FdoCommonStringUtil::Int64ToString(srid) );    
    }

    return bindStr; 
}

bool  FdoRdbmsSqlServerConnection::BindGeometriesLast() 
{ 
    return true; 
}

FdoIGeometry* FdoRdbmsSqlServerConnection::TransformGeometry( FdoIGeometry* geom, const FdoSmLpGeometricPropertyDefinition* prop, bool toFdo )
{
    FdoStringP geomType;
    bool       geogLatLong = false;

    if ( !IsGeogLatLong() )
        // No special transformation for geometry columns
        return FdoRdbmsConnection::TransformGeometry( geom, prop, toFdo );

    //TODO: check performance impact of looking up geomType for each geometry value and
    //optimize if necessary.
    FdoSmPhColumnP column = ((FdoSmLpGeometricPropertyDefinition*) prop)->GetColumn();
    
    if ( column ) 
    {
        FdoSmPhColumnGeomP geomColumn = column->SmartCast<FdoSmPhColumnGeom>();

        if ( geomColumn ) 
        {
            geomType = geomColumn->GetTypeName();
        }
    }

    if ( geomType != L"geography" )
        // No special transformation for geometry columns
        return FdoRdbmsConnection::TransformGeometry( geom, prop, toFdo );


    if ( !mGeographyConverter )
        mGeographyConverter = new FdoRdbmsSqlServerSpatialGeographyConverter();

    // For geography columns, flip the X and Y.
    return mGeographyConverter->ConvertOrdinates( geom );
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
