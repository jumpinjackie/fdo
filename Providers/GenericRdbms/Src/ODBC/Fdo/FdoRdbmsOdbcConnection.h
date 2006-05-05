#ifndef FDORDBMSODBCCONNECTION_H
#define FDORDBMSODBCCONNECTION_H	1
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


#include "FdoRdbms.h"
#include <Rdbms/Override/ODBC/OdbcOvPhysicalSchemaMapping.h>

// ODBC date/time format:
#define FDORDBMSODBCFILTER_DATE_PREFIX                 L"{d '"
#define FDORDBMSODBCFILTER_TIME_PREFIX                 L"{t '"
#define FDORDBMSODBCFILTER_TIMESTAMP_PREFIX            L"{ts '"
#define FDORDBMSODBCFILTER_DATE_FORMAT                 L"%.4d-%.2d-%.2d"
#define FDORDBMSODBCFILTER_TIME_FORMAT                 L"%.2d:%.2d:%.2d"
#define FDORDBMSODBCFILTER_DATETIME_SEPARATOR          L" "
#define FDORDBMSODBCFILTER_DATETIME_SUFFIX             L"'}"

class FdoRdbmsOdbcFilterProcessor;

class FdoRdbmsOdbcConnection: public FdoRdbmsConnection
{
private:
    FdoRdbmsOdbcFilterProcessor *mFilterProcessor;

    FdoIConnectionInfo          *mConnectionInfo;

protected:
    virtual ~FdoRdbmsOdbcConnection ();
    FdoRdbmsOdbcConnection ();

public:
	FDORDBMS_API static FdoRdbmsOdbcConnection* Create();
    
    //	Creates and returns the specified type of command object
    //	associated with the connection.
    virtual FdoICommand *CreateCommand (FdoInt32 commandType);

    virtual FdoRdbmsFilterProcessor* GetFilterProcessor();

    // Creates a Schema Manager for the Oracle Provider.
    virtual FdoSchemaManagerP NewSchemaManager(
        GdbiConnection* gdbiConnection, 
        FdoStringP currUser,
        FdoStringP currSchema
    );

    // Creates an empty Schema Mapping set appropriate for the Odbc Provider 
    virtual FdoPhysicalSchemaMapping* CreateSchemaMapping()
    {
        return FdoOdbcOvPhysicalSchemaMapping::Create();
    }

    virtual FdoIConnectionInfo* GetConnectionInfo();

    virtual FdoIConnectionCapabilities* GetConnectionCapabilities();
    virtual FdoICommandCapabilities* GetCommandCapabilities();
    virtual FdoISchemaCapabilities* GetSchemaCapabilities();
    virtual FdoIFilterCapabilities* GetFilterCapabilities();
    virtual FdoIExpressionCapabilities* GetExpressionCapabilities();

    /// <summary>Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.</summary>
    /// <returns>Returns the geometry capabilities</returns>
    virtual FdoIGeometryCapabilities* GetGeometryCapabilities();

    //
    // Converts a Odbc string date of a specific format to a FdoDateTime (time_t) format.
    virtual FdoDateTime  DbiToFdoTime( const char* time );

    //
    // Convert time_t( FdoDateTime ) to a Odbc string date of the form.
    // It return a statically allocated storage that can be overwritten by subsequent call to this or other methods.
    virtual const char* FdoToDbiTime( FdoDateTime  time );
    virtual const char* FdoToDbiTimeFilter( FdoDateTime  when );
	virtual FdoConnectionState Open ();
	virtual void Close();

    // Creates a Long Transaction Manager and its corresponding Long Transaction
    // Manager Service.
    FdoRdbmsLongTransactionManager *CreateLongTransactionManager();

    // Returns a DataStorePropertyDictionary object for this provider.
    virtual FdoIDataStorePropertyDictionary* CreateDataStoreProperties( int action );

	//Lock Manager
	FdoRdbmsLockManager *CreateLockManager();
};

#endif // FDORDBMSOdbcCONNECTION_H
