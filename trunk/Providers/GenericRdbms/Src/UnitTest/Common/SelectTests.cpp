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
#include "UnitTestUtil.h"
#include "SelectTests.h"

#define DB_SUFFIX L"_rdbitest"

SelectTests::SelectTests (void)
{
}

SelectTests::~SelectTests (void)
{
}

void SelectTests::setUp ()
{
    try
    {
        set_provider();

        static bool bDatastoreCreated = false;
        FdoStringP dataStoreName = UnitTestUtil::GetEnviron("datastore", DB_SUFFIX);
        FdoStringP userName = UnitTestUtil::GetEnviron("username");
        FdoStringP userPassword = UnitTestUtil::GetEnviron("password");
        if (!bDatastoreCreated)
        {
	        try
            {
		        FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
                FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
                connection->SetConnectionString( userConnectString );
                connection->Open();
		        FdoPtr<FdoIDestroyDataStore> pDelCmd = (FdoIDestroyDataStore*)connection->CreateCommand( FdoCommandType_DestroyDataStore );
		        FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pDelCmd->GetDataStoreProperties();
		        dictionary->SetProperty( L"DataStore",  dataStoreName);
		        pDelCmd->Execute();
		        connection->Close();
            }
            catch( FdoException* exc) { exc->Release(); }
            catch(...) { }

	        UnitTestUtil::CreateDB(false, false, DB_SUFFIX);
            bDatastoreCreated = true;
        }

        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init ());
        try
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_connect failed", RDBI_SUCCESS == do_rdbi_connect (dataStoreName, userName, userPassword));
            if ( mRdbiContext->dispatch.capabilities.supports_unicode == 1 )
                CPPUNIT_ASSERT_MESSAGE ("rdbi_set_schemaW failed", RDBI_SUCCESS == rdbi_set_schemaW (mRdbiContext, dataStoreName));
            else
                CPPUNIT_ASSERT_MESSAGE ("rdbi_set_schema failed", RDBI_SUCCESS == rdbi_set_schema (mRdbiContext, dataStoreName));
        }
        catch (CppUnit::Exception exception)
        {
            rdbi_term (&mRdbiContext);
            throw exception;
        }
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void SelectTests::tearDown ()
{
    try
    {
        try
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_disconnect failed", RDBI_SUCCESS == rdbi_disconnect (mRdbiContext));
        }
        catch (CppUnit::Exception exception)
        {
            rdbi_term (&mRdbiContext);
            throw exception;
        }
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&mRdbiContext));
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

int SelectTests::do_rdbi_init ()
{
    CPPUNIT_FAIL ("naked do_rdbi_init");
    return (0);
}

int SelectTests::do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword)
{
    CPPUNIT_FAIL ("naked do_rdbi_connect");
    return (0);
}

char *SelectTests::get_bind_var (int n)
{
    CPPUNIT_FAIL ("naked get_bind_var");
    return ("");
}

char *SelectTests::get_date_time (const struct tm *when)
{
    CPPUNIT_FAIL ("naked get_date_time");
    return ("");
}

int SelectTests::rdbi_sql_Ex( rdbi_context_def *context, int sqlid, FdoStringP sql )
{
    if (context->dispatch.capabilities.supports_unicode == 1)
        return ::rdbi_sqlW( context, sqlid, sql );
    else
        return ::rdbi_sql( context, sqlid, sql );
}

int SelectTests::rdbi_desc_slct_Ex( rdbi_context_def *context, int sqlid, int pos, int name_len, char *name, int *rdbi_type, int *binary_size, int *null_ok )
{
    if (context->dispatch.capabilities.supports_unicode == 1)
    {
        wchar_t wname[1024];
        wname[0] = L'\0';
        int rc = ::rdbi_desc_slctW( context, sqlid, pos, name_len, wname, rdbi_type, binary_size, null_ok );
        strcpy (name, FdoStringP(wname));
        return rc;
    }
    else
        return ::rdbi_desc_slct( context, sqlid, pos, name_len, name, rdbi_type, binary_size, null_ok );
}

void SelectTests::ddl ()
{
    int cursor;
    char *query;

    cursor = -1;
    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));

        // drop the table if it already exists... ignore errors
        query = "drop table foo";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        rdbi_execute (mRdbiContext, cursor, 1, 0);

        query = "create table foo (id decimal(10), description varchar(50))";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        query = "drop table foo";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_fre_cursor failed", RDBI_SUCCESS == rdbi_fre_cursor (mRdbiContext, cursor));
        cursor = -1;
    }
    catch (CppUnit::Exception exception)
    {
		const char *msg = exception.what ();
        char message[RDBI_MSG_SIZE];
        rdbi_get_msg (mRdbiContext);
#ifdef _WIN32
		WideCharToMultiByte ( CP_THREAD_ACP, 0, mRdbiContext->last_error_msg, (int)wcslen(mRdbiContext->last_error_msg), message, RDBI_MSG_SIZE, NULL,  NULL);
#else
		wcstombs ( message, mRdbiContext->last_error_msg, RDBI_MSG_SIZE);
#endif
        strcat (message, ": ");
        strcat (message, msg);
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        throw CppUnit::Exception (message);
    }
    catch (...)
    {
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

#define ANSWER "the answer to the question of the meaning of life"

void SelectTests::define ()
{
    int cursor;
    char *query;
    const char phrase[] = ANSWER;
    char answer[50];
    int rc;
    int count;
    int rows;

    cursor = -1;
    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));

        // drop the table if it already exists... ignore errors
        query = "drop table foo";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        rdbi_execute (mRdbiContext, cursor, 1, 0);

        query = "create table foo (id decimal(10), description varchar(50))";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        // insert a row
        query = "insert into foo values (42, '" ANSWER "')";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        if (mRdbiContext->dispatch.capabilities.supports_unicode == 1){
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_vaW (mRdbiContext, RDBI_VA_EXEC, cursor,
            L"select description from foo where id=42", RDBI_VA_EOL, RDBI_STRING, sizeof (answer), answer, RDBI_VA_EOL));
        }else{
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_va (mRdbiContext, RDBI_VA_EXEC, cursor,
            "select description from foo where id=42", RDBI_VA_EOL, RDBI_STRING, sizeof (answer), answer, RDBI_VA_EOL));
        }
        count = 0;
        do
        {
            rc = rdbi_fetch (mRdbiContext, cursor, 1, &rows);
            CPPUNIT_ASSERT_MESSAGE ("rdbi_fetch failed", (RDBI_SUCCESS == rc) || (RDBI_END_OF_FETCH == rc));
            if ((RDBI_END_OF_FETCH != rc) && (0 != rows))
            {
                CPPUNIT_ASSERT_MESSAGE ("fetched value incorrect", 0 == strcmp (phrase, answer));
                count++;
            }
        }
        while (RDBI_SUCCESS == rc);
        CPPUNIT_ASSERT_MESSAGE ("fetched count incorrect", 1 == count);
        CPPUNIT_ASSERT_MESSAGE ("rdbi_end_select failed", RDBI_SUCCESS == rdbi_end_select (mRdbiContext, cursor));

        query = "drop table foo";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_fre_cursor failed", RDBI_SUCCESS == rdbi_fre_cursor (mRdbiContext, cursor));
        cursor = -1;
    }
    catch (CppUnit::Exception exception)
    {
        const char *msg = exception.what ();
        char message[RDBI_MSG_SIZE];
        rdbi_get_msg (mRdbiContext);
#ifdef _WIN32
		WideCharToMultiByte ( CP_THREAD_ACP, 0, mRdbiContext->last_error_msg, (int)wcslen(mRdbiContext->last_error_msg), message, RDBI_MSG_SIZE, NULL,  NULL);
#else
		wcstombs ( message, mRdbiContext->last_error_msg, RDBI_MSG_SIZE);
#endif
        strcat (message, ": ");
        strcat (message, msg);
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        throw CppUnit::Exception (message);
    }
    catch (...)
    {
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void SelectTests::bind ()
{
    int cursor;
    char *query;
    const char phrase[] = ANSWER;
    char answer[50];
    char select[1024];
    int hit;
    int rc;
    int count;
    int rows;

    cursor = -1;
    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));

        // drop the table if it already exists... ignore errors
        query = "drop table foo";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        rdbi_execute (mRdbiContext, cursor, 1, 0);

        query = "create table foo (id decimal(10), description varchar(50))";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        // insert a couple of rows
        query = "insert into foo values (2525, 'if man is still alive')";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));
        query = "insert into foo values (42, '" ANSWER "')";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));
        query = "insert into foo values (99, 'bottles of beer on the wall')";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        hit = 42;
        sprintf (select, "select description from foo where id=%s", get_bind_var (1));
        if (mRdbiContext->dispatch.capabilities.supports_unicode == 1){
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_vaW (mRdbiContext, RDBI_VA_EXEC, cursor,
                FdoStringP(select), RDBI_INT, sizeof(hit), &hit, RDBI_VA_EOL, RDBI_STRING, sizeof (answer), answer, RDBI_VA_EOL));
        }else{
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_va (mRdbiContext, RDBI_VA_EXEC, cursor,
                FdoStringP(select), RDBI_INT, sizeof(hit), &hit, RDBI_VA_EOL, RDBI_STRING, sizeof (answer), answer, RDBI_VA_EOL));
        }
        count = 0;
        rows = 0;
        do
        {
            rc = rdbi_fetch (mRdbiContext, cursor, 1, &rows);
            CPPUNIT_ASSERT_MESSAGE ("rdbi_fetch failed", (RDBI_SUCCESS == rc) || (RDBI_END_OF_FETCH == rc));
            if ((RDBI_END_OF_FETCH != rc) && (0 != rows))
            {
                CPPUNIT_ASSERT_MESSAGE ("fetched value incorrect", 0 == strcmp (phrase, answer));
                count++;
            }
        }
        while (RDBI_SUCCESS == rc);
        CPPUNIT_ASSERT_MESSAGE ("fetched count incorrect", 1 == count);
        CPPUNIT_ASSERT_MESSAGE ("rdbi_end_select failed", RDBI_SUCCESS == rdbi_end_select (mRdbiContext, cursor));

        query = "drop table foo";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_fre_cursor failed", RDBI_SUCCESS == rdbi_fre_cursor (mRdbiContext, cursor));
        cursor = -1;
    }
    catch (CppUnit::Exception exception)
    {
        const char *msg = exception.what ();
        char message[RDBI_MSG_SIZE];
        rdbi_get_msg (mRdbiContext);
#ifdef _WIN32
		WideCharToMultiByte ( CP_THREAD_ACP, 0, mRdbiContext->last_error_msg, (int)wcslen(mRdbiContext->last_error_msg), message, RDBI_MSG_SIZE, NULL,  NULL);
#else
		wcstombs ( message, mRdbiContext->last_error_msg, RDBI_MSG_SIZE);
#endif
        strcat (message, ": ");
        strcat (message, msg);
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        throw CppUnit::Exception (message);
    }
    catch (...)
    {
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void SelectTests::describe ()
{
    int cursor;
    char *query;
    char statement[1024];
    char name[50];
    char initial[2];
    short grade;
    char salutation[5];
    double employee; // should be int
    double height; // should be float
    double salary;
    char birthday[64];
#ifdef _WIN32
    LONGLONG phone;
#else
	int64_t phone;	
#endif
    struct tm bday;
    char *p;
    int type;
    int size;
    int null_ok;
    FdoInt64 null_name;
    FdoInt64 null_initial;
    FdoInt64 null_grade;
    FdoInt64 null_salutation;
    FdoInt64 null_employee;
    FdoInt64 null_height;
    FdoInt64 null_salary;
    FdoInt64 null_birthday;
    FdoInt64 null_phone;
    int rows;

    cursor = -1;
    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));

        // drop the table if it already exists... ignore errors
        query = "drop table bar";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        rdbi_execute (mRdbiContext, cursor, 1, 0);

        // not RDBI_BLOB, RDBI_RAW, RDBI_ROWID, RDBI_GEOMETRY or RDBI_BLOB_REF

        // mysql:
        // +------------+--------------+------+-----+---------+-------+
        // | Field      | Type         | Null | Key | Default | Extra |
        // +------------+--------------+------+-----+---------+-------+
        // | name       | varchar(200) | YES  |     | NULL    |       |
        // | initials   | char(1)      | YES  |     | NULL    |       |
        // | grade      | decimal(2,0) | YES  |     | NULL    |       |
        // | salutation | varchar(4)   | YES  |     | NULL    |       |
        // | employee   | int(11)      | YES  |     | NULL    |       |
        // | height     | float        | YES  |     | NULL    |       |
        // | salary     | double       | YES  |     | NULL    |       |
        // | birthday   | datetime     | YES  |     | NULL    |       |
        // | phone      | decimal(7,0) | YES  |     | NULL    |       |
        // +------------+--------------+------+-----+---------+-------+

        // oracle:
        //  Name                                      Null?    Type
        //  ----------------------------------------- -------- ----------------------------
        //  NAME                                               VARCHAR2(200)
        //  INITIALS                                           CHAR(1)
        //  GRADE                                              NUMBER(2)
        //  SALUTATION                                         CHAR(4)
        //  EMPLOYEE                                           NUMBER(38)
        //  HEIGHT                                             FLOAT(126)
        //  SALARY                                             NUMBER
        //  BIRTHDAY                                           TIMESTAMP(6)
        //  PHONE                                              NUMBER(7)

        sprintf (statement, "create table bar ("
            "name varchar(200)," // RDBI_STRING
            "initials char," // RDBI_CHAR
            "grade decimal(2)," // RDBI_SHORT
            "salutation char(4)," // RDBI_FIXED_CHAR
            "employee integer," // want RDBI_INT but we get RDBI_DOUBLE
            "height float," // want RDBI_FLOAT but we get RDBI_DOUBLE
            "salary decimal (8, 2)," // RDBI_DOUBLE
            "phone decimal(7)," // RDBI_LONG
            "birthday timestamp)"); // RDBI_DATE
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, statement));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        // set up an bound insert
        sprintf (statement, "insert into bar values (%s, %s, %s, %s, %s, %s, %s, %s, %s)",
            get_bind_var (1),
            get_bind_var (2),
            get_bind_var (3),
            get_bind_var (4),
            get_bind_var (5),
            get_bind_var (6),
            get_bind_var (7),
            get_bind_var (8),
            allow_timestamp_notnull()? get_bind_var (9) : "NULL");
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, statement));

        // define the bind variables
// DS: why size is "sizeof (name) - 1" ?
        CPPUNIT_ASSERT_MESSAGE ("rdbi_bind failed", RDBI_SUCCESS == rdbi_bind (mRdbiContext, cursor, "1", RDBI_STRING, sizeof (name)-1, name, (char*)&null_name));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_bind failed", RDBI_SUCCESS == rdbi_bind (mRdbiContext, cursor, "2", RDBI_CHAR, sizeof (initial), initial, (char*)&null_initial));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_bind failed", RDBI_SUCCESS == rdbi_bind (mRdbiContext, cursor, "3", RDBI_SHORT, sizeof (grade), (char*)&grade, (char*)&null_grade));
// DS: why size is "sizeof (salutation) - 1" ?
        CPPUNIT_ASSERT_MESSAGE ("rdbi_bind failed", RDBI_SUCCESS == rdbi_bind (mRdbiContext, cursor, "4", RDBI_FIXED_CHAR, sizeof (salutation)-1, salutation, (char*)&null_salutation));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_bind failed", RDBI_SUCCESS == rdbi_bind (mRdbiContext, cursor, "5", RDBI_DOUBLE, sizeof (employee), (char*)&employee, (char*)&null_employee));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_bind failed", RDBI_SUCCESS == rdbi_bind (mRdbiContext, cursor, "6", RDBI_DOUBLE, sizeof (height), (char*)&height, (char*)&null_height));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_bind failed", RDBI_SUCCESS == rdbi_bind (mRdbiContext, cursor, "7", RDBI_DOUBLE, sizeof (salary), (char*)&salary, (char*)&null_salary));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_bind failed", RDBI_SUCCESS == rdbi_bind (mRdbiContext, cursor, "8", RDBI_LONG, sizeof (phone), (char*)&phone, (char*)&null_phone));
 
        if ( allow_timestamp_notnull() )
            CPPUNIT_ASSERT_MESSAGE ("rdbi_bind failed", RDBI_SUCCESS == rdbi_bind (mRdbiContext, cursor, "9", RDBI_DATE, sizeof (birthday), birthday, (char*)&null_birthday));

        int num_bound_cols = allow_timestamp_notnull()? 9 : 8;

        // describe bind variables
        for (int i = 1; i <= num_bound_cols; i++)
        {
            char bind_var[10];
            char bind_name[1024];
            CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_bind failed", RDBI_SUCCESS == rdbi_desc_bind (mRdbiContext, cursor, i, sizeof(bind_name), bind_name));
            FdoCommonOSUtil::itoa (i, bind_var);
            CPPUNIT_ASSERT_MESSAGE ("bind name different", 0 == strcmp (bind_name, bind_var));
        }

        rdbi_set_nnull(mRdbiContext, &null_name, 0, 0);
        rdbi_set_nnull(mRdbiContext, &null_initial, 0, 0);
        rdbi_set_nnull(mRdbiContext, &null_grade, 0, 0);
        rdbi_set_nnull(mRdbiContext, &null_salutation, 0, 0);
        rdbi_set_nnull(mRdbiContext, &null_employee, 0, 0);
        rdbi_set_nnull(mRdbiContext, &null_height, 0, 0);
        rdbi_set_nnull(mRdbiContext, &null_salary, 0, 0);
        rdbi_set_nnull(mRdbiContext, &null_phone, 0, 0);

        // timestamp, SQLServer accept NULL only - doesn't work to set the null ind, stii getting:
        // "Cannot insert a non-null value into a timestamp column. Use INSERT with a column list or with a default of NULL
        //for the timestamp column."
        set_timestamp_nnull((char *)&null_birthday); 

        // insert a couple of rows
        strcpy (name, "Jones");
        initial[0] = 'T';
        initial[1] = '\0';
        grade = 13;
        strcpy (salutation, "Mr  "); // SqlServer is padding with spaces a FIXED CHAR
        employee = 82828;
        height = 183.7f;
        salary = 86788.90;
        memset (&bday, 0, sizeof (struct tm));
        bday.tm_year = 1979;
        bday.tm_mon = 11;
        bday.tm_mday = 30;
        bday.tm_hour = 21;
        bday.tm_min = 17;
        bday.tm_sec = 34;
        p = get_date_time (&bday);
        strcpy (birthday, p);
        delete[] p;
        phone = 7457899;
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        strcpy (name, "Smith");
        initial[0] = 'W';
        grade = 10;
        strcpy (salutation, "Mrs "); // SqlServer is padding with spaces a FIXED CHAR
        employee = 72727;
        height = 176.9f;
        salary = 95345.50;
        bday.tm_year = 1978;
        bday.tm_mon = 4;
        bday.tm_mday = 1;
        bday.tm_hour = 11;
        bday.tm_min = 12;
        bday.tm_sec = 51;
        p = get_date_time (&bday);
        strcpy (birthday, p);

        delete[] p;
        phone = 9671111;
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        // describe the select
        query = "select * from bar";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 1, sizeof (name), name, &type, &size, &null_ok));
        CPPUNIT_ASSERT_MESSAGE ("name wrong name", 0 == FdoCommonOSUtil::stricmp ("name", name));
        CPPUNIT_ASSERT_MESSAGE ("name wrong type", RDBI_STRING == type);
        CPPUNIT_ASSERT_MESSAGE ("name wrong size", 200 == (size / get_char_size()));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 2, sizeof (name), name, &type, &size, &null_ok));
        CPPUNIT_ASSERT_MESSAGE ("initials wrong name", 0 == FdoCommonOSUtil::stricmp ("initials", name));
        CPPUNIT_ASSERT_MESSAGE ("initials wrong type", ((get_char_size() == 1) ? RDBI_CHAR : RDBI_FIXED_CHAR) == type);
        CPPUNIT_ASSERT_MESSAGE ("initials wrong size", get_single_char_size() == size);

        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 3, sizeof (name), name, &type, &size, &null_ok));
        CPPUNIT_ASSERT_MESSAGE ("grade wrong name", 0 == FdoCommonOSUtil::stricmp ("grade", name));
        CPPUNIT_ASSERT_MESSAGE ("grade wrong type", RDBI_SHORT == type);
#pragma message ("ToDo: fix MySql so it doesn't lie about the binary size")
        CPPUNIT_ASSERT_MESSAGE ("grade wrong size", 2 <= size);

        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 4, sizeof (name), name, &type, &size, &null_ok));
        CPPUNIT_ASSERT_MESSAGE ("salutation wrong name", 0 == FdoCommonOSUtil::stricmp ("salutation", name));
#pragma message ("ToDo: fix MySql so it doesn't alter the coloumn type")
        CPPUNIT_ASSERT_MESSAGE ("salutation wrong type", (RDBI_FIXED_CHAR == type) || (RDBI_STRING == type));
        CPPUNIT_ASSERT_MESSAGE ("salutation wrong size", 4 == (size/get_char_size()));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 5, sizeof (name), name, &type, &size, &null_ok));
        CPPUNIT_ASSERT_MESSAGE ("employee wrong name", 0 == FdoCommonOSUtil::stricmp ("employee", name));
 //       CPPUNIT_ASSERT_MESSAGE ("employee wrong type", RDBI_DOUBLE == type);
        //CPPUNIT_ASSERT_MESSAGE ("employee wrong size", 8 <= size);

        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 6, sizeof (name), name, &type, &size, &null_ok));
        CPPUNIT_ASSERT_MESSAGE ("height wrong name", 0 == FdoCommonOSUtil::stricmp ("height", name));
        CPPUNIT_ASSERT_MESSAGE ("height wrong type", RDBI_DOUBLE == type);
        CPPUNIT_ASSERT_MESSAGE ("height wrong size", 8 <= size);

        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 7, sizeof (name), name, &type, &size, &null_ok));
        CPPUNIT_ASSERT_MESSAGE ("salary wrong name", 0 == FdoCommonOSUtil::stricmp ("salary", name));
        CPPUNIT_ASSERT_MESSAGE ("salary wrong type", RDBI_DOUBLE == type);
        CPPUNIT_ASSERT_MESSAGE ("salary wrong size", 8 <= size);

        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 8, sizeof (name), name, &type, &size, &null_ok));
        CPPUNIT_ASSERT_MESSAGE ("phone wrong name", 0 == FdoCommonOSUtil::stricmp ("phone", name));
        CPPUNIT_ASSERT_MESSAGE ("phone wrong type", RDBI_LONG == type);
        CPPUNIT_ASSERT_MESSAGE ("phone wrong size", 4 <= size);
        
        if ( allow_timestamp_notnull() )
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 9, sizeof (name), name, &type, &size, &null_ok));
            CPPUNIT_ASSERT_MESSAGE ("birthday wrong name", 0 == FdoCommonOSUtil::stricmp ("birthday", name));
            CPPUNIT_ASSERT_MESSAGE ("birthday wrong type", RDBI_DATE == type);
            CPPUNIT_ASSERT_MESSAGE ("birthday wrong size", 11 <= size);
        }

        // define the result variables
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "1", RDBI_STRING, sizeof (name), name, (char*)&null_name));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "2", RDBI_CHAR, sizeof (initial), initial, (char*)&null_initial));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "3", RDBI_SHORT, sizeof (grade), (char*)&grade, (char*)&null_grade));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "4", RDBI_FIXED_CHAR, sizeof (salutation), salutation, (char*)&null_salutation));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "5", RDBI_DOUBLE, sizeof (employee), (char*)&employee, (char*)&null_employee));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "6", RDBI_DOUBLE, sizeof (height), (char*)&height, (char*)&null_height));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "7", RDBI_DOUBLE, sizeof (salary), (char*)&salary, (char*)&null_salary)); 
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "8", RDBI_LONG, sizeof (phone), (char*)&phone, (char*)&null_phone));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "9", RDBI_DATE, sizeof (birthday), birthday, (char*)&null_birthday));  

        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        // fetch and check the values
        CPPUNIT_ASSERT_MESSAGE ("rdbi_fetch failed", RDBI_SUCCESS == rdbi_fetch (mRdbiContext, cursor, 1, &rows));
        CPPUNIT_ASSERT_MESSAGE ("name is null", !rdbi_is_null (mRdbiContext, &null_name, 0) );
        CPPUNIT_ASSERT_MESSAGE ("name wrong value", 0 == FdoCommonOSUtil::stricmp ("Jones", name));
        CPPUNIT_ASSERT_MESSAGE ("initial is null", !rdbi_is_null (mRdbiContext, &null_initial, 0));
        CPPUNIT_ASSERT_MESSAGE ("initial wrong value", 'T' == initial[0]);
        CPPUNIT_ASSERT_MESSAGE ("grade is null", !rdbi_is_null (mRdbiContext, &null_grade, 0));
        CPPUNIT_ASSERT_MESSAGE ("grade wrong value", 13 == grade);
        CPPUNIT_ASSERT_MESSAGE ("salutation is null", !rdbi_is_null (mRdbiContext, &null_salutation, 0));
        CPPUNIT_ASSERT_MESSAGE ("salutation wrong value", 0 == strncmp ("Mr", salutation, 2));
        CPPUNIT_ASSERT_MESSAGE ("employee is null", !rdbi_is_null (mRdbiContext, &null_employee, 0));
        CPPUNIT_ASSERT_MESSAGE ("employee wrong value", 82828 == employee);
        CPPUNIT_ASSERT_MESSAGE ("height is null", !rdbi_is_null (mRdbiContext, &null_height, 0));
#pragma message ("ToDo: fix Oracle so it understands IEEE floats and doubles")
        CPPUNIT_ASSERT_MESSAGE ("height wrong value", 1e-4 > fabs (183.7f - height));
        CPPUNIT_ASSERT_MESSAGE ("salary is null", !rdbi_is_null (mRdbiContext, &null_salary, 0));
        CPPUNIT_ASSERT_MESSAGE ("salary wrong value", 1e-8 > fabs (86788.90 - salary));
        CPPUNIT_ASSERT_MESSAGE ("birthday is null", !rdbi_is_null (mRdbiContext, &null_birthday, 0));
        bday.tm_year = 1979;
        bday.tm_mon = 11;
        bday.tm_mday = 30;
        bday.tm_hour = 21;
        bday.tm_min = 17;
        bday.tm_sec = 34;
        p = get_date_time (&bday);
        if ( allow_timestamp_notnull() )
            CPPUNIT_ASSERT_MESSAGE ("birthday wrong value", 0 == FdoCommonOSUtil::stricmp (p, birthday));

        delete[] p;
        CPPUNIT_ASSERT_MESSAGE ("phone is null", !rdbi_is_null (mRdbiContext, &null_phone, 0));
        CPPUNIT_ASSERT_MESSAGE ("phone wrong value", 7457899 == phone);

        memset(salutation, '\0', sizeof(salutation));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_fetch failed", RDBI_SUCCESS == rdbi_fetch (mRdbiContext, cursor, 1, &rows));
        CPPUNIT_ASSERT_MESSAGE ("name is null", !rdbi_is_null (mRdbiContext, &null_name, 0));
        CPPUNIT_ASSERT_MESSAGE ("name wrong value", 0 == FdoCommonOSUtil::stricmp ("Smith", name));
        CPPUNIT_ASSERT_MESSAGE ("initial is null", !rdbi_is_null (mRdbiContext, &null_initial, 0));
        CPPUNIT_ASSERT_MESSAGE ("initial wrong value", 'W' == initial[0]);
        CPPUNIT_ASSERT_MESSAGE ("grade is null", !rdbi_is_null (mRdbiContext, &null_grade, 0));
        CPPUNIT_ASSERT_MESSAGE ("grade wrong value", 10 == grade);
        CPPUNIT_ASSERT_MESSAGE ("salutation is null", !rdbi_is_null (mRdbiContext, &null_salutation, 0));
        CPPUNIT_ASSERT_MESSAGE ("salutation wrong value", 0 == strncmp ("Mrs", salutation, 3));
        CPPUNIT_ASSERT_MESSAGE ("employee is null", !rdbi_is_null (mRdbiContext, &null_employee, 0));
        CPPUNIT_ASSERT_MESSAGE ("employee wrong value", 72727 == employee);
        CPPUNIT_ASSERT_MESSAGE ("height is null", !rdbi_is_null (mRdbiContext, &null_height, 0));
        CPPUNIT_ASSERT_MESSAGE ("height wrong value", 1e-4 > fabs (176.9f - height));
        CPPUNIT_ASSERT_MESSAGE ("salary is null", !rdbi_is_null (mRdbiContext, &null_salary, 0));
        CPPUNIT_ASSERT_MESSAGE ("salary wrong value", 1e-8 > fabs (95345.50 - salary));
        CPPUNIT_ASSERT_MESSAGE ("birthday is null", !rdbi_is_null (mRdbiContext, &null_birthday, 0));
        bday.tm_year = 1978;
        bday.tm_mon = 4;
        bday.tm_mday = 1;
        bday.tm_hour = 11;
        bday.tm_min = 12;
        bday.tm_sec = 51;
        p = get_date_time (&bday);
        if ( allow_timestamp_notnull() )
            CPPUNIT_ASSERT_MESSAGE ("birthday wrong value", 0 == FdoCommonOSUtil::stricmp (p, birthday));

        delete[] p;
        CPPUNIT_ASSERT_MESSAGE ("phone is null", !rdbi_is_null (mRdbiContext, &null_phone, 0));
        CPPUNIT_ASSERT_MESSAGE ("phone wrong value", 9671111 == phone);

        query = "drop table bar";
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, query));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_fre_cursor failed", RDBI_SUCCESS == rdbi_fre_cursor (mRdbiContext, cursor));
        cursor = -1;
    }
    catch (CppUnit::Exception exception)
    {
        const char *msg = exception.what ();
        char message[RDBI_MSG_SIZE];
        memset( (void*) message, 0, (size_t) RDBI_MSG_SIZE );
        rdbi_get_msg (mRdbiContext);
#ifdef _WIN32
		WideCharToMultiByte ( CP_THREAD_ACP, 0, mRdbiContext->last_error_msg, (int)wcslen(mRdbiContext->last_error_msg), message, RDBI_MSG_SIZE, NULL,  NULL);
#else
		wcstombs ( message, mRdbiContext->last_error_msg, RDBI_MSG_SIZE);
#endif
        strcat (message, ": ");
        strcat (message, msg);
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        throw CppUnit::Exception (message);
    }
    catch (...)
    {
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}
