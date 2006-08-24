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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpConnection.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPCONNECTION_H
#define FDORFPCONNECTION_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

//
//Class forward declarations
//
class FdoRfpSchemaData;
class FdoGrfpPhysicalSchemaMapping;
class FdoRfpClassData;
class FdoGrfpClassDefinition;
class FdoRfpRasterFileCollection;
class FdoRfpRasterImageCollection;
class FdoRfpSpatialContext;
class FdoRfpSpatialContextCollection;
class FdoRfpSchemaDataCollection;

//
// Implementation of FdoIConnection
//
class FdoRfpConnection :
    public FdoIConnection
{
//
// Data members
//
private:
    //Spatial contexts and active spatial context name
    FdoPtr<FdoRfpSpatialContextCollection>    m_spatialContexts;
    FdoStringP                            m_activeSpatialContext;

    //Logical feature schema
    FdoFeatureSchemasP                    m_featureSchemas;

    //Physical feature schema (Schema overrides)
    FdoSchemaMappingsP                    m_schemaMappings;

    //Schema data
    FdoPtr<FdoRfpSchemaDataCollection>        m_schemaDatas;

    //The connection string and the parameters contained in it.
    FdoStringP                            m_connectionString;
    FdoStringP                            m_defaultRasterLocation;

    // The connection state
    FdoConnectionState                    m_state;
    
    /**
     * Connection information object.
     */
    FdoPtr<FdoIConnectionInfo> mConnectionInfo;

//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
    virtual ~FdoRfpConnection(void);
    void Dispose() { delete this; }
public:
    FdoRfpConnection(void);

// 
// Exposed functions
//
public:
    /// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
    /// of the connection.</summary>
    /// <returns>Returns the connection capabilities</returns> 
    virtual FdoIConnectionCapabilities* GetConnectionCapabilities();

    /// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
    /// <returns>Returns schema capabilities</returns> 
    virtual FdoISchemaCapabilities* GetSchemaCapabilities();

    /// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
    /// <returns>Returns the command capabilities</returns> 
    virtual FdoICommandCapabilities* GetCommandCapabilities();

    /// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
    /// <returns>Returns the filter capabilities</returns> 
    virtual FdoIFilterCapabilities* GetFilterCapabilities();

    /// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
    /// <returns>Returns the expression capabilities</returns> 
    virtual FdoIExpressionCapabilities* GetExpressionCapabilities();

    /// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
    /// <returns>Returns the raster capabilities</returns> 
    virtual FdoIRasterCapabilities* GetRasterCapabilities();

    /// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
    /// <returns>Returns the topology capabilities</returns> 
    virtual FdoITopologyCapabilities* GetTopologyCapabilities();

    /// <summary>Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.</summary>
    /// <returns>Returns the geometry capabilities</returns> 
    virtual FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// <summary>Gets the connection string used to open a DataStore.</summary>
    /// <returns>Returns the connection string</returns> 
    virtual FdoString* GetConnectionString();

    /// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
    /// connection is closed.</summary>
    /// <param name="value">Input the connection string</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetConnectionString(FdoString* value);

    /// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
    /// <returns>Returns the connection info</returns> 
    virtual FdoIConnectionInfo* GetConnectionInfo();

    /// <summary>Gets the current state of the connection.</summary>
    /// <returns>Returns the current state of the connection</returns> 
    virtual FdoConnectionState GetConnectionState();

    /// <summary>Gets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability 0 will be returned</summary>
    /// <returns>Returns the time to wait (in milliseconds)</returns> 
    virtual FdoInt32 GetConnectionTimeout();

    /// <summary>Sets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability then attempting to
    /// set a timeout will result in an exception</summary>
    /// <param name="value">Input the time to wait (in milliseconds)</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetConnectionTimeout(FdoInt32 value);

    /// <summary>Opens a feature connection with the settings specified by the
    /// ConnectionString attribute of the provider-specific feature connection
    /// object.</summary>
    /// <returns>Returns nothing</returns> 
    virtual FdoConnectionState Open();

    /// <summary>Closes the connection to the DataStore</summary>
    /// <returns>Returns nothing</returns> 
    virtual void Close();

    /// <summary>Begins a transaction and returns an object that realizes
    /// FdoITransaction.</summary>
    /// <returns>Returns the transaction</returns> 
    virtual FdoITransaction* BeginTransaction();

    /// <summary>Creates and returns the specified type of command object associated with
    /// the connection.</summary>
    /// <param name="commandType">Input the command type to be created</param> 
    /// <returns>Returns the command</returns> 
    virtual FdoICommand* CreateCommand(FdoInt32 commandType);

    /// <summary>Factory function that creates an empty Schema Override set specific
    /// to this FDO Provider.</summary>
    /// <returns>Returns FdoPhysicalSchemaMapping</returns> 
    virtual FdoPhysicalSchemaMapping* CreateSchemaMapping();

    /// <summary>Sets the XML configuration information that will be used to 
    /// configure the FDO provider. SetConfigurationFile can only be called
    /// while the connection is closed.</summary>
    /// <param name="value">Input the IO Stream containing 
    /// the contents of the XML configuration. </param> 
    /// <returns>Returns nothing.</returns> 
    virtual void SetConfiguration (FdoIoStream* configStream);
    
//
// Non-exposed public methods
//
// Used by some commands
public:
    //Get the class definition by its qualified name
    void GetClassDef(const FdoPtr<FdoIdentifier>& identifier, FdoPtr<FdoClassDefinition>& classDef);

    //Get a reference to the feature schema collection
    FdoPtr<FdoFeatureSchemaCollection> GetFeatureSchemas();

    // Get a referrence to the schema mappings
    FdoPtr<FdoPhysicalSchemaMappingCollection> GetSchemaMappings();

    //Get class data of "FeatureName.ClassName"
    FdoPtr<FdoRfpClassData> GetClassData(const FdoPtr<FdoClassDefinition>&);

    //Get all spatial contexts
    FdoPtr<FdoRfpSpatialContextCollection> GetSpatialContexts();

    //Get active spatial context
    FdoPtr<FdoRfpSpatialContext> GetActiveSpatialContext();

    //Activate spatial context
    void ActivateSpatialContext(FdoString* contextName);

    // Create a spatial context
    void CreateSpatialContext(const FdoPtr<FdoRfpSpatialContext>&, bool bUpdateExist);

    // Destroy a spatial context
    void DestroySpatialContext(FdoString* contextName);

//
// Internally used helper functions
//
private:
    // Valiate that the connection is established. If not, throw out an exception.
    // It must be called at the beginning of some operations.
    void _validateOpen();

    // Valiate that the connection is not established. If not, throw out an exception.
    // It must be called at the beginning of some operations.
    void _validateClose();

    // Build up the default spatial contexts if they are unavailable in the configuration
    void _buildUpDefaultSpatialContext();

    // Build up the default feature schema if there is no configuation or the configuation contains
    // no feature schema.
    void _buildUpDefaultFeatureSchema();

    // Build up the default schema overrides according to the default feature schema
    void _buildUpDefaultOverrides();

    // According to the feature schema and schema overrides, build up the schema data
    void _buildUpSchemaDatas();

    // The classes contained in FDORFP feature schema can have and only have two properties.
    // One is the ID of the raster and the other is the raster itself. So after the schema
    // is built up, we must check its validation.
    void _validateFeatureSchema();

};

typedef FdoPtr<FdoRfpConnection> FdoRfpConnectionP;

#endif
