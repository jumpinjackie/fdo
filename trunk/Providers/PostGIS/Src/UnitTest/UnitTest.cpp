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
        //
        // Parse command line and configure tests
        //
        bpo::options_description desc("Usage options");
        desc.add_options()
            ("help", "print help message")
            ("config", bpo::value<std::string>(), "path to configuration file with connection settings")
            ("run-test", bpo::value<std::string>(), "name of test case to run")
            ("run-all", bpo::value<std::string>(), "run all tests (default)")
            ("wait", bpo::value<std::string>(), "run all tests (default: no)")
            ;

        bpo::variables_map vm;        
        bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
        bpo::notify(vm);    

        if (vm.count("help") || argc <= 1)
        {
            std::cout << desc << "\n";
            return 1;
        }

        if (vm.count("config"))
        {
            std::string const& fn = vm["config"].as<std::string>();

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

    //// Run specified test
    //if (!strstr(argv[i], "="))
    //{
    //    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry(argv[i]);
    //    runner.addTest (registry.makeTest ());
    //    all = false;
    //}

    return (rc);
}
