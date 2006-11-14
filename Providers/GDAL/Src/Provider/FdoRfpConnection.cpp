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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpConnection.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpConnection.h"
#include "FdoRfpSpatialContext.h"
#include "FdoRfpGeoRaster.h"
#include "FdoRfpClassData.h"
#include "FdoRfpSchemaData.h"
#include "FdoRfpConnectionCapabilities.h"
#include "FdoRfpSchemaCapabilities.h"
#include "FdoRfpCommandCapabilities.h"
#include "FdoRfpFilterCapabilities.h"
#include "FdoRfpExpressionCapabilities.h"
#include "FdoRfpRasterCapabilities.h"
#include "FdoRfpConnectionInfo.h"
#include "FdoRfpDefaultSchema.h"
#include "FdoRfpSelectCommand.h"
#include "FdoRfpSelectAggregate.h"
#include "FdoRfpDescribeSchemaCommand.h"
#include "FdoRfpDescribeSchemaMapping.h"
#include "FdoRfpGetSpatialContexts.h"
#include "FdoRfpDatasetCache.h"
#include "FdoRfpGlobals.h"
#include <GdalFile/Override/FdoGrfpOverrides.h>
#include <gdal.h>
#include <FdoCommonConnStringParser.h>

// external access to connection for client services
extern "C" FDOGRFP_API FdoIConnection* CreateConnection ()
{
    static int bInitialized = FALSE;

    if( !bInitialized )
    {
        GDALAllRegister();
        bInitialized = TRUE;
    }

    return (new FdoRfpConnection());
}

FdoRfpConnection::FdoRfpConnection(void) : m_state(FdoConnectionState_Closed)
{
}

FdoRfpConnection::~FdoRfpConnection(void)
{
    Close();
}

/// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
/// of the connection.</summary>
/// <returns>Returns the connection capabilities</returns> 
FdoIConnectionCapabilities* FdoRfpConnection::GetConnectionCapabilities()
{
    return new FdoRfpConnectionCapabilities();
}

/// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
/// <returns>Returns schema capabilities</returns> 
FdoISchemaCapabilities* FdoRfpConnection::GetSchemaCapabilities()
{
    return new FdoRfpSchemaCapabilities();
}

/// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
/// <returns>Returns the command capabilities</returns> 
FdoICommandCapabilities* FdoRfpConnection::GetCommandCapabilities()
{
    return new FdoRfpCommandCapabilities();
}

/// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
/// <returns>Returns the filter capabilities</returns> 
FdoIFilterCapabilities* FdoRfpConnection::GetFilterCapabilities()
{
    return new FdoRfpFilterCapabilities();
}

/// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
/// <returns>Returns the expression capabilities</returns> 
FdoIExpressionCapabilities* FdoRfpConnection::GetExpressionCapabilities()
{
    return new FdoRfpExpressionCapabilities();
}

/// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
/// <returns>Returns the raster capabilities</returns> 
FdoIRasterCapabilities* FdoRfpConnection::GetRasterCapabilities()
{
    return new FdoRfpRasterCapabilities();
}

class FdoRfpTopologyCapabilities : public FdoITopologyCapabilities
{
public:
    virtual bool SupportsTopology () { return (false); }
    virtual bool SupportsTopologicalHierarchy () { return (false); }
    virtual bool BreaksCurveCrossingsAutomatically () { return (false); }
    virtual bool ActivatesTopologyByArea (){ return (false); }
    virtual bool ConstrainsFeatureMovements (){ return (false); }
    virtual void Dispose () { delete this; }
};

/// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
/// <returns>Returns the topology capabilities</returns> 
FdoITopologyCapabilities* FdoRfpConnection::GetTopologyCapabilities ()
{


    return (new FdoRfpTopologyCapabilities ());
}

/// <summary>Gets the connection string used to open a DataStore.</summary>
/// <returns>Returns the connection string</returns> 
FdoString* FdoRfpConnection::GetConnectionString()
{
    return m_connectionString;
}

/// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
/// connection is closed.</summary>
/// <param name="value">Input the connection string</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpConnection::SetConnectionString(FdoString* value)
{
    if (value == NULL)
        value = L"";
    _validateClose();
    m_connectionString = value;
    // Update the connection property dictionary:
    FdoPtr<FdoIConnectionInfo> connInfo = GetConnectionInfo();
    FdoPtr<FdoCommonConnPropDictionary> connDict = dynamic_cast<FdoCommonConnPropDictionary*>(connInfo->GetConnectionProperties());
    connDict->UpdateFromConnectionString(m_connectionString);
}

void FdoRfpConnection::_validateOpen()
{
    if (m_state != FdoConnectionState_Open) {
        throw FdoException::Create(
            NlsMsgGet(
                GRFP_15_CONNECTION_NOT_ESTABLISHED, 
                "The FDO connection has not been 'Opened'. The attempted operation is not permitted."
            )
        );
    }
}

void FdoRfpConnection::_validateClose()
{
    if (m_state != FdoConnectionState_Closed) {
        throw FdoException::Create(
            NlsMsgGet(
                GRFP_69_CONNECTION_ESTABLISHED, 
                "The FDO connection has not been 'Closed'. The attempted operation is not permitted."
            )
        );
    }
}

/// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
/// <returns>Returns the connection info</returns> 
FdoIConnectionInfo* FdoRfpConnection::GetConnectionInfo()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new FdoRfpConnectionInfo (this);

    return FDO_SAFE_ADDREF(mConnectionInfo.p);
}

/// <summary>Gets the current state of the connection.</summary>
/// <returns>Returns the current state of the connection</returns> 
FdoConnectionState FdoRfpConnection::GetConnectionState()
{
    return m_state;
}

/// <summary>Gets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability 0 will be returned</summary>
/// <returns>Returns the time to wait (in milliseconds)</returns> 
FdoInt32 FdoRfpConnection::GetConnectionTimeout()
{
    throw FdoException::Create(NlsMsgGet(GRFP_39_CONNECTION_TIMEOUT_NOT_SUPPORTED, "Connection timeout is not supported."));
    return 0;
}

/// <summary>Sets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability then attempting to
/// set a timeout will result in an exception</summary>
/// <param name="value">Input the time to wait (in milliseconds)</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpConnection::SetConnectionTimeout(FdoInt32 value)
{
    throw FdoException::Create(NlsMsgGet(GRFP_39_CONNECTION_TIMEOUT_NOT_SUPPORTED, "Connection timeout is not supported."));
}

// Build up the default spatial contexts if they are unavailable in the configuration
void FdoRfpConnection::_buildUpDefaultSpatialContext()
{
    FdoRfpSpatialContextP defaultSC = new FdoRfpSpatialContext();
    defaultSC->SetName(FdoGrfpGlobals::DefaultSpatialContextName);
    defaultSC->SetDescription(NlsMsgGet(GRFP_67_DEFAULT_SPATIAL_CONTEXT_DESC, "System generated default FDO Spatial Context"));
    defaultSC->SetCoordinateSystem(FdoGrfpGlobals::DefaultSpatialContextCoordName);
    defaultSC->SetExtent(FdoRfpUtil::CreateGeometryAgfFromRect(FdoRfpRect(
        FdoGrfpGlobals::DefaultSpatialContextExtentMinX, 
        FdoGrfpGlobals::DefaultSpatialContextExtentMinY, 
        FdoGrfpGlobals::DefaultSpatialContextExtentMaxX,
        FdoGrfpGlobals::DefaultSpatialContextExtentMaxY)));
    defaultSC->SetExtentType(FdoSpatialContextExtentType_Static);
    defaultSC->SetXYTolerance(FdoGrfpGlobals::DefaultSpatialContextXYTolerance);
    defaultSC->SetZTolerance(FdoGrfpGlobals::DefaultSpatialContextZTolerance);
    defaultSC->SetCoordinateSystemWkt(FdoGrfpGlobals::DefaultSpatialContextWKTName);
    m_spatialContexts->Add(defaultSC);
}

// Build up the default feature schema if there is no configuation or the configuation contains
// no feature schema.
void FdoRfpConnection::_buildUpDefaultFeatureSchema()
{
    FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create(); 
    for (int i = 0; defaultSchema[i] != NULL; i++)
        stream->Write( (FdoByte*) defaultSchema[i], strlen(defaultSchema[i]));
    stream->Reset();
    FdoXmlReaderP reader = FdoXmlReader::Create(stream);
    m_featureSchemas->ReadXml(reader);    
}

// Build up the default schema overrides according to the default feature schema
void FdoRfpConnection::_buildUpDefaultOverrides()
{
    FdoStringP overrides = FdoStringP::Format(defaultOverrides, FdoGrfpGlobals::GRFPProviderName, (FdoString*)m_defaultRasterLocation);
    FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create(); 
    stream->Write((FdoByte*)(const char*)overrides, overrides.GetLength());
    stream->Reset();
    FdoXmlReaderP reader = FdoXmlReader::Create(stream);
    m_schemaMappings->ReadXml(reader);
}


// According to the feature schema and schema overrides, build up the schema data
void FdoRfpConnection::_buildUpSchemaDatas()
{
    m_schemaDatas = FdoRfpSchemaDataCollection::Create();
    FdoInt32 count = m_featureSchemas->GetCount();
    for (FdoInt32 i = 0; i < count; i++)
    {
        FdoFeatureSchemaP fs = m_featureSchemas->GetItem(i);
        //Try to find the corresponding schema mapping
        FdoPhysicalSchemaMappingP mapping = m_schemaMappings->GetItem(FdoGrfpGlobals::GRFPProviderName, fs->GetName());
        FdoGrfpPhysicalSchemaMappingP mapping1;
        if (mapping != NULL)
            mapping1 = SP_STATIC_CAST(FdoGrfpPhysicalSchemaMapping, mapping);
        FdoRfpSchemaDataP schemaData = FdoRfpSchemaData::Create(this, fs, mapping1);
        m_schemaDatas->Add(schemaData);
    }
}

// The classes contained in FDORFP feature schema can have and only have two properties.
// One is the ID of the raster and the other is the raster itself. So after the schema
// is built up, we must check its validation.
void FdoRfpConnection::_validateFeatureSchema()
{
    FdoInt32 count = m_featureSchemas->GetCount();
    for (FdoInt32 i = 0; i < count; i++)
    {
        FdoFeatureSchemaP schema = m_featureSchemas->GetItem(i);
        FdoClassesP classes = schema->GetClasses();
        FdoInt32 countClass = classes->GetCount();
        for (FdoInt32 j = 0; j < countClass; j++)
        {
            FdoClassDefinitionP classDef = classes->GetItem(j);
            FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
            //The feature class can and only can have two properties, one is the ID of
            //the raster and the other is the raster itselft.
            try
            {
                if (props->GetCount() != 2)
                    throw 0;
                int flag = 0;
                for (FdoInt32 k = 0; k < 2; k++)
                {
                    FdoPtr<FdoPropertyDefinition> prop = props->GetItem(k);
                    if (prop->GetPropertyType() == FdoPropertyType_DataProperty &&
                        static_cast<FdoDataPropertyDefinition*>(prop.p)->GetDataType() == FdoDataType_String)
                        flag |= 0x1;
                    else if (prop->GetPropertyType() == FdoPropertyType_RasterProperty)
                        flag |= 0x2;
                }
                if (flag != 0x3)
                    throw 0;
            }
            catch (int)
            {
                throw FdoException::Create(NlsMsgGet1(GRFP_46_CLASS_INVALID, "Feature class '%1$ls' is invalid.", (FdoString*)(classDef->GetQualifiedName())));
            }
        }
    }
}

/// <summary>Opens a feature connection with the settings specified by the
/// ConnectionString attribute of the provider-specific feature connection
/// object.</summary>
/// <returns>Returns nothing</returns> 
FdoConnectionState FdoRfpConnection::Open()
{
    //Ensure the connection is not alteady open
    _validateClose();

    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo ();
    FdoPtr<FdoCommonConnPropDictionary> dictionary = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());

    m_defaultRasterLocation = dictionary->GetProperty (FdoGrfpGlobals::DefaultRasterFileLocation);
    
    FdoCommonConnStringParser parser (NULL, GetConnectionString ());
    // check the validity of the connection string, i.e. it doesn’t contain unknown properties 
    // e.g. DefaultFLocation instead of DefaultFileLocation
    if (!parser.IsConnStringValid())
        throw FdoException::Create (NlsMsgGet1(GRFP_109_INVALID_CONNECTION_STRING, "Invalid connection string '%1$ls'", GetConnectionString ()));
    if (parser.HasInvalidProperties(dictionary))
        throw FdoException::Create (NlsMsgGet1(GRFP_110_INVALID_CONNECTION_PROPERTY_NAME, "Invalid connection property name '%1$ls'", parser.GetFirstInvalidPropertyName(dictionary)));

    // build up spatial contexts
    if (m_spatialContexts == NULL)
        m_spatialContexts = new FdoRfpSpatialContextCollection();
    if (m_spatialContexts->GetCount() == 0)
        _buildUpDefaultSpatialContext();
    //Set the first spatial context as the active one
    m_activeSpatialContext = FdoRfpSpatialContextP(m_spatialContexts->GetItem(0))->GetName();

    //Build up feature schema collection
    if (m_featureSchemas == NULL)
        m_featureSchemas = FdoFeatureSchemaCollection::Create(NULL);
    if (m_featureSchemas->GetCount() == 0)
        _buildUpDefaultFeatureSchema();

    //Validate each class in all feature schemas
    _validateFeatureSchema();

    //Build up schema mapping collection
    if (m_schemaMappings == NULL )
        m_schemaMappings = FdoPhysicalSchemaMappingCollection::Create();

    // if DefaultRasterLocation is set, the default logical schema and overrides
    // must be established.
    if (m_defaultRasterLocation.GetLength() != 0)
    {
        // check if the default schema is already there
        FdoPtr<FdoFeatureSchema> defaultSchema = m_featureSchemas->FindItem(FdoGrfpGlobals::DefaultSchemaName);
        if (defaultSchema == NULL)
            _buildUpDefaultFeatureSchema();
        // and then build up the default overrides
        _buildUpDefaultOverrides();
    }

    //Build up the data for each feature class
    _buildUpSchemaDatas();

    //All done, no exceptions
    m_state = FdoConnectionState_Open;

    return m_state;
}

/// <summary>Closes the connection to the DataStore</summary>
/// <returns>Returns nothing</returns> 
void FdoRfpConnection::Close()
{
    if (m_state == FdoConnectionState_Closed)
        return;
    m_schemaDatas = NULL;
    m_schemaMappings = NULL;
    m_featureSchemas = NULL;
    m_spatialContexts = NULL;
    m_activeSpatialContext = L"";
    m_defaultRasterLocation = L"";
    m_connectionString = L"";
    
    m_state = FdoConnectionState_Closed;
}

/// <summary>Begins a transaction and returns an object that realizes
/// FdoITransaction.</summary>
/// <returns>Returns the transaction</returns> 
FdoITransaction* FdoRfpConnection::BeginTransaction()
{
    throw FdoException::Create(NlsMsgGet(GRFP_42_TRANSACTION_NOT_SUPPORTED, "Trasaction is not supported"));
}

/// <summary>Creates and returns the specified type of command object associated with
/// the connection.</summary>
/// <param name="commandType">Input the command type to be created</param> 
/// <returns>Returns the command</returns> 
FdoICommand* FdoRfpConnection::CreateCommand(FdoInt32 commandType)
{
    //Ensure the connection is established
    _validateOpen();

    FdoICommand* ret;

    switch (commandType)
    {
        case FdoCommandType_Select:
            ret = new FdoRfpSelectCommand (this);
            break;
        case FdoCommandType_SelectAggregates:
            ret = new FdoRfpSelectAggregate(this);
            break;
        case FdoCommandType_DescribeSchema:
            ret = new FdoRfpDescribeSchemaCommand(this);
            break;
        case FdoCommandType_GetSpatialContexts:
            ret = new FdoRfpGetSpatialContexts(this);
            break;
        case FdoCommandType_DescribeSchemaMapping:
            ret = new FdoRfpDescribeSchemaMapping(this);
            break;
        default:
            throw FdoException::Create(NlsMsgGet(GRFP_45_COMMAND_NOT_SUPPORTED, "Command not supported."));
    }
    return ret;
}

/// <summary>Factory function that creates an empty Schema Override set specific
/// to this FDO Provider.</summary>
/// <returns>Returns FdoPhysicalSchemaMapping</returns> 
FdoPhysicalSchemaMapping* FdoRfpConnection::CreateSchemaMapping()
{
    return FdoGrfpPhysicalSchemaMapping::Create();
}

/// <summary>Sets the XML configuration information that will be used to 
/// configure the FDO provider. SetConfiguration can only be called
/// while the connection is closed.</summary>
/// <param name="value">Input the IO Stream containing 
/// the contents of the XML configuration. </param> 
/// <returns>Returns nothing.</returns> 
void FdoRfpConnection::SetConfiguration (FdoIoStream* configStream)
{
    _validateClose();

    if (configStream == NULL)
        return;
    FdoXmlReaderP reader;        
    // build up the spatial context
    configStream->Reset();
    reader = FdoXmlReader::Create(configStream);
    m_spatialContexts = new FdoRfpSpatialContextCollection();
    try {
        m_spatialContexts->ReadXml(reader);
    } catch (FdoException* e) {
        // failed parsing configuation, create a new nested exception
        // taking the exception as the cause
        throw FdoException::Create(NlsMsgGet(GRFP_76_FAIL_READ_SPATIAL_CONTEXTS, "Failed to de-serialize the spatial contexts from the configuration."), e);
    }

    // build up feature schema collection
    configStream->Reset();
    reader = FdoXmlReader::Create(configStream);
    m_featureSchemas = FdoFeatureSchemaCollection::Create(NULL);
    try {
        m_featureSchemas->ReadXml(reader);
    } catch (FdoException* e) {
        // see comments above...
        throw FdoException::Create(NlsMsgGet(GRFP_77_FAIL_READ_FEATURE_SCHEMAS, "Failed to de-serialize the feature schemas from the configuration."), e);
    }
    
    // build up schema mapping collection
    configStream->Reset();
    reader = FdoXmlReader::Create(configStream);
    m_schemaMappings = FdoPhysicalSchemaMappingCollection::Create();
    try {
        m_schemaMappings->ReadXml(reader);
    } catch (FdoException* e) {
        // see comments above...
        throw FdoException::Create(NlsMsgGet(GRFP_78_FAIL_READ_SCHEMA_MAPPINGS, "Failed to de-serialize the schema mappings from the configuration."), e);
    }

}

//Get the class definition by its qualified name
void FdoRfpConnection::GetClassDef(FdoIdentifier* identifier, FdoPtr<FdoClassDefinition>& classDef)
{
    //Ensure the connection is established
    _validateOpen();
    //First find the class
    FdoPtr<FdoIDisposableCollection> classes = m_featureSchemas->FindClass(identifier->GetText());
    if (classes->GetCount() == 1)
    {
        classDef = static_cast<FdoClassDefinition*>(classes->GetItem(0));
    }
}

//Get a reference to the feature schema collection
FdoPtr<FdoFeatureSchemaCollection> FdoRfpConnection::GetFeatureSchemas()
{
    //Ensure the connection is established
    _validateOpen();

    return m_featureSchemas;
}

// Get a referrence to the schema mappings
FdoPtr<FdoPhysicalSchemaMappingCollection> FdoRfpConnection::GetSchemaMappings()
{
    //Ensure the connection is established
    _validateOpen();

    return m_schemaMappings;
}


//Get class data of "FeatureName.ClassName"
FdoPtr<FdoRfpClassData> FdoRfpConnection::GetClassData(FdoClassDefinition* classDef)
{
    //Ensure the connection is established
    _validateOpen();

    FdoRfpSchemaDataP schemaData = m_schemaDatas->GetItem(FdoPtr<FdoSchemaElement>(classDef->GetParent())->GetName());
    FdoRfpClassDatasP classDatas = schemaData->GetClassDatas();
    FdoRfpClassDataP classData = classDatas->GetItem(classDef->GetName());
    return classData;
}

//Get all spatial contexts
FdoPtr<FdoRfpSpatialContextCollection> FdoRfpConnection::GetSpatialContexts()
{
    //Ensure the connection is established

    // in fact, we want to fetch this down in low level code before things
    // are open sometimes.

    //_validateOpen();
    return m_spatialContexts; 
}

//Get active spatial context
FdoPtr<FdoRfpSpatialContext> FdoRfpConnection::GetActiveSpatialContext()
{
    //Ensure the connection is established
    _validateOpen();
    return m_spatialContexts->GetItem(m_activeSpatialContext);
}

//Activate spatial context
void FdoRfpConnection::ActivateSpatialContext(FdoString* contextName)
{
    _validateOpen();
    FdoPtr<FdoRfpSpatialContext> spatialContext = m_spatialContexts->FindItem(contextName);
    if (spatialContext == NULL)
        throw FdoCommandException::Create(NlsMsgGet1(GRFP_60_SPATIAL_CONTEXT_NOT_EXIST, "Specified spatial context '%1$ls' does not exist.", contextName));
    m_activeSpatialContext = contextName;
}

// Create a spatial context
void FdoRfpConnection::CreateSpatialContext(FdoRfpSpatialContext* spatialContext, bool bUpdateExist)
{
    _validateOpen();
    FdoInt32 index = m_spatialContexts->IndexOf(spatialContext->GetName());
    if (index != -1 && !bUpdateExist)
        throw FdoCommandException::Create(NlsMsgGet1(GRFP_61_SPATIAL_CONTEXT_EXIST, "Specified spatial context '%1$ls' already exists.", spatialContext->GetName()));
    if (index != -1)
        m_spatialContexts->SetItem(index, spatialContext);
    else
        m_spatialContexts->Add(spatialContext);
}

// Destroy a spatial context
void FdoRfpConnection::DestroySpatialContext(FdoString* contextName)
{
    _validateOpen();
    FdoInt32 index = m_spatialContexts->IndexOf(contextName);
    if (index == -1)
        throw FdoCommandException::Create(NlsMsgGet1(GRFP_60_SPATIAL_CONTEXT_NOT_EXIST, "Specified spatial context '%1$ls' does not exist.", contextName));
    m_spatialContexts->RemoveAt(index);
    //Change the active spatial context if it is removed
    if (m_activeSpatialContext == contextName)
    {
        if (m_spatialContexts->GetCount() > 0)
            m_activeSpatialContext = FdoRfpSpatialContextP(m_spatialContexts->GetItem(0))->GetName();
        else
            m_activeSpatialContext = L"";
    }

}

// Get dataset cache.
//Get active spatial context
FdoRfpDatasetCache *FdoRfpConnection::GetDatasetCache()
{
    if( m_datasetCache.p == NULL )
        m_datasetCache = FdoRfpDatasetCache::Create();

    return FDO_SAFE_ADDREF(m_datasetCache.p);
}

class FdoRfpGeometryCapabilities : public FdoIGeometryCapabilities
{
public:
    virtual ~FdoRfpGeometryCapabilities() {}
    virtual FdoGeometryType* GetGeometryTypes( FdoInt32& length){length = 0; return NULL;}
    virtual FdoGeometryComponentType* GetGeometryComponentTypes( FdoInt32& length )    {length = 0; return NULL;}
    virtual FdoInt32 GetDimensionalities() { return 0; }
    virtual void Dispose () { delete this; }
};

/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* FdoRfpConnection::GetGeometryCapabilities()
{
    return new FdoRfpGeometryCapabilities();
}






