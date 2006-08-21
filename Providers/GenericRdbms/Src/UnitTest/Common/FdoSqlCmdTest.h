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

#ifndef CPP_UNIT_FDOSQLCMDTEST_H
#define CPP_UNIT_FDOSQLCMDTEST_H

/*
 * A test case for the connection.
 */
class FdoSqlCmdTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoSqlCmdTest );
  CPPUNIT_TEST (doGetSC);
  //CPPUNIT_TEST (CreateActivateDestroySC);
  CPPUNIT_TEST( CreateTable );
  CPPUNIT_TEST( BasicInsert );
  CPPUNIT_TEST( BindInsert );
  CPPUNIT_TEST( Query );
  CPPUNIT_TEST( QueryGeometry );
  CPPUNIT_TEST( Delete );
  CPPUNIT_TEST( DropTable );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoSqlCmdTest(void);
    virtual ~FdoSqlCmdTest(void);
    void setUp ();
    void tearDown ();
    virtual void connect ();
protected:

	virtual void doGetSC();
	int  doGetSC2();
	int  doGetSCG(FdoIConnection *conn);

	virtual void CreateActivateDestroySC();

    virtual void CreateTable ();
    virtual void BasicInsert ();
    virtual void BindInsert ();
    virtual void Query ();
    virtual void QueryGeometry();
    virtual void Delete ();
    virtual void DropTable ();

    // Provider's overrided methods
    virtual wchar_t *get_datetime_type() { return L"date"; };
    virtual wchar_t *get_datetime ()   { return L"'1999-05-29-02-24-32'"; }
    virtual wchar_t *get_datetime_sys() { return L"sysdate"; }
    virtual wchar_t *get_number_type() { return L"number"; };

    virtual void  set_provider() {};

    FdoIConnection* mConnection;

    void ReadQueryResult( FdoISQLDataReader *myReader );
    char* FdoTypeToStr( FdoDataType type );
};

#endif // CPP_UNIT_FDOSQLCMDTEST_H
