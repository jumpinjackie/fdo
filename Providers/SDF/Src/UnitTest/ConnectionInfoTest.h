#ifndef _CONNECTIONINFOTEST_H_
#define _CONNECTIONINFOTEST_H_
#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_CONNECTIONINFOTEST_H
#define CPP_UNIT_CONNECTIONINFOTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Fdo.h>
#include <TestCommonMiscUtil.h>

/* 
 * Checks whether or not the provider type and list of dependent files
 * are reported correctly.
 */

class ConnectionInfoTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( ConnectionInfoTest );
  CPPUNIT_TEST( TestProviderInfo );
  CPPUNIT_TEST_SUITE_END();

public:
    ConnectionInfoTest();
    virtual ~ConnectionInfoTest();
    void setUp();

protected:
    void TestProviderInfo();

private:
    FdoIConnection *CreateDb();
};

#endif
#endif
