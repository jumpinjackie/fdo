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

#include "FdoRdbmsMySqlConnectionInfo.h"

#include "FdoRdbmsMySqlConnection.h"
#include "FdoRdbmsMySqlCreateDataStore.h"
#include "FdoRdbmsMySqlDeleteDataStore.h"
#include "FdoRdbmsMySqlFilterProcessor.h"
#include "../SchemaMgr/SchemaManager.h"
#include "../SchemaMgr/Ph/Mgr.h"
#include "FdoRdbmsMySqlGeometryCapabilities.h"
#include "FdoRdbmsMySqlSchemaCapabilities.h"
#include "FdoRdbmsMySqlConnectionCapabilities.h"
#include "FdoRdbmsMySqlCommandCapabilities.h"
#include "FdoRdbmsMySqlFilterCapabilities.h"

#include "DbiConnection.h"
#include <Inc/Rdbi/proto.h>
extern initializer mysql_rdbi_init;

wchar_t* getComDir (); // in MySql.cpp
#define OD_MAX_RETRY_COUNT 10

FdoRdbmsMySqlConnection::FdoRdbmsMySqlConnection():
mFilterProcessor( NULL ),
mConnectionInfo(NULL)
{
}

FdoRdbmsMySqlConnection::~FdoRdbmsMySqlConnection ()
{
    if( mFilterProcessor )
        delete mFilterProcessor;

    FDO_SAFE_RELEASE(mConnectionInfo);
}

FdoRdbmsMySqlConnection* FdoRdbmsMySqlConnection::Create()
{
    FdoRdbmsMySqlConnection   *conn = new FdoRdbmsMySqlConnection();
    conn->GetDbiConnection()->InitRdbi( mysql_rdbi_init );
	return conn;
}

FdoICommand *FdoRdbmsMySqlConnection::CreateCommand (FdoInt32 commandType)
{
    FdoICommand* ret;

    switch (commandType)
    {
        case FdoCommandType_CreateDataStore:
            ret = new FdoRdbmsMySqlCreateDataStore (this);
            break;

        case FdoCommandType_DestroyDataStore:
             ret = new FdoRdbmsMySqlDeleteDataStore (this);
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
			throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_10, "Command not supported"));
			break;

		 default:
             return FdoRdbmsConnection::CreateCommand( commandType );
    }
    return (ret);
}

FdoRdbmsFilterProcessor* FdoRdbmsMySqlConnection::GetFilterProcessor()
{
    if( mFilterProcessor == NULL )
        mFilterProcessor = new FdoRdbmsMySqlFilterProcessor( this );

    return FDO_SAFE_ADDREF(mFilterProcessor);
}

FdoIConnectionInfo* FdoRdbmsMySqlConnection::GetConnectionInfo()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new FdoRdbmsMySqlConnectionInfo(this);
    FDO_SAFE_ADDREF(mConnectionInfo);
    return mConnectionInfo;
}

FdoIConnectionCapabilities* FdoRdbmsMySqlConnection::GetConnectionCapabilities()
{
    if (mConnectionCapabilities == NULL)
        mConnectionCapabilities = new FdoRdbmsMySqlConnectionCapabilities();
    FDO_SAFE_ADDREF(mConnectionCapabilities);
    return mConnectionCapabilities;
}

/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* FdoRdbmsMySqlConnection::GetGeometryCapabilities()
{
    if( mGeometryCapabilities == NULL )
        mGeometryCapabilities = new FdoRdbmsMySqlGeometryCapabilities();

    return FDO_SAFE_ADDREF(mGeometryCapabilities);
}

/// Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.
FdoISchemaCapabilities* FdoRdbmsMySqlConnection::GetSchemaCapabilities()
{
    if (mSchemaCapabilities == NULL)
        mSchemaCapabilities = new FdoRdbmsMySqlSchemaCapabilities();
    FDO_SAFE_ADDREF(mSchemaCapabilities);
    return mSchemaCapabilities;
}

FdoSchemaManagerP FdoRdbmsMySqlConnection::NewSchemaManager(
    GdbiConnection* gdbiConnection, 
    FdoStringP currUser,
    FdoStringP currSchema
)
{
    FdoSchemaManagerP schMgr = new FdoMySqlSchemaManager(
        gdbiConnection,
        currSchema
    );

    FdoSmPhMySqlMgrP physMgr = schMgr->GetPhysicalSchema()->SmartCast<FdoSmPhMySqlMgr>();

    // Pass down the location of the COM directory for MetaSchema creation scripts.
	physMgr->SetHomeDir( FdoStringP(getComDir()) );

    return schMgr;
}

 //
// Converts a MySql string date of a specific format to a FdoDateTime (time_t) format.
FdoDateTime  FdoRdbmsMySqlConnection::DbiToFdoTime( const char* timeStr )
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
// Convert time_t( FdoDateTime ) to a MySql string date of the form.
// It return a statically allocated storage that can be overwritten by subsequent call to this or other methods.
const char* FdoRdbmsMySqlConnection::FdoToDbiTime( FdoDateTime  when )
{
    char *ret = GetDbiConnection()->GetUtility()->newCharP();

	bool isDateSupplied = ((when.year != -1) || (when.month != -1) || (when.day != -1));
    bool isValidDate    = isDateSupplied && ((when.year != -1) && (when.month != -1) && (when.day != -1));

	bool isTimeSupplied = ((when.hour != -1) || (when.minute != -1));
    bool isValidTime    = isTimeSupplied && ((when.hour != -1) && (when.minute != -1));

	if ((isDateSupplied  && !isValidDate)    ||
        (isTimeSupplied  && !isValidTime)    ||
        (!isDateSupplied && !isTimeSupplied)    )
		 throw FdoException::Create(NlsMsgGet(FDORDBMS_480,
                                              "Incomplete date/time setting."));

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
		// MySQL accepts time only dates, but it needs the date fields too,
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

FdoIGeometry* FdoRdbmsMySqlConnection::Kludge3dGeomTo2D( FdoIGeometry* p3dGeom )
{
    FdoIGeometry* p2dGeom = NULL;
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    double x,y,z,m;
    FdoInt32 dimensionality;

    if ( p3dGeom->GetDerivedType() == FdoGeometryType_LineString ) {
        FdoILineString* tempLineString = (FdoILineString*) (FdoIGeometry*) p3dGeom;
        double* coordBuffer = (double*) alloca( sizeof(double) * tempLineString->GetCount() * 2 );
        int bufIx = 0;
        for ( int ix = 0; ix < tempLineString->GetCount(); ix++ ) {
            tempLineString->GetItemByMembers( ix, &x, &y, &z, &m, &dimensionality );
            coordBuffer[bufIx++] = x;
            coordBuffer[bufIx++] = y;
        }

        p2dGeom = (FdoIGeometry*) gf->CreateLineString(FdoDimensionality_XY, bufIx, coordBuffer);
    }
    else if ( p3dGeom->GetDerivedType() == FdoGeometryType_Point ) {
        FdoIPoint* tempPoint = (FdoIPoint*) (FdoIGeometry*) p3dGeom;
        tempPoint->GetPositionByMembers( &x, &y, &z, &m, &dimensionality );
        double* coordBuffer = (double*) alloca( sizeof(double) * 2 );
        int bufIx = 0;
        coordBuffer[bufIx++] = x;
        coordBuffer[bufIx++] = y;
        p2dGeom = (FdoIGeometry*) gf->CreatePoint(FdoDimensionality_XY, coordBuffer);
    }
    //TODO: add more geometry types as needed by demo
    else {
        p2dGeom = FDO_SAFE_ADDREF(p3dGeom);
    }

    return p2dGeom;
}
FdoConnectionState FdoRdbmsMySqlConnection::Open()
{
 	if( GetConnectionState() != FdoConnectionState_Open )
	{
  	    FdoConnectionState  state = FdoRdbmsConnection::Open();
	    if( state == FdoConnectionState_Open )
	    {
		    try
		    {
			    logOpen('s');
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

    return GetConnectionState();
}

void FdoRdbmsMySqlConnection::Close()
{
	if( GetConnectionState() == FdoConnectionState_Open )
		delOpen();
		
	FdoRdbmsConnection::Close();
}

// TODO: externalize messages in logOpen
void FdoRdbmsMySqlConnection::logOpen(char accessMode)
{
	int					i;
	FdoStringP			sql_stmt;
	int					new_user_num;
	int					user_num;
	char				lower_access[2];
	int					rc;
	GdbiStatement*		insertStmt = NULL;
	GdbiStatement*		gdbiStmt = NULL;
	GdbiQueryResult*	gdbiResult = NULL;
		
    FdoSmPhMgrP phMgr = GetSchemaManager()->GetPhysicalSchema();

	if (!phMgr->FindDbObject(phMgr->GetDcDbObjectName(FDO_DBOPEN_TABLE)))
		return;

    lower_access[0] = tolower(accessMode);
	lower_access[1] = '\0';

	// check if f_dbopen exists


	// TODO Delete "orphan" user rows in f_dbopen
	// There is no MySQL table to retrieve active sessions.


	GetDbiConnection()->GetGdbiCommands()->tran_begin("update_db_open_table");

	// get thread id of the current session
	for (i = 0; i < OD_MAX_RETRY_COUNT; i++) {
		new_user_num = -1;
		if (i == 0)
		{
			sql_stmt =
			FdoStringP::Format(L"select %ls, %ls, %ls from %ls where %ls = (select max(%ls) from %ls ) for update",
				(FdoString*)FDO_DBUSER_COLUMN, (FdoString*)FDO_ACCESS_MODE_COLUMN, (FdoString*)FDO_USER_NUM_COLUMN,
				(FdoString*)FDO_DBOPEN_TABLE, (FdoString*)FDO_USER_NUM_COLUMN, (FdoString*)FDO_USER_NUM_COLUMN, 
				(FdoString*)FDO_DBOPEN_TABLE);
			gdbiStmt = GetDbiConnection()->GetGdbiConnection()->Prepare((const wchar_t*)sql_stmt);
		}
		gdbiResult = gdbiStmt->ExecuteQuery();
		gdbiResult->ReadNext();
		rc = GetDbiConnection()->GetGdbiCommands()->err_stat();
		if (rc == RDBI_END_OF_FETCH)
			new_user_num = 0;
		else	{
			if (gdbiResult->GetIsNull((char *)GetDbiConnection()->GetUtility()->UnicodeToUtf8(FDO_ACCESS_MODE_COLUMN)) == false)	{
					FdoInt8 s_access;
					s_access = gdbiResult->GetInt8( (const wchar_t*)FDO_ACCESS_MODE_COLUMN,NULL, NULL );
					if (s_access == 'e' || tolower(accessMode) == 'e')	{
						gdbiResult->Close();
						GetDbiConnection()->GetGdbiCommands()->tran_end("update_db_open_table");
						throw FdoConnectionException::Create(L"Database is exclusively locked");
					}
			}
			user_num = 
				(int)gdbiResult->GetInt64((char *)GetDbiConnection()->GetUtility()->UnicodeToUtf8(FDO_USER_NUM_COLUMN), 
						                    NULL, NULL);
			/*user_num = 
				gdbiResult->GetInt32((char *)GetDbiConnection()->GetUtility()->UnicodeToUtf8(FDO_USER_NUM_COLUMN), 
						                    NULL, NULL);*/
			new_user_num = user_num +1;
		
		}
		// end select
		if( gdbiResult )
		{
			gdbiResult->End();
			delete gdbiResult;
			gdbiResult = NULL;
		}
		if( gdbiStmt )
		{
			delete gdbiStmt;
			gdbiStmt = NULL;
		}
		if (i == 0)	{
			sql_stmt = FdoStringP::Format(L"insert into %ls (%ls, %ls, %ls, %ls, %ls) values (?, ?, ?, current_date(), connection_id())",
				(FdoString*)FDO_DBOPEN_TABLE, (FdoString*)FDO_DBUSER_COLUMN, (FdoString*)FDO_ACCESS_MODE_COLUMN, (FdoString*)FDO_USER_NUM_COLUMN, 
				(FdoString*)FDO_OPENDATE_COLUMN, (FdoString*)FDO_SESSION_COLUMN);
			insertStmt = GetDbiConnection()->GetGdbiConnection()->Prepare((const wchar_t*)sql_stmt);
		}

        FdoStringP user = GetDbiConnection()->GetUser();

		insertStmt->Bind(1, GDBI_DB_NAME_SIZE, (const char *)user, NULL);
		insertStmt->Bind(2, sizeof(char), (const char *)&lower_access, NULL);
		insertStmt->Bind(3, &new_user_num, NULL);
		try	{
			insertStmt->ExecuteNonQuery();
			break;
		}
		catch (FdoException *ex)	{
			rc = GetDbiConnection()->GetGdbiCommands()->err_stat();	
			if (rc == RDBI_DUPLICATE_INDEX) {
				ex->Release();
				//debug1("Retrying... tried to insert usernum: %d",new_user_num);
				continue;
			}
			else	{
				GetDbiConnection()->GetGdbiCommands()->tran_end("update_db_open_table");
				insertStmt->Free();
				delete insertStmt;
				insertStmt = NULL;
				throw ex;
			}
		}
	}
	
	GetDbiConnection()->GetGdbiCommands()->tran_end("update_db_open_table");
	if( insertStmt )
	{
		insertStmt->Free();
		delete insertStmt;
	}
	if( gdbiResult )
	{
		gdbiResult->Close();
		delete gdbiResult;
		gdbiResult = NULL;
	}
	if (i == OD_MAX_RETRY_COUNT)
        throw FdoConnectionException::Create(L"Max retry count (%1$d) exceeded. Open database failed.");
	
	// Remember user number set in F_DbOpen
	SetUserNum(new_user_num);
}
void FdoRdbmsMySqlConnection::delOpen()
{
	int rc;

    FdoSmPhMgrP phMgr = GetSchemaManager()->GetPhysicalSchema();

	if (!phMgr->FindDbObject(phMgr->GetDcDbObjectName(FDO_DBOPEN_TABLE)))
		return;

	if (GetUserNum() != -1)
	{
		FdoStringP sql_stmt = 
			FdoStringP::Format(L"delete from %ls where %ls = %d", (FdoString*)FDO_DBOPEN_TABLE, 
								(FdoString*)FDO_USER_NUM_COLUMN, GetUserNum());
		GdbiStatement* gdbiStmt = GetDbiConnection()->GetGdbiConnection()->Prepare((const wchar_t*)sql_stmt);
		rc = gdbiStmt->ExecuteNonQuery();
		gdbiStmt->Free();

		SetUserNum(-1);
        SetUserSessionId(-1);
	}
}

// Creates a Long Transaction Manager and its corresponding Long Transaction
// Manager Service.
FdoRdbmsLongTransactionManager *FdoRdbmsMySqlConnection::CreateLongTransactionManager()
{

    // There is no long transaction support in MySQL.

    return NULL;

}

FdoIDataStorePropertyDictionary*  FdoRdbmsMySqlConnection::CreateDataStoreProperties( int action )
{
	return (FdoIDataStorePropertyDictionary *)new FdoRdbmsMySqlDataStorePropertyDictionary( this, action );
}
FdoRdbmsLockManager *FdoRdbmsMySqlConnection::CreateLockManager()
{
	return NULL;
}

FdoICommandCapabilities* FdoRdbmsMySqlConnection::GetCommandCapabilities()
{
    if (mCommandCapabilities == NULL)
        mCommandCapabilities = new FdoRdbmsMySqlCommandCapabilities();
    FDO_SAFE_ADDREF(mCommandCapabilities);
    return mCommandCapabilities;
}

FdoIFilterCapabilities *FdoRdbmsMySqlConnection::GetFilterCapabilities()
{
    if (mFilterCapabilities == NULL)
        mFilterCapabilities = new FdoRdbmsMysqlFilterCapabilities();
    FDO_SAFE_ADDREF(mFilterCapabilities);
    return mFilterCapabilities;	
}
