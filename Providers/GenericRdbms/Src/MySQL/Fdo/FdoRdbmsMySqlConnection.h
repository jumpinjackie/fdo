#ifndef FDORDBMSMySqlCONNECTION_H
#define FDORDBMSMySqlCONNECTION_H	1
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
#include <Rdbms/Override/MySQL/MySqlOvPhysicalSchemaMapping.h>
#include "../../GenericLongTransactionManager/GenericLongTransactionManager.h"
#include "../LongTransactionManagerService/MySqlLongTransactionManagerService.h"

class FdoRdbmsMySqlFilterProcessor;

class FdoRdbmsMySqlConnection: public FdoRdbmsConnection
{
private:
    FdoRdbmsMySqlFilterProcessor *mFilterProcessor;

    FdoIConnectionInfo          *mConnectionInfo;
	void						logOpen(char access);
	void						delOpen();

protected:
    virtual ~FdoRdbmsMySqlConnection ();
    FdoRdbmsMySqlConnection ();

public:
	static FdoRdbmsMySqlConnection* Create();
    
    //	Creates and returns the specified type of command object
    //	associated with the connection.
    virtual FdoICommand *CreateCommand (FdoInt32 commandType);

    virtual FdoRdbmsFilterProcessor* GetFilterProcessor();

    // Creates a Schema Manager for the MySQL Provider.
    virtual FdoSchemaManagerP NewSchemaManager(
        GdbiConnection* gdbiConnection, 
        FdoStringP currUser,
        FdoStringP currSchema
    );


    // Creates an empty Schema Mapping set appropriate for the MySQL Provider 
    FDORDBMS_API virtual FdoPhysicalSchemaMapping* CreateSchemaMapping()
    {
        return FdoMySQLOvPhysicalSchemaMapping::Create();
    }

    virtual FdoIConnectionInfo* GetConnectionInfo();
    virtual FdoIConnectionCapabilities* GetConnectionCapabilities();

    /// <summary>Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.</summary>
    /// <returns>Returns the geometry capabilities</returns>
    virtual FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
    /// <returns>Returns schema capabilities</returns> 
	virtual FdoISchemaCapabilities* GetSchemaCapabilities();

    //
    // Converts a MySql string date of a specific format to a FdoDateTime (time_t) format.
    virtual FdoDateTime  DbiToFdoTime( const char* time );

    //
    // Convert time_t( FdoDateTime ) to a MySql string date of the form.
    // It return a statically allocated storage that can be overwritten by subsequent call to this or other methods.
    virtual const char* FdoToDbiTime( FdoDateTime  time );

    //Temporary Hack for demo'ing the MySql provider with MAP. Currently MAP sends all geometries
    //as 3D, but the MySql provider can only handle 2D. This function converts 3d geometries to 2d.
    virtual FdoIGeometry* Kludge3dGeomTo2D( FdoIGeometry* p3dGeom );
	virtual FdoConnectionState Open ();
	virtual void Close();

    // Creates a Long Transaction Manager and its corresponding Long Transaction
    // Manager Service.
    FdoRdbmsLongTransactionManager *CreateLongTransactionManager();

	// Returns a DataStorePropertyDictionary object for this provider.
	virtual FdoIDataStorePropertyDictionary* CreateDataStoreProperties( int action );

	// Lock Manager
	FdoRdbmsLockManager *CreateLockManager();

	virtual FdoICommandCapabilities* GetCommandCapabilities();

	virtual FdoIFilterCapabilities* GetFilterCapabilities();
};

#endif // FDORDBMSMySqlCONNECTION_H
