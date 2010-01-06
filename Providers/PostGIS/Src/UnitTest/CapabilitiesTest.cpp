//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "Pch.h"
#include "CapabilitiesTest.h"
#include "BaseTestCase.h"
#include "TestConfig.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CapabilitiesTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CapabilitiesTest, "CapabilitiesTest");

CapabilitiesTest::CapabilitiesTest()
{
}

CapabilitiesTest::~CapabilitiesTest()
{
}

void CapabilitiesTest::testConnectionCapabilities()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIConnectionCapabilities> cc = conn->GetConnectionCapabilities();

        CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect threading capability",
            FdoThreadCapability_PerConnectionThreaded,
            cc->GetThreadCapability());

        FdoInt32 size = 0;
        FdoSpatialContextExtentType* sct = cc->GetSpatialContextTypes(size);
        CPPUNIT_ASSERT(NULL != sct);
        CPPUNIT_ASSERT_EQUAL(1, size);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect spatial context type",
            FdoSpatialContextExtentType_Static, sct[0]);

        FdoLockType* lkt = cc->GetLockTypes(size);
        CPPUNIT_ASSERT(NULL != lkt);
        CPPUNIT_ASSERT_EQUAL(1, size);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect lock type",
            FdoLockType_Transaction, lkt[0]);

        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsLocking());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsTimeout());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsTransactions());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsLongTransactions());
        CPPUNIT_ASSERT_EQUAL(true, cc->SupportsSQL());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsConfiguration());
        CPPUNIT_ASSERT_EQUAL(true, cc->SupportsMultipleSpatialContexts());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsCSysWKTFromCSysName());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsWrite());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsMultiUserWrite());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsFlush());
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void CapabilitiesTest::testCommandsCapabilities()
{
   try
   {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoICommandCapabilities> cc = conn->GetCommandCapabilities();

        FdoInt32 size = 0;
        FdoInt32* commands = cc->GetCommands(size);
        CPPUNIT_ASSERT(NULL != commands);
        CPPUNIT_ASSERT_EQUAL(10, size);
        CPPUNIT_ASSERT(FdoCommandType_Select == commands[0]);
        CPPUNIT_ASSERT(FdoCommandType_Insert == commands[1]);
		CPPUNIT_ASSERT(FdoCommandType_Update == commands[2]);
        CPPUNIT_ASSERT(FdoCommandType_Delete == commands[3]);
        CPPUNIT_ASSERT(FdoCommandType_SQLCommand == commands[4]);
        CPPUNIT_ASSERT(FdoCommandType_DescribeSchema == commands[5]);
        CPPUNIT_ASSERT(FdoCommandType_CreateDataStore == commands[6]);
		CPPUNIT_ASSERT(FdoCommandType_DestroyDataStore == commands[7]);
        CPPUNIT_ASSERT(FdoCommandType_ListDataStores == commands[8]);
        CPPUNIT_ASSERT(FdoCommandType_GetSpatialContexts == commands[9]);

        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsParameters());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsTimeout());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsSelectExpressions());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsSelectFunctions());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsSelectDistinct());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsSelectOrdering());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsSelectGrouping());
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void CapabilitiesTest::testSchemaCapabilities()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoISchemaCapabilities> sc = conn->GetSchemaCapabilities();

        FdoInt32 size = 0;

        FdoClassType* ct = sc->GetClassTypes(size);
        CPPUNIT_ASSERT(NULL != ct);
        CPPUNIT_ASSERT_EQUAL(2, size);
        CPPUNIT_ASSERT_EQUAL(FdoClassType_FeatureClass, ct[0]);
        CPPUNIT_ASSERT_EQUAL(FdoClassType_Class, ct[1]);

        FdoDataType* dt = sc->GetDataTypes(size);
        CPPUNIT_ASSERT(NULL != dt);
        CPPUNIT_ASSERT_EQUAL(10, size);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_Boolean, dt[0]);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_Byte, dt[1]);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_DateTime, dt[2]);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_Decimal, dt[3]);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_Double, dt[4]);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_Int16, dt[5]);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_Int32, dt[6]);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_Int64, dt[7]);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_Single, dt[8]);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_String, dt[9]);
        
        FdoDataType* adt = sc->GetSupportedAutoGeneratedTypes(size);
        CPPUNIT_ASSERT(NULL != adt);
        CPPUNIT_ASSERT_EQUAL(1, size);
        CPPUNIT_ASSERT_EQUAL(FdoDataType_Int32, adt[0]);

        FdoDataType* sipt = sc->GetSupportedIdentityPropertyTypes(size);
        CPPUNIT_ASSERT(NULL == sipt);
        CPPUNIT_ASSERT_EQUAL(0, size);

        // TODO: Add tests for following features
        //FdoInt64 GetMaximumDataValueLength(FdoDataType type);
        //FdoInt32 GetMaximumDecimalPrecision();
        //FdoInt32 GetMaximumDecimalScale();
        //FdoInt32 GetNameSizeLimit(FdoSchemaElementNameType name);
        //FdoString* GetReservedCharactersForName();

        // TODO: Review flags below
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsAssociationProperties());
        CPPUNIT_ASSERT_EQUAL(true, sc->SupportsAutoIdGeneration());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsCompositeId());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsCompositeUniqueValueConstraints());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsDataStoreScopeUniqueIdGeneration());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsDefaultValue());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsExclusiveValueRangeConstraints());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsInclusiveValueRangeConstraints());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsInheritance());
        CPPUNIT_ASSERT_EQUAL(true, sc->SupportsMultipleSchemas());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsNetworkModel());
        CPPUNIT_ASSERT_EQUAL(true, sc->SupportsNullValueConstraints());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsObjectProperties());
        CPPUNIT_ASSERT_EQUAL(true, sc->SupportsSchemaModification());
        CPPUNIT_ASSERT_EQUAL(true, sc->SupportsSchemaOverrides());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsUniqueValueConstraints());
        CPPUNIT_ASSERT_EQUAL(false, sc->SupportsValueConstraintsList());
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void CapabilitiesTest::testFilterCapabilities()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIFilterCapabilities> fc = conn->GetFilterCapabilities();

        FdoInt32 size = 0;

        FdoConditionType* ct = fc->GetConditionTypes(size);
        CPPUNIT_ASSERT(NULL != ct);
        CPPUNIT_ASSERT_EQUAL(5, size);
        CPPUNIT_ASSERT_EQUAL(FdoConditionType_Comparison, ct[0]);
        CPPUNIT_ASSERT_EQUAL(FdoConditionType_Like, ct[1]);
        CPPUNIT_ASSERT_EQUAL(FdoConditionType_In, ct[2]);
        CPPUNIT_ASSERT_EQUAL(FdoConditionType_Null, ct[3]);
        CPPUNIT_ASSERT_EQUAL(FdoConditionType_Spatial, ct[4]);

        FdoSpatialOperations* sop = fc->GetSpatialOperations(size);
        CPPUNIT_ASSERT(NULL != sop);
        CPPUNIT_ASSERT_EQUAL(8, size);
        CPPUNIT_ASSERT_EQUAL(FdoSpatialOperations_Contains, sop[0]);
        CPPUNIT_ASSERT_EQUAL(FdoSpatialOperations_Crosses, sop[1]);
        CPPUNIT_ASSERT_EQUAL(FdoSpatialOperations_Disjoint, sop[2]);
        CPPUNIT_ASSERT_EQUAL(FdoSpatialOperations_Equals, sop[3]);
        CPPUNIT_ASSERT_EQUAL(FdoSpatialOperations_Intersects, sop[4]);
        CPPUNIT_ASSERT_EQUAL(FdoSpatialOperations_Overlaps, sop[5]);
        CPPUNIT_ASSERT_EQUAL(FdoSpatialOperations_Touches, sop[6]);
        CPPUNIT_ASSERT_EQUAL(FdoSpatialOperations_Within, sop[7]);

        // TODO: Add tests after implemetned
        //FilterCapabilities::GetDistanceOperations(FdoInt32& size)
        //FilterCapabilities::SupportsGeodesicDistance()
        //FilterCapabilities::SupportsNonLiteralGeometricOperations()
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void CapabilitiesTest::testExpressionCapabilities()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIFilterCapabilities> fc = conn->GetFilterCapabilities();

        FdoInt32 size = 0;

        CPPUNIT_FAIL("NOT IMPLEMENTED");
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void CapabilitiesTest::testGeometryCapabilities()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIFilterCapabilities> fc = conn->GetFilterCapabilities();

        FdoInt32 size = 0;

        CPPUNIT_FAIL("NOT IMPLEMENTED");
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void CapabilitiesTest::testTopologyCapabilities()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIFilterCapabilities> fc = conn->GetFilterCapabilities();

        FdoInt32 size = 0;

        CPPUNIT_FAIL("NOT IMPLEMENTED");
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}
