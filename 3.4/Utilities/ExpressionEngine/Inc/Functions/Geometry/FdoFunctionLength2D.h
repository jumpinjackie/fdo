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
#ifndef FDOFUNCTIONLENGTH2D_H
#define FDOFUNCTIONLENGTH2D_H

#include <FdoExpressionEngine.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonMiscUtil.h>

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

#include <wctype.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>

#include <FdoExpressionEngineINonAggregateFunction.h>


// ============================================================================
// The class FdoFunctionLength2D implements the Expression Engine geometry
// function LENGTH.
// ============================================================================

class FdoFunctionLength2D : public FdoExpressionEngineINonAggregateFunction
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Function to create an instance of this class.

        EXPRESSIONENGINE_API static FdoFunctionLength2D *Create (FdoBoolean computeGeodetic = false);

        // CreateObject:
        //  Function to create an instance of this class.

        EXPRESSIONENGINE_API virtual FdoFunctionLength2D *CreateObject ();

        // GetFunctionDefinition:
        //  The function returns the function definition for the geometry
        //  function LENGTH. The definition includes the list of supported
        //  signatures for the function.

        EXPRESSIONENGINE_API virtual FdoFunctionDefinition *GetFunctionDefinition ();

        // Evaluate:
        //  The function determines the function result and returns it back to
        //  the calling routine.

        EXPRESSIONENGINE_API virtual FdoLiteralValue *Evaluate (
                                    FdoLiteralValueCollection *literal_values);


	protected:
        // FdoFunctionLength2D:
        //  The function represents the class constructor.

        EXPRESSIONENGINE_API FdoFunctionLength2D (FdoBoolean computeGeodetic = false);

        // ~FdoFunctionLength2D:
        //  The function represents the class destructor.

        EXPRESSIONENGINE_API ~FdoFunctionLength2D ();

        // CreateFunctionDefinition:
        //  The routine creates the function definition for the geometry func-
        //  tion LENGTH.

    private:

        // ********************************************************************
        // *                        Private Interfaces                        *
        // ********************************************************************

        void CreateFunctionDefinition ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose () { delete this; };

        // Validate:
        //  The function validates the provided parameters for the geometry
        //  function LENGTH.

        void Validate (FdoLiteralValueCollection *literal_values);


        // ********************************************************************
        // *                      Private Member Variables                    *
        // ********************************************************************

        // function_definition:
        //  The variable references the function definition for the geometry
        //  function LENGTH.

        FdoFunctionDefinition *function_definition;

        // is_validated:
        //  For performance reasons the arguments passed to the procedure
        //  processing the request is done once only for the time of its
        //  execution. This variable stores whether or not the validation
        //  has been performed.

        bool is_validated;

		// Compute geodetic/euclidian distances.
		FdoBoolean compute_geodetic;

        FdoPtr<FdoDoubleValue> return_double_value;

};  //  class FdoFunctionLength2D

#endif
