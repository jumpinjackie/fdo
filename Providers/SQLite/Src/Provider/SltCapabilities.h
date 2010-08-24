// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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

#ifndef SLCAPABILITIES_H
#define SLCAPABILITIES_H
#define  FDO_FUNCTION_VARIANCE  L"Variance"  // Returns the measure of the dispersion around their mean value.
#define  FDO_FUNCTION_LOG10     L"Log10"     // Returns the base-10 logarithm of the value.

#include <FdoExpressionEngine.h>
#include <Functions/Geometry/FdoFunctionX.h>
#include <Functions/Geometry/FdoFunctionY.h>
#include <Functions/Geometry/FdoFunctionZ.h>
#include <Functions/Geometry/FdoFunctionM.h>

class SltCapabilities  : public FdoIConnectionCapabilities,
                         public FdoISchemaCapabilities,
                         public FdoICommandCapabilities,
                         public FdoIFilterCapabilities,
                         public FdoIExpressionCapabilities,
                         public FdoIRasterCapabilities,
                         public FdoITopologyCapabilities,
                         public FdoIGeometryCapabilities
{

    //-------------------------------------------------------
    // FdoIConnectionCapabilities implementation
    //-------------------------------------------------------

    virtual FdoThreadCapability GetThreadCapability()               { return FdoThreadCapability_PerConnectionThreaded; }
    virtual bool SupportsLocking()                                  { return false; }
    virtual FdoLockType* GetLockTypes(FdoInt32& size)               { size=0; return NULL; }
    virtual bool SupportsTimeout()                                  { return false; }
    virtual bool SupportsTransactions()                             { return true; }
    virtual bool SupportsLongTransactions()                         { return false; }
    virtual bool SupportsSQL()                                      { return true;  }
    virtual bool SupportsConfiguration()                            { return false; }
    virtual bool SupportsMultipleSpatialContexts()                  { return true; }
    virtual bool SupportsCSysWKTFromCSysName()                      { return false; }
    virtual bool SupportsWrite()                                    { return true; }
    virtual bool SupportsMultiUserWrite()                           { return false; }
    virtual FdoSpatialContextExtentType* GetSpatialContextTypes(FdoInt32& length)
    {
        static FdoSpatialContextExtentType spatialTypes[] =
        {
                                                                //          FdoSpatialContextExtentType_Static,
                                                                            FdoSpatialContextExtentType_Dynamic
        };

        length = sizeof(spatialTypes) / sizeof(FdoSpatialContextExtentType);
        return spatialTypes;
    }
    virtual bool SupportsFlush()                                    { return false; }
    virtual bool SupportsJoins() const                              { return true; }
    virtual FdoInt32 GetJoinTypes() const                           { return (FdoInt32)(FdoJoinType_Cross|FdoJoinType_Inner|FdoJoinType_LeftOuter); }
    virtual bool SupportsSubSelects() const                         { return true; }


    //-------------------------------------------------------
    // FdoISchemaCapabilities implementation
    //-------------------------------------------------------

    virtual bool SupportsInheritance()                              { return false; }
    virtual bool SupportsMultipleSchemas()                          { return false; }
    virtual bool SupportsObjectProperties()                         { return false; }
    virtual bool SupportsAssociationProperties()                    { return false; }
    virtual bool SupportsSchemaOverrides()                          { return false; }
    virtual bool SupportsNetworkModel()                             { return false; }
    virtual bool SupportsAutoIdGeneration()                         { return true;  }
    virtual bool SupportsDataStoreScopeUniqueIdGeneration()         { return false; }
    virtual bool SupportsSchemaModification()                       { return true;  }
    virtual bool SupportsInclusiveValueRangeConstraints()           { return true; }
    virtual bool SupportsExclusiveValueRangeConstraints()           { return true; }
    virtual bool SupportsValueConstraintsList()                     { return true; }
    virtual bool SupportsNullValueConstraints()                     { return true;  }
    virtual bool SupportsUniqueValueConstraints()                   { return true;  }
    virtual bool SupportsCompositeUniqueValueConstraints()          { return true; }
    virtual bool SupportsCompositeId()                              { return true; }
    virtual bool SupportsDefaultValue()                             { return true; }


    virtual FdoInt64 GetMaximumDataValueLength(FdoDataType dataType){ return -1; }
    virtual FdoInt32 GetMaximumDecimalPrecision()                   { return -1; }
    virtual FdoInt32 GetMaximumDecimalScale()                       { return -1; }
    virtual FdoInt32 GetNameSizeLimit(FdoSchemaElementNameType name){ return -1; }
    virtual FdoString* GetReservedCharactersForName()               { return L".:"; }

    virtual FdoDataType* GetSupportedAutoGeneratedTypes(FdoInt32& length)
    {
        static FdoDataType dataTypes[] =
        {
                                                                //          FdoDataType_Boolean,
                                                                //          FdoDataType_Byte,
                                                                //          FdoDataType_DateTime,
                                                                //          FdoDataType_Decimal,
                                                                //          FdoDataType_Double,
                                                                //          FdoDataType_Int16,
                                                                            FdoDataType_Int32,
                                                                            FdoDataType_Int64,
                                                                //          FdoDataType_Single,
                                                                //          FdoDataType_String,
                                                                //          FdoDataType_BLOB,
                                                                //          FdoDataType_CLOB,
                                                                //          FdoDataType_UniqueID
        };

        length = sizeof(dataTypes) / sizeof(FdoDataType);
        return dataTypes;
    }

    virtual FdoDataType* GetSupportedIdentityPropertyTypes(FdoInt32& length)
    {
        static FdoDataType dataTypes[] =
        {
                                                                            FdoDataType_Boolean,
                                                                            FdoDataType_Byte,
                                                                            FdoDataType_DateTime,
                                                                            FdoDataType_Decimal,
                                                                            FdoDataType_Double,
                                                                            FdoDataType_Int16,
                                                                            FdoDataType_Int32,
                                                                            FdoDataType_Int64,
                                                                            FdoDataType_Single,
                                                                            FdoDataType_String,
                                                                //          FdoDataType_BLOB,
                                                                //          FdoDataType_CLOB,
                                                                //          FdoDataType_UniqueID
        };

        length = sizeof(dataTypes) / sizeof(FdoDataType);
        return dataTypes;
    }

    virtual FdoClassType* GetClassTypes(FdoInt32& length)
    {
        static FdoClassType classTypes[] =
        {
                                                                            FdoClassType_Class,
                                                                            FdoClassType_FeatureClass
        };

        length = sizeof(classTypes) / sizeof(FdoClassType);
        return classTypes;
    }

    virtual FdoDataType* GetDataTypes(FdoInt32& length)
    {
        static FdoDataType dataTypes[] =
        {
                                                                            FdoDataType_Boolean,
                                                                            FdoDataType_Byte,
                                                                            FdoDataType_DateTime,
                                                                            FdoDataType_Decimal,
                                                                            FdoDataType_Double,
                                                                            FdoDataType_Int16,
                                                                            FdoDataType_Int32,
                                                                            FdoDataType_Int64,
                                                                            FdoDataType_Single,
                                                                            FdoDataType_String,
                                                                            FdoDataType_BLOB,
                                                                //          FdoDataType_CLOB,
                                                                //          FdoDataType_UniqueID
        };

        length = sizeof(dataTypes) / sizeof(FdoDataType);
        return dataTypes;
    }

    //-------------------------------------------------------
    // FdoICommandCapabilities implementation
    //-------------------------------------------------------

    virtual FdoInt32* GetCommands(FdoInt32& size)
    {
    // read-write commands
        static const FdoInt32 commandsReadWrite[] =
        {
                                                                            FdoCommandType_Select,
                                                                            FdoCommandType_Insert,
                                                                            FdoCommandType_Delete,
                                                                            FdoCommandType_Update,
                                                                            FdoCommandType_DescribeSchema,
                                                                            FdoCommandType_ApplySchema,
                                                                //          FdoCommandType_DestroySchema,
                                                                //          FdoCommandType_ActivateSpatialContext,
                                                                            FdoCommandType_CreateSpatialContext,
                                                                //          FdoCommandType_DestroySpatialContext,
                                                                            FdoCommandType_GetSpatialContexts,
                                                                //          FdoCommandType_CreateMeasureUnit,
                                                                //          FdoCommandType_DestroyMeasureUnit,
                                                                //          FdoCommandType_GetMeasureUnits,
                                                                            FdoCommandType_SQLCommand,
                                                                //          FdoCommandType_AcquireLock,
                                                                //          FdoCommandType_GetLockInfo,
                                                                //          FdoCommandType_GetLockedObjects,
                                                                //          FdoCommandType_GetLockOwners,
                                                                //          FdoCommandType_ReleaseLock,
                                                                //          FdoCommandType_ActivateLongTransaction,
                                                                //          FdoCommandType_CommitLongTransaction,
                                                                //          FdoCommandType_CreateLongTransaction,
                                                                //          FdoCommandType_GetLongTransactions,
                                                                //          FdoCommandType_FreezeLongTransaction,
                                                                //          FdoCommandType_RollbackLongTransaction,
                                                                //          FdoCommandType_ActivateLongTransactionCheckpoint,
                                                                //          FdoCommandType_CreateLongTransactionCheckpoint,
                                                                //          FdoCommandType_GetLongTransactionCheckpoints,
                                                                //          FdoCommandType_RollbackLongTransactionCheckpoint,
                                                                //          FdoCommandType_ChangeLongTransactionPrivileges,
                                                                //          FdoCommandType_GetLongTransactionPrivileges,
                                                                //          FdoCommandType_ChangeLongTransactionSet,
                                                                //          FdoCommandType_GetLongTransactionsInSet,
                                                                            FdoCommandType_SelectAggregates,
                                                                            FdoCommandType_CreateDataStore,
                                                                //          FdoCommandType_DestroyDataStore,
                                                                            FdoCommandType_ExtendedSelect,
        };

        size = sizeof(commandsReadWrite) / sizeof(FdoCommandType);
        return (FdoInt32*)commandsReadWrite;
    }
    
    virtual bool SupportsParameters()                               { return true; }
    //virtual bool SupportsTimeout()                                { return false; }
    virtual bool SupportsSelectExpressions()                        { return true;  }
    virtual bool SupportsSelectFunctions()                          { return true;  }
    virtual bool SupportsSelectDistinct()                           { return true;  }
    virtual bool SupportsSelectOrdering()                           { return true; }
    virtual bool SupportsSelectGrouping()                           { return false; }


    //-------------------------------------------------------
    // FdoIFilterCapabilities implementation
    //-------------------------------------------------------

    virtual FdoConditionType* GetConditionTypes(FdoInt32& length)
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
    
    virtual FdoSpatialOperations* GetSpatialOperations(FdoInt32& length)
    {
        static FdoSpatialOperations spatialOperations[] =
        {
                                                                            FdoSpatialOperations_Contains,
                                                                            FdoSpatialOperations_Crosses,
                                                                            FdoSpatialOperations_Disjoint,
                                                                            FdoSpatialOperations_Equals,
                                                                            FdoSpatialOperations_Intersects,
                                                                            FdoSpatialOperations_Overlaps,
                                                                            FdoSpatialOperations_Touches,
                                                                            FdoSpatialOperations_Within,
                                                                            FdoSpatialOperations_CoveredBy,
                                                                            FdoSpatialOperations_Inside,
                                                                            FdoSpatialOperations_EnvelopeIntersects
        };

        length = sizeof(spatialOperations) / sizeof(FdoSpatialOperations);
        return spatialOperations;
    }
    
    virtual FdoDistanceOperations* GetDistanceOperations(FdoInt32& length)
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
    
    virtual bool SupportsGeodesicDistance()                         { return false; }
    virtual bool SupportsNonLiteralGeometricOperations()            { return false; }

    //-------------------------------------------------------
    // FdoIExpressionCapabilities implementation
    //-------------------------------------------------------

    virtual FdoExpressionType* GetExpressionTypes(FdoInt32& length)
    {
        static FdoExpressionType expressionTypes[] =
        {
                                                                            FdoExpressionType_Basic,
                                                                            FdoExpressionType_Function,
                                                                //          FdoExpressionType_Parameter
        };

        length = sizeof(expressionTypes) / sizeof(FdoExpressionType);
        return expressionTypes;
    }
    
    virtual FdoFunctionDefinitionCollection* GetFunctions()
    {
        FdoFunctionDefinitionCollection* ret = FdoFunctionDefinitionCollection::Create ();
    
        // Add well-known functions we support to the list:
        FdoPtr<FdoFunctionDefinitionCollection> wellKnownFunctions = FdoExpressionEngine::GetStandardFunctions();
        FdoPtr<FdoFunctionDefinition> wellKnownFunction;

        // we will remove all this after we will support all Standard functions
        // TODO process all extra parameters e.g. 'ALL'/'DISTINCT'
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_COUNT);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_MIN);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_AVG);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_MAX);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_SUM);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_SPATIALEXTENTS);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_MEDIAN);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_STDDEV);
        ret->Add(wellKnownFunction);
/*
        // for now just avoid exposing extra functions since it needs to be registered in EE
        // Is safe to copy the signatures since we don't alter them and functions get the same parameters
        // this will save us memory allocation and time...
        FdoPtr<FdoSignatureDefinitionCollection> signatures = FdoSignatureDefinitionCollection::Create();
        FdoPtr<FdoReadOnlySignatureDefinitionCollection> rdSignatures = wellKnownFunction->GetSignatures();
        if (rdSignatures != NULL && rdSignatures->GetCount())
        {
            for (int idx = 0; idx < rdSignatures->GetCount(); idx++)
            {
                FdoPtr<FdoSignatureDefinition> item = rdSignatures->GetItem(idx);
                signatures->Add(item);
            }
        }
        // TODO localize this
        FdoString *desc = L"Represents a measure of the dispersion around their mean value.";
        wellKnownFunction = FdoFunctionDefinition::Create(FDO_FUNCTION_VARIANCE, desc, true, signatures, FdoFunctionCategoryType_Aggregate);
        ret->Add(wellKnownFunction);
*/
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_ABS);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_ACOS);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_ASIN);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_ATAN);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_ATAN2);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_COS);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_EXP);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_LN);
        ret->Add(wellKnownFunction);
/*
        // for now just avoid exposing extra functions since it needs to be registered in EE
        // Is safe to copy the signatures since we don't alter them and functions get the same parameters
        // this will save us memory allocation and time...
        signatures = FdoSignatureDefinitionCollection::Create();
        rdSignatures = wellKnownFunction->GetSignatures();
        if (rdSignatures != NULL && rdSignatures->GetCount())
        {
            for (int idx = 0; idx < rdSignatures->GetCount(); idx++)
            {
                FdoPtr<FdoSignatureDefinition> item = rdSignatures->GetItem(idx);
                signatures->Add(item);
            }
        }
        // TODO localize this
        desc = L"Represents the base-10 logarithm of the value.";
        wellKnownFunction = FdoFunctionDefinition::Create(FDO_FUNCTION_LOG10, desc, true, signatures, FdoFunctionCategoryType_Math);
        ret->Add(wellKnownFunction);
*/
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_LOG);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_MOD);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_POWER);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_REMAINDER);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_SIN);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_SQRT);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TAN);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_CEIL);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_FLOOR);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_ROUND);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_SIGN);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TRUNC);
        ret->Add(wellKnownFunction);
        
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_CONCAT);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_INSTR);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_LENGTH);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_LOWER);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_LPAD);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_LTRIM);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_RPAD);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_RTRIM);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_SOUNDEX);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_SUBSTR);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TRANSLATE);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TRIM);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_UPPER);
        ret->Add(wellKnownFunction);

        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_ADDMONTHS);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_CURRENTDATE);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_EXTRACT);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_EXTRACTTODOUBLE);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_EXTRACTTOINT);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_MONTHSBETWEEN);
        ret->Add(wellKnownFunction);

        // Add function X to the list of supported function
        FdoPtr<FdoExpressionEngineIFunction> funcX = FdoFunctionX::Create();
        ret->Add(FdoPtr<FdoFunctionDefinition>(funcX->GetFunctionDefinition()));

        // Add function Y to the list of supported function
        FdoPtr<FdoExpressionEngineIFunction> funcY = FdoFunctionY::Create();
        ret->Add(FdoPtr<FdoFunctionDefinition>(funcY->GetFunctionDefinition()));

        // Add function Z to the list of supported function
        FdoPtr<FdoExpressionEngineIFunction> funcZ = FdoFunctionZ::Create();
        ret->Add(FdoPtr<FdoFunctionDefinition>(funcZ->GetFunctionDefinition()));

        // Add function M to the list of supported function
        FdoPtr<FdoExpressionEngineIFunction> funcM = FdoFunctionM::Create();
        ret->Add(FdoPtr<FdoFunctionDefinition>(funcM->GetFunctionDefinition()));

        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_AREA2D);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_LENGTH2D);
        ret->Add(wellKnownFunction);

        // TODO process extra parameter
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TODATE);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TODOUBLE);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TOFLOAT);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TOINT32);
        ret->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TOINT64);
        ret->Add(wellKnownFunction);
        // TODO process extra parameter
        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_TOSTRING);
        ret->Add(wellKnownFunction);

        wellKnownFunction = wellKnownFunctions->GetItem(                    FDO_FUNCTION_NULLVALUE);
        ret->Add(wellKnownFunction);
        
        return ret;
    }

    //-------------------------------------------------------
    // FdoIRasterCapabilities implementation
    //-------------------------------------------------------

    virtual bool SupportsRaster ()                                  { return false; }
    virtual bool SupportsStitching ()                               { return false; }
    virtual bool SupportsSubsampling ()                             { return false; }
    virtual bool SupportsDataModel (FdoRasterDataModel* model)      { return false; }

    //-------------------------------------------------------
    // FdoITopologyCapabilities implementation
    //-------------------------------------------------------

    virtual bool SupportsTopology()                                 { return false; }
    virtual bool SupportsTopologicalHierarchy()                     { return false; }
    virtual bool BreaksCurveCrossingsAutomatically()                { return false; }
    virtual bool ActivatesTopologyByArea()                          { return false; }
    virtual bool ConstrainsFeatureMovements()                       { return false; }


    //-------------------------------------------------------
    // FdoIGeometryCapabilities implementation
    //-------------------------------------------------------

    virtual FdoGeometryType* GetGeometryTypes( FdoInt32& length)
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
                                                                            FdoGeometryType_CurveString,
                                                                            FdoGeometryType_CurvePolygon,
                                                                            FdoGeometryType_MultiCurveString,
                                                                            FdoGeometryType_MultiCurvePolygon
        };

        length = sizeof(geomTypes) / sizeof(FdoGeometryType);
        return geomTypes;
    }
    
    virtual FdoGeometryComponentType* GetGeometryComponentTypes( FdoInt32& length )
    {
        static FdoGeometryComponentType compTypes[] = 
        {
                                                                            FdoGeometryComponentType_LinearRing,
                                                                            FdoGeometryComponentType_CircularArcSegment,
                                                                            FdoGeometryComponentType_LineStringSegment,
                                                                            FdoGeometryComponentType_Ring
        };

        length = sizeof(compTypes) / sizeof(FdoGeometryComponentType);
        return compTypes;
    }
    
    virtual FdoInt32 GetDimensionalities()
        { 
            return 
                                                                            FdoDimensionality_XY 
                                                                          | FdoDimensionality_Z 
                                                                          | FdoDimensionality_M; 
    }

    //----------------------------------------------------------------------
    //  Bookkeeping code
    //----------------------------------------------------------------------

public:
    
    SltCapabilities() : m_refCount(1) {}

    SLT_IMPLEMENT_REFCOUNTING

};

#endif
