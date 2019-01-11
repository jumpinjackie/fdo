#ifndef SELECT_ORDER_BY_TEST_H
#define SELECT_ORDER_BY_TEST_H

#include "Fdo.h"
#include <cppunit/extensions/HelperMacros.h>

class SelectOrderByTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SelectOrderByTests);
    CPPUNIT_TEST(SelectOrderBy);

    CPPUNIT_TEST_SUITE_END();

  public:
    SelectOrderByTests(void);

  public:
    ~SelectOrderByTests(void);
    void SelectOrderBy();

  protected:
    void InsertRow(FdoIConnection *Conn, FdoString *ClassName, double *Ordinates, long OrdinatesCount, long Id, FdoString *Text);
};

#endif