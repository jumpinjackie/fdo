/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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

#ifndef FDOWMSCONNECTION_H
#define FDOWMSCONNECTION_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoWmsServiceMetadata.h"
#include <Fdo/Schema/FeatureSchemaCollection.h>
#include <WMS/Override/FdoWmsOvPhysicalSchemaMapping.h>

// Class Declarations
class FdoWmsConnection : public FdoIConnection
{
public:
    // <summary>Default Contructor.</summary>
    // <returns>Returns nothing</returns> 
    FdoWmsConnection (void);

protected:
    // <summary>Virtual Destructor.</summary>
    // <returns>Returns nothing</returns> 
    virtual ~FdoWmsConnection(void);

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    void Dispose (void);

public:
    /// <summary>Gets all the supported Styles for sepcific layer.</summary>
    /// <returns>Returns string collection of all supported style names.</returns> 
    FdoStringCollection* GetSupportedStyles(FdoString* layerName);
 
    /// <summary>Gets all the supported CRS names for sepcific layer.</summary>
    /// <returns>Returns string collection of all supported CRS names.</returns> 
    FdoStringCollection* GetSupportedCRSNames(FdoString* layerName);

    /// <summary>Gets all the supported image formats from WMS server.</summary>
    /// <returns>Returns string collection of all supported image formats.</returns> 
    FdoStringCollection* GetSupportedImageFormats();

    /// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
    /// of the connection.</summary>
    /// <returns>Returns the connection capabilities</returns> 
    FdoIConnectionCapabilities* GetConnectionCapabilities ();

    /// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
    /// <returns>Returns schema capabilities</returns> 
    FdoISchemaCapabilities* GetSchemaCapabilities ();

    /// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
    /// <returns>Returns the command capabilities</returns> 
    FdoICommandCapabilities* GetCommandCapabilities ();

    /// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
    /// <returns>Returns the filter capabilities</returns> 
    FdoIFilterCapabilities* GetFilterCapabilities ();

    /// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
    /// <returns>Returns the expression capabilities</returns> 
    FdoIExpressionCapabilities* GetExpressionCapabilities ();

    /// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
    /// <returns>Returns the raster capabilities</returns> 
    FdoIRasterCapabilities* GetRasterCapabilities ();

    /// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
    /// <returns>Returns the topology capabilities</returns> 
    FdoITopologyCapabilities* GetTopologyCapabilities ();

    /// <summary>Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.</summary>
    /// <returns>Returns the geometry capabilities</returns> 
    virtual FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// <summary>Gets the connection string used to open a DataStore.</summary>
    /// <returns>Returns the connection string</returns> 
    FdoString* GetConnectionString ();

    /// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
    /// connection is closed.</summary>
    /// <param name="value">Input the connection string</param> 
    /// <returns>Returns nothing</returns> 
    void SetConnectionString (FdoString* value);

    /// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
    /// <returns>Returns the connection info</returns> 
    FdoIConnectionInfo* GetConnectionInfo ();

    /// <summary>Gets the current state of the connection.</summary>
    /// <returns>Returns the current state of the connection</returns> 
    FdoConnectionState GetConnectionState ();

    /// <summary>Gets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability 0 will be returned</summary>
    /// <returns>Returns the time to wait (in milliseconds)</returns> 
    FdoInt32 GetConnectionTimeout ();

    /// <summary>Sets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability then attempting to
    /// set a timeout will result in an exception</summary>
    /// <param name="value">Input the time to wait (in milliseconds)</param> 
    /// <returns>Returns nothing</returns> 
    void SetConnectionTimeout (FdoInt32 value);

    /// <summary>Opens a feature connection with the settings specified by the
    /// ConnectionString attribute of the provider-specific feature connection
    /// object.</summary>
    /// <returns>Returns nothing</returns> 
    FdoConnectionState Open ();

    /// <summary>Closes the connection to the DataStore</summary>
    /// <returns>Returns nothing</returns> 
    void Close ();

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

    /// \brief
    /// Forces the writes of any cached data to the targed datastore.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void Flush() {}

public:
    /// <summary>Gets the name of the WMS server set through the connection string.</summary>
    /// <returns>Returns the name of the WMS Server</returns> 
    FdoString* GetWmsServerName();

    /// <summary>Gets the WMS Metadata.</summary>
    /// <returns>Returns the ServiceMetadata interface.</returns> 
    FdoWmsServiceMetadata* GetWmsServiceMetadata();

    /// <summary>Determines if the connection has been configured from a configuration file.</summary>
    /// <returns>Returns the connections configured state.</returns> 
    FdoBoolean IsConfigured();

	/// <summary>Gets the physical schema mappings.</summary>
	/// <returns>Returns the physical schema mappins. Returns NULL if the connection is not configured.</returns>
	FdoPhysicalSchemaMappingCollection* GetSchemaMappings (FdoBoolean bGenerateDefault = false);

	/// <summary>Set the active spatial context.</summary>
	/// <returns>Returns nothing.</returns>
	void SetActiveSpatialContext (FdoString* value);
	
	/// <summary>Get the active spatial context.</summary>
	/// <returns>Returns the name of the active spatial context.</returns>
	FdoString* GetActiveSpatialContext ();

	/// <summary>Get the feature schemas of the connection.</summary>
	/// <returns>Returns the feature shcema collection.</returns>
	FdoFeatureSchemaCollection* GetSchemas ();

	/// <summary>Get the dictionary that contains the mappings between FDO applicable class names and WMS layer names.</summary>
	/// <returns>Returns the layer name dictionary.</returns>
	FdoDictionary* GetNamedLayerMappings ();

	/// <summary>Search the layer with the specified name in all the layers and child layers.</summary>
	/// <returns>Return the layer with the specified name. If not found, return NULL.</returns>
	FdoWmsLayer* FindLayer (FdoWmsLayerCollection* layers, FdoString* name);

	/// <summary>Search the class and it's parents for its raster property definition.</summary>
	/// <returns>Return the raster definition for the specified class. If not found, return NULL.</returns>
	FdoRasterPropertyDefinition* FindRasterProperty (FdoClassDefinition* featClass);

    /// <summary>If the user hasn't specified the image format using the configuration file, then the format 
    /// will be determined in the following order if the server supports: PNG, JPEG, TIFF and GIF. </summary>
    FdoString* GetDefaultImageFormat();

    // Gets the version from client request
    FdoStringP GetRequestWMSVersion(const wchar_t* pStr);

protected:
    /**
     * The connection string containing details about how to connect to FdoWms.
     */
    FdoStringP mConnectionString;

    /**
     * The active spatial context.
     */
    FdoStringP mActiveSpatialContext;

    /**
     * Connection information object.
     */
    FdoPtr<FdoIConnectionInfo> mConnectionInfo;

    /**
     * WMS Server
     */
    FdoStringP mServer;

	/**
	 * Connection state
	 */
	FdoConnectionState mState;

    /**
    * WMS Server Capabilities
    **/
    FdoWmsServiceMetadataP mWmsServiceMetadata; 

    /**
    * Configuration file elements (logical schema + schema mappings):
    **/
	FdoFeatureSchemasP mConfigLogicalSchemas;
	FdoSchemaMappingsP mConfigSchemaMappings;

    /**
     * Flag true when configuration file is specified.
     */
    FdoBoolean mConfigured;

	/**
	 * Cached feature schemas
	 */
	FdoFeatureSchemasP mSchemas;

	/**
	 * Cached class/layer name mapping
	 */
	FdoDictionaryP mLayerMappings;

private:
    // test
    void _processLayerStyles(FdoWmsLayer* layers, FdoStringCollection* styleNames);
    void _processLayerCRSNames(FdoWmsLayer* layer, FdoStringCollection* crsNames);

    // build up the class/layer name mapping
    void _buildUpClassLayerMappings (FdoWmsLayerCollection* layers);
    void _buildUpClassLayerMapping (FdoWmsLayer* layer);

	// build up the feature schemas
	void _buildUpDefaultSchemaMappings ();

	// helper function to create a feature class from a WMS layer and 
	// add the class into the collection
	void _addFeatureClass (FdoClassCollection* featClasses, FdoWmsLayer* layer, FdoClassDefinition* parent);

	// add properties "FeatId" and "Raster" to the class. Since "FeatId"
	// is identifier of all WMS layer classes, it will be added to
	// m_baseProperties and m_identityProperties; "Raster" is also read-only
	// as "FeatId", so it will be added to m_baseProperties as well.
	void _setProperties (FdoFeatureClass* featClass);

	// add base properties to the feature class explicitly.
	void _setBaseProperties (FdoFeatureClass* featClass, FdoClassDefinition* parent);

	// set the spatial context associated to the raster property of the class
	void _setDefaultSpatialContextAssociation (FdoClassDefinition* featClass);	

    // Creates a set of default physical schema mappings for the current connection
    void _buildUpDefaultPhysicalSchemaMappings();

};

#endif // FDOWMSCONNECTION_H
