//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "StdAfx.h"

// fdo::postgis::test
#include "TestConfig.h"
// std
#include <iostream>
#include <string>
// boost
#include <boost/program_options.hpp>
namespace bpo = boost::program_options;
// cppunit
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

int main(int argc, char* argv[])
{
    int rc = 0;
    bool wait = true;
    bool all = true;

    try
    {
#pragma message ("!!TODO!!: Fix UnitTest calss to implicit() that stopped working after migration to boost 1_34_1")
        //
        // Parse command line and configure tests
        //
        bpo::options_description desc("Usage options");
        desc.add_options()
            ("help", "print help message")
            ("config-file", bpo::value<std::string>(), "set input file with tests configuration")
            ("run-test", bpo::value<std::string>(), "name of test case to run")
            ("run-all", bpo::value<std::string>()/*->implicit()*/, "run all tests (default)")
            ("wait", bpo::value<std::string>()/*->implicit()*/, "run all tests (default: no)")
            ;

        bpo::variables_map vm;        
        bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
        bpo::notify(vm);    

        if (vm.count("help") || argc <= 1)
        {
            std::cout << desc << "\n";
            return 1;
        }

        if (vm.count("config-file"))
        {
            std::string const& fn = vm["config-file"].as<std::string>();

            fdo::postgis::test::gTestConfig.setup(fn);
        }

        //
        // Run tests 
        //

        CppUnit::TextUi::TestRunner runner;

        if (all)
            runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
        if (runner.run("", !wait))
            rc = 0;
        else
            rc = 1;
    }
    catch (std::exception& e)
    {
        std::cerr << "Failure: " << e.what() << std::endl;
        rc = 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception type!\n";
        rc = 1;
    }


    

    //
    //// Run specified test
    //if (!strstr(argv[i], "="))
    //{
    //    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry(argv[i]);
    //    runner.addTest (registry.makeTest ());
    //    all = false;
    //}



    return (rc);
}


// OLD & OBSOLETE
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//    // Parameters:
//    //    -GUI          Run the tests using the Windows dialog interface
//    //    -NoWAIT       When using the text-based interface, do not wait
//    //                  for the user to press RETURN before exiting.
//    //    <test>*       When using the text based interface, run only the
//    //                  specified test(s). Requires support in each tests
//    //                  cpp file to add the test name to a registry of the
//    //                  same name, e.g.:
//    //          CPPUNIT_TEST_SUITE_REGISTRATION (CreateDeleteTests);
//    //          CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (CreateDeleteTests, "CreateDeleteTests");
//    //
//    //  The connect strings values can be passed thru the commandline.
//    //  The valid options are
//    //    service=<service value>
//    //    username=<username value>
//    //    password=<password value>
//    //    datastore=<datastore value>
//
//    LPTSTR cmd_line = GetCommandLine();
//
//    char *tmpCmdLine = new char[strlen(cmd_line)+1];
//    strcpy(tmpCmdLine, cmd_line);
//    char *tok = strtok(tmpCmdLine, " ");
//    while (tok != NULL)
//    {
//        char *str;
//        if (str = strstr(tok, "="))
//            _putenv(tok);
//        tok = strtok(NULL, " ");
//    }
//    bool gui = false;
//    strcpy (tmpCmdLine, cmd_line);
//    _strupr (tmpCmdLine);
//    gui = (NULL != strstr (tmpCmdLine, "-GUI"));
//    delete [] tmpCmdLine;
//
//	#ifndef __AFXWIN_H__
//        gui = false; // Revert command line requirement to false
//	#endif
//
//    if (gui)
//	#ifdef __AFXWIN_H__
//        return (AfxWinMain (GetModuleHandle (NULL), NULL, cmd_line, SW_SHOW));
//	#else
//		return 1; // GUI required and MFC not included....
//	#endif
//    else
//    {
//        CppUnit::TextUi::TestRunner runner;
//        bool wait;
//        bool all;
//        int ret;
//
//        wait = true;
//        all = true;
//        if (1 < argc)
//        {
//            for (int i = 1; i < argc; i++)
//                if ('-' == argv[i][0])
//                    wait &= (0 != _stricmp (argv[i], "-NoWAIT"));
//                else
//                {
//                    if (!strstr(argv[i], "="))
//                    {
//                        CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry (argv[i]);
//                        runner.addTest (registry.makeTest ());
//                        all = false;
//                    }
//                }
//        }
//        if (all)
//            runner.addTest (CppUnit::TestFactoryRegistry::getRegistry().makeTest ());
//        if (runner.run ("", wait))
//            ret = 0;
//        else
//            ret = 1;
//	    return (ret);
//    }
//}

