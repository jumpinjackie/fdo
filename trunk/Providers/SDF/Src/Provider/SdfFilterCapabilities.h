// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
class SdfFilterCapabilities : public FdoIFilterCapabilities
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // default constructor
        SdfFilterCapabilities();

    protected:
        // default destructor
        virtual ~SdfFilterCapabilities();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
        // dispose this object
        SDF_API virtual void Dispose();

    //-------------------------------------------------------
    // FdoIFilterCapabilities implementation
    //-------------------------------------------------------

    public:
        // Returns an array of the ConditionTypes the feature provider supports.
        // The length parameter gives the number of condition types in the array.
        SDF_API virtual FdoConditionType* GetConditionTypes(FdoInt32& length);

        // Returns an array of the SpatialOperations the feature provider supports.
        // The length parameter gives the number of spatial operations in the array.
        SDF_API virtual FdoSpatialOperations* GetSpatialOperations(FdoInt32& length);

        // Returns an array of the DistanceOperations the feature provider supports.
        // The length parameter gives the number of distance operations in the array.
        SDF_API virtual FdoDistanceOperations* GetDistanceOperations(FdoInt32& length);

        // Returns true if the feature provider supports geodesic distance measurement,
        // and false if it only supports linear distance measurement.
        SDF_API virtual bool SupportsGeodesicDistance();

        // Returns true if spatial and distance operations can be applied between two
        // geometric properties, and false if spatial and distance operations can be
        // applied between a geometric property and a literal geometry.
        SDF_API virtual bool SupportsNonLiteralGeometricOperations();
};

