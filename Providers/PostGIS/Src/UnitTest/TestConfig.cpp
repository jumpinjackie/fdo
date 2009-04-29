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
#include "Pch.h"
#include "TestConfig.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
namespace ba =  boost::algorithm;

namespace fdo { namespace postgis { namespace test {

//
// Global instance of common configuration
//
TestConfig gTestConfig;


TestConfig::TestConfig() : ready_(false)
{
}

TestConfig::~TestConfig()
{
}

void TestConfig::setup(std::string const& file)
{
    std::ifstream incfg(file.c_str());
    if (!incfg)
    {
        std::string msg("Can not open config file: ");
        msg += file;
        throw std::runtime_error(msg);
    }

    std::string line;
    std::getline(incfg, line);
    
    typedef std::vector<std::string> tokens_t;
    tokens_t opts;
    ba::split(opts, line, ba::is_any_of(";"));

    data_.connstr.clear();

    for (tokens_t::const_iterator it = opts.begin(); it != opts.end(); ++it)
    {
        tokens_t opt;
        ba::split(opt, *it, ba::is_any_of("="));
        
        ba::to_lower(opt.at(0));
        data_.connstr[opt.at(0)] = opt.at(1);
    }

    ready_ = true;
}

bool TestConfig::isReady() const
{
    return ready_;
}

///////////////////////////////////////////////////////////////////////////////
// Configuration settings accessors
///////////////////////////////////////////////////////////////////////////////

FdoStringP TestConfig::getProviderName() const
{
    validate();
    return L"OSGeo.PostGIS.3.4";
}

FdoStringP TestConfig::getProviderVersion() const
{
    validate();
    return L"3.4.0.0";
}

FdoStringP TestConfig::getFdoVersion() const
{
    validate();
    return L"3.4.0.0";
}

FdoStringP TestConfig::getPropertyProvider() const
{
    return getProperty("provider");
}

FdoStringP TestConfig::getPropertyUsername() const
{
    return getProperty("username");
}

FdoStringP TestConfig::getPropertyPassword() const
{
    return getProperty("password");
}

FdoStringP TestConfig::getPropertyService() const
{
    return getProperty("service");
}

FdoStringP TestConfig::getPropertyDatastore() const
{
    return getProperty("datastore");
}

FdoStringP TestConfig::getConnectionString() const
{
    FdoStringP cs =
        FdoStringP::Format(L"service=%ls;username=%ls;password=%ls;datastore=%ls",
        static_cast<FdoString*>(getPropertyService()),
        static_cast<FdoString*>(getPropertyUsername()),
        static_cast<FdoString*>(getPropertyPassword()),
        static_cast<FdoString*>(getPropertyDatastore()));

    return cs;
}

///////////////////////////////////////////////////////////////////////////////
// Private function members
///////////////////////////////////////////////////////////////////////////////

void TestConfig::validate() const
{
    if (!ready_)
        throw std::runtime_error("TestConfig object has not been initialized!");
}

FdoStringP TestConfig::getProperty(std::string const& name) const
{
    validate();

    Data::connstr_t::const_iterator pos = data_.connstr.find(name);
    if (data_.connstr.end() == pos)
    {
        std::string msg("Can not find property: " + name);
        throw std::runtime_error(msg);
    }

    return FdoStringP(pos->second.c_str());
}



}}} // namespace fdo::postgis::test
