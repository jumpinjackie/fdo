/*
* Copyright (C) 2006  SL-King d.o.o
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


#ifndef _c_KgOraConnection_h
#define _c_KgOraConnection_h

#ifdef _WIN32
#pragma once
#endif // _WIN32


#include "c_KgKmlFdoData.h"


using namespace std;



class c_KgKmlConnection : public FdoIConnection
{
    friend class c_KgKmlSelectCommand;
    friend class KgOraGetSpatialContextsCommand;
    friend class c_KgOraFeatureReader;

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
    //oracle::occi::Environment *m_OcciEnvironment;
    
public:    
    string m_OraConnectionUserName; // username used to open connection to oracle
    string m_OraConnectionPassword; // passwor
    string m_OraConnectionDbLink; // db link
    string m_OraSchemaName; // if not empty than provider will display classes just from this oracle schema
                              // if empty that will display classes from all schemas
                              
    string m_FdoViewsTable;
                                  
    int m_OracleMainVersion;
	  int m_OracleSubVersion;
	  
	  
	  
protected:   

  int m_ConnNo;
	
	//FdoPtr<c_KgKmlSchemaDesc> m_SchemaDesc;
	
	FdoPtr<c_KgKmlFdoData> m_KmlFdoData;
	

    
	

   


	/**
	 * Globals to keep track of opened connections 
	 */
	static	FdoCommonThreadMutex g_Mutex;
	static	int g_GlobalRefCount;



public:
    c_KgKmlConnection (void);
    virtual ~c_KgKmlConnection(void);

    // GisIDisposable interface

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    void Dispose ();

    // FdoIConnection overrides

    /// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
    /// of the connection.</summary>
    /// <returns>Returns the connection capabilities</returns> 
    FDOKGKML_API FdoIConnectionCapabilities* GetConnectionCapabilities ();

    /// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
    /// <returns>Returns schema capabilities</returns> 
    FDOKGKML_API FdoISchemaCapabilities* GetSchemaCapabilities ();

    /// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
    /// <returns>Returns the command capabilities</returns> 
    FDOKGKML_API FdoICommandCapabilities* GetCommandCapabilities ();

    /// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
    /// <returns>Returns the filter capabilities</returns> 
    FDOKGKML_API FdoIFilterCapabilities* GetFilterCapabilities ();

    /// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
    /// <returns>Returns the expression capabilities</returns> 
    FDOKGKML_API FdoIExpressionCapabilities* GetExpressionCapabilities ();

    /// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
    /// <returns>Returns the raster capabilities</returns> 
    FDOKGKML_API FdoIRasterCapabilities* GetRasterCapabilities ();

    /// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
    /// <returns>Returns the topology capabilities</returns> 
    FDOKGKML_API FdoITopologyCapabilities* GetTopologyCapabilities ();

    /// <summary>Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.</summary>
    /// <returns>Returns the geometry capabilities</returns> 
    FDOKGKML_API virtual FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// <summary>Gets the connection string used to open a DataStore.</summary>
    /// <returns>Returns the connection string</returns> 
    FDOKGKML_API FdoString* GetConnectionString ();

    /// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
    /// connection is closed.</summary>
    /// <param name="value">Input the connection string</param> 
    /// <returns>Returns nothing</returns> 
    FDOKGKML_API void SetConnectionString (FdoString* value);

    /// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
    /// <returns>Returns the connection info</returns> 
    FDOKGKML_API FdoIConnectionInfo* GetConnectionInfo ();

    /// <summary>Gets the current state of the connection.</summary>
    /// <returns>Returns the current state of the connection</returns> 
    FDOKGKML_API FdoConnectionState GetConnectionState ();

    /// <summary>Gets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability 0 will be returned</summary>
    /// <returns>Returns the time to wait (in milliseconds)</returns> 
    FDOKGKML_API FdoInt32 GetConnectionTimeout ();

    /// <summary>Sets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability then attempting to
    /// set a timeout will result in an exception</summary>
    /// <param name="value">Input the time to wait (in milliseconds)</param> 
    /// <returns>Returns nothing</returns> 
    FDOKGKML_API void SetConnectionTimeout (FdoInt32 value);

    /// <summary>Opens a feature connection with the settings specified by the
    /// ConnectionString attribute of the provider-specific feature connection
    /// object.</summary>
    /// <returns>Returns nothing</returns> 
    FDOKGKML_API FdoConnectionState Open ();

    /// <summary>Closes the connection to the DataStore</summary>
    /// <returns>Returns nothing</returns> 
    FDOKGKML_API void Close ();

    /// <summary>Begins a transaction and returns an object that realizes
    /// FdoITransaction.</summary>
    /// <returns>Returns the transaction</returns> 
    FdoITransaction* BeginTransaction ();

    /// <summary>Creates and returns the specified type of command object associated with
    /// the connection.</summary>
    /// <param name="commandType">Input the command type to be created</param> 
    /// <returns>Returns the command</returns> 
    FDOKGKML_API virtual FdoICommand* CreateCommand (FdoInt32 commandType);

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
    FDOKGKML_API c_KgKmlFdoData* GetKmlFdoData();
    c_KgKmlSchemaDesc* GetSchemaDesc();
	  c_KgKmlSpatialContextCollection* GetSpatialContexts(bool bDynamic = false);
	  
	  /*
	  oracle::occi::Statement* OCCI_CreateStatement();
	  void OCCI_Commit();
	  void OCCI_Rollback();
	  
	  void OCCI_TerminateStatement(oracle::occi::Statement* Statement);
    oracle::occi::Connection * GetOcciConnection() { return m_OcciConnection; }  ;	
    oracle::occi::Environment * GetOcciEnvironment() { return m_OcciEnvironment; }  ;	
    */
    bool GetOracleSridDesc(FdoGeometricPropertyDefinition* GeomProp,c_KgKmlSridDesc& OraSridDesc);
    bool GetOracleSridDesc(FdoClassDefinition* ClassDef,c_KgKmlSridDesc& OraSrid);
    
    int GetOracleMainVersion() { return m_OracleMainVersion; };
	  int GetOracleSubVersion() { return m_OracleSubVersion; }

	
	
	//FdoIFeatureReader* Select(FdoIdentifier* ClassId, FdoFilter* Filter, FdoIdentifierCollection* Props);
	//void ApplyFilter(FdoFilter* Filter,string& WhereBuff);
	
	//FdoIDataReader* SelectAggregates(FdoIdentifier* ClassId, FdoFilter* Filter, FdoIdentifierCollection* Props);
	
	#ifdef _DEBUG
	  void TestArrayFetch(FdoIdentifier* ClassId, FdoFilter* Filter, FdoIdentifierCollection* Props);
    

	#endif


  static void ReadConfigXML();
	
};

#endif 
