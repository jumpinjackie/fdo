#ifndef VRT_TESTS_H
#define VRT_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class VrtTests : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(VrtTests);
    CPPUNIT_TEST(TestCase_DescribeSchema);
    CPPUNIT_TEST_SUITE_END();

public:
    VrtTests();
    ~VrtTests();

    void TestCase_DescribeSchema();
};

#endif //VRT_TESTS_H