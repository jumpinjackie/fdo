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
#include "SqlServerFdoCapabilityTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoCapabilityTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoCapabilityTest, "FdoCapabilityTest");


//void SqlServerFdoCapabilityTest::InitSchemaCapabilitiesFlags ()
//{
//
//    // Only set those flags that need to be changed to FALSE.
//
//    m_expectedSchemaCapabilities->supportsNetworkModel                   = false;
//    m_expectedSchemaCapabilities->supportsExclusiveValueRangeConstraints = false;
//    m_expectedSchemaCapabilities->supportsInclusiveValueRangeConstraints = false;
//    m_expectedSchemaCapabilities->supportsValueConstraintsList           = false;
//
//}  //  InitSchemaCapabilitiesFlags ()

void SqlServerFdoCapabilityTest::InitSchemaCapabilitiesMaxDataValueLimits ()
{

    m_expectedSchemaCapabilities->blobDataValueLimit     = (FdoInt64)(-1);
    m_expectedSchemaCapabilities->booleanDataValueLimit  = (FdoInt64)sizeof(FdoBoolean);
    m_expectedSchemaCapabilities->byteDataValueLimit     = (FdoInt64)sizeof(FdoByte);
    m_expectedSchemaCapabilities->clobDataValueLimit     = (FdoInt64)(-1);
    m_expectedSchemaCapabilities->dateTimeDataValueLimit = (FdoInt64)sizeof(FdoDateTime);
    m_expectedSchemaCapabilities->decimalDataValueLimit  = (FdoInt64)(38+38);
    m_expectedSchemaCapabilities->doubleDataValueLimit   = (FdoInt64)sizeof(FdoDouble);
    m_expectedSchemaCapabilities->int16DataValueLimit    = (FdoInt64)sizeof(FdoInt16);
    m_expectedSchemaCapabilities->int32DataValueLimit    = (FdoInt64)sizeof(FdoInt32);
    m_expectedSchemaCapabilities->int64DataValueLimit    = (FdoInt64)sizeof(FdoInt64);
    m_expectedSchemaCapabilities->singleDataValueLimit   = (FdoInt64)sizeof(FdoFloat);
    m_expectedSchemaCapabilities->stringDataValueLimit   = (FdoInt64)(2147483648 - 1);

}  //  InitSchemaCapabilitiesMaxDataValueLimits ()

void SqlServerFdoCapabilityTest::InitSchemaCapabilitiesNameSizeLimits ()
{

    m_expectedSchemaCapabilities->classNameLimit     = 200;
    m_expectedSchemaCapabilities->datastoreNameLimit = 123;
    m_expectedSchemaCapabilities->descriptionLimit   = 255;
    m_expectedSchemaCapabilities->propertyNameLimit  = 200;
    m_expectedSchemaCapabilities->schemaNameLimit    = 200;

}  //  InitSchemaCapabilitiesNameSizeLimits ()

void SqlServerFdoCapabilityTest::SetProvider ()
{

	UnitTestUtil::SetProvider( "SQLServerSpatial" );

}  //  SetProvider ()
