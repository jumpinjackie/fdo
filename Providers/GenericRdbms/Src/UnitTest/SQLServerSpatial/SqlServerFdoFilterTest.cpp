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
#include "SqlServerFdoFilterTest.h"
#include "UnitTestUtil.h"
#include "FdoRdbmsConnection.h"
#include "../../SqlServerSpatial/Fdo/FdoRdbmsSqlServerConnection.h"
#include "../../SqlServerSpatial/Fdo/FdoRdbmsSqlServerFilterProcessor.h"


CPPUNIT_TEST_SUITE_REGISTRATION(SqlServerFdoFilterTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SqlServerFdoFilterTest, "FdoFilterTest");


// ----------------------------------------------------------------------------
// --                          Test Setup Functions                          --
// ----------------------------------------------------------------------------

FdoInt32 SqlServerFdoFilterTest::GetDimensionality ()

// +---------------------------------------------------------------------------
// | The function returns the dimensionality the provider supports.
// +---------------------------------------------------------------------------

{

    return (2);

}  //  GetDimensionality ()   

void SqlServerFdoFilterTest::SetProvider ()

// +---------------------------------------------------------------------------
// | The function sets the provider property.
// +---------------------------------------------------------------------------

{

    UnitTestUtil::SetProvider("SQLServerSpatial");

}  //  SetProvider ()


// ----------------------------------------------------------------------------
// --                      General Supporting Functions                      --
// ----------------------------------------------------------------------------

void SqlServerFdoFilterTest::TranslateFilter (FdoFilter   *filter,
                                              bool        is_nesting_expected,
                                              UnitTestIds unit_test_id)

// +---------------------------------------------------------------------------
// | The function executes the nested filter SQL tests and checks if nesting of
// | a filter statement is applied only if necessary.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool                             is_nested         = false;

    FdoStringP                       generated_sql;

    FdoRdbmsSqlServerConnection      *rdbms_connection = NULL;

    FdoRdbmsSqlServerFilterProcessor *filter_processor = NULL;

    // Start the process.

    printf(" >>> Generating the SQL representation for the filter \n");

    // Generate the SQL statement for the given filter.

    rdbms_connection = (FdoRdbmsSqlServerConnection*)(m_connection.p);
    filter_processor = (FdoRdbmsSqlServerFilterProcessor*)
                                    rdbms_connection->GetFilterProcessor();
    generated_sql    = filter_processor->FilterToSql(
                                                    filter,
                                                    L"flt_c1",
                                                    SqlCommandType_Select,
                                                    FdoCommandType_Select);

    // Check whether or not the generated SQL statement is nested. An exception
    // is issued if it is nested but not expected or vise versa.

    printf(" >>> Checking whether or not the SQL statement is nested. \n");

    is_nested = CheckForNesting(GetValidationString(generated_sql,
                                                    unit_test_id));

    if ((is_nested ) && (is_nesting_expected ))
        printf(" >>> ... SQL statement is nested as expected. \n");

    if ((!is_nested) && (!is_nesting_expected))
        printf(" >>> ... SQL statement is not nested as expected. \n");

    if ((is_nested ) && (!is_nesting_expected)) {

        printf(" ... generated SQL statement >>%s<< \n", generated_sql);
        throw FdoException::Create(L"Unexpected nesting of filter in SQL");

    }  //  if ((is_nested ) && (!is_nesting_expected)) ...

    if ((!is_nested) && (is_nesting_expected )) {

        printf(" ... generated SQL statement >>%s<< \n", generated_sql);
        throw FdoException::Create(L"Unexpected not-nesting of filter in SQL");

    }  //  ((!is_nested) && (is_nesting_expected )) ...

}  //  TranslateFilter ()


// ----------------------------------------------------------------------------
// --                            Helper Functions                            --
// ----------------------------------------------------------------------------

FdoStringP SqlServerFdoFilterTest::GetValidationString (
                                                FdoStringP  sql_statement,
                                                UnitTestIds unit_test_id)

// +---------------------------------------------------------------------------
// | The generated SQL statement is a select-statement. This function deter-
// | mines the part that represents the filter and returns it back to the
// | calling routine.
// +---------------------------------------------------------------------------

{

    // The generated SQL statement contains a SQL statement that contains more
    // than just the filter SQL representation. The following separates the
    // filter SQL representation and returns it back to the caller. The pro-
    // cess of getting that part depends on the executed test.

    switch (unit_test_id) {

      case MAXORFILTERSELECTTEST:
      case NEGATIONORFILTERSELECTTEST_2:
      case NESTEDFILTERSQLTEST:
        return sql_statement.Right(L"WHERE"); 

      case NEGATIONORFILTERSELECTTEST_1:
        return (sql_statement.Right(L"NOT")).Mid(3, 1000);

      default:
        return L"";

    }  //  switch ...

}  // GetValidationString ()


