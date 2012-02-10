// 
// Copyright (C) 2008  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 

#include "stdafx.h"

static wchar_t* DataType (FdoDataType type)
{
    wchar_t* ret;

    switch (type)
    {
        case FdoDataType_Boolean: // Represents a Boolean value of true or false.
            ret = L"FdoDataType_Boolean";
            break;
        case FdoDataType_Byte: // Represents unsigned 8-bit integers with values between 0 and 255.
            ret = L"FdoDataType_Byte";
            break;
        case FdoDataType_DateTime: // Represents a date and time value.
            ret = L"FdoDataType_DateTime";
            break;
        case FdoDataType_Decimal: // Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28 with 28-29 significant digits.
            ret = L"FdoDataType_Decimal";
            break;
        case FdoDataType_Double: // Represents a floating point value ranging from approximately 5.0 x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
            ret = L"FdoDataType_Double";
            break;
        case FdoDataType_Int16: // Represents signed 16-bit integers with values between -32768 and 32767.
            ret = L"FdoDataType_Int16";
            break;
        case FdoDataType_Int32: // Represents signed 32-bit integers with values between -2147483648 and 2147483647.
            ret = L"FdoDataType_Int32";
            break;
        case FdoDataType_Int64: // Represents signed 64-bit integers with values between -9223372036854775808 and 9223372036854775807.
            ret = L"FdoDataType_Int64";
            break;
        case FdoDataType_Single: // Represents floating point values ranging from approximately 1.5 x 10^-45 to 3.4 x 10^38 with a precision of 7 digits.
            ret = L"FdoDataType_Single";
            break;
        case FdoDataType_String: // Represents a Unicode character strings.
            ret = L"FdoDataType_String";
            break;
        case FdoDataType_BLOB: // Represents a binary large object stored as a collection of bytes.
            ret = L"FdoDataType_BLOB";
            break;
        case FdoDataType_CLOB: // Represents a character large object stored as a collection of  characters.
            ret = L"FdoDataType_CLOB";
            break;
        default:
            ret = L"Unknown return type";
            break;
    }

    return (ret);
}

static wchar_t* Command (int command)
{
    wchar_t* ret;

    switch (command)
    {
        case FdoCommandType_Select:
            ret = L"FdoCommandType_Select";
            break;
        case FdoCommandType_Insert:
            ret = L"FdoCommandType_Insert";
            break;
        case FdoCommandType_Delete:
            ret = L"FdoCommandType_Delete";
            break;
        case FdoCommandType_Update:
            ret = L"FdoCommandType_Update";
            break;
        case FdoCommandType_DescribeSchema:
            ret = L"FdoCommandType_DescribeSchema";
            break;
        case FdoCommandType_DescribeSchemaMapping:
            ret = L"FdoCommandType_DescribeSchemaMapping";
            break;
        case FdoCommandType_ApplySchema:
            ret = L"FdoCommandType_ApplySchema";
            break;
        case FdoCommandType_DestroySchema:
            ret = L"FdoCommandType_DestroySchema";
            break;
        case FdoCommandType_ActivateSpatialContext:
            ret = L"FdoCommandType_ActivateSpatialContext";
            break;
        case FdoCommandType_CreateSpatialContext:
            ret = L"FdoCommandType_CreateSpatialContext";
            break;
        case FdoCommandType_DestroySpatialContext:
            ret = L"FdoCommandType_DestroySpatialContext";
            break;
        case FdoCommandType_GetSpatialContexts:
            ret = L"FdoCommandType_GetSpatialContexts";
            break;
        case FdoCommandType_CreateMeasureUnit:
            ret = L"FdoCommandType_CreateMeasureUnit";
            break;
        case FdoCommandType_DestroyMeasureUnit:
            ret = L"FdoCommandType_DestroyMeasureUnit";
            break;
        case FdoCommandType_GetMeasureUnits:
            ret = L"FdoCommandType_GetMeasureUnits";
            break;
        case FdoCommandType_SQLCommand:
            ret = L"FdoCommandType_SQLCommand";
            break;
        case FdoCommandType_AcquireLock:
            ret = L"FdoCommandType_AcquireLock";
            break;
        case FdoCommandType_GetLockInfo:
            ret = L"FdoCommandType_GetLockInfo";
            break;
        case FdoCommandType_GetLockedObjects:
            ret = L"FdoCommandType_GetLockedObjects";
            break;
        case FdoCommandType_GetLockOwners:
            ret = L"FdoCommandType_GetLockOwners";
            break;
        case FdoCommandType_ReleaseLock:
            ret = L"FdoCommandType_ReleaseLock";
            break;
        case FdoCommandType_ActivateLongTransaction:
            ret = L"FdoCommandType_ActivateLongTransaction";
            break;
        case FdoCommandType_DeactivateLongTransaction:
            ret = L"FdoCommandType_DeactivateLongTransaction";
            break;
        case FdoCommandType_CommitLongTransaction:
            ret = L"FdoCommandType_CommitLongTransaction";
            break;
        case FdoCommandType_CreateLongTransaction:
            ret = L"FdoCommandType_CreateLongTransaction";
            break;
        case FdoCommandType_GetLongTransactions:
            ret = L"FdoCommandType_GetLongTransactions";
            break;
        case FdoCommandType_FreezeLongTransaction:
            ret = L"FdoCommandType_FreezeLongTransaction";
            break;
        case FdoCommandType_RollbackLongTransaction:
            ret = L"FdoCommandType_RollbackLongTransaction";
            break;
        case FdoCommandType_ActivateLongTransactionCheckpoint:
            ret = L"FdoCommandType_ActivateLongTransactionCheckpoint";
            break;
        case FdoCommandType_CreateLongTransactionCheckpoint:
            ret = L"FdoCommandType_CreateLongTransactionCheckpoint";
            break;
        case FdoCommandType_GetLongTransactionCheckpoints:
            ret = L"FdoCommandType_GetLongTransactionCheckpoints";
            break;
        case FdoCommandType_RollbackLongTransactionCheckpoint:
            ret = L"FdoCommandType_RollbackLongTransactionCheckpoint";
            break;
        case FdoCommandType_ChangeLongTransactionPrivileges:
            ret = L"FdoCommandType_ChangeLongTransactionPrivileges";
            break;
        case FdoCommandType_GetLongTransactionPrivileges:
            ret = L"FdoCommandType_GetLongTransactionPrivileges";
            break;
        case FdoCommandType_ChangeLongTransactionSet:
            ret = L"FdoCommandType_ChangeLongTransactionSet";
            break;
        case FdoCommandType_GetLongTransactionsInSet:
            ret = L"FdoCommandType_GetLongTransactionsInSet";
            break;
        case FdoCommandType_NetworkShortestPath:
            ret = L"FdoCommandType_NetworkShortestPath";
            break;
        case FdoCommandType_NetworkAllPaths:
            ret = L"FdoCommandType_NetworkAllPaths";
            break;
        case FdoCommandType_NetworkReachableNodes:
            ret = L"FdoCommandType_NetworkReachableNodes";
            break;
        case FdoCommandType_NetworkReachingNodes:
            ret = L"FdoCommandType_NetworkReachingNodes";
            break;
        case FdoCommandType_NetworkNearestNeighbors:
            ret = L"FdoCommandType_NetworkNearestNeighbors";
            break;
        case FdoCommandType_NetworkWithinCost:
            ret = L"FdoCommandType_NetworkWithinCost";
            break;
        case FdoCommandType_NetworkTSP:
            ret = L"FdoCommandType_NetworkTSP";
            break;
        case FdoCommandType_ActivateTopologyArea:
            ret = L"FdoCommandType_ActivateTopologyArea";
            break;
        case FdoCommandType_DeactivateTopologyArea:
            ret = L"FdoCommandType_DeactivateTopologyArea";
            break;
        case FdoCommandType_ActivateTopologyInCommandResult:
            ret = L"FdoCommandType_ActivateTopologyInCommandResult";
            break;
        case FdoCommandType_DeactivateTopologyInCommandResults:
            ret = L"FdoCommandType_DeactivateTopologyInCommandResults";
            break;
        case FdoCommandType_SelectAggregates:
            ret = L"FdoCommandType_SelectAggregates";
            break;
        default:
            ret = L"Unknown command";
            break;
    }

    return (ret);
}
static wchar_t* Lock (FdoLockType lock)
{
    wchar_t* ret;

    switch (lock)
    {
        case FdoLockType_None:
            ret = L"FdoLockType_None";
            break;
        case FdoLockType_Shared:
            ret = L"FdoLockType_Shared";
            break;
        case FdoLockType_Exclusive:
            ret = L"FdoLockType_Exclusive";
            break;
        case FdoLockType_Transaction:
            ret = L"FdoLockType_Transaction";
            break;
        case FdoLockType_Unsupported:
            ret = L"FdoLockType_Unsupported";
            break;
        case FdoLockType_AllLongTransactionExclusive:
            ret = L"FdoLockType_AllLongTransactionExclusive";
            break;
        case FdoLockType_LongTransactionExclusive:
            ret = L"FdoLockType_LongTransactionExclusive";
            break;
    }

    return (ret);
}

static wchar_t* GeometryType (FdoGeometryType type)
{
    wchar_t* ret;

    switch (type)
    {
        case FdoGeometryType_Point:
            ret = L"Point";
            break;
        case FdoGeometryType_LineString:
            ret = L"LineString";
            break;
        case FdoGeometryType_Polygon:
            ret = L"Polygon";
            break;
        case FdoGeometryType_MultiPoint:
            ret = L"MultiPoint";
            break;
        case FdoGeometryType_MultiLineString:
            ret = L"MultiLineString";
            break;
        case FdoGeometryType_MultiPolygon:
            ret = L"MultiPolygon";
            break;
        case FdoGeometryType_MultiGeometry:
            ret = L"MultiGeometry";
            break;
        case FdoGeometryType_CurveString:
            ret = L"CurveString";
            break;
        case FdoGeometryType_CurvePolygon:
            ret = L"CurvePolygon";
            break;
        case FdoGeometryType_MultiCurveString:
            ret = L"MultiCurveString";
            break;
        case FdoGeometryType_MultiCurvePolygon:
            ret = L"MultiCurvePolygon";
            break;
    }

    return (ret);
}

static wchar_t* GeometryComponentType (FdoGeometryComponentType type)
{
    wchar_t* ret;

    switch (type)
    {
        case FdoGeometryComponentType_LinearRing:
            ret = L"LinearRing";
            break;
        case FdoGeometryComponentType_CircularArcSegment:
            ret = L"CircularArcSegment";
            break;
        case FdoGeometryComponentType_LineStringSegment:
            ret = L"LineStringSegment";
            break;
        case FdoGeometryComponentType_Ring:
            ret = L"Ring";
            break;
    }
    return (ret);
}

#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif

{
    IProviderRegistry* registry;
    const FdoProviderCollection* providers;
    const FdoProvider* provider;
    IConnectionManager* manager;
    FdoIConnection* connection;
    FdoIConnectionCapabilities* connection_capabilities;
    FdoThreadCapability threads;
    FdoSpatialContextExtentType* contexts;
    FdoICommandCapabilities* command_capabilities;
    wchar_t* s;
    int length;
    int* commands;
    FdoIExpressionCapabilities* expression_capabilities;
    FdoExpressionType* expression_types;
    FdoFunctionDefinitionCollection* definitions;
    FdoReadOnlyArgumentDefinitionCollection* args;
    FdoArgumentDefinition* arg;
    FdoIFilterCapabilities* filter_capabilities;
    FdoConditionType* condition_types;
    FdoSpatialOperations* spatial_operations;
    FdoDistanceOperations* distance_operations;
    FdoISchemaCapabilities* schema_capabilities;
    FdoClassType* class_types;
    FdoDataType* data_types;
    FdoIGeometryCapabilities* geometry_capabilities;
    FdoGeometryType* geometry_types;
    FdoGeometryComponentType* geometry_component_types;
    FdoInt32 dimensionalities;
    FdoITopologyCapabilities* topology_capabilities;
    FdoIRasterCapabilities* raster_capabilities;
    int ret;

    ret = 1;
    
    try
    {
        registry = FdoFeatureAccessManager::GetProviderRegistry ();
        providers = registry->GetProviders ();
        for (int i = 0; i < providers->GetCount (); i++)
        {
            provider = providers->GetItem (i);
            wprintf (L"%ls\n", provider->GetName ());
            manager = FdoFeatureAccessManager::GetConnectionManager ();
            connection = manager->CreateConnection (provider->GetName ());

            // display the connection capabilities
            connection_capabilities = connection->GetConnectionCapabilities ();
            threads = connection_capabilities->GetThreadCapability();
            switch (threads)
            {
                case FdoThreadCapability_SingleThreaded:
                    s = L"The provider is not thread safe.";
                    break;
                case FdoThreadCapability_PerConnectionThreaded:
                    s = L"The provider supports a single thread per connection.";
                    break;
                case FdoThreadCapability_PerCommandThreaded:
                    s = L"The provider supports a single thread per command.";
                    break;
                case FdoThreadCapability_MultiThreaded:
                    s = L"The provider is fully multi-threaded.";
                    break;
                default:
                    s = L"Unknown thread capability type.";
                    break;
            }
            wprintf (L"    %ls\n", s);
            contexts = connection_capabilities->GetSpatialContextTypes (length);
            for (int i = 0; i < length; i++)
            {
                switch (*contexts)
                {
                    case FdoSpatialContextExtentType_Static:
                        s = L"static";
                        break;
                    case FdoSpatialContextExtentType_Dynamic:
                        s = L"dynamic";
                        break;
                    default:
                        s = L"Unknown spatial context type.";
                        break;
                }
                contexts++;
                wprintf (L"    The provider spatial context extent is %ls.\n", s);
            }
            wprintf (L"    The provider does %lssupport locking.\n", (connection_capabilities->SupportsLocking () ? L"" : L"not "));
            if (connection_capabilities->SupportsLocking ())
            {
                FdoInt32 size;
                FdoLockType* types = connection_capabilities->GetLockTypes (size);
                for (int i = 0; i < size; i++)
                {
                    wprintf (L"        The provider supports lock type %ls.\n", Lock (*types));
                    types++;
                }
            }
            wprintf (L"    The provider does %lssupport connection timeout.\n", (connection_capabilities->SupportsTimeout () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport transactions.\n", (connection_capabilities->SupportsTransactions () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport long transactions.\n", (connection_capabilities->SupportsLongTransactions () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport SQL commands.\n", (connection_capabilities->SupportsSQL () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport configuration.\n", (connection_capabilities->SupportsConfiguration () ? L"" : L"not "));
            connection_capabilities->Release ();

            // display the command capabilities
            command_capabilities = connection->GetCommandCapabilities ();
            commands = command_capabilities->GetCommands (length);
            for (int i = 0; i < length; i++)
            {
                wprintf (L"    The provider supports command %ls.\n", Command (*commands));
                commands++;
            }
            wprintf (L"    The provider commands do %lssupport parameterization.\n", (command_capabilities->SupportsParameters () ? L"" : L"not "));
            wprintf (L"    The provider commands do %lssupport timeout.\n", (command_capabilities->SupportsTimeout () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport select functions in Select commands.\n", (command_capabilities->SupportsSelectFunctions () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport select distinct in Select commands.\n", (command_capabilities->SupportsSelectDistinct () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport ordering in Select commands.\n", (command_capabilities->SupportsSelectOrdering () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport grouping in Select commands.\n", (command_capabilities->SupportsSelectGrouping () ? L"" : L"not "));
            command_capabilities->Release ();

            expression_capabilities = connection->GetExpressionCapabilities ();
            expression_types = expression_capabilities->GetExpressionTypes (length);
            for (int i = 0; i < length; i++)
            {
                switch (*expression_types)
                {
                    case FdoExpressionType_Basic:
                        s = L"basic arithmetic expressions";
                        break;
                    case FdoExpressionType_Function:
                        s = L"function evaluations";
                        break;
                    case FdoExpressionType_Parameter:
                        s = L"parameter substitutions";
                        break;
                    default:
                        s = L"Unknown expression type";
                        break;
                }
                expression_types++;
                wprintf (L"    The provider expressions support %ls.\n", s);
            }
            definitions = expression_capabilities->GetFunctions();
            if (0 != definitions->GetCount())
                wprintf (L"    Functions:\n", s);
            for (int i = 0; i < definitions->GetCount(); i++)
            {
                FdoFunctionDefinition*  definition;

                definition = definitions->GetItem(i);
                wprintf (L"        // %ls\n", definition->GetDescription ());
                wprintf (L"        %ls ", DataType (definition->GetReturnType ()));
                wprintf (L" %ls(", definition->GetName ());
                args = definition->GetArguments ();
                for (int j = 0; j < args->GetCount (); j++)
                {
                    if (0 != j)
                        wprintf (L", ");
                    arg = args->GetItem (j);
                    wprintf (L"%ls %ls", DataType(arg->GetDataType ()), arg->GetName()); // ? GetDescription()
                    arg->Release ();
                }
                wprintf (L");\n");
                definition->Release();
            }
            definitions->Release();
            expression_capabilities->Release ();

            filter_capabilities = connection->GetFilterCapabilities ();
            condition_types = filter_capabilities->GetConditionTypes (length);
            for (int i = 0; i < length; i++)
            {
                switch (*condition_types)
                {
                    case FdoConditionType_Comparison: // Tests if one expression is equal, not equal, greater than, less than, greater than or equal to, or less than or equal to another expression.
                        s = L"FdoConditionType_Comparison";
                        break;
                    case FdoConditionType_Like: // Tests if the value of a specified data property is matches a specified pattern.
                        s = L"FdoConditionType_Like";
                        break;
                    case FdoConditionType_In: // Tests if the value of a specified data property is within a given set of literal values.
                        s = L"FdoConditionType_In";
                        break;
                    case FdoConditionType_Null: // Tests if the value of a specified data property is null.
                        s = L"FdoConditionType_Null";
                        break;
                    case FdoConditionType_Spatial: // Tests whether the value of a geometric property and a literal geometric value satisfy the spatial relationship implied by the operation.
                        s = L"FdoConditionType_Spatial";
                        break;
                    case FdoConditionType_Distance: // Tests whether the value of a geometric property is within or beyond a specified distance of a literal geometric value.
                        s = L"FdoConditionType_Distance";
                        break;
                    default:
                        s = L"Unknown condition type.";
                        break;
                }
                condition_types++;
                wprintf (L"    The provider filter conditions include %ls.\n", s);
            }
            spatial_operations = filter_capabilities->GetSpatialOperations (length);
            for (int i = 0; i < length; i++)
            {
                switch (*spatial_operations)
                {
                    case FdoSpatialOperations_Contains: // Test to see if the geometric property value spatially contains the literal geometric value.
                        s = L"FdoSpatialOperations_Contains";
                        break;
                    case FdoSpatialOperations_Crosses: // Test to see if the geometric property value spatially crosses the given geometry.
                        s = L"FdoSpatialOperations_Crosses";
                        break;
                    case FdoSpatialOperations_Disjoint: // Test to see if the geometric property value spatially is spatially disjoint from the given geometry.
                        s = L"FdoSpatialOperations_Disjoint";
                        break;
                    case FdoSpatialOperations_Equals: // Test to see if the geometric property value is spatially equal to the given geometry.
                        s = L"FdoSpatialOperations_Equals";
                        break;
                    case FdoSpatialOperations_Intersects: // Test to see if the geometric property value spatially intersects the given geometry.
                        s = L"FdoSpatialOperations_Intersects";
                        break;
                    case FdoSpatialOperations_Overlaps: // Test to see if the geometric property value spatially overlaps the given geometry.
                        s = L"FdoSpatialOperations_Overlaps";
                        break;
                    case FdoSpatialOperations_Touches: // Test to see if the geometric property value spatially touches the given geometry.
                        s = L"FdoSpatialOperations_Touches";
                        break;
                    case FdoSpatialOperations_Within: // Test to see if the geometric property value is spatially within the given geometry.
                        s = L"FdoSpatialOperations_Within";
                        break;
                    case FdoSpatialOperations_CoveredBy: // Test to see if the geometric property value is covered by the interior or boundary of the given geometry.
                        s = L"FdoSpatialOperations_CoveredBy";
                        break;
                    case FdoSpatialOperations_Inside:
                        s = L"FdoSpatialOperations_Inside";
                        break;
                    case FdoSpatialOperations_EnvelopeIntersects: // Test to see if the envelope of the referenced geometric property value spatially intersects the given geometry.
                        s = L"FdoSpatialOperations_EnvelopeIntersects";
						break;
                    default:
                        s = L"Unknown spatial operation";
                        break;
                }
                spatial_operations++;
                wprintf (L"    The provider spatial operations include %ls.\n", s);
            }

            distance_operations = filter_capabilities->GetDistanceOperations (length);
            for (int i = 0; i < length; i++)
            {
                switch (*distance_operations)
                {
                    case FdoDistanceOperations_Beyond: // Test to see if the geometric property value lies beyond a specified distance of a literal geometric value.
                        s = L"FdoDistanceOperations_Beyond";
                        break;
                    case FdoDistanceOperations_Within: // Test to see if the geometric property value lies within a specified distance of a literal geometric value.
                        s = L"FdoDistanceOperations_Within";
                        break;
                    default:
                        s = L"Unknown distance operation.";
                        break;
                }
                distance_operations++;
                wprintf (L"    The provider distance operations include %ls.\n", s);
            }

            wprintf (L"    The provider does %lssupport geodesic distance measurement.\n", (filter_capabilities->SupportsGeodesicDistance () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport inter-property spatial operations.\n", (filter_capabilities->SupportsNonLiteralGeometricOperations () ? L"" : L"not "));
            filter_capabilities->Release ();

            schema_capabilities = connection->GetSchemaCapabilities ();
            class_types = schema_capabilities->GetClassTypes (length);
            for (int i = 0; i < length; i++)
            {
                switch (*class_types)
                {
                    case FdoClassType_Class: // Represents the concrete class type Class.
                        s = L"FdoClassType_Class";
                        break;
                    case FdoClassType_FeatureClass: // Represents the concrete class type FeatureClass.
                        s = L"FdoClassType_FeatureClass";
                        break;
                    case FdoClassType_NetworkClass:
                        s = L"FdoClassType_NetworkClass";
                        break;
                    case FdoClassType_NetworkLayerClass:
                        s = L"FdoClassType_NetworkLayerClass";
                        break;
                    case FdoClassType_NetworkNodeClass:
                        s = L"FdoClassType_NetworkNodeClass";
                        break;
                    case FdoClassType_NetworkLinkClass:
                        s = L"FdoClassType_NetworkLinkClass";
                        break;
                    default:
                        s = L"unknown class type";
                        break;
                }
                class_types++;
                wprintf (L"    The provider schema class types include %ls.\n", s);
            }

            data_types = schema_capabilities->GetDataTypes (length);
            for (int i = 0; i < length; i++)
            {
                wprintf (L"    The provider data types include %ls.\n", DataType (*data_types));
                data_types++;
            }
            wprintf (L"    The provider does %lssupport inheritance.\n", (schema_capabilities->SupportsInheritance () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport multiple schemas.\n", (schema_capabilities->SupportsMultipleSchemas () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport object properties.\n", (schema_capabilities->SupportsObjectProperties () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport association properties.\n", (schema_capabilities->SupportsAssociationProperties () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport schema overrides.\n", (schema_capabilities->SupportsSchemaOverrides () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport the network model.\n", (schema_capabilities->SupportsNetworkModel () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport automatic feature id generation.\n", (schema_capabilities->SupportsAutoIdGeneration () ? L"" : L"not "));
            wprintf (L"    The provider does %lssupport data store scope unique id generation.\n", (schema_capabilities->SupportsDataStoreScopeUniqueIdGeneration () ? L"" : L"not "));
            data_types = schema_capabilities->GetSupportedAutoGeneratedTypes (length);
            for (int i = 0; i < length; i++)
            {
                wprintf (L"    The provider data types for auto-generation include %ls.\n", DataType (*data_types));
                data_types++;
            }
            wprintf (L"    The provider does %lssupport schema modification.\n", (schema_capabilities->SupportsSchemaModification () ? L"" : L"not "));
            schema_capabilities->Release ();

            geometry_capabilities = connection->GetGeometryCapabilities ();
            geometry_types = geometry_capabilities->GetGeometryTypes (length);
            for (int i = 0; i < length; i++)
            {
                wprintf (L"    The provider geometry types include %ls.\n", GeometryType (*geometry_types));
                geometry_types++;
            }
            geometry_component_types = geometry_capabilities->GetGeometryComponentTypes (length);
            for (int i = 0; i < length; i++)
            {
                wprintf (L"    The provider geometry component types include %ls.\n", GeometryComponentType (*geometry_component_types));
                geometry_component_types++;
            }
            dimensionalities = geometry_capabilities->GetDimensionalities ();
            wprintf (L"    The provider geometry dimensionalities include XY");
            if (0 != (FdoDimensionality_Z & dimensionalities))
                wprintf (L", Z");
            if (0 != (FdoDimensionality_M & dimensionalities))
                wprintf (L", M");
            wprintf (L"\n");
            geometry_capabilities->Release ();

            topology_capabilities = connection->GetTopologyCapabilities ();
            wprintf (L"    The provider does %lssupport topology.\n", (topology_capabilities->SupportsTopology () ? L"" : L"not "));
            if (topology_capabilities->SupportsTopology ())
            {
                wprintf (L"        The provider does %lssupport topology heirarchy.\n", (topology_capabilities->SupportsTopologicalHierarchy () ? L"" : L"not "));
                wprintf (L"        The provider does %lsbreak curve crossings automatically.\n", (topology_capabilities->BreaksCurveCrossingsAutomatically () ? L"" : L"not "));
                wprintf (L"        The provider does %lsactivate topology by area.\n", (topology_capabilities->ActivatesTopologyByArea () ? L"" : L"not "));
                wprintf (L"        The provider does %lsconstrain feature movements.\n", (topology_capabilities->ConstrainsFeatureMovements () ? L"" : L"not "));
            }
            topology_capabilities->Release ();
            raster_capabilities = connection->GetRasterCapabilities ();
            wprintf (L"    The provider does %lssupport raster.\n", (raster_capabilities->SupportsRaster () ? L"" : L"not "));
            if (raster_capabilities->SupportsRaster ())
            {
                wprintf (L"        The provider does %lssupport stitching.\n", (raster_capabilities->SupportsStitching () ? L"" : L"not "));
                wprintf (L"        The provider does %lssupport subsampling.\n", (raster_capabilities->SupportsSubsampling () ? L"" : L"not "));
            }
            raster_capabilities->Release ();

            connection->Release ();
            ret = 0;
        }
    }
    catch (FdoClientServiceException* exception)
    {
        wprintf (L"%ls\n", exception->GetExceptionMessage ());
        delete exception;
    }

    return (ret);
}

