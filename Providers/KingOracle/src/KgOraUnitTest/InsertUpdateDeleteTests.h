#ifndef INSERT_UPDATE_DELETE_TESTS_H
#define INSERT_UPDATE_DELETE_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class InsertUpdateDeleteTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(InsertUpdateDeleteTests);
    CPPUNIT_TEST(Insert_BadClassName);
    CPPUNIT_TEST(Update_BadClassName);
    CPPUNIT_TEST(Delete_BadClassName);
    CPPUNIT_TEST(InsertUpdateDelete);
    CPPUNIT_TEST(InsertCurvedPolygon);
    // FIXME: This is not currently working on Linux
    //CPPUNIT_TEST(InsertUnicode);
    CPPUNIT_TEST(InsertMultiPoint);
    CPPUNIT_TEST(InsertMultiGeometry);
    //CPPUNIT_TEST( Update_BLDG_POLYGON );
    CPPUNIT_TEST_SUITE_END();

  public:
    InsertUpdateDeleteTests(void);

  public:
    ~InsertUpdateDeleteTests(void);

    void Insert_BadClassName();
    void Update_BadClassName();
    void Delete_BadClassName();

    void InsertUpdateDelete();
    void InsertCurvedPolygon();
    void InsertUnicode();

    void InsertMultiPoint();
    void InsertMultiGeometry();

    //void Update_BLDG_POLYGON();

  protected:
    void PrepareTables(FdoIConnection *Connection);
};

#endif