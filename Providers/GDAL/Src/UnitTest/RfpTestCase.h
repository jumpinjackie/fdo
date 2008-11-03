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

#ifndef RFPTESTCASE_H
#define RFPTESTCASE_H

#ifdef _WIN32
#pragma once
#endif

class RfpTestCase : public CppUnit::TestCase
{
public:
    virtual void setUp();
    virtual void tearDown();
    virtual FdoIConnection* CreateConnection ()
        {
            FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
            return (manager->CreateConnection (L"OSGeo.Gdal"));
        }

protected:
    virtual void _setUp() = 0;
    virtual void _tearDown() = 0;

};

#endif
