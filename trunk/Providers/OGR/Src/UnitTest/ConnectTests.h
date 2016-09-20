#ifndef CONNECT_TESTS_H
#define CONNECT_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class ConnectTests : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(ConnectTests);
    CPPUNIT_TEST(TestCase_Empty);
    CPPUNIT_TEST(TestCase_DXF);
    CPPUNIT_TEST(TestCase_MapInfo);
    CPPUNIT_TEST(TestCase_SHP);
    CPPUNIT_TEST(TestCase_GeoJSON);
    CPPUNIT_TEST(TestCase_VRT);
    CPPUNIT_TEST(TestCase_DGN);
    CPPUNIT_TEST(TestCase_E00);
    CPPUNIT_TEST(TestCase_BNA);
    CPPUNIT_TEST_SUITE_END();

public:
    ConnectTests();
    ~ConnectTests();

    void TestCase_Empty();

    void TestCase_DXF();
    void TestCase_MapInfo();
    void TestCase_SHP();
    void TestCase_GeoJSON();
    void TestCase_VRT();
    void TestCase_DGN();
    void TestCase_E00();
    void TestCase_BNA();
};

#endif //CONNECT_TESTS_H