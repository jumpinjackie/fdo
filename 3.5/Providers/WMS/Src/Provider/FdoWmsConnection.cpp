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

#include "stdafx.h"

#include <wctype.h>
#include <malloc.h>
#include <string.h>
#include <gdal_priv.h>

#include "FdoWmsDelegate.h"
#include "FdoWmsGlobals.h"
#include "FdoWmsConnection.h"
#include "FdoWmsConnectionCapabilities.h"
#include "FdoWmsCommandCapabilities.h"
#include "FdoWmsExpressionCapabilities.h"
#include "FdoWmsFilterCapabilities.h"
#include "FdoWmsSchemaCapabilities.h"
#include "FdoWmsRasterCapabilities.h"
#include "FdoWmsTopologyCapabilities.h"
#include "FdoWmsGeometryCapabilities.h"
#include "FdoWmsConnectionInfo.h"
#include "FdoWmsDescribeSchemaCommand.h"
#include "FdoWmsDescribeSchemaMappingCommand.h"
#include "FdoWmsSelectCommand.h"
#include "FdoWmsGetSpatialContextsCommand.h"
#include "FdoWmsActivateSpatialContextCommand.h"
#include "FdoWmsSelectAggregatesCommand.h"
#include "FdoWmsGetImageFormats.h"
#include "FdoWmsGetFeatureClassStyles.h"
#include "FdoWmsGetFeatureClassCRSNames.h"
#include "FdoWmsUtils.h"
#include "FdoWmsXmlGlobals.h"
#include "FdoWmsRequestMetadata.h"
#include "WMS/FdoWmsCommandType.h"

#include <Fdo/Schema/FeatureClass.h>
#include <FdoCommonConnStringParser.h>

/// Class GDALRegister is used to register and unregister GDAL library.
/// The constructor calls GDALAllRegister() to register all the drivers to
/// GDAL and the destructor calls GDALDestroyDriverManager() to unregister
/// the drivers. The global instance "g_GDALRegister" is used to ensure that
/// GDAL is initialized before any GDAL related calls are issued.
class GDALRegister
{
public:
    // Default constructor registers all drivers to initialize GDAL.
    GDALRegister ()
    {
        GDALAllRegister();
    }

    // Destructor to unregister all drivers of GDAL.
    ~GDALRegister ()
    {
        GDALDestroyDriverManager();
    }
};

// The only instance to register GDAL.
GDALRegister g_GDALRegister;


// external access to connection for client services
extern "C" FDOWMS_API FdoIConnection* CreateConnection ()
{
    return (new FdoWmsConnection ());
}

FdoWmsConnection::FdoWmsConnection (void)
    : mState(FdoConnectionState_Closed), mConfigured (false)
{
}

FdoWmsConnection::~FdoWmsConnection (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void FdoWmsConnection::Dispose ()
{
    delete this;
}

/// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
/// of the connection.</summary>
/// <returns>Returns the connection capabilities</returns> 
FdoIConnectionCapabilities* FdoWmsConnection::GetConnectionCapabilities ()
{
    return (new FdoWmsConnectionCapabilities ());
}

/// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
/// <returns>Returns schema capabilities</returns> 
FdoISchemaCapabilities* FdoWmsConnection::GetSchemaCapabilities ()
{
    return (new FdoWmsSchemaCapabilities ());
}

/// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
/// <returns>Returns the command capabilities</returns> 
FdoICommandCapabilities* FdoWmsConnection::GetCommandCapabilities ()
{
    return (new FdoWmsCommandCapabilities ());
}

/// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
/// <returns>Returns the filter capabilities</returns> 
FdoIFilterCapabilities* FdoWmsConnection::GetFilterCapabilities ()
{
    return (new FdoWmsFilterCapabilities ());
}

/// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
/// <returns>Returns the expression capabilities</returns> 
FdoIExpressionCapabilities* FdoWmsConnection::GetExpressionCapabilities ()
{
    return (new FdoWmsExpressionCapabilities ());
}

/// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
/// <returns>Returns the raster capabilities</returns> 
FdoIRasterCapabilities* FdoWmsConnection::GetRasterCapabilities (void)
{
    return (new FdoWmsRasterCapabilities ());
}

/// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
/// <returns>Returns the topology capabilities</returns> 
FdoITopologyCapabilities* FdoWmsConnection::GetTopologyCapabilities ()
{
    return (new FdoWmsTopologyCapabilities ());
}


/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* FdoWmsConnection::GetGeometryCapabilities()
{
    return (new FdoWmsGeometryCapabilities());
}

/// <summary>Gets the connection string used to open a DataStore.</summary>
/// <returns>Returns the connection string</returns> 
FdoString* FdoWmsConnection::GetConnectionString ()
{
    return (mConnectionString);
}

/// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
/// connection is closed.</summary>
/// <param name="value">Input the connection string</param> 
/// <returns>Returns nothing</returns> 
void FdoWmsConnection::SetConnectionString (FdoString* value)
{
    FdoConnectionState state = GetConnectionState();
    if (state == FdoConnectionState_Closed || state == FdoConnectionState_Pending)
    {
        // Update the connection string:
        mConnectionString = value;

        // Update the connection property dictionary:
        FdoPtr<FdoIConnectionInfo> connInfo = GetConnectionInfo();
        FdoPtr<FdoCommonConnPropDictionary> connDict = static_cast<FdoCommonConnPropDictionary*>(connInfo->GetConnectionProperties());
        connDict->UpdateFromConnectionString(mConnectionString);
    }
    else 
    {
        throw FdoCommandException::Create (NlsMsgGet(FDOWMS_CONNECTION_ALREADY_OPEN, "The connection is already open."));
    }
}

/// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
/// <returns>Returns the connection info</returns> 
FdoIConnectionInfo* FdoWmsConnection::GetConnectionInfo ()
{
    if (mConnectionInfo == NULL) {
        mConnectionInfo = new FdoWmsConnectionInfo (this);
    }

    return FDO_SAFE_ADDREF(mConnectionInfo.p);
}

/// <summary>Gets the current state of the connection.</summary>
/// <returns>Returns the current state of the connection</returns> 
FdoConnectionState FdoWmsConnection::GetConnectionState ()
{
    return mState;
}

/// <summary>Gets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability 0 will be returned</summary>
/// <returns>Returns the time to wait (in milliseconds)</returns> 
FdoInt32 FdoWmsConnection::GetConnectionTimeout ()
{
    return (0);
}

/// <summary>Sets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability then attempting to
/// set a timeout will result in an exception</summary>
/// <param name="value">Input the time to wait (in milliseconds)</param> 
/// <returns>Returns nothing</returns> 
void FdoWmsConnection::SetConnectionTimeout (FdoInt32 value)
{
    throw FdoCommandException::Create (NlsMsgGet(FDOWMS_CONNECTION_TIMEOUT_UNSUPPORTED, "Connection timeout is not supported."));
}

FdoString* FdoWmsConnection::_getOriginalLayerName (FdoString* mangledLayerName)
{
    FdoPtr<FdoDictionary> nameMappings = GetNamedLayerMappings ();
    if (nameMappings != NULL)
    {
        // 1. the layer name is mangled
        FdoPtr<FdoDictionaryElement> nameMapping = nameMappings->FindItem (mangledLayerName);
        if (nameMapping != NULL)
        {
            return nameMapping->GetValue ();
        }

        // 2. the input layer name may be the original one, so we should also verify it.
        FdoInt32 cnt = nameMappings->GetCount ();
        for (int i=0; i<cnt; i++)
        {
            FdoPtr<FdoDictionaryElement> nameMapping = nameMappings->GetItem (i);
            FdoStringP originalName = nameMapping->GetValue ();
            if (wcscmp (originalName, mangledLayerName) == 0)
                return mangledLayerName;
        }
    }
    
    // if not found, just return empty string.
    return L"";
}

// should call after open
// Child return the style supportted by its parents
FdoStringCollection* FdoWmsConnection::GetSupportedStyles(FdoString* featureClass)
{
    FdoPtr<FdoWmsServiceMetadata> metadata = this->GetWmsServiceMetadata ();
    FdoPtr<FdoWmsCapabilities> capa = static_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());
    FdoWmsLayerCollectionP layers = capa->GetLayers();

    // Resolve FdoWmsCapabiites exposing multiple Root layers. According the WMS specification, only one root layer is allowed.
    // If there are layers retrievable from the WMS Server
    FdoStringsP styleNames = FdoStringCollection::Create();
    if (layers->GetCount() > 0) 
    {
        // NOTE: Currently, the default WMS provider configuration will create one featureclass refer to one layer
        FdoStringP layerName = _getOriginalLayerName(featureClass);
        // Find in all layers, including the ROOT
        FdoPtr<FdoWmsLayer> currentLayer = FindLayer(layers, layerName);
        if (currentLayer == NULL)
        {
            throw FdoException::Create (NlsMsgGet(FDOWMS_12001_LAYER_NOT_EXIST, 
            "The WMS layer '%1$ls' does not exist.", 
            (FdoString *) layerName));
        }
        _processLayerStyles(currentLayer, styleNames);
     
    }
    return FDO_SAFE_ADDREF(styleNames.p);
}


void FdoWmsConnection::_processLayerStyles(FdoWmsLayer* layer, FdoStringCollection* styleNames)
{
    FdoWmsStyleCollectionP styles = layer->GetStyles();
    for (int i = 0; i < styles->GetCount(); i++)
    {
        FdoWmsStyleP style = styles->GetItem(i);
        if ( styleNames->IndexOf (style->GetName()) == -1 )
		{
			styleNames->Add(style->GetName());
        }
    }

    // get the heritage style from parents
    FdoPtr<FdoWmsLayer> parentLayer = layer->GetParent();
    if (parentLayer != NULL)
    {
        _processLayerStyles(parentLayer, styleNames);
    }

}
// Should call after open
// Child return the CRS supportted by its parents
FdoStringCollection* FdoWmsConnection::GetSupportedCRSNames(FdoString* featureClass)
{
    //test for getting all styles: Maggie
    FdoPtr<FdoWmsServiceMetadata> metadata = this->GetWmsServiceMetadata ();
    FdoPtr<FdoWmsCapabilities> capa = static_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());
    FdoWmsLayerCollectionP layers = capa->GetLayers();
    
    // Resolve FdoWmsCapabiites exposing multiple Root layers. According the WMS specification, only one root layer is allowed.
    // If there are layers retrievable from the WMS Server
    FdoStringsP crsNames = FdoStringCollection::Create();
    if (layers->GetCount() > 0) 
    {
        // NOTE: Currently, the default WMS provider configuration will create one featureclass refer to one layer
        FdoStringP layerName = _getOriginalLayerName(featureClass);

        // Find in all layers, including the ROOT
        FdoPtr<FdoWmsLayer> currentLayer = FindLayer(layers, layerName);
        if (currentLayer == NULL)
        {
            throw FdoException::Create (NlsMsgGet(FDOWMS_12001_LAYER_NOT_EXIST, 
            "The WMS layer '%1$ls' does not exist.", 
            (FdoString *) layerName));
        }
        _processLayerCRSNames(currentLayer, crsNames, capa->GetCRSNames());

    }
    return FDO_SAFE_ADDREF(crsNames.p);
}

void FdoWmsConnection::_processLayerCRSNames(FdoWmsLayer* layer, FdoStringCollection* crsNames, FdoStringCollection* intersectCRSSet)
{
    FdoPtr<FdoWmsBoundingBoxCollection> boundingboxs = layer->GetBoundingBoxes();
    for (int i = 0; i < boundingboxs->GetCount(); i++)
    {
        FdoWmsBoundingBoxP box = boundingboxs->GetItem(i);
        FdoStringP crs = box->GetCRS();
        if ( ((intersectCRSSet == NULL) || (intersectCRSSet->IndexOf(crs) != -1))
            && (crsNames->IndexOf(crs) == -1) )
		{
            crsNames->Add(crs);
        }
    }
    
    // get the heritage CRS from parent
    FdoPtr<FdoWmsLayer> parentLayer = layer->GetParent();
    if (parentLayer != NULL)
    {
        _processLayerCRSNames(parentLayer, crsNames, intersectCRSSet);
    }

}

FdoStringCollection* FdoWmsConnection::GetSupportedImageFormats()
{
	// get all supported format
    FdoWmsServiceMetadataP metadata = GetWmsServiceMetadata ();
	FdoPtr<FdoWmsCapabilities> capa = static_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());
    FdoPtr<FdoOwsRequestMetadataCollection> reqMetadatas = capa->GetRequestMetadatas ();

    FdoPtr<FdoOwsRequestMetadata> reqMetadata = reqMetadatas->FindItem (FdoWmsXmlGlobals::WmsGetMapRequest);
    if (reqMetadata == NULL) {
        reqMetadata = reqMetadatas->FindItem (FdoWmsXmlGlobals::WmsGetMapRequest2);
    }
    if (reqMetadata == NULL) {
		throw FdoCommandException::Create (NlsMsgGet (FDOWMS_12005_GETMAP_NOT_SUPPORTED, "The WMS GetMap request is not supported."));
    }

    FdoWmsRequestMetadata* getMapMetadata = static_cast<FdoWmsRequestMetadata*>(reqMetadata.p);   
    FdoStringsP formats = getMapMetadata->GetFormats ();

    // retrive the image format only
    FdoStringsP imageFormats = FdoStringCollection::Create();
    for (int i = 0; i < formats->GetCount(); i++)
    {
        FdoStringP name = formats->GetString(i);
        if (name.Contains(FdoWmsGlobals::ImageFormatPrefix))
        {
            imageFormats->Add(name);
        }
    }
    return FDO_SAFE_ADDREF(imageFormats.p);
}
/// <summary>Opens a feature connection with the settings specified by the
/// ConnectionString attribute of the provider-specific feature connection
/// object.</summary>
/// <returns>Returns nothing</returns> 
FdoConnectionState FdoWmsConnection::Open ()
{
    if (GetConnectionState() == FdoConnectionState_Open)
        return FdoConnectionState_Open;
    
    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo ();
    FdoPtr<FdoCommonConnPropDictionary> dictionary = static_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());

    FdoStringP location = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyFeatureServer);
    if (0 == location.GetLength() && !mConfigured) 
    {
        throw FdoException::Create (NlsMsgGet(FDOWMS_CONNECTION_REQUIRED_PROPERTY_NULL, 
            "The required connection property '%1$ls' cannot be set to NULL.", 
            FdoWmsGlobals::ConnectionPropertyFeatureServer));
    }

    FdoStringP user = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyUsername);
    FdoStringP password = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyPassword);

    FdoCommonConnStringParser parser (NULL, GetConnectionString ());
    // check the validity of the connection string, i.e. it doesn’t contain unknown properties
    // e.g. DefaultFLocation instead of DefaultFileLocation
    if (!parser.IsConnStringValid())
        throw FdoException::Create (NlsMsgGet(FDOWMS_INVALID_CONNECTION_STRING, "Invalid connection string '%1$ls'", GetConnectionString ()));
    if(parser.HasInvalidProperties(dictionary.p))
        throw FdoException::Create (NlsMsgGet(FDOWMS_INVALID_CONNECTION_PROPERTY_NAME, "Invalid connection property name '%1$ls'", parser.GetFirstInvalidPropertyName(dictionary.p) ));

    mLayerMappings = FdoDictionary::Create();

    FdoStringP pVersion = GetRequestWMSVersion(location);
    FdoWmsDelegateP wmsDelegate = FdoWmsDelegate::Create(location, user, password);
    mWmsServiceMetadata = wmsDelegate->GetServiceMetadata(pVersion);
    if (!wcscmp(mWmsServiceMetadata->GetVersion(), L"1.0.0"))
    {
        Close ();
        throw FdoException::Create (NlsMsgGet(FDOWMS_VERSION_NOT_SUPPORTED, "The WMS version '%1$ls' is not supported.", L"1.0.0"));
    }
    FdoPtr<FdoWmsServiceMetadata> metadata = this->GetWmsServiceMetadata ();
    FdoPtr<FdoWmsCapabilities> capa = static_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());
    if (capa)
        capa->FillUpGeographicDataLayers();
    if (mConfigured)
    {
        // Some layer names may contain ":" or "." characters which are not allowed
        // in FDO class name, so the layer names will be mangled to replace
        // such characters with blank " ".
        if (mLayerMappings != NULL && mLayerMappings->GetCount () == 0)
        {
            if (capa != NULL)
            {
                FdoPtr<FdoWmsLayerCollection> layers = capa->GetLayers ();
                _buildUpClassLayerMappings (layers);
            }
        }
    }
    else
    {
        _buildUpDefaultSchemaMappings ();
        _buildUpDefaultPhysicalSchemaMappings ();
    }

    mServer = location;
    mState = FdoConnectionState_Open;

    return mState;
}

/// <summary>Closes the connection to the DataStore</summary>
/// <returns>Returns nothing</returns> 
void FdoWmsConnection::Close ()
{
    // reset the WMS server name
    mServer = L"";
    mActiveSpatialContext = "";

    // Clear the config file cache:
    mWmsServiceMetadata = NULL;
    mConfigLogicalSchemas = NULL;
    mConfigSchemaMappings = NULL;
    mLayerMappings = NULL;
    
    // mark the connection as not configured
    mConfigured = false;

    // Clear the cached schemas
    mSchemas = NULL;

    // change the state to FdoConnectionState_Closed
    mState = FdoConnectionState_Closed;
}

/// <summary>Begins a transaction and returns an object that realizes
/// FdoITransaction.</summary>
/// <returns>Returns the transaction</returns> 
FdoITransaction* FdoWmsConnection::BeginTransaction ()
{
    throw FdoCommandException::Create(NlsMsgGet(FDOWMS_CONNECTION_TRANSACTIONS_NOT_SUPPORTED, "FDO WMS Provider does not support transactions."));
}

/// <summary>Creates and returns the specified type of command object associated with
/// the connection.</summary>
/// <param name="commandType">Input the command type to be created</param> 
/// <returns>Returns the command</returns> 
FdoICommand* FdoWmsConnection::CreateCommand (FdoInt32 commandType)
{
    FdoPtr<FdoICommand> ret;

    switch (commandType)
    {
        case FdoCommandType_Select:
            ret = new FdoWmsSelectCommand (this);
            break;
        case FdoCommandType_DescribeSchema:
            ret = new FdoWmsDescribeSchemaCommand (this);
            break;
		case FdoCommandType_DescribeSchemaMapping:
			ret = new FdoWmsDescribeSchemaMappingCommand (this);
			break;
		case FdoCommandType_GetSpatialContexts:
			ret = new FdoWmsGetSpatialContextsCommand (this);
			break;
		case FdoCommandType_ActivateSpatialContext:
			ret = new FdoWmsActivateSpatialContextCommand (this);
			break;
        case FdoCommandType_SelectAggregates:
            ret = new FdoWmsSelectAggregatesCommand(this);
            break;
        case FdoWmsCommandType_GetImageFormats:
            ret = new FdoWmsGetImageFormats(this);
            break;
        case FdoWmsCommandType_GetFeatureClassStyles:
            ret = new FdoWmsGetFeatureClassStyles(this);
            break;
        case FdoWmsCommandType_GetFeatureClassCRSNames:
            ret = new FdoWmsGetFeatureClassCRSNames(this);
            break;
        default:
            throw FdoCommandException::Create (NlsMsgGet(FDOWMS_CONNECTION_COMMAND_NOT_SUPPORTED, "The command %1$d is not supported.", (int)commandType));
    }

    return (FDO_SAFE_ADDREF (ret.p));
}


/// <summary>Factory function that creates an empty Schema Override set specific
/// to this FDO Provider.</summary>
/// <returns>Returns FdoPhysicalSchemaMapping</returns> 
FdoPhysicalSchemaMapping* FdoWmsConnection::CreateSchemaMapping()
{
    return FdoWmsOvPhysicalSchemaMapping::Create();
}

/// <summary>Sets the XML configuration stream used to configure the Data Store. 
/// SetConfiguration can only be called while the connection is closed.</summary>
/// <param name="configStream">Input the XML configuration stream</param> 
/// <returns>Returns nothing</returns> 
void FdoWmsConnection::SetConfiguration(FdoIoStream* configStream)
{
    VALIDATE_ARGUMENT(configStream);

    if (mState != FdoConnectionState_Closed) {
        throw FdoConnectionException::Create(NlsMsgGet(FDOWMS_CONNECTION_ALREADY_OPEN, "The connection is already open."));
    }

    FDO_SAFE_RELEASE(mConfigLogicalSchemas.p);
    FDO_SAFE_RELEASE(mConfigSchemaMappings.p);

    //////////////////////////////////////////////////////////////////////////
    // Read logical FDO schemas from XML stream:
    //////////////////////////////////////////////////////////////////////////

    configStream->Reset();
    FdoXmlReaderP reader = FdoXmlReader::Create(configStream);
    mConfigLogicalSchemas = FdoFeatureSchemaCollection::Create(NULL);

    try 
    {
        mConfigLogicalSchemas->ReadXml(reader);
    } 
    catch (FdoException* e) 
    {
        FdoSchemaExceptionP schemaException = 
            FdoSchemaException::Create(
                NlsMsgGet(FDOWMS_SCHEMA_FAIL_READ_FEATURE_SCHEMAS, "Failed to de-serialize the feature schemas from the configuration."),
                e);
        e->Release();
        throw schemaException;
    }


    //////////////////////////////////////////////////////////////////////////
    // Read FDO schema mappings from XML stream:
    //////////////////////////////////////////////////////////////////////////

    configStream->Reset();
    reader = FdoXmlReader::Create(configStream);
    FdoSchemaMappingsP localSchemaMappings = FdoPhysicalSchemaMappingCollection::Create();
    
    try 
    {
        localSchemaMappings->ReadXml(reader);
    } 
    catch (FdoException* e) 
    {
        FdoSchemaExceptionP schemaException = 
            FdoSchemaException::Create(
                NlsMsgGet(FDOWMS_SCHEMA_FAIL_READ_SCHEMA_MAPPINGS, "Failed to de-serialize the schema mappings from the configuration."),
                e);
        e->Release();
        throw schemaException;
    }

    mConfigSchemaMappings = FdoPhysicalSchemaMappingCollection::Create();
    for (FdoInt32 i=0; i<localSchemaMappings->GetCount(); i++) {
        FdoPhysicalSchemaMappingP schemaMapping = localSchemaMappings->GetItem(i);
        FdoWmsOvPhysicalSchemaMapping* wmsSchemaMapping = static_cast<FdoWmsOvPhysicalSchemaMapping*>(schemaMapping.p);
        if (wmsSchemaMapping != NULL) {
            mConfigSchemaMappings->Add(wmsSchemaMapping);
        }
    }

    // verify that each logical shema/class has its associated physical schema/class mapping
    for (FdoInt32 i=0; i<mConfigLogicalSchemas->GetCount(); i++)
    {
        FdoPtr<FdoFeatureSchema> logicalSchema = mConfigLogicalSchemas->GetItem (i);
        FdoString* schemaName = logicalSchema->GetName ();
        FdoPtr<FdoPhysicalSchemaMapping> schemaMapping = mConfigSchemaMappings->GetItem (this, schemaName);
        if (schemaMapping == NULL)
            throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_SCHEMA_MAPPING_NOT_FOUND, "Physical schema mapping '%1$ls' was not found.", schemaName));

        FdoWmsOvPhysicalSchemaMapping* wmsSchemaMapping = static_cast<FdoWmsOvPhysicalSchemaMapping *> (schemaMapping.p);
        FdoWmsOvClassesP physicalClasses = wmsSchemaMapping->GetClasses ();

        FdoPtr<FdoClassCollection> logicalClasses = logicalSchema->GetClasses ();
        for (FdoInt32 j=0; j<logicalClasses->GetCount(); j++)
        {
            FdoPtr<FdoClassDefinition> logicalClass = logicalClasses->GetItem (j);
            FdoString* className = logicalClass->GetName ();
            FdoWmsOvClassDefinitionP physicalClass = physicalClasses->FindItem (className);
            if (physicalClass == NULL)
                throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_PHYSICAL_CLASS_NOT_FOUND, "Physical class definition '%1$ls' was not found.", className));
        }
    }

    // verify that each physical schema/class has its associated logical schema/class
    for (FdoInt32 i=0; i<mConfigSchemaMappings->GetCount(); i++)
    {
        FdoPtr<FdoPhysicalSchemaMapping> schemaMapping = mConfigSchemaMappings->GetItem (i);
        FdoWmsOvPhysicalSchemaMapping* physicalSchema = static_cast<FdoWmsOvPhysicalSchemaMapping *> (schemaMapping.p);
        FdoString* schemaName = physicalSchema->GetName ();
        FdoPtr<FdoFeatureSchema> logicalSchema = mConfigLogicalSchemas->FindItem (schemaName);
        if (logicalSchema == NULL)
            throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_LOGICAL_SCHEMA_NOT_FOUND, "Physical schema mapping '%1$ls' has no associated logical schema.", schemaName));

        FdoPtr<FdoClassCollection> logicalClasses = logicalSchema->GetClasses ();
        FdoWmsOvClassesP physicalClasses = physicalSchema->GetClasses ();
        for (FdoInt32 j=0; j<physicalClasses->GetCount(); j++)
        {
            FdoWmsOvClassDefinitionP physicalClass = physicalClasses->GetItem (j);
            FdoString* className = physicalClass->GetName ();
            FdoPtr<FdoClassDefinition> logicalClass = logicalClasses->FindItem (className);
            if (logicalClass == NULL)
                throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_LOGICAL_CLASS_NOT_FOUND, "Physical class '%1$ls' has no associated logical class definition.", className));
        }
    }

    // Set spatial context association for raster properties
    for (FdoInt32 i=0; i<mConfigLogicalSchemas->GetCount (); i++)
    {
        FdoFeatureSchemaP featSchema = mConfigLogicalSchemas->GetItem (i);
        FdoWmsOvPhysicalSchemaMappingP schemaMapping = static_cast<FdoWmsOvPhysicalSchemaMapping *> (mConfigSchemaMappings->GetItem (this, featSchema->GetName ()));
        FdoPtr<FdoClassCollection> featClasses = featSchema->GetClasses ();
        FdoPtr<FdoWmsOvClassCollection> classMappings = schemaMapping->GetClasses ();
        for (FdoInt32 j=0; j<featClasses->GetCount(); j++)
        {
            FdoPtr<FdoClassDefinition> featClass = featClasses->GetItem (j);
            FdoPtr<FdoWmsOvClassDefinition> classMapping = classMappings->GetItem (featClass->GetName ());
            FdoPtr<FdoWmsOvRasterDefinition> rasterDefinition = classMapping->GetRasterDefinition ();
            FdoString* spatialContext = rasterDefinition->GetSpatialContextName ();
            VALIDATE_ARGUMENT (spatialContext);

            // Get the raster property of the class
            FdoPtr<FdoPropertyDefinitionCollection> props = featClass->GetProperties ();
            FdoString* rasterPropName = rasterDefinition->GetName ();
            FdoPtr<FdoPropertyDefinition> prop = props->FindItem (rasterPropName);
            if (prop == NULL)
                throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_PHYSICAL_RASTER_PROPERTY_NOT_FOUND, "Logical raster property '%1$ls' was not mapped to a physical property.", rasterPropName));

            if (prop->GetPropertyType () != FdoPropertyType_RasterProperty)
                throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_PROPERTY_NOT_RASTER_PROPERTY, "Property '%1ls' is not a raster property.", rasterPropName));

            FdoRasterPropertyDefinition* rasterProp = static_cast<FdoRasterPropertyDefinition *> (prop.p);
            rasterProp->SetSpatialContextAssociation (spatialContext);            
        }
    }

    mConfigured = true;
}

/// <summary>Gets the name of the WMS server set through the connection string.</summary>
/// <returns>Returns the name of the WMS Server</returns> 
FdoString* FdoWmsConnection::GetWmsServerName()
{
    return mServer;
}

/// <summary>Gets the WMS Metadata.</summary>
/// <returns>Returns the ServiceMetadata interface.</returns> 
FdoWmsServiceMetadata* FdoWmsConnection::GetWmsServiceMetadata()
{
    return FDO_SAFE_ADDREF(mWmsServiceMetadata.p);
}

/// <summary>Determines if the connection has been configured from a configuration file.</summary>
/// <returns>Returns the connections configured state.</returns> 
FdoBoolean FdoWmsConnection::IsConfigured ()
{
    return (mConfigured);
}

/// <summary>Set the active spatial context.</summary>
/// <returns>Returns nothing.</returns>
void FdoWmsConnection::SetActiveSpatialContext (FdoString* value)
{
    mActiveSpatialContext = value;
}

/// <summary>Get the active spatial context.</summary>
/// <returns>Returns the name of the active spatial context.</returns>
FdoString* FdoWmsConnection::GetActiveSpatialContext ()
{
    return mActiveSpatialContext;
}

/// <summary>Get the feature schemas of the connection.</summary>
/// <returns>Returns the feature shcema collection.</returns>
FdoFeatureSchemaCollection* FdoWmsConnection::GetSchemas ()
{
    if (mConfigured)
        return FDO_SAFE_ADDREF (mConfigLogicalSchemas.p);
    else
        return FDO_SAFE_ADDREF (mSchemas.p);
}

/// <summary>Gets the physical schema mappings.</summary>
/// <returns>Returns the physical schema mappins. Returns NULL if the connection is not configured.</returns>
FdoPhysicalSchemaMappingCollection* FdoWmsConnection::GetSchemaMappings (FdoBoolean bGenerateDefault)
{
    if (mConfigured || bGenerateDefault) 
        return FDO_SAFE_ADDREF (mConfigSchemaMappings.p);
    else
        return NULL;
}

/// <summary>Creates a set of default physical schema mappings for the current connection.</summary>
/// <returns>Returns the physical schema mappins. Returns NULL if the connection is not configured.</returns>
void FdoWmsConnection::_buildUpDefaultPhysicalSchemaMappings()
{
    mConfigSchemaMappings = FdoPhysicalSchemaMappingCollection::Create();
    for (FdoInt32 i=0; i<mSchemas->GetCount(); i++)
    {
        FdoFeatureSchemaP featureSchema = mSchemas->GetItem(i);
        FdoWmsOvPhysicalSchemaMappingP configMapping = FdoWmsOvPhysicalSchemaMapping::Create();
        configMapping->SetName(featureSchema->GetName());

        FdoWmsOvClassesP configClasses = configMapping->GetClasses();
        FdoClassesP featureClasses = featureSchema->GetClasses();
        for (FdoInt32 j=0; j<featureClasses->GetCount(); j++)
        {
            FdoClassDefinitionP featureClassDefn = featureClasses->GetItem(j);
            if (!featureClassDefn->GetIsAbstract()) 
            {
                FdoWmsOvClassDefinitionP configClassDefn = FdoWmsOvClassDefinition::Create();
                configClassDefn->SetName(featureClassDefn->GetName());

                FdoWmsOvRasterDefinitionP configRasterDefn = FdoWmsOvRasterDefinition::Create();
                FdoRasterPropertyP featureRasterDefn = FindRasterProperty(featureClassDefn);

                configRasterDefn->SetName(featureRasterDefn->GetName());

                configRasterDefn->SetImageFormat(GetDefaultImageFormat()); 

                configRasterDefn->SetBackgroundColor(L"0xFFFFFF");

                FdoString* scName = featureRasterDefn->GetSpatialContextAssociation();
                configRasterDefn->SetSpatialContextName(scName);

                FdoWmsOvLayersP configLayersCollection = configRasterDefn->GetLayers();
                FdoWmsOvLayerDefinitionP configLayerDefn = FdoWmsOvLayerDefinition::Create();

                FdoPtr<FdoDictionaryElement> nameElement = mLayerMappings->GetItem (configClassDefn->GetName());
                FdoString* layerName = nameElement->GetValue ();
                configLayerDefn->SetName(layerName);

                configLayerDefn->SetStyle(NULL);
                configLayersCollection->Add(configLayerDefn);
        
                configClassDefn->SetRasterDefinition(configRasterDefn);
                configClasses->Add(configClassDefn);
            }
        }

        mConfigSchemaMappings->Add(configMapping);
    }
}

/// <summary>If the user hasn't specified the image format using the configuration file, then the format 
/// will be determined in the following order if the server supports: PNG, JPEG, TIFF and GIF. </summary>
FdoString* FdoWmsConnection::GetDefaultImageFormat()
{
    FdoString* imageFormat = FdoWmsGlobals::RasterMIMEFormat_PNG;

    FdoWmsServiceMetadataP metadata = GetWmsServiceMetadata ();
    FdoPtr<FdoWmsCapabilities> capa = static_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());
    FdoPtr<FdoOwsRequestMetadataCollection> reqMetadatas = capa->GetRequestMetadatas ();

    FdoPtr<FdoOwsRequestMetadata> reqMetadata = reqMetadatas->FindItem (FdoWmsXmlGlobals::WmsGetMapRequest);
    if (reqMetadata == NULL) {
        reqMetadata = reqMetadatas->FindItem (FdoWmsXmlGlobals::WmsGetMapRequest2);
    }
    if (reqMetadata == NULL) {
        throw FdoCommandException::Create (NlsMsgGet (FDOWMS_12005_GETMAP_NOT_SUPPORTED, "The WMS GetMap request is not supported."));
    }

    FdoWmsRequestMetadata* getMapMetadata = static_cast<FdoWmsRequestMetadata*>(reqMetadata.p);
    FdoStringsP imageFormats = getMapMetadata->GetFormats ();

    // Find the most suitable image format which the server supports. If the user doesn't use
    // configuration file, the image format will be determined as following:
    //    "PNG" is prefered is it's supported; Otherwise "TIFF"; Otherwise "JPEG"; Otherwise "GIF"        

    if (imageFormats->IndexOf (FdoWmsGlobals::RasterMIMEFormat_PNG) != -1)
        imageFormat = FdoWmsGlobals::RasterMIMEFormat_PNG;
    else if (imageFormats->IndexOf (FdoWmsGlobals::RasterMIMEFormat_TIFF) != -1)
        imageFormat = FdoWmsGlobals::RasterMIMEFormat_TIFF;
    else if (imageFormats->IndexOf (FdoWmsGlobals::RasterMIMEFormat_JPEG) != -1)
        imageFormat = FdoWmsGlobals::RasterMIMEFormat_JPEG;
    else if (imageFormats->IndexOf (FdoWmsGlobals::RasterMIMEFormat_GIF) != -1)
        imageFormat = FdoWmsGlobals::RasterMIMEFormat_GIF;

    return imageFormat;
}

// build up the feature schemas
void FdoWmsConnection::_buildUpDefaultSchemaMappings ()
{
    //
    // This method builds up the default WMS schema that is used when no WMS configuration file is present.
    //
    if (!mSchemas)
    {
        FdoPtr<FdoWmsCapabilities> capa = static_cast<FdoWmsCapabilities *>(mWmsServiceMetadata->GetCapabilities ());
        mSchemas = FdoFeatureSchemaCollection::Create (NULL);

        //
        // For Default Schemas the Schema name is WMS_Schema
        //
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (FdoWmsGlobals::DefaultSchemaName, FdoWmsGlobals::DefaultSchemaDescription);

        //
        // Add the newly created schema to the schema collection
        //
        mSchemas->Add (schema);
        FdoPtr<FdoClassCollection> featClasses = schema->GetClasses ();

        //
        // Retrieve the WMS layer definitons retrieved from the WMS capabilities document.
        // Recursively create feature class(es) from the WMS layer and it's children and add the class(es) to the class collection.
        //
        FdoWmsLayerCollectionP layers = capa->GetLayers();
        for (int k = 0; k < layers->GetCount(); k++) {
            FdoPtr<FdoWmsLayer> layer = layers->GetItem (k);
            _addFeatureClass (featClasses, layer, NULL);
        }

        //
        // Accept the changes made to the schema
        //
        schema->AcceptChanges();
    }
}

// helper function to generate a valid FDO class name from a WMS layer name
FdoStringP _generateValidFdoClassName(FdoStringP& layerName)
{
    // If these characters are in the layer name, adding the class to the schema will fail
    // FOR NOW... Remove these characters and substitute with empty spaces 
    FdoStringP modLayerName = layerName.Replace( L".", L" " );
    modLayerName = modLayerName.Replace( L":", L" " );

    // Trim off any leading/trailing spaces
    int origLen = modLayerName.GetLength();
    FdoString* checkStr = (FdoString*)modLayerName;
    if (NULL != wcschr(L" ", checkStr[0]))
        modLayerName = modLayerName.Right(L" ");

    origLen = modLayerName.GetLength();
    checkStr = (FdoString*)modLayerName;
    if (NULL != wcschr(L" ", checkStr[origLen-1]))
        modLayerName = modLayerName.Mid(0, origLen-1);

    // Return the modified layer name
    return modLayerName;
}

// helper function to create a feature class from a WMS layer and add the class into the collection
void FdoWmsConnection::_addFeatureClass (FdoClassCollection* featClasses, FdoWmsLayer* layer, FdoClassDefinition* parent)
{
    FdoBoolean bAbstract = false;

    // If the layer name is empty this means the WMS layer is abstract and cannot be
    // queried from. Use the layer Title and the class name
    FdoStringP layerName = layer->GetName ();
    if (layerName.GetLength() == 0) {
        bAbstract = true;
        layerName = layer->GetTitle ();
    }

    // Generate a valid FDO class name from a WMS layer name
    FdoStringP modLayerName = _generateValidFdoClassName(layerName);
    
    // If the layer name is valid, we can try and add the layer
    FdoPtr<FdoClassDefinition> featureClassDef;
    if (layerName.GetLength() != 0) 
    {
        // If the FDO Feature class exists, we cannot add it
        FdoPtr<FdoClassDefinition> classDef = featClasses->FindItem (modLayerName);
        if (NULL == classDef)
        {
            // Map the modified FDO class name to the WMS layer name so that the layer name can be used 
            // in the FdoISelect command to return 
            FdoDictionaryElementP dictElement = FdoDictionaryElement::Create(modLayerName, layerName);
            mLayerMappings->Add(dictElement);

            // Create the FDO Feature class and set it's properties
            FdoPtr<FdoFeatureClass> featClass = FdoFeatureClass::Create ();        

            // Set the unique layer name
            featClass->SetName (modLayerName);

            // Set the layer description
            FdoString* title = layer->GetTitle ();
            if (FdoCommonStringUtil::StringCompare(title, L"") != 0) {
                featClass->SetDescription (title);
            }
            else {
                featClass->SetDescription (layer->GetAbstract ());
            }

            // Flag the class as Abstract
            if (bAbstract)
            {
                featClass->SetIsAbstract (true);
            }

            // Used the parent FdoClassDefinition passed to this method as the parent class
            featClass->SetBaseClass (parent);

            // If this is a top level class, add the FeatId and Raster properties to the class definition
            if (parent == NULL)
            {
                _setProperties (featClass);
            }
            else
            {
                _setBaseProperties (featClass, parent);
            }

            // Create ans set the Feature classes capabilities
            FdoClassCapabilitiesP classCapabilities = FdoClassCapabilities::Create(*featClass.p);
            classCapabilities->SetSupportsLocking(false);
            classCapabilities->SetSupportsLongTransactions(false);
            classCapabilities->SetSupportsWrite(false);
            featClass->SetCapabilities(classCapabilities);

            // Associate the spatial context with the feature class' raster property
            _setDefaultSpatialContextAssociation (featClass);
            
            // Add the newly defined feature class to the schema
            featClasses->Add (featClass);

            // Set the outer class definition object
            featureClassDef = FDO_SAFE_ADDREF(featClass.p);
        }
    }

    // Recursively iterate through the child layers, adding feature classes for each child layer
    FdoWmsLayerCollectionP childLayers = layer->GetLayers ();
    if (NULL != childLayers)
    {
        FdoInt32 cntChildLayers = childLayers->GetCount ();
        for (FdoInt32 i=0; i<cntChildLayers; i++)
        {
            FdoWmsLayerP childLayer = childLayers->GetItem (i);
            _addFeatureClass (featClasses, childLayer, featureClassDef.p != NULL ? featureClassDef.p : parent);
        }
    }
}

// add properties "FeatId" and "Raster" to the class. Since "FeatId"
// is identifier of all WMS layer classes, it will be added to
// m_baseProperties and m_identityProperties; "Raster" is also read-only
// as "FeatId", so it will be added to m_baseProperties as well.
void FdoWmsConnection::_setProperties (FdoFeatureClass* featClass)
{
    FdoPtr<FdoPropertyDefinitionCollection> props = featClass->GetProperties ();
    FdoPtr<FdoDataPropertyDefinitionCollection> idProps = featClass->GetIdentityProperties ();

    FdoPtr<FdoDataPropertyDefinition> propFeatId = FdoDataPropertyDefinition::Create (FdoWmsGlobals::PropertyFeatId, FdoWmsGlobals::PropertyFeatIdDescription);
    FdoPtr<FdoRasterPropertyDefinition> propRaster = FdoRasterPropertyDefinition::Create (FdoWmsGlobals::PropertyRaster, FdoWmsGlobals::PropertyRasterDescription);

    FdoPtr<FdoRasterDataModel> dataModel = FdoRasterDataModel::Create ();
    propRaster->SetDefaultDataModel (dataModel);

    props->Add (propFeatId);
    props->Add (propRaster);

    idProps->Add (propFeatId);    
}

// add base properties to the feature class explicitly.
void FdoWmsConnection::_setBaseProperties (FdoFeatureClass* featClass, FdoClassDefinition* parent)
{
    if (parent == NULL)
        throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDOWMS_COPY_BASE_PROPERTIES_PARENT),
                                                                L"FdoWmsConnection::_setBaseProperties"));

    FdoPtr<FdoPropertyDefinitionCollection> parentProps = parent->GetProperties ();
    if (parentProps->GetCount () > 0)
    {
        featClass->SetBaseProperties (parentProps);    
    }
    else
    {
        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> parentBaseProps = parent->GetBaseProperties ();
        FdoInt32 cntBaseProps = parentBaseProps->GetCount ();
        FdoPtr<FdoPropertyDefinitionCollection> baseProps = FdoPropertyDefinitionCollection::Create (NULL);
        for (FdoInt32 i=0; i<cntBaseProps; i++)
        {
            FdoPtr<FdoPropertyDefinition> prop = parentBaseProps->GetItem (i);
            baseProps->Add (prop);
        }

        featClass->SetBaseProperties (baseProps);
    }
}

/// <summary>Get the dictionary that contains the mappings between FDO applicable class names and WMS layer names.</summary>
/// <returns>Returns the layer name dictionary.</returns>
FdoDictionary* FdoWmsConnection::GetNamedLayerMappings ()
{
    return FDO_SAFE_ADDREF(mLayerMappings.p);
}

void FdoWmsConnection::_setDefaultSpatialContextAssociation (FdoClassDefinition* featClass)
{
    FdoString* className = featClass->GetName ();
    FdoPtr<FdoDictionaryElement> element = mLayerMappings->GetItem (className);
    FdoString* layerName = element->GetValue ();
    FdoString* crsName = NULL;
    FdoWmsLayerP layer;

    // If a layer name is associated to the class definition find the associated layer 
    if (layerName != NULL)
    {
        // Retrieve the WMS capabilities returned by the WMS server
        FdoPtr<FdoWmsCapabilities> capa = static_cast<FdoWmsCapabilities *>(mWmsServiceMetadata->GetCapabilities ());
        FdoPtr<FdoWmsLayerCollection> layers = capa->GetLayers ();        

        // Search for the list of layers with the specified name in all the layers and child layers
        // returned by the server
        layer = FindLayer (layers, layerName);
        while (layer != NULL)
        {
            // Get the CRS names associated to the WMS layers, as defined by the server's capabilities response
            FdoStringsP crsNames = layer->GetCoordinateReferenceSystems ();
            if (crsNames->GetCount () > 0)
            {
                // If the layer supports the default CRS (EPSG:4326) find the index of the default CRS
                // (Use both default CRS names in the search. A server may support one or both)
                FdoInt32 indexDefaultCRS = crsNames->IndexOf(FdoWmsGlobals::DefaultEPSGCode);
                if (indexDefaultCRS == -1) 
                {
                    indexDefaultCRS = crsNames->IndexOf(FdoWmsGlobals::DefaultEPSGCode2);
                }
                
                // If the index of the default CRS was found, use it to retrieve the CRS name
                if (indexDefaultCRS != -1) 
                {
                    crsName = crsNames->GetString (indexDefaultCRS);
                }
                // Otherwise, get the first spatial context found in the list of the spatial contexts
                // associated to the WMS layer
                else
                {
                    crsName = crsNames->GetString (0);
                }

                // Stop processing layers
                break;
            }
            else
            {
                // If the current layer has no CRS specified, then we should look into its parent layer
                // if it has parent.
                layer = layer->GetParent ();
            }
        }

        // If a CRS name is associated to the layer, use it to set the classes spatial context association
        if (crsName != NULL)
        {
            // First we search the raster property in the BaseProperties
            FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = featClass->GetBaseProperties ();
            for (FdoInt32 i=0; i<baseProps->GetCount (); i++)
            {
                FdoPtr<FdoPropertyDefinition> baseProp = baseProps->GetItem (i);
                FdoRasterPropertyDefinition* rasterProp = dynamic_cast<FdoRasterPropertyDefinition *> (baseProp.p);
                if (rasterProp != NULL)
                {
                    FdoString* scName = rasterProp->GetSpatialContextAssociation();
                    if (scName == NULL || wcslen(scName) == 0) 
                    {
                        rasterProp->SetSpatialContextAssociation (crsName);
                    }

                    return;
                }
            }

            // If the raster property does not exist in the base properties, check the properties list
            FdoPtr<FdoPropertyDefinitionCollection> props = featClass->GetProperties ();
            for (FdoInt32 i=0; i<props->GetCount (); i++)
            {
                FdoPtr<FdoPropertyDefinition> prop = props->GetItem (i);
                FdoRasterPropertyDefinition* rasterProp = dynamic_cast<FdoRasterPropertyDefinition *> (prop.p);
                if (rasterProp != NULL)
                {
                    FdoString* scName = rasterProp->GetSpatialContextAssociation();
                    if (scName == NULL || wcslen(scName) == 0) 
                    {
                        rasterProp->SetSpatialContextAssociation (crsName);
                    }

                    return;
                }
            }

            // The class does not have a raster property
            if (!featClass->GetIsAbstract())
                throw FdoException::Create (NlsMsgGet (FDOWMS_FEATURE_NO_RASTER_PROPERTY, "Class '%1$ls' does not contain a Raster property.", className));

        }
        else
        {
            // The layer must have CRS information except the "abstract" layers.
            if (!featClass->GetIsAbstract())
                throw FdoException::Create (NlsMsgGet (FDOWMS_12002_LAYER_HAS_NO_CRS, "The WMS layer or its parent layers has no CRS."));
        }
    }
    else
    {
        // A layer was not associated to the FDO class
        throw FdoException::Create (NlsMsgGet (FDOWMS_12001_LAYER_NOT_EXIST, "The WMS layer '%1$ls' does not exist.", className));
    }
}

/// <summary>Search the class and it's parents for its raster property definition.</summary>
/// <returns>Return the raster definition for the specified class. If not found, return NULL.</returns>
FdoRasterPropertyDefinition* FdoWmsConnection::FindRasterProperty (FdoClassDefinition* featClass)
{
    if (featClass == NULL) {
        throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDOWMS_FIND_RASTER_PROPERTIES_CLASS),
                                                                L"FdoWmsConnection::FindRasterProperty"));
    }

    FdoRasterPropertyDefinition* rasterProp = NULL;
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = featClass->GetBaseProperties ();
    for (FdoInt32 i=0; i<baseProps->GetCount (); i++) {
        FdoPtr<FdoPropertyDefinition> baseProp = baseProps->GetItem (i);
        if (baseProp->GetPropertyType() == FdoPropertyType_RasterProperty) {
            rasterProp = static_cast<FdoRasterPropertyDefinition *> (baseProp.p);
            break;
        }
    }

    FdoPtr<FdoPropertyDefinitionCollection> props = featClass->GetProperties ();
    for (FdoInt32 i=0; i<props->GetCount (); i++) {
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem (i);
        if (prop->GetPropertyType() == FdoPropertyType_RasterProperty) {
            rasterProp = static_cast<FdoRasterPropertyDefinition *> (prop.p);
            break;
        }
    }

    return FDO_SAFE_ADDREF(rasterProp);
}

/// <summary>Search the layer with the specified name in all the layers and child layers.</summary>
/// <returns>Return the layer with the specified name. If not found, return NULL.</returns>
FdoWmsLayer* FdoWmsConnection::FindLayer (FdoWmsLayerCollection* layers, FdoString* name)
{
    FdoWmsLayer* ret = NULL;
    for (FdoInt32 i=0; i<layers->GetCount (); i++)
    {
        FdoPtr<FdoWmsLayer> layer = layers->GetItem (i);
        FdoString* layerName = layer->GetName ();
        if (layerName == NULL || wcslen (layerName) == 0)
        {
            layerName = layer->GetTitle ();
        }

        if (layerName == NULL)
            throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_SCHEMA_LAYER_NO_TITLE, "A WMS layer has no name or title."));

        if (wcscmp (name, layerName) == 0)
        {
            ret = layer;
            break;
        }

        // find in the child layers
        FdoPtr<FdoWmsLayerCollection> childLayers = layer->GetLayers ();
        ret = FindLayer (childLayers, name);
        if (ret)
        {
            return FDO_SAFE_ADDREF (ret);
        }
    }

    return FDO_SAFE_ADDREF (ret);
}

void FdoWmsConnection::_buildUpClassLayerMappings (FdoWmsLayerCollection *layers)
{
    FdoInt32 cntLayers = layers ? layers->GetCount () : 0;
    for (int i=0; i<cntLayers; i++)
    {
        FdoPtr<FdoWmsLayer> layer = layers->GetItem (i);
        _buildUpClassLayerMapping (layer);
        FdoPtr<FdoWmsLayerCollection> childLayers = layer->GetLayers ();
        _buildUpClassLayerMappings (childLayers);
    }
}

void FdoWmsConnection::_buildUpClassLayerMapping (FdoWmsLayer* layer)
{
    // Retrieve the layer 'Name' from the WMS layer. We first try  
    // the actual layer name. If that is not set, we use the layer 
    // title as the name
    FdoStringP layerName = layer->GetName ();
    if (layerName.GetLength() == 0) {
        layerName = layer->GetTitle ();
        if (layerName.GetLength() == 0) {
            return;
        }
    }

    // Generate a valid FDO class name from a WMS layer name
    FdoStringP modLayerName = _generateValidFdoClassName(layerName);

    // If not already in the layer collection, add it. 
    // NOTE: Several abstract layers may have the same title.
    // This would cause the second item to be igniored
    if (mLayerMappings->FindItem (modLayerName) == NULL)
    {
        FdoPtr<FdoDictionaryElement> pair = FdoDictionaryElement::Create (modLayerName, layerName);
        mLayerMappings->Add (pair);
    }
}

FdoStringP FdoWmsConnection::GetRequestWMSVersion(const wchar_t* pStr)
{
    FdoStringP pRetStr;
    wchar_t tmpBuf[21];
    const wchar_t* pVersion = L"version=";
    if (pStr == NULL)
        return pRetStr;
    int idx = 0, idxVers = 0, idxResPos = -1;
    while(*(pStr+idx) != L'\0' && *(pVersion+idxVers) != L'\0')
    {
        if (towlower(*(pStr+idx)) == *(pVersion+idxVers))
        {
            idxResPos = (idxResPos == -1) ? idx : idxResPos;
            idxVers++;
        }
        else
        {
            idxVers = 0;
            idx = (idxResPos != -1) ? idxResPos : idx;
            idxResPos = -1;
        }
        idx++;
    }
    if (idxResPos == -1)
        return pRetStr;

    idx = idxResPos+8;
    while (*(pStr+idx) != L'\0' && *(pStr+idx) != L'&') idx++;
    int szcopy = (20 < idx-idxResPos-8) ? (20) : (idx-idxResPos-8);
    wcsncpy(tmpBuf, pStr+idxResPos+8, szcopy );
    tmpBuf[szcopy] = L'\0';
    pRetStr = tmpBuf;
    return pRetStr;
}
