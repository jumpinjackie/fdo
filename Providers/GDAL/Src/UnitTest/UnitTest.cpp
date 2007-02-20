/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "UnitTest.h"
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <fstream>
#include <malloc.h>

#include "RfpNoConfigTest.h"
#include "RfpTestExample1.h"
#include "RfpOverridesSerializeTest.h"
#include "RfpTestLogOutputter.h"
#include "RfpTestGetRaster.h"
#include "RfpTestThorough.h"
#include "RfpTestMosaic.h"
#include "RfpTestRasterConversion.h"
#include "RfpTestGeotiff.h"
#include "RfpTestMrSID.h"
#include "RfpTestJp2.h"
#include "RfpTestEcw.h"
#include "RfpTestDem.h"
#include "RfpTestBand.h"
#include "RfpTestBandConfig.h"
#include "RfpTestSpatialContext.h"
#include "RfpTestAAIGrid.h"
#include "RfpTestPng.h"
#include "RfpTestJpg.h"

#include <iostream>

// The following macros are used as switches to determine
// which Test Cases will be excuted.

#define TEST_NO_CONFIG
#define TEST_EXAMPLE1
#define TEST_OVERRIDES_SERIALIZE
#define TEST_THOROUGHTEST
#define TEST_GET_RASTER
#define TEST_GEOTIFF
#define TEST_RASTERCONVERSION
#define TEST_DEM
#define TEST_BAND_CONFIG
#define TEST_BAND
#define TEST_AAIGrid
#define TEST_PNG
#define TEST_JPG

//#define TEST_SPATIAL_CONTEXT
//#define TEST_ECW
//#define TEST_JP2
//#define TEST_MRSID
//#define TEST_MOSAIC

void UnitTestMainProc()
{
	CppUnit::TextUi::TestRunner runner;

        std::cout << "Test Report\n";

#ifdef TEST_JPG	
	runner.addTest(RfpTestJpg::suite());
#endif

#ifdef TEST_PNG	
	runner.addTest(RfpTestPng::suite());
#endif

#ifdef TEST_AAIGrid
	runner.addTest(RfpTestAAIGird::suite());
#endif

#ifdef TEST_NO_CONFIG	
	runner.addTest(RfpNoConfigTest::suite());
#endif

#ifdef TEST_EXAMPLE1
	runner.addTest(RfpTestExample1::suite());
#endif

#ifdef TEST_OVERRIDES_SERIALIZE
	runner.addTest(RfpOverridesSerializeTest::suite());
#endif

#ifdef TEST_GET_RASTER
	runner.addTest(RfpTestGetRaster::suite());
#endif

#ifdef TEST_THOROUGHTEST
	runner.addTest(RfpTestThorough::suite());
#endif

#ifdef TEST_MOSAIC
	runner.addTest(RfpTestMosaic::suite());
#endif

#ifdef TEST_RASTERCONVERSION
	runner.addTest(RfpTestRasterConversion::suite());
#endif

#ifdef TEST_GEOTIFF
	runner.addTest(RfpTestGeotiff::suite());
#endif

#ifdef TEST_MRSID
	runner.addTest(RfpTestMrSID::suite());
#endif

#ifdef TEST_JP2
	runner.addTest(RfpTestJp2::suite());
#endif

#ifdef TEST_ECW
	runner.addTest(RfpTestEcw::suite());
#endif

#ifdef TEST_DEM
	runner.addTest(RfpTestDem::suite());
#endif

#ifdef TEST_BAND
	runner.addTest(RfpTestBand::suite());
#endif

#ifdef TEST_BAND_CONFIG
	runner.addTest(RfpTestBandConfig::suite());
#endif

#ifdef TEST_SPATIAL_CONTEXT
	runner.addTest(RfpTestSpatialContext::suite());
#endif

#ifndef LOG_TESTS_RESULT
//#define LOG_TESTS_RESULT
#endif

	// Add other tests here...


#ifdef LOG_TESTS_RESULT
	// save the test results to file.
	std::string fileName = "src/UnitTest/TestLogs/";
	fileName += "RfpLog_";
	fileName += ".xml";
	std::ofstream* logfile = new std::ofstream(fileName.c_str(), std::ios_base::app);
	RfpTestLogOutputter * outputter = new RfpTestLogOutputter(&runner.result(), *logfile);
	runner.setOutputter(outputter);
#endif //LOG_TESTS_RESULT


#ifdef _WIN32
	// Set second parameter to 'true' to check the output result until press <ENTER>.
    LPSTR cmd_line = GetCommandLine ();
    char *tmp = (char*)_alloca (strlen (cmd_line) + 1);
    strcpy (tmp, cmd_line);
    strupr (tmp);
    bool wait = (NULL == strstr (tmp, "-NOWAIT"));

	runner.run("", wait); 
#else
	runner.setOutputter( new CppUnit::TextOutputter( 
                           &runner.result(),
                           std::cout ) );
	runner.run("", false); 

#endif
}

void prependTabs(FdoInt32 tabLevel)
{
    for (FdoInt32 i = 0; i < tabLevel; i++)
        wprintf(L"  ");
}

void PrintException(FdoException* exception)
{
	FdoException*	currentException = exception;
	FdoInt32		tabLevel = 0;
	while (currentException != NULL) 
	{
		prependTabs(tabLevel++);
		wprintf(L"%s\n", currentException->GetExceptionMessage());
		if (currentException != exception)
			currentException->Release();
		currentException = currentException->GetCause();
	}
}

