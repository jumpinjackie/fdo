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
#include "OdbcReaderTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcOracleReaderTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleReaderTest, "ReaderTest");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcSqlServerReaderTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerReaderTest, "ReaderTest");

//CPPUNIT_TEST_SUITE_REGISTRATION( OdbcMySqlReaderTest );
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlReaderTest, "ReaderTest");

void OdbcReaderTest::PrepareConnection ()
{
    try
    {
        m_connection = UnitTestUtil::GetProviderConnectionObject();
        if (DataBaseType_None != mSetup.GetTypeDB() )
        {
            // Set up databases that are not prefabricated.
            StringConnTypeRequest connectionType = Connection_NoDatastore;
            if (DataBaseType_Oracle == mSetup.GetTypeDB() )
                connectionType = Connection_OraSetup;
            m_connection->SetConnectionString ( UnitTestUtil::GetConnectionString(connectionType) );
            m_connection->Open();
            mSetup.CreateDataStore(m_connection, L"");
            mSetup.CreateAcadSchema(m_connection);
            mSetup.CreateNonAcadSchema(m_connection);
        }
        //m_connection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDSN) );
        //m_connection->Open();
    }
    catch (FdoException *ex)
    {
        m_connection = NULL;
        TestCommonFail (ex);
    }
}
