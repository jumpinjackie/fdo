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

#ifndef CPP_UNIT_FDOIMPORTEXPORTTEST_H
#define CPP_UNIT_FDOIMPORTEXPORTTEST_H

#include <Sm/SchemaManager.h>

/* 
 * FdoImportExportTest tests XML configuration file handling. It tests the serializing
 * and deserializing of Spatial Contexts and Feature Schemas between an Oracle Provider
 * DataStore and FDO XML.
 */
class FdoImportExportTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoImportExportTest );
  CPPUNIT_TEST( Test1 );
  CPPUNIT_TEST( Test2_3_4 );
  CPPUNIT_TEST( Test5_6_7 );
  CPPUNIT_TEST( Test8 );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoImportExportTest(void);
    virtual ~FdoImportExportTest(void);
    void setUp ();
    void tearDown ();

protected:
    virtual void  set_provider() {};   

    void Test1();
    void Test2_3_4();
    void Test5_6_7();
    void Test8();
    void DoTest( 
        FdoIoStream* inStream, 
        FdoIoStream* outStream1, 
        FdoIoStream* outStream2, 
        FdoXmlSpatialContextFlags* flags,
        FdoString* dbName,
        FdoBoolean createDB = true,
        FdoBoolean importSC = true,
        FdoBoolean importSchemas = true
    );
    void Export( FdoIConnection* connection, FdoIoStream* stream, FdoXmlSpatialContextFlags* flags );
    void Import( 
        FdoIConnection* connection, 
        FdoIoStream* stream, 
        FdoXmlSpatialContextFlags* flags,
        FdoBoolean importSC = true,
        FdoBoolean importSchemas = true
    );
	void CreateConfig1( FdoIoStream* stream );
	void CreateConfig2( FdoIoStream* stream );
	void CreateConfigBadWkt( FdoIoStream* stream );
    void AddSchema( FdoXmlWriter* writer, FdoInt32 idx );
    void AddSC( FdoXmlWriter* writer, FdoString* name, FdoString* description, FdoString* coordSys, FdoString* wkt, FdoInt32 idx );
    void AddFeature( FdoIInsert* insertCommand, FdoInt32 idx  );
    void _addMapping( FdoXmlWriter* writer, FdoString* name, FdoString* provider, FdoString* xmlns );
    FdoByteArray* SerializeExtent( double minX, double minY, double maxX, double maxY );

    void _overrideBend( 
        FdoString* inFile, 
        FdoString* outFile, 
        FdoStringP providerName, 
        FdoStringP xmlns,
        FdoStringP tablespace = L"",
        bool oneAutoIncrement = true
    );
    
    virtual void AddMapping( FdoXmlWriter* writer, FdoString* name, FdoInt32 providerVersion );
    virtual FdoStringP GetWKT( FdoString* coordSys );
    virtual FdoStringP OverrideBend( FdoString* inFile );

	static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name );
protected:
};

#endif // CPP_UNIT_FDOIMPORTEXPORTTEST_H
