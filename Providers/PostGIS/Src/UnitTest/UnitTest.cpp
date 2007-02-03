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
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main (int argc, char *argv[])
{
#ifndef _WIN32
    extern void ctSetup();
    ctSetup();
#endif

    CppUnit::TextUi::TestRunner runner;
    bool all;
    int ret;

    for (int i = 1; i < argc; i++)
    {
        char *tok = strtok (argv[i], " ");
        if (tok)
        {
            char *str;
            str = strstr (tok, "=");
            if (str)
                putenv (tok);
        }
    }
    all = true;
    for (int i = 1; i < argc; i++)
    {
        if ('-' == argv[i][0])
        {
        }
        else
        {
            if (!strstr (argv[i], "="))
            {
                CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry (argv[i]);
                runner.addTest (registry.makeTest ());
                all = false;
            }
        }
    }
    if (all)
        runner.addTest (CppUnit::TestFactoryRegistry::getRegistry ().makeTest ());


    runner.setOutputter (new CppUnit::CompilerOutputter (&runner.result (), std::cerr));
    ret = runner.run () ? 0 : 1;

    return (ret);
}


