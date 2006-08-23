 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
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
#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_APPLYSCHEMATEST_H
#define CPP_UNIT_APPLYSCHEMATEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Fdo.h>


/* 
 * A test case for ApplySchemaCommand.
 *
 * Loads some schemas into a database.
 */

class ApplySchemaTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( ApplySchemaTest );
  CPPUNIT_TEST( TestSchema );
  CPPUNIT_TEST_SUITE_END();

public:
    ApplySchemaTest(void);
    virtual ~ApplySchemaTest(void);
    void setUp ();

protected:

    void TestSchema ();

	void CreateAcadSchema( FdoIConnection* connection );
	void CreateElectricSchema( FdoIConnection* connection );
	void CreateLandSchema( FdoIConnection* connection );
    void CreateLandSchema( FdoFeatureSchemaCollection* pSchemas );
	void CreateLongStringSchema( FdoIConnection* connection );
	void CreateErrorSchema( FdoIConnection* connection );

	void DeleteAcadSchema( FdoIConnection* connection );

    void ModElectricSchema( FdoIConnection* connection );
    void ModElectricSchema( FdoFeatureSchemaCollection* pSchemas );
/*
	void ModLandSchema( FdoIConnection* connection );
*/
	void RedefineGeometry( FdoIConnection* connection );
/*
	void DelPropertyError( FdoIConnection* connection );
*/
	void ModDelSchemas( FdoIConnection* connection );
    void ModDelElectricSchema( FdoFeatureSchemaCollection* pSchemas );
    void ModDelAcadSchema( FdoFeatureSchemaCollection* pSchemas );
	void ReAddElements( FdoIConnection* connection );
	void ModErrors( FdoIConnection* connection );
	void ModErrors2( FdoIConnection* connection );
/*
    virtual void GetClassCapabilities( FdoIConnection* connection );
    void CheckBaseProperties( FdoIConnection* connection );
*/
    void CopySchemas( 
        FdoFeatureSchemaCollection* pSchemas,
        FdoFeatureSchemaCollection* pSchemas2
    );
};

#endif
