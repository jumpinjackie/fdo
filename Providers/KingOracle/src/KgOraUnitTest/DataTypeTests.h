#ifndef DATA_TYPE_TESTS_H
#define DATA_TYPE_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class DataTypeTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(DataTypeTests);
    CPPUNIT_TEST(DataTypes);
    CPPUNIT_TEST(GeometryTransform);
    CPPUNIT_TEST_SUITE_END();

  public:
    DataTypeTests(void);

  public:
    ~DataTypeTests(void);

    void DataTypes();
    void GeometryTransform();

  private:
    std::string IntToString(int value);
};

#endif