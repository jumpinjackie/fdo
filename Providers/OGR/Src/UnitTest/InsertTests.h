#ifndef INSERT_TESTS_H
#define INSERT_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class InsertTests : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(InsertTests);
    CPPUNIT_TEST(TestCase_Insert);
    CPPUNIT_TEST(TestCase_InsertReadOnly);
    CPPUNIT_TEST(TestCase_InsertBadClassName);
    CPPUNIT_TEST_SUITE_END();

public:
    InsertTests();
    ~InsertTests();

    void TestCase_Insert();
    void TestCase_InsertReadOnly();
    void TestCase_InsertBadClassName();

private:
    void CopySourceFilesTo(const wchar_t* dir);
};

#endif //INSERT_TESTS_H