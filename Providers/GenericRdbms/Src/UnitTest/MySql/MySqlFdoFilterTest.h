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

#ifndef MYSQL_FILTERTEST_H
#define MYSQL_FILTERTEST_H

#include "FdoFilterTest.h"


// =============================================================================
// | The file contains the definition of the class MySqlFdoFilterTest. It 
// | overwrites common code if required for the execution in a MySQL ebviron-
// | ment.
// =============================================================================

class MySqlFdoFilterTest : public FdoFilterTest
{

  friend class FdoRdbmsConnection;

    CPPUNIT_TEST_SUB_SUITE (MySqlFdoFilterTest, FdoFilterTest);
    CPPUNIT_TEST_SUITE_END ();


protected:

    //-------------------------------------------------------------------------
    //                       Test Environment Setup Functions
    //-------------------------------------------------------------------------

    //  GetDimensionality:
    //      The function returns the dimensionality the provider supports.

    FdoInt32 GetDimensionality ();   

    //  SetProvider:
    //      Sets the provider for the current test. Any Provider that supports
    //      these tests need to implement it.

    void SetProvider ();   


    //-------------------------------------------------------------------------
    //                       General Supporting Functions
    //-------------------------------------------------------------------------

    //  TranslateFilter:
    //      The function requests the SQL representation of the given filter
    //      and checks the result whether or not nesting or grouping has been
    //      used. If nesting or grouping has been used where none is expected
    //      or nesting or grouping has not been used when expected an exception
    //      is issued.

    void TranslateFilter (FdoFilter   *filter,
                          bool        is_nesting_expected,
                          bool        is_grouping_expected,
                          UnitTestIds unit_test_id);


private:

    //-------------------------------------------------------------------------
    //                               Helper Functions
    //-------------------------------------------------------------------------

    //  GetValidationStringGrouping:
    //      The generated SQL statement is a select-statement. This function
    //      determines the part that represents the filter and returns it back
    //      to the calling routine.

    FdoStringP GetValidationStringGrouping (FdoStringP  sql_statement,
                                            UnitTestIds unit_test_id);   

    //  GetValidationStringNesting:
    //      The generated SQL statement is a select-statement. This function
    //      determines the part that represents the filter and returns it back
    //      to the calling routine.

    FdoStringP GetValidationStringNesting (FdoStringP  sql_statement,
                                           UnitTestIds unit_test_id);   



};  //  class MySqlFdoFilterTest

#endif
