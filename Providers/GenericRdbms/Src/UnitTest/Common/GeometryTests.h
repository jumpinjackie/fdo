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
 *
 */

#ifndef CPP_UNIT_GEOMETRYTESTS_H
#define CPP_UNIT_GEOMETRYTESTS_H

#include <Inc/Rdbi/proto.h>

class GeometryTests :
    public CppUnit::TestCase
{
protected:
    rdbi_context_def* mRdbiContext;
    int mId;
    char    mSql[1024];


private:
    CPPUNIT_TEST_SUITE (GeometryTests);
    CPPUNIT_TEST (ddl);
    CPPUNIT_TEST (define);
    CPPUNIT_TEST (bind);
    CPPUNIT_TEST( RoundTripGeometries );
    CPPUNIT_TEST_SUITE_END ();

public:
    GeometryTests (void);
    virtual ~GeometryTests (void);
	virtual void setUp ();
	virtual void tearDown ();

protected:
	virtual void ddl ();
	virtual void define ();
	virtual void bind ();
	void describe ();
    virtual void    RoundTripGeometries();

    int rdbi_sql_Ex( rdbi_context_def *context, int sqlid, FdoStringP sql );

    virtual void set_provider() {  CPPUNIT_FAIL ("naked set_provider() called"); };
    virtual int do_rdbi_init ();
    virtual int do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword);
    virtual char *get_bind_var (int n);
    virtual char *get_geom_conv () { return ""; };
    virtual char *get_date_time (const struct tm *when);
    virtual char *get_geometry_type ();
    virtual char *get_geometry_text (FdoIGeometry *geometry);
    virtual int  do_insert_geometry( int cursor, FdoInt32 *featId, FdoIGeometry **geometry );

    void    createDb();
    void    deleteDb();

    void    RoundTripGeometry_Point();
    void    RoundTripGeometry_MultiPoint();

    void    RoundTripGeometry_LineString();
    void    RoundTripGeometry_MultiLineString();

    void    RoundTripGeometry_CurveString();
    void    RoundTripGeometry_MultiCurveString();

    void    RoundTripGeometry_Polygon();
    void    RoundTripGeometry_MultiPolygon();
    void    RoundTripGeometry_RectangularMultiPolygon();

    void    RoundTripGeometry_CurvePolygon();
    void    RoundTripGeometry_MultiCurvePolygon();

    void            connect ();
    void            disconnect ();

    void            fetch_geom( FdoGeometryType geom_type, long feat_num, int num_ords );
    void            set_geom_feat_fgf( int operation, FdoGeometryType geom_type, int num_ords, long* feat_num, FdoIGeometry * specialGeom = NULL );
    void            create_feat_fgf( FdoGeometryType geom_type, int num_ords, long *feat_num, FdoIGeometry * specialGeom = NULL );
    void            update_feat_fgf( FdoGeometryType geom_type, long feat_num, int num_ords, FdoIGeometry * specialGeom = NULL );
    void            RoundTripGeometry( FdoGeometryType geomType, FdoIGeometry * specialGeom = NULL );
    void            check_geom( long feat_num, FdoByteArray *in_ba );

    void    updateSpatialContext_0();
    FdoICurveString * createCurveString( FdoFgfGeometryFactory * gf, int num_ords, double * line_points );
    char *geometryType2char( FdoGeometryType geomType );

private:
    FdoPtr<FdoIConnection> mConn;
    static const char*  mTraceFilePath;

    long            mFeatNum1;

    int             mFdoDimensionality;  // xy, xyz, xym, xyzm
    int             mNumPointOrdinates;  // 2, 3, 4

    int             mNumLINE1_ORDINATES;

    double         *mLINE1_POINTS;       // 2D, 3D, 4D
    double         *mLINE2_POINTS;       // 2D, 3D, 4D
 
    double         *mRING1_POINTS;
    double         *mRING2_POINTS;
    double         *mRING3_POINTS;
    double         *mRING4_POINTS;


};

#endif // CPP_UNIT_GEOMETRYTESTS_H

