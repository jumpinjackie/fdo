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

#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_WMSOVERRIDESTEST_H
#define CPP_UNIT_WMSOVERRIDESTEST_H

#include "WmsTests.h"

class OverridesTest : public WmsTests
{
  CPPUNIT_TEST_SUITE( OverridesTest );
  CPPUNIT_TEST( TestCreateSchemaOverrides );
  CPPUNIT_TEST( TestSimple );
  CPPUNIT_TEST( TestImageFormatBackwardsComp);
  CPPUNIT_TEST( TestConflictImageFormatConfig);
  CPPUNIT_TEST( TestDefaultImageFormat);
  CPPUNIT_TEST( TestSetConfiguration );
#ifdef _FDO_RUN_INTERNAL_TEST
  CPPUNIT_TEST( TestSetConfiguration2); 
#endif
  CPPUNIT_TEST( TestSetConfiguration3); 
  CPPUNIT_TEST( TestRequestSpcialImageFormat);
  CPPUNIT_TEST( TestRequestUnsupportedImage);
  CPPUNIT_TEST( TestNoDefaultDataModel ); 
  CPPUNIT_TEST( TestQualifiedName );
  CPPUNIT_TEST_SUITE_END();

public:
    OverridesTest(void);
    virtual ~OverridesTest(void);

protected:
    void TestSimple();
    void TestImageFormatBackwardsComp();
    void TestConflictImageFormatConfig();
    void TestDefaultImageFormat();
    void TestCreateSchemaOverrides();
    void TestSetConfiguration();
	void TestSetConfiguration2();
    void TestSetConfiguration3();
    void TestRequestSpcialImageFormat();
    void TestRequestUnsupportedImage();
    void TestNoDefaultDataModel();
    void TestQualifiedName();

private:
    void TestNewConfiguration(FdoWmsOvPhysicalSchemaMapping* schemaMapping, FdoInt32 schemaIndex);
    void TestBaseProperties(FdoPhysicalElementMapping* elementMapping);
    void Stream2File(FdoIoStream* stream, FdoString* fileName);
    void TestConfiguration(FdoWmsOvPhysicalSchemaMapping* schemaMapping);

private:
    FdoStringsP schemaNames;
    FdoStringsP classNames;
    FdoStringsP rasterPropertyNames;
    FdoStringsP layerNames;
    FdoStringsP styleNames;
};

#endif // CPP_UNIT_WMSOVERRIDESTEST_H
