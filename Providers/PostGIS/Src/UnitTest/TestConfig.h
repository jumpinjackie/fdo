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
#ifndef FDOPOSTGIS_TESTCONFIG_H_INCLUDED
#define FDOPOSTGIS_TESTCONFIG_H_INCLUDED

#include <map>
#include <string>
#include <FdoCommon.h>

namespace fdo { namespace postgis { namespace test {

class TestConfig
{
public:

    // Constructor does not initialize object.
    // Call TestConfig::setup() instead.
    TestConfig();

    // Destructor
    ~TestConfig();

    // Initialize config instance with settings from a file.
    void setup(std::string const& file);

    // Check if configuration is ready to use.
    bool isReady() const;

    //
    // Configuration settings accessors
    //

    FdoStringP getProviderName() const;
    FdoStringP getProviderFullName() const;
    FdoStringP getPropertyUsername() const;
    FdoStringP getPropertyPassword() const;
    FdoStringP getPropertyService() const;
    FdoStringP getPropertyDatastore() const;
    FdoStringP getConnectionString() const;

private:
 
    // Check state of the configuration object and throw if it's unusable.
    void validate() const;

    FdoStringP getProperty(std::string const& name) const;

    // Flag indicating if the configuration instance has been setup
    // and is ready to use.
    bool ready_;

    // Internal configuration data
    struct Data
    {
        typedef std::map<std::string, std::string> connstr_t;
        connstr_t connstr;
        Data() {}
    };

    Data data_;

}; // class TestConfig

//
// Global provider of common configuration settings
//

extern TestConfig gTestConfig;

}}} // namespace fdo::postgis::test

#endif // FDOPOSTGIS_TESTCONFIG_H_INCLUDED
