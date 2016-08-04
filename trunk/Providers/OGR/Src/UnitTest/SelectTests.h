#ifndef SELECT_TESTS_H
#define SELECT_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class SelectTests : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(SelectTests);
    CPPUNIT_TEST(TestCase_SelectAll);
    CPPUNIT_TEST(TestCase_SelectAllExplicitPropertyList);
    CPPUNIT_TEST(TestCase_SelectAllComputedProperties);
    CPPUNIT_TEST(TestCase_SelectAggregateCount);
    CPPUNIT_TEST(TestCase_SelectAggregateDistinct);
    CPPUNIT_TEST(TestCase_SelectAggregateSpatialExtent);
    CPPUNIT_TEST_SUITE_END();

public:
    SelectTests();
    ~SelectTests();

    void TestCase_SelectAll();
    void TestCase_SelectAllExplicitPropertyList();
    void TestCase_SelectAllComputedProperties();
    void TestCase_SelectAggregateCount();
    void TestCase_SelectAggregateDistinct();
    void TestCase_SelectAggregateSpatialExtent();
};

#endif //SELECT_TESTS_H