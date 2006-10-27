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


#include "Pch.h"
#include "CapabilityTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CapabilityTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( CapabilityTest, "CapabilityTest");

CapabilityTest::CapabilityTest(void)
{
}

CapabilityTest::~CapabilityTest(void)
{
}

void CapabilityTest::TestCapabilities ()
{
    try
    {
        int size = 0;
        FdoPtr<FdoIConnection> connection = WmsTests::GetConnection ();

        //
        // Test FdoIConnectionCapabilities
        //
	    FdoPtr<FdoIConnectionCapabilities> connectionCapabilities = connection->GetConnectionCapabilities();

        FdoThreadCapability threadCapability = connectionCapabilities->GetThreadCapability();
        CPPUNIT_ASSERT(threadCapability == FdoThreadCapability_PerConnectionThreaded);

        FdoSpatialContextExtentType *spatialContextTypes = connectionCapabilities->GetSpatialContextTypes(size);
        CPPUNIT_ASSERT(size == 1);
        CPPUNIT_ASSERT(spatialContextTypes != NULL);
        CPPUNIT_ASSERT(spatialContextTypes[0] == FdoSpatialContextExtentType_Static);

        bool supportsLocking = connectionCapabilities->SupportsLocking();
        CPPUNIT_ASSERT(supportsLocking == false);

        bool supportsLongTransactions = connectionCapabilities->SupportsLongTransactions();
        CPPUNIT_ASSERT(supportsLongTransactions == false);

        bool supportsSQL = connectionCapabilities->SupportsSQL();
        CPPUNIT_ASSERT(supportsSQL == false);

        bool supportsTimeout = connectionCapabilities->SupportsTimeout();
        CPPUNIT_ASSERT(supportsTimeout == false);

        bool supportsTransactions = connectionCapabilities->SupportsTransactions();
        CPPUNIT_ASSERT(supportsTransactions == false);

        FdoLockType* lockTypes = connectionCapabilities->GetLockTypes(size);
        CPPUNIT_ASSERT(lockTypes == NULL);
        CPPUNIT_ASSERT(size == 0);

        //
        // Test FdoICommandCapabilities
        //
        FdoPtr<FdoICommandCapabilities> commandCapabilities = connection->GetCommandCapabilities();

        FdoInt32 *commands = commandCapabilities->GetCommands(size);
        CPPUNIT_ASSERT(size == 5);
        CPPUNIT_ASSERT(commands != NULL);
        CPPUNIT_ASSERT(commands[0] == FdoCommandType_Select);
        CPPUNIT_ASSERT(commands[1] == FdoCommandType_DescribeSchema);
		CPPUNIT_ASSERT(commands[2] == FdoCommandType_DescribeSchemaMapping);
        CPPUNIT_ASSERT(commands[3] == FdoCommandType_GetSpatialContexts);
        CPPUNIT_ASSERT(commands[4] == FdoCommandType_SelectAggregates);

        bool supportsParameters = commandCapabilities->SupportsParameters();
        CPPUNIT_ASSERT(supportsParameters == false);

        bool supportsTimeout2 = commandCapabilities->SupportsTimeout();
        CPPUNIT_ASSERT(supportsTimeout2 == false);

        bool supportsSelectExpressions = commandCapabilities->SupportsSelectExpressions();
        CPPUNIT_ASSERT(supportsSelectExpressions == false);

        bool supportsSelectFunctions = commandCapabilities->SupportsSelectFunctions();
        CPPUNIT_ASSERT(supportsSelectFunctions == true);

        bool supportsSelectDistinct = commandCapabilities->SupportsSelectDistinct();
        CPPUNIT_ASSERT(supportsSelectDistinct == false);

        bool supportsSelectOrdering = commandCapabilities->SupportsSelectOrdering();
        CPPUNIT_ASSERT(supportsSelectOrdering == false);

        bool supportsSelectGrouping = commandCapabilities->SupportsSelectGrouping();
        CPPUNIT_ASSERT(supportsSelectGrouping == false);

        //
        // Test FdoISchemaCapabilities
        //
	    FdoPtr<FdoISchemaCapabilities> schemaCapabilities = connection->GetSchemaCapabilities();

        bool supportsInheritance = schemaCapabilities->SupportsInheritance();
        CPPUNIT_ASSERT(supportsInheritance == true);

        bool supportsMultipleSchemas = schemaCapabilities->SupportsMultipleSchemas();
        CPPUNIT_ASSERT(supportsMultipleSchemas == false);

        bool supportsObjectProperties = schemaCapabilities->SupportsObjectProperties();
        CPPUNIT_ASSERT(supportsObjectProperties == false);

        bool supportsAssociations = schemaCapabilities->SupportsAssociationProperties();
        CPPUNIT_ASSERT(supportsAssociations == false);

        bool supportsSchemaOverrides = schemaCapabilities->SupportsSchemaOverrides();
        CPPUNIT_ASSERT(supportsSchemaOverrides == true);

        bool supportsNetworkModel = schemaCapabilities->SupportsNetworkModel();
        CPPUNIT_ASSERT(supportsNetworkModel == false);

        bool supportsAutoIdGeneration = schemaCapabilities->SupportsAutoIdGeneration();
        CPPUNIT_ASSERT(supportsAutoIdGeneration == false);

        bool supportsDSIdGeneration = schemaCapabilities->SupportsDataStoreScopeUniqueIdGeneration();
        CPPUNIT_ASSERT(supportsDSIdGeneration == false);

        bool supportsSchemsMod = schemaCapabilities->SupportsSchemaModification();
        CPPUNIT_ASSERT(supportsSchemsMod == false);

        FdoClassType *classTypes = schemaCapabilities->GetClassTypes(size);
        CPPUNIT_ASSERT(size == 1);
        CPPUNIT_ASSERT(classTypes != NULL);        
        CPPUNIT_ASSERT(classTypes[0] == FdoClassType_FeatureClass);

        FdoDataType *dataTypes = schemaCapabilities->GetDataTypes(size);
        CPPUNIT_ASSERT(size == 2);
        CPPUNIT_ASSERT(dataTypes != NULL);
        CPPUNIT_ASSERT(dataTypes[0] == FdoDataType_String);
        CPPUNIT_ASSERT(dataTypes[1] == FdoDataType_BLOB);
        
        FdoDataType *autoGenDataTypes = schemaCapabilities->GetSupportedAutoGeneratedTypes(size);
        CPPUNIT_ASSERT(size == 0);
        CPPUNIT_ASSERT(autoGenDataTypes == NULL);

        //
        // Test FdoIFilterCapabilities
        //
        FdoPtr<FdoIFilterCapabilities> filterCapabilities = connection->GetFilterCapabilities();

        bool supportsGeodesicDistance = filterCapabilities->SupportsGeodesicDistance();
        CPPUNIT_ASSERT(supportsGeodesicDistance == false);

        bool nonLiteralGeometricOperations = filterCapabilities->SupportsNonLiteralGeometricOperations();
        CPPUNIT_ASSERT(nonLiteralGeometricOperations == false);

        FdoDistanceOperations *distanceOperations = filterCapabilities->GetDistanceOperations(size);
        CPPUNIT_ASSERT(size == 0);
        CPPUNIT_ASSERT(distanceOperations == NULL);

        FdoConditionType *conditionTypes = filterCapabilities->GetConditionTypes(size);
        CPPUNIT_ASSERT(size == 0);
        CPPUNIT_ASSERT(conditionTypes == NULL);

        FdoSpatialOperations *spatialOperations = filterCapabilities->GetSpatialOperations(size);
        CPPUNIT_ASSERT(size == 0);
        CPPUNIT_ASSERT(spatialOperations == NULL);
        
        //
        // Test FdoIExpressionCapabilities
        //
        FdoPtr<FdoIExpressionCapabilities> expressionCapabilities = connection->GetExpressionCapabilities();
        FdoExpressionType *expressionTypes = expressionCapabilities->GetExpressionTypes(size);
        CPPUNIT_ASSERT(size == 1);
        CPPUNIT_ASSERT(expressionTypes != NULL);
        CPPUNIT_ASSERT(expressionTypes[0] == FdoExpressionType_Function);

        FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCapabilities->GetFunctions();
        for (int i=0; i<functions->GetCount(); i++ )
        {
            FdoPtr<FdoFunctionDefinition> func = functions->GetItem(i);
#ifdef _DEBUG
            wprintf(L"Func: %ls: %ls\n", func->GetName(), func->GetDescription() );
#endif
            FdoPtr<FdoReadOnlyArgumentDefinitionCollection> args = func->GetArguments();
            for (int j=0; j<args->GetCount(); j++)
            {
                FdoPtr<FdoArgumentDefinition>arg = args->GetItem( j );
#ifdef _DEBUG
                wprintf(L"\tArg%d= %ls: %ls\n", j, arg->GetName(), arg->GetDescription());
#endif
            }
        }

        // RESAMPLE function
		FdoPtr<FdoFunctionDefinition> resampleFunction = functions->GetItem(0);
		CPPUNIT_ASSERT(STRCASEEQ(resampleFunction->GetName(), L"RESAMPLE"));
		CPPUNIT_ASSERT(resampleFunction->GetReturnType() == FdoDataType_BLOB);

		FdoPtr<FdoReadOnlyArgumentDefinitionCollection> args = resampleFunction->GetArguments();
		CPPUNIT_ASSERT(args->GetCount() == 7);

		FdoPtr<FdoArgumentDefinition> rasterArg = args->GetItem(0);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"raster"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_BLOB);

		rasterArg = args->GetItem(1);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"minX"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Double);

		rasterArg = args->GetItem(2);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"minY"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Double);

		rasterArg = args->GetItem(3);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"maxX"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Double);

		rasterArg = args->GetItem(4);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"maxY"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Double);

		rasterArg = args->GetItem(5);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"height"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Int32);

		rasterArg = args->GetItem(6);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"width"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Int32);

		// CLIP function
		FdoPtr<FdoFunctionDefinition> clipFunction = functions->GetItem(1);
		CPPUNIT_ASSERT(STRCASEEQ(clipFunction->GetName(), L"CLIP"));
		CPPUNIT_ASSERT(clipFunction->GetReturnType() == FdoDataType_BLOB);

		args = clipFunction->GetArguments();
		CPPUNIT_ASSERT(args->GetCount() == 5);

		rasterArg = args->GetItem(0);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"raster"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_BLOB);

		rasterArg = args->GetItem(1);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"minX"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Double);

		rasterArg = args->GetItem(2);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"minY"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Double);

		rasterArg = args->GetItem(3);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"maxX"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Double);

		rasterArg = args->GetItem(4);
		CPPUNIT_ASSERT(STRCASEEQ(rasterArg->GetName(), L"maxY"));
		CPPUNIT_ASSERT(rasterArg->GetDataType() == FdoDataType_Double);

		// SpatialExtents function
		FdoPtr<FdoFunctionDefinition> selectFunction = functions->GetItem(2);
		CPPUNIT_ASSERT(STRCASEEQ(selectFunction->GetName(), L"SpatialExtents"));


        //
        // Test FdoIRasterCapabilities
        //
        FdoPtr<FdoIRasterCapabilities> rasterCapabilities = connection->GetRasterCapabilities();

        bool supportsRaster = rasterCapabilities->SupportsRaster();
        CPPUNIT_ASSERT(supportsRaster == true);

        bool supportsStiching = rasterCapabilities->SupportsStitching();
        CPPUNIT_ASSERT(supportsStiching == false);

        bool supportsSubsampling = rasterCapabilities->SupportsSubsampling();
        CPPUNIT_ASSERT(supportsSubsampling == true);

		FdoPtr<FdoRasterDataModel> dm = FdoRasterDataModel::Create();
        dm->SetOrganization(FdoRasterDataOrganization_Row);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == false);
		dm->SetOrganization(FdoRasterDataOrganization_Image);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == false);
		dm->SetOrganization(FdoRasterDataOrganization_Pixel);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == true);

		// bitional
		dm->SetDataModelType(FdoRasterDataModelType_Bitonal);
		dm->SetBitsPerPixel(1);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == true);
		dm->SetBitsPerPixel(8);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == false);

        // gray
		dm->SetDataModelType(FdoRasterDataModelType_Gray);
		dm->SetBitsPerPixel(8);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == true);
		dm->SetBitsPerPixel(24);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == false);

        // palette	
		dm->SetDataModelType(FdoRasterDataModelType_Palette);
		dm->SetBitsPerPixel(8);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == true);
		dm->SetBitsPerPixel(24);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == false);

        // rgb
		dm->SetDataModelType(FdoRasterDataModelType_RGB);
		dm->SetBitsPerPixel(24);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == true);
		dm->SetBitsPerPixel(32);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == false);

		// rgba
		dm->SetDataModelType(FdoRasterDataModelType_RGBA);
		dm->SetBitsPerPixel(32);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == true);
		dm->SetBitsPerPixel(24);
		CPPUNIT_ASSERT(rasterCapabilities->SupportsDataModel(dm) == false);

        //
        // Test FdoITopologyCapabilities
        //
        FdoPtr<FdoITopologyCapabilities> topoCapabilities = connection->GetTopologyCapabilities();

        CPPUNIT_ASSERT(topoCapabilities->SupportsTopology() == false);
        CPPUNIT_ASSERT(topoCapabilities->SupportsTopologicalHierarchy() == false);
        CPPUNIT_ASSERT(topoCapabilities->BreaksCurveCrossingsAutomatically() == false);
        CPPUNIT_ASSERT(topoCapabilities->ActivatesTopologyByArea() == false);
        CPPUNIT_ASSERT(topoCapabilities->ConstrainsFeatureMovements() == false);

        //
        // Test FdoIGeometryCapabilities
        //
        FdoPtr<FdoIGeometryCapabilities> geomCapabilities = connection->GetGeometryCapabilities();

        FdoGeometryType* geomTypes = geomCapabilities->GetGeometryTypes(size);
        CPPUNIT_ASSERT(size == 1);
        CPPUNIT_ASSERT(geomTypes[0] == FdoGeometryType_Polygon);

        FdoGeometryComponentType* geomCompTypes = geomCapabilities->GetGeometryComponentTypes(size);
        CPPUNIT_ASSERT(size == 1);
        CPPUNIT_ASSERT(geomCompTypes[0] = FdoGeometryComponentType_LinearRing);

        FdoInt32 dimensionalities = geomCapabilities->GetDimensionalities();
        CPPUNIT_ASSERT(dimensionalities == FdoDimensionality_XY);
    }
    catch (FdoException *ex)
    {
        WmsTests::fail(ex);
    }
}
