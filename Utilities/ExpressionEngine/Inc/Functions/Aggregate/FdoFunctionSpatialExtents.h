//
// 
// Copyright (C) 2004-2007  Autodesk, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// 
//
#ifndef FDOFUNCTIONSPATIALEXTENTS_H
#define FDOFUNCTIONSPATIALEXTENTS_H

#include <FdoExpressionEngine.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonMiscUtil.h>

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

#include <wctype.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>
#include <vector>

#include <FdoExpressionEngineIAggregateFunction.h>
#include <Functions/Aggregate/CacheValueCollection.h>


// ============================================================================
// The class FdoFunctionSpatialExtents implements the Expression Engine func-
// tion SPATIALEXTENTS.
// ============================================================================

class FdoFunctionSpatialExtents : public FdoExpressionEngineIAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        static FdoFunctionSpatialExtents *Create ();

        // CreateObject:
        //  Function to create an instance of this class.

        virtual FdoFunctionSpatialExtents *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the function
        //  SPATIALEXTENTS. The definition includes the list of supported
        //  signatures for the function.

        virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Process:
        //  The function executes the call of the function SPATIALEXTENTS on
        //  an expression value.

        virtual void Process (FdoLiteralValueCollection *literal_values);

        // GetResult:
        //  The function returns the result of the function SPATIALEXTENTS.

        virtual FdoLiteralValue *GetResult ();


    private:

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        // FdoFunctionSpatialExtents:
        //  The function represents the class constructor.

        FdoFunctionSpatialExtents ();

        // ~FdoFunctionSpatialExtents:
        //  The function represents the class destructor.

        ~FdoFunctionSpatialExtents ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the function
        //  SPATIALEXTENTS.

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // ProcessRequest:
        //  The function processes a call to the Expression Engine function
        //  SPATIALEXTENTS.

        void ProcessRequest (FdoGeometryValue *geometry_value);

        // Validate:
        //  The function validates the provided parameters for the function
        //  SPATIALEXTENTS.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the function
        //  SPATIALEXTENTS.

        FdoFunctionDefinition *function_definition;

        // result_geometry_value:
        //  The variable references the function result.

        FdoPtr<FdoIGeometry> result_geometry_value;

        // result_geometry_value_set:
        //  A flag indicating whether or not the geometry value has been set.

        bool result_geometry_value_set;

        // is_validated:
        //  For performance reasons the arguments passed to the procedure
        //  processing the request is done once only for the time of its
        //  execution. This variable stores whether or not the validation
        //  has been performed.

        bool is_validated;

};  //  class FdoFunctionSpatialExtents

#endif
