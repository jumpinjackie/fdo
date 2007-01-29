#ifndef _EXTENDEDSELECTTESTS_H_
#define _EXTENDEDSELECTTESTS_H_


#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"


class ExtendedSelectTests : public ShpTests
{
  CPPUNIT_TEST_SUITE( ExtendedSelectTests );
  CPPUNIT_TEST( WorldTest );
  CPPUNIT_TEST_SUITE_END();

public:
    ExtendedSelectTests(void);
    virtual ~ExtendedSelectTests(void);
    virtual void setUp ();

protected:
	void WorldTest();
};


#endif
