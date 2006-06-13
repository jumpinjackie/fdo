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

#ifndef SPATIALFILTERTESTS_H
#define SPATIALFILTERTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ShpTests.h"

class SpatialFilterTests :
    public ShpTests
{
    CPPUNIT_TEST_SUITE (SpatialFilterTests);
    CPPUNIT_TEST (SelectAll);
    CPPUNIT_TEST (EnvelopeIntersects);
    CPPUNIT_TEST (Within);
    CPPUNIT_TEST (Inside);
    CPPUNIT_TEST (Intersects);
    CPPUNIT_TEST (PerformanceTests_Roads);
    CPPUNIT_TEST (PerformanceTests_Lakes);
    CPPUNIT_TEST (Crosses);
    CPPUNIT_TEST (Touches);
    CPPUNIT_TEST (Bogus);
    CPPUNIT_TEST_SUITE_END ();

    static FdoPtr<FdoIConnection> mConnection;

public:
    SpatialFilterTests (void);
    virtual ~SpatialFilterTests (void);
	void setUp ();
	void tearDown ();

protected:
    void SelectAll ();
	void EnvelopeIntersects ();
    void Within ();
    void Inside ();
	void Intersects ();
    void PerformanceTests_Roads();
    void PerformanceTests_Lakes();
    void Crosses ();
    void Touches ();
    void Bogus ();

private:
    void    runPerformanceTests( FdoString* class_name, int total_feats, int within_feats, int inside_feats );
    int     runSpatialQuery( FdoString* class_name, FdoSpatialOperations spOp, double *polyOrdsXY, int numOrds );
    int     runSpatialQuery( FdoString* class_name, FdoSpatialOperations spOp, FdoIPolygon* poly );
    int     runSpatialQueryWithPolygonFromFile( FdoString* class_name1, FdoSpatialOperations spOp, FdoString* class_name2 );
    FdoIPolygon* getPolygonFromFile( FdoString* file_name, FdoString* class_name );
};

#endif // FILTERTESTS_H

