#ifndef FDOSMPHCOORDINATESYSTEM_H
#define FDOSMPHCOORDINATESYSTEM_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Mgr.h>

#define FdoSmPhMatrix3D double[3][3];
#define FdoSmPhVector3D double[3];

// This Physical class encapsulates a Coordinate System from the RDBMS.
class FdoSmPhCoordinateSystem : public FdoSmPhSchemaElement
{
public:
    ~FdoSmPhCoordinateSystem(void);

    /// Returns the Well-know text defining the coordinate system
    FdoString* GetWkt();

    /// returns the Spatial Reference Identifier
    FdoInt64 GetSrid();

    /// Constructs an instance from the given attributes.
	FdoSmPhCoordinateSystem(
        FdoSmPhMgrP phMgr,
        FdoString* name,
        FdoString* description,
        FdoInt64 srid,
        FdoString* Wkt
    );

    // It is expected that most PostGis databases will store geometry coordinates
    // as X East, Y North even if coordinate system WKT reassigns the axes. If this
    // is the case then no coordinate transformations are needed when transferring
    // geometries between PostGIS and FDO.
    //
    // Just in case there are databases whose geometries actually follow the axis
    // assignment, the following transformation functions are supplied:

    // Given a point from PostGIS, transform it to FDO format (X East, Y North)
    void TransformOut( double& x, double& y, double& z);

    // Given a point from FDO, transform it to PostGIS
    void TransformIn( double& x, double& y, double& z);

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhCoordinateSystem() {}

    void ParseWkt( FdoStringP inWKT );

    // 3D vector. 
    // TODO: check if this can be replaced by more general code.
    class Vector
    {
    public:
        Vector();
        Vector( double x, double y, double z );
        ~Vector();

        void Get( double& x, double& y, double& z );

        void Set( double x, double y, double z );
        void Set( Vector& src );

        // Get ordinate and 0-base index.
        double& operator[]( int index );

        // Copy ordinates to this vector from given vector
        void operator=(Vector& src);

        // Multiple this vector with given vector. 
        // Returns ( this[0] * rhs[0] + this[1] * rhs[1] + this[2] * rhs[2]
        double operator*(Vector& rhs);

    private:
        double mVector[3];
    };

    // 3D matrix. 
    // TODO: check if this can be replaced by more general code.
    class Matrix
    {
    public:
        // Initialize to identity matrix when identity = true
        Matrix( bool identity = true );
        ~Matrix();

        // Swap X values with Y values
        Matrix SwapXY();

        // Copy from src into this matrix
        void Set(Matrix& src);

        // Gets the inverse of this matrix
        Matrix Inverse();

        // Get the row at the 0-base index
        Vector& operator[]( int index );
        // Copy from src into this matrix
        void operator=(Matrix& src);

        // Multiply this and the rhs matrix.
        Matrix operator*(Matrix& rhs);

        // Mulitple this matrix with a vector
        Vector operator*(Vector& rhs);

    private:
        // The rows in the matrix.
        Vector mMatrix[3];
    };

private:
    FdoStringP                  mWkt;
	FdoInt64                    mSrid;			// Coordinate system SRID. 

    Matrix                      mTransformOut;
    Matrix                      mTransformIn;
};

typedef FdoPtr<FdoSmPhCoordinateSystem> FdoSmPhCoordinateSystemP;

#endif


