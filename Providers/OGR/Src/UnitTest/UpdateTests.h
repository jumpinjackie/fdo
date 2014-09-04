#ifndef UPDATE_TESTS_H
#define UPDATE_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class UpdateTests : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(UpdateTests);
    CPPUNIT_TEST(TestCase_Update);
    CPPUNIT_TEST(TestCase_UpdateReadOnly);
    CPPUNIT_TEST_SUITE_END();

public:
    UpdateTests();
    ~UpdateTests();

    void TestCase_Update();
    void TestCase_UpdateReadOnly();
};

#endif //UPDATE_TESTS_H