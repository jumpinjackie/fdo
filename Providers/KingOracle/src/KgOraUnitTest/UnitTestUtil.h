#ifndef UNIT_TEST_UTIL_H
#define UNIT_TEST_UTIL_H

#include "Fdo.h"
#include "TestCommonMiscUtil.h"

#define D_CONN_USERNAME L"fdounittest"
#define D_CONN_PASS     L"fdounittest"
#define D_CONN_SERVICE  L"//localhost:1521/xe"

class c_Oci_Connection;

class UnitTestUtil
{
public:
    static FdoIConnection* CreateOraConnection(FdoString* connectionString);
    static FdoIConnection* CreateOraConnection(FdoString* service, FdoString* username, FdoString* password);
    static FdoIConnection* CreateDefaultConnection();
    static c_Oci_Connection* CreateOCIConnection();

    static FdoStringP GetFeatureClass(FdoString* tableName, FdoString* geomColumn, FdoString* userName = NULL);

    static FdoStringP GetUsername();
    static FdoStringP GetPassword();
    static FdoStringP GetService();
};

#define ASSERT_CAN_CONNECT(conn) \
    try { \
        CPPUNIT_ASSERT(conn->Open() == FdoConnectionState_Open); \
    } catch (FdoException* ex) { \
        TestCommonFail(ex); \
    }

#endif