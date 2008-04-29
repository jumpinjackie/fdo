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

#ifndef CPP_UNIT_FDODATASTORETEST_H
#define CPP_UNIT_FDODATASTORETEST_H

#define UNIT_TEST_DB_LKMODE_FDO		true // false is NONE

class FdoDatastoreTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( FdoDatastoreTest );
	CPPUNIT_TEST( predelete );
	CPPUNIT_TEST( Cmd_CreateDatastore );
	CPPUNIT_TEST( Cmd_ListDatastores );
	CPPUNIT_TEST( Cmd_DestroyDatastore );
	CPPUNIT_TEST( TestReservedName );

	// USE IT ON DEMAND
	//CPPUNIT_TEST( DropAllMyDatastores );

	CPPUNIT_TEST_SUITE_END();

public:
    FdoDatastoreTest(void);
    virtual ~FdoDatastoreTest(void);
    void setUp ();

protected:
	virtual void  set_provider() {}   
	virtual FdoStringP  get_lt_mode() { return L"";} 

	void predelete();
    void Cmd_ListDatastores ();
	void Cmd_CreateDatastore ();
	void Cmd_DestroyDatastore ();
    virtual void TestReservedName();
	int  ListDatastores( bool include );
    void CreateDatastore(FdoIConnection* connection, FdoString* dsName);
	void DestroyDatastore(bool ignoreEx);
	void ListDatabaseProperties(FdoIDataStorePropertyDictionary* dictionary);
	void DropAllMyDatastores();
    virtual void ReservedName(FdoIConnection* connection);
};

#endif // CPP_UNIT_FDODATASTORETEST_H
