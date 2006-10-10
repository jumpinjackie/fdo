// 
//  
//  Copyright (C) 2006 Autodesk Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  


//fwd decl
class OGRDataSource;
class OGRLayer;
class OGRFeature;

class OgrConnection : public FdoIConnection, 
                      public FdoIConnectionCapabilities,
                      public FdoISchemaCapabilities,
                      public FdoICommandCapabilities,
                      public FdoIFilterCapabilities,
                      public FdoIExpressionCapabilities,
                      public FdoIRasterCapabilities,
                      public FdoITopologyCapabilities,
                      public FdoIGeometryCapabilities,
                      public FdoIConnectionInfo,
                      public FdoIConnectionPropertyDictionary
{
public:

    OgrConnection();

protected:
    virtual ~OgrConnection();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

protected:
    
    OGR_API virtual void Dispose();

    //-------------------------------------------------------
    // FdoIConnection implementation
    //-------------------------------------------------------

public:
    
    //FdoIConnection functions requiring OGR specific logic
    //implemented in the .cpp file
    OGR_API virtual FdoString* GetConnectionString();
    OGR_API virtual void SetConnectionString(FdoString* value);
    OGR_API virtual FdoConnectionState Open();
    OGR_API virtual void Close();
    OGR_API virtual FdoICommand* CreateCommand(FdoInt32 commandType);

    
    //FdoIConnection simple function implementations
    OGR_API virtual FdoIConnectionCapabilities* GetConnectionCapabilities() 
        { return FDO_SAFE_ADDREF((FdoIConnectionCapabilities*)this); }
    OGR_API virtual FdoISchemaCapabilities* GetSchemaCapabilities()
        { return FDO_SAFE_ADDREF((FdoISchemaCapabilities*)this); }
    OGR_API virtual FdoICommandCapabilities* GetCommandCapabilities()
        { return FDO_SAFE_ADDREF((FdoICommandCapabilities*)this); }
    OGR_API virtual FdoIFilterCapabilities* GetFilterCapabilities()
        { return FDO_SAFE_ADDREF((FdoIFilterCapabilities*)this); }
    OGR_API virtual FdoIExpressionCapabilities* GetExpressionCapabilities()
        { return FDO_SAFE_ADDREF((FdoIExpressionCapabilities*)this); }
    OGR_API virtual FdoIRasterCapabilities* GetRasterCapabilities()
        { return FDO_SAFE_ADDREF((FdoIRasterCapabilities*)this); }
    OGR_API virtual FdoITopologyCapabilities* GetTopologyCapabilities()
        { return FDO_SAFE_ADDREF((FdoITopologyCapabilities*)this); }
    OGR_API virtual FdoIGeometryCapabilities* GetGeometryCapabilities()
        { return FDO_SAFE_ADDREF((FdoIGeometryCapabilities*)this); }
    OGR_API virtual FdoIConnectionInfo* GetConnectionInfo()
        { return FDO_SAFE_ADDREF((FdoIConnectionInfo*)this); }
    OGR_API virtual FdoConnectionState GetConnectionState() 
        { return m_connState; }
    OGR_API virtual FdoInt32 GetConnectionTimeout() 
        { return 0; }
    OGR_API virtual void SetConnectionTimeout(FdoInt32 value) 
        {}
    OGR_API virtual FdoITransaction* BeginTransaction() 
        { return NULL; }
    OGR_API virtual FdoPhysicalSchemaMapping* CreateSchemaMapping() 
        { return NULL; }
    OGR_API virtual void SetConfiguration(FdoIoStream* stream) 
        {}


    //-------------------------------------------------------
    // FdoIConnectionCapabilities implementation
    //-------------------------------------------------------

    OGR_API virtual FdoThreadCapability GetThreadCapability() 
        { return FdoThreadCapability_PerConnectionThreaded; }
    OGR_API virtual bool SupportsLocking()
        { return false; }
    OGR_API virtual FdoLockType* GetLockTypes(FdoInt32& size) 
        { size=0; return NULL; }
    OGR_API virtual bool SupportsTimeout()
        { return false; }
    OGR_API virtual bool SupportsTransactions()
        { return false; }
    OGR_API virtual bool SupportsLongTransactions()
        { return false; }
    OGR_API virtual bool SupportsSQL()
        { return false; }
    OGR_API virtual bool SupportsConfiguration()
        { return false; }
    OGR_API virtual bool SupportsMultipleSpatialContexts()
        { return false; }
    OGR_API virtual bool SupportsCSysWKTFromCSysName()
        { return false; }
    OGR_API virtual bool SupportsWrite()
        { return false; }
    OGR_API virtual bool SupportsMultiUserWrite()
        { return false; }
    OGR_API virtual FdoSpatialContextExtentType* GetSpatialContextTypes(FdoInt32& length)
    {
        static FdoSpatialContextExtentType spatialTypes[] =
        {
            //FdoSpatialContextExtentType_Static,
            FdoSpatialContextExtentType_Dynamic
        };

        length = sizeof(spatialTypes) / sizeof(FdoSpatialContextExtentType);
        return spatialTypes;
    }


    //-------------------------------------------------------
    // FdoISchemaCapabilities implementation
    //-------------------------------------------------------

    OGR_API virtual bool SupportsInheritance()                      { return false; }
    OGR_API virtual bool SupportsMultipleSchemas()                  { return false; }
    OGR_API virtual bool SupportsObjectProperties()                 { return false; }
    OGR_API virtual bool SupportsAssociationProperties()            { return false; }
    OGR_API virtual bool SupportsSchemaOverrides()                  { return false; }
    OGR_API virtual bool SupportsNetworkModel()                     { return false; }
    OGR_API virtual bool SupportsAutoIdGeneration()                 { return false; }
    OGR_API virtual bool SupportsDataStoreScopeUniqueIdGeneration() { return false; }
    OGR_API virtual bool SupportsSchemaModification()               { return false; }
    OGR_API virtual bool SupportsInclusiveValueRangeConstraints()   { return false; }
    OGR_API virtual bool SupportsExclusiveValueRangeConstraints()   { return false; }
    OGR_API virtual bool SupportsValueConstraintsList()             { return false; }
    OGR_API virtual bool SupportsNullValueConstraints()             { return false; }
    OGR_API virtual bool SupportsUniqueValueConstraints()           { return false; }
    OGR_API virtual bool SupportsCompositeUniqueValueConstraints()  { return false; }
    OGR_API virtual bool SupportsCompositeId()                      { return false; }
    OGR_API virtual bool SupportsDefaultValue()                     { return false; }


    OGR_API virtual FdoInt64 GetMaximumDataValueLength(FdoDataType dataType) { return -1; }
    OGR_API virtual FdoInt32 GetMaximumDecimalPrecision()                    { return -1; }
    OGR_API virtual FdoInt32 GetMaximumDecimalScale()                        { return -1; }
    OGR_API virtual FdoInt32 GetNameSizeLimit(FdoSchemaElementNameType name) { return -1; }
    OGR_API virtual FdoString* GetReservedCharactersForName()                { return NULL; }
    
    OGR_API virtual FdoDataType* GetSupportedAutoGeneratedTypes(FdoInt32& length)
    {
        static FdoDataType dataTypes[] =
        {
//        FdoDataType_Boolean,
//        FdoDataType_Byte,
//        FdoDataType_DateTime,
//        FdoDataType_Decimal,
//        FdoDataType_Double,
//        FdoDataType_Int16,
            FdoDataType_Int32,
//        FdoDataType_Int64,
//        FdoDataType_Single,
//        FdoDataType_String,
//        FdoDataType_BLOB,
//        FdoDataType_CLOB,
//        FdoDataType_UniqueID
        };

        length = sizeof(dataTypes) / sizeof(FdoDataType);
        return dataTypes;
    }
    
    OGR_API virtual FdoDataType* GetSupportedIdentityPropertyTypes(FdoInt32& length)
    {
       static FdoDataType dataTypes[] =
        {
//        FdoDataType_Boolean,
//        FdoDataType_Byte,
//        FdoDataType_DateTime,
//        FdoDataType_Decimal,
//        FdoDataType_Double,
//        FdoDataType_Int16,
            FdoDataType_Int32,
//        FdoDataType_Int64,
//        FdoDataType_Single,
//        FdoDataType_String,
//        FdoDataType_BLOB,
//        FdoDataType_CLOB,
//        FdoDataType_UniqueID
        };

        length = sizeof(dataTypes) / sizeof(FdoDataType);
        return dataTypes;
    }

    OGR_API virtual FdoClassType* GetClassTypes(FdoInt32& length)
    {
        static FdoClassType classTypes[] =
        {
            FdoClassType_Class,
            FdoClassType_FeatureClass
        };

        length = sizeof(classTypes) / sizeof(FdoClassType);
        return classTypes;
    }
    
    OGR_API virtual FdoDataType* GetDataTypes(FdoInt32& length)
    {
        static FdoDataType dataTypes[] =
        {
//            FdoDataType_Boolean,
//            FdoDataType_Byte,
            FdoDataType_DateTime,
//            FdoDataType_Decimal,
            FdoDataType_Double,
//            FdoDataType_Int16,
            FdoDataType_Int32,
//            FdoDataType_Int64,
//            FdoDataType_Single,
            FdoDataType_String,
//      FdoDataType_BLOB,
//      FdoDataType_CLOB,
//      FdoDataType_UniqueID
        };

        length = sizeof(dataTypes) / sizeof(FdoDataType);
        return dataTypes;
    }

    //-------------------------------------------------------
    // FdoICommandCapabilities implementation
    //-------------------------------------------------------

    OGR_API virtual FdoInt32* GetCommands(FdoInt32& size)
    {
    // read-write commands
        static const FdoInt32 commandsReadWrite[] =
        {
            FdoCommandType_Select,
            FdoCommandType_Insert,
            FdoCommandType_Delete,
            FdoCommandType_Update,
            FdoCommandType_DescribeSchema,
//        FdoCommandType_ApplySchema,
//      FdoCommandType_DestroySchema,
//      FdoCommandType_ActivateSpatialContext,
//        FdoCommandType_CreateSpatialContext,
//      FdoCommandType_DestroySpatialContext,
            FdoCommandType_GetSpatialContexts,
//      FdoCommandType_CreateMeasureUnit,
//      FdoCommandType_DestroyMeasureUnit,
//      FdoCommandType_GetMeasureUnits,
//      FdoCommandType_SQLCommand,
//      FdoCommandType_AcquireLock,
//      FdoCommandType_GetLockInfo,
//      FdoCommandType_GetLockedObjects,
//      FdoCommandType_GetLockOwners,
//      FdoCommandType_ReleaseLock,
//      FdoCommandType_ActivateLongTransaction,
//      FdoCommandType_CommitLongTransaction,
//      FdoCommandType_CreateLongTransaction,
//      FdoCommandType_GetLongTransactions,
//      FdoCommandType_FreezeLongTransaction,
//      FdoCommandType_RollbackLongTransaction,
//      FdoCommandType_ActivateLongTransactionCheckpoint,
//      FdoCommandType_CreateLongTransactionCheckpoint,
//      FdoCommandType_GetLongTransactionCheckpoints,
//      FdoCommandType_RollbackLongTransactionCheckpoint,
//      FdoCommandType_ChangeLongTransactionPrivileges,
//      FdoCommandType_GetLongTransactionPrivileges,
//      FdoCommandType_ChangeLongTransactionSet,
//      FdoCommandType_GetLongTransactionsInSet,
            FdoCommandType_SelectAggregates,
//        FdoCommandType_CreateDataStore,
//        FdoCommandType_DestroyDataStore,
        };

        size = sizeof(commandsReadWrite) / sizeof(FdoCommandType);
        return (FdoInt32*)commandsReadWrite;
    }
    
    OGR_API virtual bool SupportsParameters() { return false; }
    //OGR_API virtual bool SupportsTimeout() { return false; }
    OGR_API virtual bool SupportsSelectExpressions() { return true; }
    OGR_API virtual bool SupportsSelectFunctions() { return true; }
    OGR_API virtual bool SupportsSelectDistinct() { return true; }
    OGR_API virtual bool SupportsSelectOrdering() { return false; }
    OGR_API virtual bool SupportsSelectGrouping() { return false; }


    //-------------------------------------------------------
    // FdoIFilterCapabilities implementation
    //-------------------------------------------------------

    OGR_API virtual FdoConditionType* GetConditionTypes(FdoInt32& length)
    {
        static FdoConditionType conditionTypes[] =
        {
            FdoConditionType_Comparison,
            FdoConditionType_Like,
            FdoConditionType_In,
            FdoConditionType_Null,
            FdoConditionType_Spatial
        };

        length = sizeof(conditionTypes) / sizeof(FdoConditionType);
        return conditionTypes;
    }
    
    OGR_API virtual FdoSpatialOperations* GetSpatialOperations(FdoInt32& length)
    {
        static FdoSpatialOperations spatialOperations[] =
        {
//        FdoSpatialOperations_Contains,
//      FdoSpatialOperations_Crosses,
//        FdoSpatialOperations_Disjoint,
//      FdoSpatialOperations_Equals,
//        FdoSpatialOperations_Intersects,
//      FdoSpatialOperations_Overlaps,
//      FdoSpatialOperations_Touches,
//        FdoSpatialOperations_Within,
//      FdoSpatialOperations_CoveredBy,
//      FdoSpatialOperations_Inside,
            FdoSpatialOperations_EnvelopeIntersects
        };

        length = sizeof(spatialOperations) / sizeof(FdoSpatialOperations);
        return spatialOperations;
    }
    
    OGR_API virtual FdoDistanceOperations* GetDistanceOperations(FdoInt32& length)
    {
//  static FdoDistanceOperations distanceOperations[] =
//  {
//      FdoDistanceOperations_Beyond,
//      FdoDistanceOperations_Within
//  };

//  length = sizeof(distanceOperations) / sizeof(FdoDistanceOperations);
//  return distanceOperations;

    // currently no support
        length = 0;
        return NULL;
    }
    
    OGR_API virtual bool SupportsGeodesicDistance() 
        { return false; }
    OGR_API virtual bool SupportsNonLiteralGeometricOperations() 
        { return false; }


    //-------------------------------------------------------
    // FdoIExpressionCapabilities implementation
    //-------------------------------------------------------

    OGR_API virtual FdoExpressionType* GetExpressionTypes(FdoInt32& length)
    {
        static FdoExpressionType expressionTypes[] =
        {
            FdoExpressionType_Basic,
            FdoExpressionType_Function,
//      FdoExpressionType_Parameter
        };

        length = sizeof(expressionTypes) / sizeof(FdoExpressionType);
        return expressionTypes;
    }
    
    OGR_API virtual FdoFunctionDefinitionCollection* GetFunctions()
    {
        FdoFunctionDefinitionCollection* ret = FdoFunctionDefinitionCollection::Create ();
    
    // Add well-known functions we support to the list:
        FdoPtr<FdoFunctionDefinitionCollection> wellKnownFunctions = GetWellKnownFunctions();
        FdoPtr<FdoFunctionDefinition> wellKnownFunction;
        wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_COUNT);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_MIN);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_AVG);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_MAX);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_SUM);
        ret->Add(wellKnownFunction);
    /*
        wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_CONCAT);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_SPATIALEXTENTS);
        ret->Add(wellKnownFunction);
    */
    // Add NOT-well-known functions we support to the list:

        return ret;
    }

    //-------------------------------------------------------
    // FdoIRasterCapabilities implementation
    //-------------------------------------------------------

    OGR_API virtual bool SupportsRaster ()                              { return false; }
    OGR_API virtual bool SupportsStitching ()                           { return false; }
    OGR_API virtual bool SupportsSubsampling ()                         { return false; }
    OGR_API virtual bool SupportsDataModel (FdoRasterDataModel* model)  { return false; }

    //-------------------------------------------------------
    // FdoITopologyCapabilities implementation
    //-------------------------------------------------------

    OGR_API virtual bool SupportsTopology()                     { return false; }
    OGR_API virtual bool SupportsTopologicalHierarchy()         { return false; }
    OGR_API virtual bool BreaksCurveCrossingsAutomatically()    { return false; }
    OGR_API virtual bool ActivatesTopologyByArea()              { return false; }
    OGR_API virtual bool ConstrainsFeatureMovements()           { return false; }


    //-------------------------------------------------------
    // FdoIGeometryCapabilities implementation
    //-------------------------------------------------------

    OGR_API virtual FdoGeometryType* GetGeometryTypes( FdoInt32& length)
    {
        static FdoGeometryType geomTypes[] = 
        {
            FdoGeometryType_Point,
            FdoGeometryType_LineString,
            FdoGeometryType_Polygon,
            FdoGeometryType_MultiPoint,
            FdoGeometryType_MultiLineString,
            FdoGeometryType_MultiPolygon,
            FdoGeometryType_MultiGeometry,
//        FdoGeometryType_CurveString,
//        FdoGeometryType_CurvePolygon,
//        FdoGeometryType_MultiCurveString,
//        FdoGeometryType_MultiCurvePolygon
        };

        length = sizeof(geomTypes) / sizeof(FdoGeometryType);
        return geomTypes;
    }
    
    OGR_API virtual FdoGeometryComponentType* GetGeometryComponentTypes( FdoInt32& length )
    {
        static FdoGeometryComponentType compTypes[] = 
        {
            FdoGeometryComponentType_LinearRing,
//        FdoGeometryComponentType_CircularArcSegment,
            FdoGeometryComponentType_LineStringSegment,
            FdoGeometryComponentType_Ring
        };

        length = sizeof(compTypes) / sizeof(FdoGeometryComponentType);
        return compTypes;
    }
    
    OGR_API virtual FdoInt32 GetDimensionalities()
        { return FdoDimensionality_XY | FdoDimensionality_Z; /* | FdoDimensionality_M;*/ }


    //-------------------------------------------------------
    // FdoIConnectionInfo implementation
    //-------------------------------------------------------

    OGR_API virtual FdoString* GetProviderName()
        { return L"OSGeo.OGR.0.1"; }
    OGR_API virtual FdoString* GetProviderDisplayName()
        { return L"FDO provider for OGR"; }
    OGR_API virtual FdoString* GetProviderDescription()
        { return L"I'm the FDO provider for OGR, dude."; }
    OGR_API virtual FdoString* GetProviderVersion()
        { return L"0.1.0.0"; }
    OGR_API virtual FdoString* GetFeatureDataObjectsVersion()
        { return L"3.2.0.0"; }
    OGR_API virtual FdoIConnectionPropertyDictionary* GetConnectionProperties()
        { return FDO_SAFE_ADDREF((FdoIConnectionPropertyDictionary*)this); }


//-------------------------------------------------------
// FdoIConnectionPropertyDictionary declaration
//-------------------------------------------------------

    OGR_API virtual FdoString** GetPropertyNames(FdoInt32& count);
    OGR_API virtual FdoString* GetProperty(FdoString* name);
    OGR_API virtual void SetProperty(FdoString* name, FdoString* value);
    OGR_API virtual FdoString* GetPropertyDefault(FdoString* name);
    OGR_API virtual bool IsPropertyRequired(FdoString* name);
    OGR_API virtual bool IsPropertyProtected(FdoString* name);
    OGR_API virtual bool IsPropertyEnumerable(FdoString* name);
    OGR_API virtual FdoString** EnumeratePropertyValues(FdoString* name, FdoInt32& count);
    OGR_API virtual FdoString* GetLocalizedName(FdoString* name);
    OGR_API virtual bool IsPropertyFileName(FdoString* name);
    OGR_API virtual bool IsPropertyFilePath(FdoString* name);
    OGR_API virtual bool IsPropertyDatastoreName(FdoString* name);

//--------------------------------------------------------
//   Command execution functions
//--------------------------------------------------------

    FdoFeatureSchemaCollection* DescribeSchema();
    FdoISpatialContextReader* GetSpatialContexts();
    FdoIFeatureReader* Select(FdoIdentifier* fcname, FdoFilter* filter, FdoIdentifierCollection* props);
    FdoIDataReader* SelectAggregates(FdoIdentifier* fcname, 
                                     FdoIdentifierCollection* properties,
                                     bool bDistinct,
                                     FdoOrderingOption eOrderingOption,
                                     FdoIdentifierCollection* ordering,
                                     FdoFilter* filter,
                                     FdoIdentifierCollection* grouping);
    FdoInt32 Update(FdoIdentifier* fcname, FdoFilter* filter, FdoPropertyValueCollection* propvals);
    FdoInt32 Delete(FdoIdentifier* fcname, FdoFilter* filter);
    FdoIFeatureReader* Insert(FdoIdentifier* fcname, FdoPropertyValueCollection* propvals);
    OGRDataSource* GetOGRDataSource() { return m_poDS; }

private :
    
    std::map<std::wstring, std::wstring>* m_mProps;
    std::wstring m_connStr;
    FdoConnectionState m_connState;
    FdoFeatureSchemaCollection* m_pSchema;

    OGRDataSource* m_poDS;
};


//Base class for all commands. Defines common functionality and
//behavior.
template <class COMMAND> class OgrCommand : public COMMAND
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        OgrCommand(OgrConnection* connection)
        {
            m_connection = connection;
            ((FdoIConnection*)m_connection)->AddRef();
        }

    protected:
        // default destructor
        virtual ~OgrCommand()
        {
            ((FdoIConnection*)m_connection)->Release();
        }

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
        OGR_API virtual void Dispose() { delete this; }

    //-------------------------------------------------------
    // FdoICommand implementation
    //-------------------------------------------------------

    public:
        OGR_API virtual FdoIConnection* GetConnection()
        {
            ((FdoIConnection*)m_connection)->AddRef();
            return m_connection;
        }

        OGR_API virtual FdoITransaction* GetTransaction() { return NULL; }
        OGR_API virtual void SetTransaction(FdoITransaction* value) {}
        OGR_API virtual FdoInt32 GetCommandTimeout() { return 0;}
        OGR_API virtual void SetCommandTimeout(FdoInt32 value) {}
        OGR_API virtual FdoParameterValueCollection* GetParameterValues(){return NULL;}
        OGR_API virtual void Prepare(){}
        OGR_API virtual void Cancel(){}

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    protected:
        OgrConnection* m_connection;

};

//Base class  template for all feature commands. Defines common functionality and
//behavior.
template <class FEATURECOMMAND> class OgrFeatureCommand : public OgrCommand<FEATURECOMMAND>
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        OgrFeatureCommand(OgrConnection* connection)
    : OgrCommand<FEATURECOMMAND>(connection)
        {
            m_className = NULL;
            m_filter = NULL;
        }

    protected:
        virtual ~OgrFeatureCommand()
        {
            FDO_SAFE_RELEASE(m_className);
            FDO_SAFE_RELEASE(m_filter);
        }

    //-------------------------------------------------------
    // FdoIFeatureCommand implementation
    //-------------------------------------------------------

    public:
        OGR_API virtual FdoIdentifier* GetFeatureClassName()
        {
            return FDO_SAFE_ADDREF(m_className);
        }

        OGR_API virtual void SetFeatureClassName(FdoIdentifier* value)
        {
            FDO_SAFE_RELEASE(m_className);
            m_className = value;
            if (m_className) m_className->AddRef();
        }

        OGR_API virtual void SetFeatureClassName(FdoString* value)
        {
            FDO_SAFE_RELEASE(m_className);
            if (value != NULL)
                m_className = FdoIdentifier::Create(value);
        }

        OGR_API virtual FdoFilter* GetFilter()
        {
            return FDO_SAFE_ADDREF(m_filter);
        }

        OGR_API virtual void SetFilter(FdoFilter* value)
        {
            FDO_SAFE_RELEASE(m_filter);
            m_filter = value;
            if (m_filter) m_filter->AddRef();
        }

        OGR_API virtual void SetFilter(FdoString* value)
        {
            FDO_SAFE_RELEASE(m_filter);
            if (value != NULL)
                m_filter = FdoFilter::Parse(value);
        }

    protected:

        FdoIdentifier* m_className;
        FdoFilter* m_filter;
};


class OgrDescribeSchema : public OgrCommand<FdoIDescribeSchema>
{
    public:
        OGR_API OgrDescribeSchema(OgrConnection* connection) 
            : OgrCommand<FdoIDescribeSchema>(connection) 
        {}

    protected:
        OGR_API virtual ~OgrDescribeSchema(){}

    //-------------------------------------------------------
    // FdoIDescribeSchema implementation
    //-------------------------------------------------------

    public:
        OGR_API virtual FdoString* GetSchemaName() { return L"OGRSchema"; }
        OGR_API virtual void SetSchemaName(FdoString* value) { ; }
        OGR_API virtual FdoFeatureSchemaCollection* Execute() { return m_connection->DescribeSchema();}
};


class OgrSelect : public OgrFeatureCommand<FdoISelect>
{

    public:
        OGR_API OgrSelect(OgrConnection* connection)
            : OgrFeatureCommand<FdoISelect>(connection)
        {
            m_properties = FdoIdentifierCollection::Create();
        }

    protected:
        //OGR_API OgrSelect();
        OGR_API virtual ~OgrSelect()
        {
            m_properties->Release();
        }

    //-------------------------------------------------------
    // FdoISelect implementation
    //-------------------------------------------------------

    public:
        OGR_API virtual FdoIdentifierCollection* GetPropertyNames() 
            { return FDO_SAFE_ADDREF(m_properties); }
        OGR_API virtual FdoLockType GetLockType() {return FdoLockType_Exclusive;};
        OGR_API virtual void SetLockType(FdoLockType value) {};
        OGR_API virtual FdoLockStrategy GetLockStrategy() {return FdoLockStrategy_All;};
        OGR_API virtual void SetLockStrategy(FdoLockStrategy value) {};
        OGR_API virtual FdoIFeatureReader* Execute() 
        {
            return m_connection->Select(m_className, m_filter, m_properties);
        }
        OGR_API virtual FdoIFeatureReader* ExecuteWithLock() {return NULL; }
        OGR_API virtual FdoILockConflictReader* GetLockConflicts() { return NULL; }
        OGR_API virtual FdoIdentifierCollection* GetOrdering(){ return NULL; }
        OGR_API virtual void SetOrderingOption( FdoOrderingOption  option ) {}
        OGR_API virtual FdoOrderingOption GetOrderingOption( ){ return (FdoOrderingOption)0; }

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    private:
        FdoIdentifierCollection* m_properties;
};


class OgrSelectAggregates : public OgrFeatureCommand<FdoISelectAggregates>
{

    public:
        OgrSelectAggregates(OgrConnection *connection) :
            OgrFeatureCommand<FdoISelectAggregates>(connection),
        m_bDistinct(false), m_eOrderingOption(FdoOrderingOption_Ascending), m_grfilter(NULL)
        {
            m_grouping = FdoIdentifierCollection::Create();
            m_ordering = FdoIdentifierCollection::Create();
            m_properties = FdoIdentifierCollection::Create();
        }

    protected:
        virtual ~OgrSelectAggregates(void)
        {
            m_grouping->Release();
            m_ordering->Release();
            m_properties->Release();
        }
        
    //-------------------------------------------------------
    // FdoISelectAggregates implementation
    //-------------------------------------------------------
        
    public:
        virtual FdoIdentifierCollection* GetPropertyNames () { return FDO_SAFE_ADDREF(m_properties);}
        virtual FdoIDataReader* Execute () 
        { 
            return m_connection->SelectAggregates(m_className, m_properties, m_bDistinct, m_eOrderingOption, m_ordering, m_grfilter, m_grouping); 
        }
        virtual void SetDistinct( bool value ) { m_bDistinct = value; }
        virtual bool GetDistinct( ) { return m_bDistinct; }
        virtual FdoIdentifierCollection* GetGrouping() { return FDO_SAFE_ADDREF(m_grouping); }
        virtual void SetGroupingFilter( FdoFilter* filter ) 
        {
            FDO_SAFE_RELEASE(m_grfilter);
            m_grfilter = filter;
            if (m_grfilter) m_grfilter->AddRef();
        } 
        virtual FdoFilter* GetGroupingFilter( ) { return FDO_SAFE_ADDREF(m_filter); }
        virtual FdoIdentifierCollection* GetOrdering() { return FDO_SAFE_ADDREF(m_ordering); }
        virtual void SetOrderingOption( FdoOrderingOption  option ) { m_eOrderingOption = option; }
        virtual FdoOrderingOption GetOrderingOption( ) { return m_eOrderingOption; }

    private:
        FdoIdentifierCollection* m_properties;
        bool m_bDistinct;
        FdoOrderingOption m_eOrderingOption;
        FdoIdentifierCollection* m_ordering;
        FdoFilter* m_grfilter;
        FdoIdentifierCollection* m_grouping;
};


class OgrUpdate : public OgrFeatureCommand<FdoIUpdate>
{
    public:
        OGR_API OgrUpdate(OgrConnection* connection) : OgrFeatureCommand<FdoIUpdate>(connection)
        {
            m_properties = FdoPropertyValueCollection::Create();
        }

    protected:
        OGR_API virtual ~OgrUpdate()
        {
            FDO_SAFE_RELEASE(m_properties);
        }

    //-------------------------------------------------------
    // FdoIUpdate implementation
    //-------------------------------------------------------

    public:
        OGR_API virtual FdoPropertyValueCollection* GetPropertyValues() { return FDO_SAFE_ADDREF(m_properties); }
        OGR_API virtual FdoInt32 Execute()
        {
            return m_connection->Update(m_className, m_filter, m_properties);
        }
        OGR_API virtual FdoILockConflictReader* GetLockConflicts() { return NULL; }

    private:
        FdoPropertyValueCollection* m_properties;
};

class OgrDelete : public OgrFeatureCommand<FdoIDelete>
{
    public:
        OgrDelete(OgrConnection* connection) : OgrFeatureCommand<FdoIDelete>(connection)
        {}

    protected:
        virtual ~OgrDelete() {}

    //-------------------------------------------------------
    // FdoIDelete implementation
    //-------------------------------------------------------

    public:
        OGR_API virtual FdoInt32 Execute()
        {
            return m_connection->Delete(m_className, m_filter);
        }
        OGR_API virtual FdoILockConflictReader* GetLockConflicts() { return NULL; }
};

class OgrInsert : public OgrCommand<FdoIInsert>
{
    public:
        OGR_API OgrInsert(OgrConnection* connection) : OgrCommand<FdoIInsert>(connection)
        {
            m_className = NULL;
            m_properties = FdoPropertyValueCollection::Create();
        }

    protected:
        OGR_API virtual ~OgrInsert()
        {
            FDO_SAFE_RELEASE(m_className);
            FDO_SAFE_RELEASE(m_properties);
        }

    //-------------------------------------------------------
    // FdoIInsert implementation
    //-------------------------------------------------------

    public:
        OGR_API virtual FdoIdentifier* GetFeatureClassName() { return FDO_SAFE_ADDREF(m_className); }
        OGR_API virtual void SetFeatureClassName(FdoIdentifier* value)
        {
            FDO_SAFE_RELEASE(m_className);
            m_className = FDO_SAFE_ADDREF(value);
        }
        OGR_API virtual void SetFeatureClassName(FdoString* value)
        {
            FDO_SAFE_RELEASE(m_className);
            m_className = NULL;
            if (value)
                m_className = FdoIdentifier::Create(value);
        }
        OGR_API virtual FdoPropertyValueCollection* GetPropertyValues() { return FDO_SAFE_ADDREF(m_properties); }
        OGR_API virtual FdoBatchParameterValueCollection* GetBatchParameterValues() { return NULL; }
        OGR_API virtual FdoIFeatureReader* Execute()
        {
            return m_connection->Insert(m_className, m_properties);
        }

    private:
        FdoIdentifier* m_className;
        FdoPropertyValueCollection* m_properties;
};



class OgrGetSpatialContexts : public OgrCommand<FdoIGetSpatialContexts>
{
    public:

        OgrGetSpatialContexts(OgrConnection* connection)
            : OgrCommand<FdoIGetSpatialContexts>(connection)
        {}

    protected:

        virtual ~OgrGetSpatialContexts() {}


    //-------------------------------------------------------
    // FdoIGetSpatialContexts implementation
    //-------------------------------------------------------
    
    public:

        OGR_API virtual const bool GetActiveOnly() { return m_bActiveOnly; }
        OGR_API virtual void SetActiveOnly(const bool value) {m_bActiveOnly = value; }
        OGR_API virtual FdoISpatialContextReader* Execute() { return m_connection->GetSpatialContexts(); }

    private:

        bool m_bActiveOnly;
};


//spatial context reader -- returned when executing get spatial contexts command
class OgrSpatialContextReader : public FdoISpatialContextReader
{
    //-------------------------------------------------------
    // Constructors / Destructor
    //-------------------------------------------------------

    public:

        OgrSpatialContextReader (OgrConnection* conn);

    protected:

        virtual ~OgrSpatialContextReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

        void Dispose();

    //-------------------------------------------------------
    // FdoISpatialContextReader implementation
    //-------------------------------------------------------

    public:

        OGR_API virtual FdoString* GetName();
        OGR_API virtual FdoString* GetDescription();
        OGR_API virtual FdoString* GetCoordinateSystem();
        OGR_API virtual FdoString* GetCoordinateSystemWkt();
        OGR_API virtual FdoSpatialContextExtentType GetExtentType();
        OGR_API virtual FdoByteArray* GetExtent();
        OGR_API virtual const double GetXYTolerance();
        OGR_API virtual const double GetZTolerance();
        OGR_API virtual const bool IsActive();
        OGR_API virtual bool ReadNext();

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

    private:
        OgrConnection* m_connection;
        int m_nIndex;
        std::wstring m_wkt;
};

//feature reader -- returned when executing a select command
class OgrFeatureReader : public FdoIFeatureReader
{
    public:
        OGR_API OgrFeatureReader(OgrConnection* connection,
                                 OGRLayer* layer, FdoIdentifierCollection* props);

    protected:
        OGR_API virtual ~OgrFeatureReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
    // dispose this object
        OGR_API virtual void Dispose();

    //-------------------------------------------------------
    // FdoIFeatureReader implementation
    //-------------------------------------------------------

    public:
        OGR_API virtual FdoClassDefinition* GetClassDefinition();
        OGR_API virtual FdoInt32 GetDepth();
        OGR_API virtual bool GetBoolean(FdoString* propertyName);
        OGR_API virtual FdoByte GetByte(FdoString* propertyName);
        OGR_API virtual FdoDateTime GetDateTime(FdoString* propertyName);
        OGR_API virtual double GetDouble(FdoString* propertyName);
        OGR_API virtual FdoInt16 GetInt16(FdoString* propertyName);
        OGR_API virtual FdoInt32 GetInt32(FdoString* propertyName);
        OGR_API virtual FdoInt64 GetInt64(FdoString* propertyName);
        OGR_API virtual float GetSingle(FdoString* propertyName);
        OGR_API virtual FdoString* GetString(FdoString* propertyName);
        OGR_API virtual FdoLOBValue* GetLOB(FdoString* propertyName);
        OGR_API virtual FdoIStreamReader* GetLOBStreamReader(FdoString* propertyName );
        OGR_API virtual bool IsNull(FdoString* propertyName);
        OGR_API virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName);
        OGR_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);
        OGR_API virtual const FdoByte* GetGeometry(FdoString* propertyName, FdoInt32* len);
        OGR_API virtual FdoIRaster* GetRaster(FdoString* propertyName);
        OGR_API virtual bool ReadNext();
        OGR_API virtual void Close();
        virtual FdoDataType GetDataType( FdoString* propertyName ); 

    private:
        OgrConnection* m_connection;
        FdoIdentifierCollection* m_props;

        OGRLayer* m_poLayer;
        OGRFeature* m_poFeature;

        std::map<long, std::wstring> m_sprops;
        
        unsigned char* m_fgf;
        unsigned char* m_wkb;
        size_t m_fgflen;
};


//data reader -- returned when executing a select aggregates command
class OgrDataReader : public FdoIDataReader
{
    public:

        OGR_API OgrDataReader(OgrConnection* conn, OGRLayer* layer, FdoIdentifierCollection* ids);
    protected:

        OGR_API virtual void Dispose();
        OGR_API virtual ~OgrDataReader();

    //------------------------------------------------------------
    // FdoIDataReader implementation
    //------------------------------------------------------------
    public:

        OGR_API virtual FdoInt32 GetPropertyCount();
        OGR_API virtual FdoString* GetPropertyName(FdoInt32 index);
        OGR_API virtual FdoDataType GetDataType(FdoString* propertyName);
        OGR_API virtual FdoPropertyType GetPropertyType(FdoString* propertyName);
        OGR_API virtual bool GetBoolean(FdoString* propertyName);
        OGR_API virtual FdoByte GetByte(FdoString* propertyName);
        OGR_API virtual FdoDateTime GetDateTime(FdoString* propertyName);
        OGR_API virtual double GetDouble(FdoString* propertyName);
        OGR_API virtual FdoInt16 GetInt16(FdoString* propertyName);
        OGR_API virtual FdoInt32 GetInt32(FdoString* propertyName);
        OGR_API virtual FdoInt64 GetInt64(FdoString* propertyName);
        OGR_API virtual float GetSingle(FdoString* propertyName);
        OGR_API virtual FdoString* GetString(FdoString* propertyName);
        OGR_API virtual FdoLOBValue* GetLOB(FdoString* propertyName);
        OGR_API virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );
        OGR_API virtual bool IsNull(FdoString* propertyName);
        OGR_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);
        OGR_API virtual FdoIRaster* GetRaster(FdoString* propertyName);
        OGR_API virtual bool ReadNext();
        OGR_API virtual void Close();

    private:
        OgrConnection* m_connection;
        OGRLayer* m_poLayer;
        OGRFeature* m_poFeature;
        
        std::map<long, std::wstring> m_sprops;
        std::map<int, std::wstring> m_propnames;
        std::map<std::wstring, std::string> m_namemap;
        bool m_bUseNameMap;
};
