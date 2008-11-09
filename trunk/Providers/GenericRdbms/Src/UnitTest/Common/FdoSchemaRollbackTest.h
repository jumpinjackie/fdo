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

#ifndef CPP_UNIT_FDOSCHEMAROLLBACKTEST_H
#define CPP_UNIT_FDOSCHEMAROLLBACKTEST_H

/* 
 * Suite for testing the post-rollback synchronization of the physical schema with the metaschema.
 */
class FdoSchemaRollbackTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoSchemaRollbackTest );

  CPPUNIT_TEST( Test_nnnn );
  CPPUNIT_TEST( Test_nnyn );
  CPPUNIT_TEST( Test_nynn );
  CPPUNIT_TEST( Test_nyyn );
  CPPUNIT_TEST( Test_ynnn );
  CPPUNIT_TEST( Test_ynyn );
  CPPUNIT_TEST( Test_yynn );
  CPPUNIT_TEST( Test_yyyn );
  CPPUNIT_TEST( Test_ynny );

  CPPUNIT_TEST_SUITE_END();


public:
    FdoSchemaRollbackTest(void);
    virtual ~FdoSchemaRollbackTest(void);
    void setUp ();

protected:
    virtual void  set_provider()
    {
    }

    void Test_nnnn();
	void Test_nnyn();
	void Test_nynn();
	void Test_nyyn();
	void Test_ynnn();
	void Test_ynyn();
	void Test_yynn();
	void Test_yyyn();
	void Test_ynny();
	void DoTest ( bool bPreClear, const wchar_t* schemaName, bool bRollbackOnly, bool bCommit );
	void CreateAcadSchema( FdoIConnection* connection );
	void CreateElectricSchema( FdoIConnection* connection );
	void ModAcadSchema( FdoIConnection* connection );
	void ModElectricSchema( FdoIConnection* connection );

	FdoStringP OutFileName(bool bPreClear, const wchar_t* schemaName, bool bRollbackOnly, bool bCommit );
	FdoStringP MasterFileName(bool bPreClear, const wchar_t* schemaName, bool bRollbackOnly, bool bCommit );
	
    bool mCanAddNotNullCol;
};

#endif // CPP_UNIT_FDOSCHEMAROLLBACKTEST_H
