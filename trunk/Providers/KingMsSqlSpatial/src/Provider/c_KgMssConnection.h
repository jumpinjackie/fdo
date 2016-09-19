/*
* Copyright (C) 2007  Haris Kurtagic
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
*/


#ifndef _c_KgMssConnection_h
#define _c_KgMssConnection_h

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "c_KgOtl.h"
#include "c_KgOtlStreamReader.h"
#include <c_KgMssSpatialContext.h>


#include <FdoCommonThreadMutex.h>



using namespace std;

class c_KgMssSchemaDesc;

class c_KgMssConnection : public FdoIConnection
{
    friend class c_KgMssSelectCommand;
    friend class KgOraGetSpatialContextsCommand;
    friend class c_KgMssFeatureReader;

protected:
    /**
     * The connection string 
     */
    FdoStringP m_ConnectionString;

    /**
     * The connection state (closed, pending, open).
     */
    FdoConnectionState m_ConnectionState;

    /**
     * The active spatial context.
     */
    FdoStringP m_ActiveSpatialContext;

    /**
     * Connection information object.
     */
    FdoPtr<FdoIConnectionInfo> m_ConnectionInfo;

    //oracle::occi::Connection * m_OcciConnection;
    c_KgOtl m_KgOtl;
public:    
    string m_OraConnectionUserName; // username used to open connection to oracle
    string m_OraConnectionPassword; // password
    string m_OraConnectionDbLink; // db link
    string m_OraSchemaName; // if not empty than provider will display classes just from this oracle schema
                              // if empty that will display classes from all schemas
                              
    string m_FdoViewsTable;
                                  
    int m_OracleMainVersion;
	  int m_OracleSubVersion;
protected:   


	FdoPtr<c_KgMssSchemaDesc> m_SchemaDesc;
	//FdoPtr<FdoFeatureSchemaCollection> m_FdoSchemas;
	//FdoPtr<c_KgMssOverPhysicalSchemaMapping> m_PhSchemaMapping;

    
	

   


	/**
	 * Globals to keep track of opened connections 
	 */
	static	FdoCommonThreadMutex g_Mutex;
	static	int g_GlobalRefCount;



public:
    c_KgMssConnection (void);
    virtual ~c_KgMssConnection(void);

    // GisIDisposable interface

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    void Dispose ();

    // FdoIConnection overrides

    /// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
    /// of the connection.</summary>
    /// <returns>Returns the connection capabilities</returns> 
    FDOKGMSS_API FdoIConnectionCapabilities* GetConnectionCapabilities ();

    /// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
    /// <returns>Returns schema capabilities</returns> 
    FDOKGMSS_API FdoISchemaCapabilities* GetSchemaCapabilities ();

    /// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
    /// <returns>Returns the command capabilities</returns> 
    FDOKGMSS_API FdoICommandCapabilities* GetCommandCapabilities ();

    /// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
    /// <returns>Returns the filter capabilities</returns> 
    FDOKGMSS_API FdoIFilterCapabilities* GetFilterCapabilities ();

    /// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
    /// <returns>Returns the expression capabilities</returns> 
    FDOKGMSS_API FdoIExpressionCapabilities* GetExpressionCapabilities ();

    /// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
    /// <returns>Returns the raster capabilities</returns> 
    FDOKGMSS_API FdoIRasterCapabilities* GetRasterCapabilities ();

    /// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
    /// <returns>Returns the topology capabilities</returns> 
    FDOKGMSS_API FdoITopologyCapabilities* GetTopologyCapabilities ();

    /// <summary>Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.</summary>
    /// <returns>Returns the geometry capabilities</returns> 
    FDOKGMSS_API virtual FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// <summary>Gets the connection string used to open a DataStore.</summary>
    /// <returns>Returns the connection string</returns> 
    FDOKGMSS_API FdoString* GetConnectionString ();

    /// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
    /// connection is closed.</summary>
    /// <param name="value">Input the connection string</param> 
    /// <returns>Returns nothing</returns> 
    FDOKGMSS_API void SetConnectionString (FdoString* value);

    /// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
    /// <returns>Returns the connection info</returns> 
    FDOKGMSS_API FdoIConnectionInfo* GetConnectionInfo ();

    /// <summary>Gets the current state of the connection.</summary>
    /// <returns>Returns the current state of the connection</returns> 
    FDOKGMSS_API FdoConnectionState GetConnectionState ();

    /// <summary>Gets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability 0 will be returned</summary>
    /// <returns>Returns the time to wait (in milliseconds)</returns> 
    FDOKGMSS_API FdoInt32 GetConnectionTimeout ();

    /// <summary>Sets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability then attempting to
    /// set a timeout will result in an exception</summary>
    /// <param name="value">Input the time to wait (in milliseconds)</param> 
    /// <returns>Returns nothing</returns> 
    FDOKGMSS_API void SetConnectionTimeout (FdoInt32 value);

    /// <summary>Opens a feature connection with the settings specified by the
    /// ConnectionString attribute of the provider-specific feature connection
    /// object.</summary>
    /// <returns>Returns nothing</returns> 
    FDOKGMSS_API FdoConnectionState Open ();

    /// <summary>Closes the connection to the DataStore</summary>
    /// <returns>Returns nothing</returns> 
    FDOKGMSS_API void Close ();

    /// <summary>Begins a transaction and returns an object that realizes
    /// FdoITransaction.</summary>
    /// <returns>Returns the transaction</returns> 
    FdoITransaction* BeginTransaction ();

    /// <summary>Creates and returns the specified type of command object associated with
    /// the connection.</summary>
    /// <param name="commandType">Input the command type to be created</param> 
    /// <returns>Returns the command</returns> 
    FdoICommand* CreateCommand (FdoInt32 commandType);

    /// <summary>Factory function that creates an empty Schema Override set specific
    /// to this FDO Provider.</summary>
    /// <returns>Returns FdoPhysicalSchemaMapping</returns> 
    FdoPhysicalSchemaMapping* CreateSchemaMapping();

    /// <summary>Sets the XML configuration stream used to configure the Data Store. 
    /// SetConfiguration can only be called while the connection is closed.</summary>
    /// <param name="configStream">Input the XML configuration stream</param> 
    /// <returns>Returns nothing</returns> 
    void SetConfiguration(FdoIoStream* configStream);

    #ifdef _FDO_3_2
    /// \brief
    /// Forces the writes of any cached data to the targed datastore.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void Flush();
    #endif

////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////
    
    c_KgMssSchemaDesc* GetSchemaDesc();
	  c_KgMssSpatialContextCollection* GetSpatialContexts(bool bDynamic = false);
	  
	  
	  long OTL_ExecuteSql(const char*Sql);
	  otl_stream* OTL_CreateStream(int BuffSize,const char* SqlStm,const int implicit_select=otl_explicit_select);
	  void OTL_DeleteStream(otl_stream*& Stream);
	  void OTL_OpenStream(otl_stream*& Stream,int BuffSize,const char* SqlStm,const int implicit_select=otl_explicit_select);
	  void OTL_SetCommit(otl_stream* Stream,int Commit);
	  void OTL_FlushStream(otl_stream* Stream);
	  long OTL_GetRpc(otl_stream* Stream);
	  void OTL_Commit();
	  
	  //oracle::occi::Statement* OCCI_CreateStatement();	  
	  //void c_KgMssConnection::OCCI_Commit();
	  //void OCCI_TerminateStatement(oracle::occi::Statement* Statement);
    //oracle::occi::Connection * GetOcciConnection() { return m_OcciConnection; }  ;	
    
    bool GetOracleSridDesc(FdoGeometricPropertyDefinition* GeomProp,c_KgMssSridDesc& OraSridDesc);
    bool GetOracleSridDesc(FdoClassDefinition* ClassDef,c_KgMssSridDesc& OraSrid);
    
    int GetOracleMainVersion() { return m_OracleMainVersion; };
	  int GetOracleSubVersion() { return m_OracleSubVersion; }

	
	
	
	
	#ifdef _DEBUG
	  void TestArrayFetch(FdoIdentifier* ClassId, FdoFilter* Filter, FdoIdentifierCollection* Props);
	#endif


	
};

#endif 
