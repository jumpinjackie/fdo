#ifndef DELETE_TESTS_H
#define DELETE_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class DeleteTests : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(DeleteTests);
    CPPUNIT_TEST(TestCase_DeleteFiltered);
    CPPUNIT_TEST(TestCase_DeleteAll);
    CPPUNIT_TEST(TestCase_DeleteBadClassName);
    CPPUNIT_TEST_SUITE_END();

public:
    DeleteTests();
    virtual ~DeleteTests();

    void TestCase_DeleteFiltered();
    void TestCase_DeleteAll();
    void TestCase_DeleteBadClassName();

private:
    void CopySourceFilesTo(const wchar_t* dir);
};

#endif //DELETE_TESTS_H