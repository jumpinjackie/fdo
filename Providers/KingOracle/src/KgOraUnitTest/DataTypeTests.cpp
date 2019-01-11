#include "Pch.h"
#include "DataTypeTests.h"
#include "UnitTestUtil.h"
#include "TestCommonMiscUtil.h"
#include "Fdo.h"
#include <cmath>

#pragma warning(disable : 4996)

CPPUNIT_TEST_SUITE_REGISTRATION(DataTypeTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DataTypeTests, "DataTypeTests");

DataTypeTests::DataTypeTests(void)
{
}

DataTypeTests::~DataTypeTests(void)
{
}

std::string DataTypeTests::IntToString(int value)
{
    char buffer[32];
    int copied = sprintf(buffer, "%d", value);
    return std::string(buffer, copied);
}

void DataTypeTests::DataTypes()
{
    FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateDefaultConnection();
    ASSERT_CAN_CONNECT(conn)

    // create test table with different data types
    FdoPtr<FdoISQLCommand> comm = (FdoISQLCommand *)conn->CreateCommand(FdoCommandType_SQLCommand);

    try
    {
        comm->SetSQLStatement(L"CREATE TABLE DataTypeTests( \
                          dt_varchar2 VARCHAR2(64), dt_int32 NUMBER(8,0) \
                          ,dt_double NUMBER(10,3), dt_byte CHAR(1) \
                          ,dt_charstring CHAR(128), dt_binfloat BINARY_FLOAT \
                          ,dt_bindouble BINARY_DOUBLE, dt_datetime DATE \
                          ,dt_date DATE ,dt_time DATE \
                          ,dt_geom SDO_GEOMETRY, dt_clob CLOB )");
        comm->ExecuteNonQuery();
    }
    catch (FdoException *) // exception because table already exist - skeep it
    {
    }

    try
    {

        // add data to be tested      // TO_DATE('01-01-1997', 'MM-DD-YYYY') TO_DATE('10-10-1', 'HH24-MI-SS')
        comm->SetSQLStatement(L"INSERT INTO DataTypeTests VALUES('dt_varchar2' ,10 ,10.10 ,'b' ,'dt_charstring' ,20.20, 111.111 \
                        ,TO_DATE('01-05-1997 15:16:17','MM-DD-YYYY HH24-MI-SS') \
                        ,TO_DATE('12-02-1998','MM-DD-YYYY') \
                        ,TO_DATE('9:10:11','HH24-MI-SS') \
                        ,NULL,'some CLOB value'\
                        )");
        comm->ExecuteNonQuery();
        comm->SetSQLStatement(L"INSERT INTO DataTypeTests VALUES('dt_varchar2' ,10 ,10.10 ,'b' ,'dt_charstring' ,20.20, 111.111 \
                        ,TO_DATE('01-05-1997 15:16:17','MM-DD-YYYY HH24-MI-SS') \
                        ,TO_DATE('12-02-1998','MM-DD-YYYY') \
                        ,TO_DATE('9:10:11','HH24-MI-SS') \
                        ,NULL,'some CLOB value'\
                        )");
        comm->ExecuteNonQuery();

        // query table row
        comm->SetSQLStatement(L"SELECT \
                        dt_varchar2, dt_int32  \
                        ,dt_double , dt_byte  \
                        ,dt_charstring , dt_binfloat  \
                        ,dt_bindouble , dt_datetime  \
                        ,dt_date  ,dt_time ,dt_geom, dt_clob \
                         from DataTypeTests");
        FdoPtr<FdoISQLDataReader> sqlreader = comm->ExecuteReader();

        // fetch and test values
        while (sqlreader->ReadNext())
        {

            int ccount = sqlreader->GetColumnCount();
            if (ccount != 12)
            {
                CPPUNIT_FAIL("Column count is not 12");
            }

            FdoStringP colname;
            int colind = 0;

            // test column 1
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_varchar2"))
                if (coln != L"dt_varchar2")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_String)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                FdoStringP colval = sqlreader->GetString(colname);
                coln = colval.Lower();
                //if (strcmpi((const char *)colval, "dt_varchar2"))
                if (coln != L"dt_varchar2")
                {
                    CPPUNIT_FAIL("Wrong column value");
                }
            }

            // test column 2
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_int32"))
                if (coln != L"dt_int32")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_Int32)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                int colval = sqlreader->GetInt32(colname);
                if (colval != 10)
                {
                    CPPUNIT_FAIL("Wrong column value");
                }
            }
            // test column 3
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_double"))
                if (coln != L"dt_double")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                //The original test we copied this from was verifiying decimal, but due
                //to bit rot on the original test, this assertion might be invalid and
                //that we are indeed verifying it is a double.
                if (dtype != FdoDataType_Double) //FdoDataType_Decimal)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                double colval = sqlreader->GetDouble(colname);
                if (colval != 10.10)
                {
                    CPPUNIT_FAIL("Wrong column value");
                }
            }
            // test column 4
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_byte"))
                if (coln != L"dt_byte")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_Byte)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                FdoByte colval = sqlreader->GetByte(colname);
                if (colval != 'b')
                {
                    CPPUNIT_FAIL("Wrong column value");
                }
            }
            // test column 5
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_charstring"))
                if (coln != L"dt_charstring")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_String)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                FdoStringP colval = sqlreader->GetString(colname);
                /*
                char tempbuff[64];
                strncpy(tempbuff, (const char *)colval, strlen("dt_charstring"));
                tempbuff[strlen("dt_charstring")] = 0;
                if (strcmpi(tempbuff, "dt_charstring"))
                */
                FdoStringP colv = colval.Lower();
                wchar_t tempbuff[64];
                wcsncpy(tempbuff, (FdoString*)colv, wcslen(L"dt_charstring"));
                tempbuff[wcslen(L"dt_charstring")] = 0;
                FdoStringP temp = tempbuff;
                if (temp != L"dt_charstring")
                {
                    std::string msg = "Wrong column value. Got: ";
                    msg += colv;
                    msg += "(len: ";
                    msg += IntToString(colv.GetLength());
                    msg += ")";
                    CPPUNIT_FAIL(msg);
                }
            }
            // test column 6
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_binfloat"))
                if (coln != L"dt_binfloat")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_Single)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                float colval = sqlreader->GetSingle(colname);
                if (fabs(colval - 20.20) > 0.000001)
                {
                    CPPUNIT_FAIL("Wrong column value");
                }
            }
            // test column 7
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_bindouble"))
                if (coln != L"dt_bindouble")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_Double)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                double colval = sqlreader->GetDouble(colname);
                if (colval != 111.111)
                {
                    CPPUNIT_FAIL("Wrong column value");
                }
            }

            // test column 8
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_datetime"))
                if (coln != L"dt_datetime")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_DateTime)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDateTime colval = sqlreader->GetDateTime(colname);
                if (colval.year != 1997 || colval.month != 1 || colval.day != 5 || colval.hour != 15 || colval.minute != 16 || colval.seconds != 17)
                {
                    CPPUNIT_FAIL("Wrong column value");
                }
            }

            // test column 9
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_date"))
                if (coln != L"dt_date")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_DateTime)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDateTime colval = sqlreader->GetDateTime(colname);
                if (colval.year != 1998 || colval.month != 12 || colval.day != 2)
                {
                    CPPUNIT_FAIL("Wrong column value");
                }
            }
            // test column 10
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_time"))
                if (coln != L"dt_time")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_DateTime)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDateTime colval = sqlreader->GetDateTime(colname);
                if (colval.hour != 9 || colval.minute != 10 || colval.seconds != 11)
                {
                    CPPUNIT_FAIL("Wrong column value");
                }
            }

            // test column 11
            colind++;
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_geom"))
                if (coln != L"dt_geom")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_GeometricProperty)
                {
                    std::string msg = "Wrong column property type. Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
            }

            // test column 12 - CLOB
            colind++;
        #ifdef _WIN32
            {
                colname = sqlreader->GetColumnName(colind);
                //printf("Column: %d - %S\n", colind, (FdoString*)colname);
                FdoStringP coln = colname.Lower();
                const char* utColn = (const char*)coln;
                //if (strcmpi((const char *)colname, "dt_clob"))
                if (coln != L"dt_clob")
                {
                    std::string msg = "Wrong column name. Got: ";
                    msg += utColn;
                    CPPUNIT_FAIL(msg.c_str());
                }
                FdoPropertyType ptype = sqlreader->GetPropertyType(colname);
                if (ptype != FdoPropertyType_DataProperty)
                {
                    std::string msg = "Wrong column property type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)ptype);
                    CPPUNIT_FAIL(msg);
                }
                FdoDataType dtype = sqlreader->GetColumnType(colname);
                if (dtype != FdoDataType_CLOB)
                {
                    std::string msg = "Wrong column data type for (";
                    msg += utColn;
                    msg += "). Got: ";
                    msg += IntToString((int)dtype);
                    CPPUNIT_FAIL(msg);
                }
                if (sqlreader->IsNull(colname))
                {
                    CPPUNIT_FAIL("Invalid NULL value!");
                }
                FdoPtr<FdoLOBValue> lobval = sqlreader->GetLOB(colname);
                FdoPtr<FdoByteArray> barray = lobval->GetData();
                FdoByte *byte = barray->GetData();
                long size = barray->GetCount();

                FdoByteArray::Append(barray, 0);
                FdoByteArray::Append(barray, 0);

                long size2 = barray->GetCount();
                FdoByte *bytes2 = barray->GetData();

                FdoString *str = (FdoString *)bytes2;

                FdoStringP strp(str, true);
                long len = strp.GetLength();
                wchar_t* expectedStr = L"some CLOB value";
                int expectedLength = wcslen(expectedStr);
                if (strp.ICompare(expectedStr))
                {
                    std::string msg = "Invalid CLOB value! Got: '";
                    msg += (const char*)strp;
                    msg += "'";
                    CPPUNIT_FAIL(msg);
                }
                if (len != expectedLength)
                {
                    std::string msg = "Invalid CLOB length! Got: ";
                    msg += IntToString(len);
                    CPPUNIT_FAIL(msg);
                }
            }
        #else
        #warning TODO/FIXME: Reading CLOB data does not work properly on Linux yet. Test disabled until then.
        #endif
        }

        sqlreader->Close();

        //comm->SetSQLStatement(L"DROP TABLE DataTypeTests");
        //comm->ExecuteNonQuery();

        conn->Close();
    }
    catch (FdoException *ex)
    {
        TestCommonFail(ex);
    }

} //end of DataTypeTests::DataTypes

//
// Test geometry transform from sdo_geometry to FDF fromat and back to sdo_geometry
//
void DataTypeTests::GeometryTransform()
{
    FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateDefaultConnection();
    ASSERT_CAN_CONNECT(conn)

    // create test table with different data types
    FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand *)conn->CreateCommand(FdoCommandType_SQLCommand);

    try
    {
        sqlcomm->SetSQLStatement(L"DROP TABLE ut_GeometryTransform");
        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException* ex)
    {
        FDO_SAFE_RELEASE(ex);
    }

    try
    {
        sqlcomm->SetSQLStatement(L"CREATE TABLE ut_GeometryTransform( \
                          fid NUMBER(10,0) \
                          ,geom SDO_GEOMETRY,name VARCHAR2(100) NOT NULL )");
        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException * ex)
    {
        FDO_SAFE_RELEASE(ex);
    }
    try
    {
        // create primary key
        sqlcomm->SetSQLStatement(L"alter table ut_GeometryTransform add constraint ut_GeometryTransform_pk primary key ( fid)");
        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException * ex)
    {
        FDO_SAFE_RELEASE(ex);
    }
    try
    {
        // insert user_sdo_geom_metadata
        sqlcomm->SetSQLStatement(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('ut_GeometryTransform'\
                      , 'GEOM',SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005),SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005))\
                      ,NULL)");

        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException * ex)
    {
        FDO_SAFE_RELEASE(ex);
    }

    try
    {
        sqlcomm->SetSQLStatement(L"DROP TABLE ut_GeometryTransform_sid");
        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException* ex)
    {
        FDO_SAFE_RELEASE(ex);
    }

    try
    {
        //-- Create index
        sqlcomm->SetSQLStatement(L"CREATE INDEX ut_GeometryTransform_sid ON ut_GeometryTransform (\"GEOM\")\
                            INDEXTYPE IS \"MDSYS\".\"SPATIAL_INDEX\" PARAMETERS ('sdo_indx_dims=2')");

        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException * ex)
    {
        FDO_SAFE_RELEASE(ex);
    }

    try
    {
        sqlcomm->SetSQLStatement(L"DROP TABLE ut_GeometryTransform2");
        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException* ex)
    {
        FDO_SAFE_RELEASE(ex);
    }

    try
    {
        sqlcomm->SetSQLStatement(L"CREATE TABLE ut_GeometryTransform2( \
                          fid NUMBER(10,0) \
                          ,geom SDO_GEOMETRY,name VARCHAR2(100) NOT NULL )");
        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException * ex)
    {
        FDO_SAFE_RELEASE(ex);
    }
    try
    {
        // create primary key
        sqlcomm->SetSQLStatement(L"alter table ut_GeometryTransform2 add constraint ut_GeometryTransform2_pk primary key ( fid)");
        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException * ex)
    {
        FDO_SAFE_RELEASE(ex);
    }
    try
    {
        // insert user_sdo_geom_metadata
        sqlcomm->SetSQLStatement(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('ut_GeometryTransform2'\
                      , 'GEOM',SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005),SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005))\
                      ,NULL)");

        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException * ex)
    {
        FDO_SAFE_RELEASE(ex);
    }
    try
    {

        //-- Create index
        sqlcomm->SetSQLStatement(L"CREATE INDEX ut_GeometryTransform2_sid ON ut_GeometryTransform2 (\"GEOM\")\
                            INDEXTYPE IS \"MDSYS\".\"SPATIAL_INDEX\" PARAMETERS ('sdo_indx_dims=2')");

        sqlcomm->ExecuteNonQuery();
    }
    catch (FdoException * ex) // exception because table already exist - skeep it
    {
        FDO_SAFE_RELEASE(ex);
    }

    try
    {
        FdoStringP src_classname = UnitTestUtil::GetFeatureClass(L"UT_GEOMETRYTRANSFORM", L"GEOM");

        FdoPtr<FdoIInsert> comm_insert = (FdoIInsert *)conn->CreateCommand(FdoCommandType_Insert);
        comm_insert->SetFeatureClassName(src_classname);

        FdoPtr<FdoPropertyValueCollection> propcol = comm_insert->GetPropertyValues();

        // Fid property
        FdoPtr<FdoDataValue> fid_val = FdoDataValue::Create(1);
        FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FID", fid_val);

        // Geometry property
        FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();

        double ordinates[10] = {1, 1, 10, 1, 10, 10, 1, 10, 1, 1};

        FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0, 10, ordinates);
        FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring, NULL);

        FdoPtr<FdoByteArray> barray = fgf->GetFgf(polygon);

        FdoPtr<FdoGeometryValue> geom_val = FdoGeometryValue::Create(barray);

        FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOM", geom_val);

        // Name string property
        FdoPtr<FdoStringValue> name_val = (FdoStringValue *)FdoDataValue::Create(L"Name1");
        FdoPtr<FdoPropertyValue> propval_name = FdoPropertyValue::Create(L"NAME", name_val);

        propcol->Add(propval_fid);
        propcol->Add(propval_geom);
        propcol->Add(propval_name);

        FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();

        // Read Source features
        FdoPtr<FdoISelect> comm_select = (FdoISelect *)conn->CreateCommand(FdoCommandType_Select);

        comm_select->SetFeatureClassName(src_classname);

        FdoPtr<FdoIFeatureReader> freader = comm_select->Execute();

        // fetch and test values

        while (freader->ReadNext())
        {
            long fid = freader->GetInt32(L"FID");

            FdoPtr<FdoByteArray> fgf = freader->GetGeometry(L"GEOM");
        }

        freader->Close();

        sqlcomm->SetSQLStatement(L"DROP TABLE ut_GeometryTransform");
        sqlcomm->ExecuteNonQuery();

        sqlcomm->SetSQLStatement(L"DROP TABLE ut_GeometryTransform2");
        sqlcomm->ExecuteNonQuery();

        conn->Close();
    }

    catch (FdoException *ex)
    {
        TestCommonFail(ex);
    }

} //end of DataTypeTests::GeometryTransform()
