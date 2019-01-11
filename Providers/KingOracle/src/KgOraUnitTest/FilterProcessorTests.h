#ifndef FILTER_PROCESSOR_TESTS_H
#define FILTER_PROCESSOR_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class FilterProcessorTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(FilterProcessorTests);
    CPPUNIT_TEST(ProcessSpatialCondition);
    CPPUNIT_TEST(ProcessSpatialConditionSRID);
    CPPUNIT_TEST(EnvelopeIntersect);
    CPPUNIT_TEST_SUITE_END();

  public:
    FilterProcessorTests(void);

  public:
    ~FilterProcessorTests(void);

    void ProcessSpatialCondition();
    void ProcessSpatialConditionSRID();
    void EnvelopeIntersect();
};

#endif