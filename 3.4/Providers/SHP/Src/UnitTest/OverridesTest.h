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
 */

#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_SHPOVERRIDESTEST_H
#define CPP_UNIT_SHPOVERRIDESTEST_H

#include "ShpTests.h"
#include <SHP/FdoShpOverrides.h>

class OverridesTest : public ShpTests
{
  CPPUNIT_TEST_SUITE( OverridesTest );
  CPPUNIT_TEST( TestCreateSchemaOverrides );
  CPPUNIT_TEST( TestSimple );
  CPPUNIT_TEST( TestSetConfiguration );
  CPPUNIT_TEST( TestSetEmptyConfiguration );
  CPPUNIT_TEST( TestReadWithOverrides );
  CPPUNIT_TEST( TestWriteWithOverrides );
  CPPUNIT_TEST_SUITE_END();

public:
    OverridesTest(void);
    virtual ~OverridesTest(void);

protected:
    void TestSimple();
    void TestCreateSchemaOverrides();
    void TestSetConfiguration();
    void TestSetEmptyConfiguration ();
    void TestReadWithOverrides ();
    void TestWriteWithOverrides ();

private:
    void TestNewConfiguration(FdoShpOvPhysicalSchemaMapping* schemaMapping, FdoInt32 schemaIndex);
    void TestBaseProperties(FdoPhysicalElementMapping* elementMapping);
    void Stream2File(FdoIoStream* stream, FdoString* fileName);
    void TestConfiguration(FdoShpOvPhysicalSchemaMapping* schemaMapping);
    void VerifySchema (FdoIConnection* conn);

private:
    FdoStringsP schemaNames;
    FdoStringsP classNames;
    FdoStringsP propertyNames;
    FdoStringsP columnNames;
    FdoStringsP fileNames;

    // Schema constants:
    const static FdoString* schemaName;

    // Ontario class constants:
    const static FdoString* classNameOntario;
    const static FdoString* shapefileOntario;
    const static FdoString* propOntarioId;
    const static FdoString* propOntarioGeom;
    const static FdoString* propOntarioPerimeter;
    const static FdoString* propOntarioArea;
    const static FdoString* colOntarioPerimeter;
    const static FdoString* colOntarioArea;

    // Nanboku class constants:
    const static FdoString* classNameNanboku;
    const static FdoString* shapefileNanboku;
    const static FdoString* propNanbokuId;
    const static FdoString* propNanbokuGeom;
    const static FdoString* propNanbokuGL;
    const static FdoString* colNanbokuGL;
    const static FdoString* filterNanboku;
};

#endif // CPP_UNIT_ODBCOVERRIDESTEST_H

